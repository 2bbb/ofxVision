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
#   define OFX_VISION_CFAUTORELEASE(obj) obj
#else
#   define OFX_VISION_BRIDGE_TRANSFER
#   define OFX_VISION_BRIDGE_RETAINED
#   define OFX_VISION_RETAIN(obj) [obj retain]
#   define OFX_VISION_RELEASE(obj) [obj release]
#   define OFX_VISION_AUTORELEASE(obj) [obj autorelease]
#   define OFX_VISION_CFAUTORELEASE(obj) CFAutorelease(obj)
#endif

#include "ofxVisionUtility.h"
#include "ofxVisionObservation.h"

#include "ofBaseTypes.h"

#import <Foundation/Foundation.h>
#import <IOSurface/IOSurface.h>
#import <CoreVideo/CoreVideo.h>
#import <CoreImage/CoreImage.h>
#import <Vision/Vision.h>

#include <limits>

namespace ofx {
    namespace Vision {
        Observation::Rectangle toOF(VNRectangleObservation *r);
#if OFX_VISION_VERSION_CHECK_X(10, 15)
        Observation::SaliencyImage toOF(VNSaliencyImageObservation *saliencyResult);
#endif
        
#if OFX_VISION_VERSION_CHECK(11, 0)
        Observation::DetectedPoint toOF(VNDetectedPoint *p);
#endif
        
#if OFX_VISION_VERSION_CHECK(11, 0)
        Observation::RecognizedPoint toOF(VNRecognizedPoint *p);
        
        void convHandPose(VNHumanHandPoseObservation *hand,
                          VNHumanHandPoseObservationJointName name,
                          Observation::RecognizedPoint &point);
#if OFX_VISION_VERSION_CHECK(11, 0)
        Observation::HandPose toOF(VNHumanHandPoseObservation *hand);
#endif // #if OFX_VISION_VERSION_CHECK(11, 0)
        
#endif // #ifdef __OBJC__
        
#if OFX_VISION_VERSION_CHECK(11, 0)
        void convBodyPose(VNHumanBodyPoseObservation *body,
                          VNHumanBodyPoseObservationJointName name,
                          Observation::RecognizedPoint &point);
#endif
        
#if OFX_VISION_VERSION_CHECK(11, 0)
        Observation::BodyPose toOF(VNHumanBodyPoseObservation *body);
#endif
        
        Observation::IndexPath toOF(NSIndexPath *path);
        
#if OFX_VISION_VERSION_CHECK(11, 0)
        std::shared_ptr<Observation::Contour> toOF(VNContour *c);
#endif
        
#if OFX_VISION_VERSION_CHECK(11, 0)
        Observation::Contours toOF(VNContoursObservation *cs);
#endif
        
        Observation::FaceLandmarkRegion2D toOF(VNFaceLandmarkRegion2D *f);
        Observation::FaceLandmarks2D toOF(VNFaceLandmarks2D *l);
        float toOF(NSNumber *n);
        
#if OFX_VISION_VERSION_CHECK_X(10, 13)
        Observation::Face toOF(VNFaceObservation *f);
#endif
    };
};

#endif /* ofxVisionObjC_ARCMacro_h */
#endif /* __OBJC__ */
