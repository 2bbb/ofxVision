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
        
        BaseRequest *Target::createRequest() const {
            auto request = [[Target::Request alloc] init];
            
            request.maximumHandCount = settings.maximumHandCount;
            
            return OFX_VISION_AUTORELEASE(request);
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
