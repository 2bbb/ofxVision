//
//  ofxVisionDetectHumanHandPose.m
//
//  Created by 2bit on 2023/06/30.
//

#import "ofxVisionObjCUtility.h"

#import "ofxVisionDetectHumanHandPose.h"
#include "ofxVisionUtility.h"

#include "ofLog.h"

#if OFX_VISION_VERSION_CHECK(11, 0)

namespace ofx {
    namespace Vision {
        using Target = DetectHumanHandPose;
        
        Target::ResultType Target::detectWithCIImage(ofxVisionCIImage *image) {
            auto request = createRequest();
            NSError *err = nil;
            [handler performRequests:@[request]
                           onCIImage:(CIImage *)image
                         orientation:kCGImagePropertyOrientationUp
                               error:&err];
            if(err) {
                ofLogError("ofxVisionPersonSegmentation") << err.description.UTF8String;
                return {};
            }
            return createResult(request);
        }
        
#include "details/detect_impl.inl"

        Target::Request *Target::createRequest() const {
            auto request = OFX_VISION_AUTORELEASE([[Target::Request alloc] init]);
            request.maximumHandCount = settings.maximumHandCount;
            return request;
        }
        
        Target::ResultType Target::createResult(void *req) const {
            Target::Request *request = (Target::Request *)req;
            Target::ResultType results;
            for(VNHumanHandPoseObservation *hand in request.results) {
                results.push_back(toOF(hand));
            }
            return results;
        }
    }; // namespace Vision
}; // namespace ofx

#endif
