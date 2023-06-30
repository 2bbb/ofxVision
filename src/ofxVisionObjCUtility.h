//
//  ofxVisionObjC_ARCMacro.h
//
//  Created by 2bit on 2023/06/29.
//

#ifdef __OBJC__
#ifndef ofxVisionObjC_ARCMacro_h
#define ofxVisionObjC_ARCMacro_h

#if __has_feature(objc_arc)
#   define OFX_VISION_BRIDGE_TRANSFER __bridge_transfer
#   define OFX_VISION_BRIDGE_RETAINED __bridge_retained
#   define OFX_VISION_RETAIN(obj) obj
#   define OFX_VISION_RELEASE(obj) obj
#   define OFX_VISION_AUTORELEASE(obj) obj
#else
#   define OFX_VISION_BRIDGE_TRANSFER
#   define OFX_VISION_BRIDGE_RETAINED
#   define OFX_VISION_RETAIN(obj) [obj retain]
#   define OFX_VISION_RELEASE(obj) [obj release]
#   define OFX_VISION_AUTORELEASE(obj) [obj autorelease]
#endif

#include "ofxVisionUtility.h"
#include "ofxVisionObservation.h"

#include "ofBaseTypes.h"

#import <IOSurface/IOSurface.h>
#import <CoreVideo/CoreVideo.h>
#import <CoreImage/CoreImage.h>
#import <Vision/Vision.h>


namespace ofx {
    namespace Vision {
        namespace {
            CIImage *toCIImage(const ofBaseHasPixels &pix)
            { return [CIImage imageWithCGImage:ofBaseHasPixelsToCGImageRef(pix)]; };
            CIImage *toCIImage(CVPixelBufferRef pixelBuffer)
            { return [CIImage imageWithCVPixelBuffer:pixelBuffer]; };
            CIImage *toCIImage(IOSurfaceRef surface)
            { return [CIImage imageWithIOSurface:surface]; };
            
            Observation::SaliencyImage toOF(VNSaliencyImageObservation *saliencyResult) {
                Observation::SaliencyImage result;
                CVPixelBufferRef pixelBuffer = saliencyResult.pixelBuffer;
                result.pixels = pixelBufferToOfFloatImage(pixelBuffer);
                auto &salients = result.salientObjects;
                for(VNRectangleObservation *rect in saliencyResult.salientObjects) {
                    salients.emplace_back();
                    auto &s = salients.back();
                    s.uuid = rect.uuid.UUIDString.UTF8String;
                    s.segmentationMask.pixels = pixelBufferToOfFloatImage(rect.globalSegmentationMask.pixelBuffer);
                    if(rect.globalSegmentationMask.featureName) s.segmentationMask.featureName =  rect.globalSegmentationMask.featureName.UTF8String;
                    s.topLeft = toOF(rect.topLeft);
                    s.topRight = toOF(rect.topRight);
                    s.bottomLeft = toOF(rect.bottomLeft);
                    s.bottomRight = toOF(rect.bottomRight);
                    s.boundingBox = toOF(rect.boundingBox);
                }
                return result;
            }
            
            Observation::DetectedPoint toOF(VNDetectedPoint *p) {
                Observation::DetectedPoint point;
                point.position.x = p.x;
                point.position.y = 1.0f - p.y;
                point.confidence = p.confidence;
                return point;
            }
            
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
            Observation::HandPose toOF(VNHumanHandPoseObservation *hand) {
                if(!hand) return {};
                Observation::HandPose pose;
                pose.chirality = (Observation::HandPose::Chirality)hand.chirality;
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
            
            void convBodyPose(VNHumanBodyPoseObservation *body,
                              VNHumanBodyPoseObservationJointName name,
                              Observation::RecognizedPoint &point)
            {
                NSError *err = nil;
                auto p = [body recognizedPointForJointName:name error:&err];
                if(p) point = toOF(p);
                else point.confidence = -1.0f;
            }

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
        }
    };
};

#endif /* ofxVisionObjC_ARCMacro_h */
#endif /* __OBJC__ */
