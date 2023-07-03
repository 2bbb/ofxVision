//
//  ofxVisionObjCUtility.mm
//
//  Created by 2bit on 2023/07/03.
//

#import "ofxVisionObjCUtility.h"

namespace ofx {
    namespace Vision {
        Observation::Rectangle toOF(VNRectangleObservation *r) {
            Observation::Rectangle rect;
            rect.uuid = r.uuid.UUIDString.UTF8String;
            if (@available(macOS 12.0, *)) {
                rect.segmentationMask.pixels = pixelBufferToOfFloatImage(r.globalSegmentationMask.pixelBuffer);
            } else {
                // Fallback on earlier versions
            }
            if (@available(macOS 12.0, *)) {
                rect.segmentationMask.featureName =  r.globalSegmentationMask.featureName.UTF8String ?: "";
            } else {
                // Fallback on earlier versions
                rect.segmentationMask.featureName = "";
            }
            rect.topLeft = toOF(r.topLeft);
            rect.topRight = toOF(r.topRight);
            rect.bottomLeft = toOF(r.bottomLeft);
            rect.bottomRight = toOF(r.bottomRight);

            rect.boundingBox = toOF(r.boundingBox);
            return rect;
        }

#if OFX_VISION_VERSION_CHECK_X(10, 15)
        Observation::SaliencyImage toOF(VNSaliencyImageObservation *saliencyResult) {
            Observation::SaliencyImage result;
            result.uuid = saliencyResult.uuid.UUIDString.UTF8String;
            result.featureName =  saliencyResult.featureName.UTF8String ?: "";
            CVPixelBufferRef pixelBuffer = saliencyResult.pixelBuffer;
            result.pixels = pixelBufferToOfFloatImage(pixelBuffer);
            auto &salients = result.salientObjects;
            salients.resize(saliencyResult.salientObjects.count);
            for(auto i = 0ul; i < salients.size(); ++i) {
                salients[i] = toOF(saliencyResult.salientObjects[i]);
            }
            return result;
        }
#endif
        
#if OFX_VISION_VERSION_CHECK(11, 0)
        Observation::DetectedPoint toOF(VNDetectedPoint *p) {
            Observation::DetectedPoint point;
            point.position.x = p.x;
            point.position.y = 1.0f - p.y;
            point.confidence = p.confidence;
            return point;
        }
#endif
        
#if OFX_VISION_VERSION_CHECK(11, 0)
        Observation::RecognizedPoint toOF(VNRecognizedPoint *p) {
            Observation::RecognizedPoint point;
            point.position.x = p.x;
            point.position.y = 1.0f - p.y;
            point.confidence = p.confidence;
            if(p.identifier) point.identifier = p.identifier.UTF8String;
            return point;
        }
        
        void convHandPose(VNHumanHandPoseObservation *hand,
                          VNHumanHandPoseObservationJointName name,
                          Observation::RecognizedPoint &point)
        {
            NSError *err = nil;
            auto p = [hand recognizedPointForJointName:name error:&err];
            if(p) point = toOF(p);
            else point.confidence = -1.0f;
        }
#if OFX_VISION_VERSION_CHECK(11, 0)
        Observation::HandPose toOF(VNHumanHandPoseObservation *hand) {
            if(!hand) return {};
            Observation::HandPose pose;
            if (@available(macOS 12.0, *)) {
                pose.chirality = (Observation::HandPose::Chirality)hand.chirality;
            } else {
                // Fallback on earlier versions
            }
#define conv(JointName, point) convHandPose(hand, VNHumanHandPoseObservationJointName##JointName, point)
            conv(ThumbTip, pose.thumb.tip);
            conv(ThumbIP,  pose.thumb.ip);
            conv(ThumbMP,  pose.thumb.mp);
            conv(ThumbCMC, pose.thumb.cmc);
            
            conv(IndexTip, pose.index.tip);
            conv(IndexDIP, pose.index.dip);
            conv(IndexPIP, pose.index.pip);
            conv(IndexMCP, pose.index.mcp);
            
            conv(MiddleTip, pose.middle.tip);
            conv(MiddleDIP, pose.middle.dip);
            conv(MiddlePIP, pose.middle.pip);
            conv(MiddleMCP, pose.middle.mcp);

            conv(RingTip, pose.ring.tip);
            conv(RingDIP, pose.ring.dip);
            conv(RingPIP, pose.ring.pip);
            conv(RingMCP, pose.ring.mcp);

            conv(LittleTip, pose.little.tip);
            conv(LittleDIP, pose.little.dip);
            conv(LittlePIP, pose.little.pip);
            conv(LittleMCP, pose.little.mcp);

            conv(Wrist, pose.wrist);
#undef conv
            return pose;
        }
#endif // #if OFX_VISION_VERSION_CHECK(11, 0)
        
#endif // #ifdef __OBJC__
        
#if OFX_VISION_VERSION_CHECK(11, 0)
        void convBodyPose(VNHumanBodyPoseObservation *body,
                          VNHumanBodyPoseObservationJointName name,
                          Observation::RecognizedPoint &point)
        {
            NSError *err = nil;
            auto p = [body recognizedPointForJointName:name error:&err];
            if(p && err == nil) point = toOF(p);
            else point.confidence = -1.0f;
        }
#endif
        
#if OFX_VISION_VERSION_CHECK(11, 0)
        Observation::BodyPose toOF(VNHumanBodyPoseObservation *body) {
            if(!body) return {};
            Observation::BodyPose pose;
#define conv(JointName, point) convBodyPose(body, VNHumanBodyPoseObservationJointName##JointName, pose.point)
            conv(LeftEar,  head.leftEar);
            conv(LeftEye,  head.leftEye);
            conv(Nose,     head.nose);
            conv(RightEye, head.rightEye);
            conv(RightEar, head.rightEar);
            conv(Neck,     head.neck);
            
            conv(LeftShoulder, arms.left.shoulder);
            conv(LeftElbow,    arms.left.elbow);
            conv(LeftElbow,    arms.left.wrist);
            
            conv(RightShoulder, arms.right.shoulder);
            conv(RightElbow,    arms.right.elbow);
            conv(RightElbow,    arms.right.wrist);
            
            conv(Root, waist);
            
            conv(LeftHip,   legs.left.hip);
            conv(LeftKnee,  legs.left.knee);
            conv(LeftAnkle, legs.left.ankle);
            
            conv(RightHip,   legs.right.hip);
            conv(RightKnee,  legs.right.knee);
            conv(RightAnkle, legs.right.ankle);
#undef conv
            return pose;
        }
#endif
        
        Observation::IndexPath toOF(NSIndexPath *path) {
            Observation::IndexPath indexPath;
            indexPath.resize(path.length);
            for(auto i = 0ul; i < indexPath.size(); ++i) {
                indexPath[i] = [path indexAtPosition:i];
            }
            return indexPath;
        } // Observation::IndexPath toOF(NSIndexPath *path)
        
#if OFX_VISION_VERSION_CHECK(11, 0)
        std::shared_ptr<Observation::Contour> toOF(VNContour *c) {
            auto contour = std::make_shared<Observation::Contour>();
            contour->aspectRatio = c.aspectRatio;
            contour->indexPath = toOF(c.indexPath);
            contour->normalizedPoints.resize(c.pointCount);
            for(auto i = 0ul; i < contour->normalizedPoints.size(); ++i) {
                contour->normalizedPoints[i].x = c.normalizedPoints[i][0];
                contour->normalizedPoints[i].y = 1.0f - c.normalizedPoints[i][1];
            }
            contour->childContours.resize(c.childContourCount);
            for(auto i = 0ul; i < contour->childContours.size(); ++i) {
                contour->childContours[i] = toOF(c.childContours[i]);
            }
            return contour;
        } // Observation::Contour toOF(VNContour *c)
#endif
        
#if OFX_VISION_VERSION_CHECK(11, 0)
        Observation::Contours toOF(VNContoursObservation *cs) {
            Observation::Contours contours;
            
            contours.uuid = cs.uuid.UUIDString.UTF8String;
            contours.confidence = cs.confidence;
            
            contours.contourCount = cs.contourCount;
            contours.topLevelContours.resize(cs.topLevelContourCount);
            for(auto i = 0ul; i < contours.topLevelContours.size(); ++i) {
                contours.topLevelContours[i] = toOF(cs.topLevelContours[i]);
            }
            return contours;
        } // Observation::Contours toOF(VNContoursObservation *cs)
#endif
        
        Observation::FaceLandmarkRegion2D toOF(VNFaceLandmarkRegion2D *f) {
            Observation::FaceLandmarkRegion2D face;
            if (@available(macOS 13.0, *)) {
                switch(f.pointsClassification) {
                    case VNPointsClassificationDisconnected:
                        face.pointsClassification = Observation::PointsClassification::Disconnected;
                        break;
                    case VNPointsClassificationOpenPath:
                        face.pointsClassification = Observation::PointsClassification::OpenPath;
                        break;
                    case VNPointsClassificationClosedPath:
                        face.pointsClassification = Observation::PointsClassification::ClosedPath;
                        break;
                }
            } else {
                // Fallback on earlier versions
                face.pointsClassification = Observation::PointsClassification::NotImplemented;
            }
            face.normalizedPoints.resize(f.pointCount);
            for(auto i = 0ul; i < face.normalizedPoints.size(); ++i) {
                face.normalizedPoints[i].x = f.normalizedPoints[i].x;
                face.normalizedPoints[i].y = 1.0f - f.normalizedPoints[i].y;
            }
            return face;
        } // Observation::FaceLandmarkRegion2D toOF(VNFaceLandmarkRegion2D *f)
        
        Observation::FaceLandmarks2D toOF(VNFaceLandmarks2D *l) {
            Observation::FaceLandmarks2D landmark;
            if(l == nil) return landmark;
            landmark.confidence = l.confidence;
            
#define conv(name) landmark.name = toOF(l.name)
            conv(allPoints);
            conv(faceContour);
            
            conv(leftEye);
            conv(leftEyebrow);
            conv(leftPupil);
            
            conv(rightEye);
            conv(rightEyebrow);
            conv(rightPupil);
            
            conv(nose);
            conv(noseCrest);
            
            conv(medianLine);
            
            conv(outerLips);
            conv(innerLips);
#undef conv
            
            return landmark;
        } // Observation::FaceLandmarks2D toOF(VNFaceLandmarks2D *l)
        
        float toOF(NSNumber *n) {
            return n ? n.floatValue : std::numeric_limits<float>::quiet_NaN();
        }
        
#if OFX_VISION_VERSION_CHECK_X(10, 13)
        Observation::Face toOF(VNFaceObservation *f) {
            Observation::Face face;
            
            face.uuid = f.uuid.UUIDString.UTF8String;
            if (@available(macOS 12.0, *)) {
                face.segmentationMask.pixels = pixelBufferToOfFloatImage(f.globalSegmentationMask.pixelBuffer);
                face.segmentationMask.featureName =  f.globalSegmentationMask.featureName.UTF8String ?: "";
            } else {
                // Fallback on earlier versions
                face.segmentationMask.featureName = "";
            }
            face.boundingBox = toOF(f.boundingBox);

            face.landmarks = toOF(f.landmarks);
            
            if (@available(macOS 10.14, *)) {
                face.roll = toOF(f.roll);
                face.yaw = toOF(f.yaw);
            } else {
                // Fallback on earlier versions
                face.roll = std::numeric_limits<float>::quiet_NaN();
                face.yaw = std::numeric_limits<float>::quiet_NaN();
            }
            
            if (@available(macOS 12.0, *)) {
                face.pitch = toOF(f.pitch);
            } else {
                // Fallback on earlier versions
                face.pitch = std::numeric_limits<float>::quiet_NaN();
            }
            
            if (@available(macOS 10.15, *)) {
                face.faceCaptureQuality = toOF(f.faceCaptureQuality);
            } else {
                // Fallback on earlier versions
                face.faceCaptureQuality = std::numeric_limits<float>::quiet_NaN();
            }
            
            return face;
        } // Observation::Face toOF(VNFaceObservation *f)
#endif
    }
}
