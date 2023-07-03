//
//  ofxVisionDetectHumanBodyPose.mm
//
//  Created by 2bit on 2023/06/30.
//

#import "ofxVisionObjCUtility.h"

#import "ofxVisionDetectHumanBodyPose.h"
#include "ofxVisionUtility.h"

#include "ofLog.h"

#if OFX_VISION_VERSION_CHECK(11, 0)

namespace ofx {
    namespace Vision {
        using Target = DetectHumanBodyPose;
        
        BaseRequest *Target::createRequest() const
        {
            auto request = OFX_VISION_AUTORELEASE([[Target::Request alloc] init]);
            
            return request;
        }
        
        Target::ResultType Target::createResult(void *req) const {
            Target::Request *request = (Target::Request *)req;
            Target::ResultType results;
            for(VNHumanBodyPoseObservation *body in request.results) {
                results.push_back(toOF(body));
            }
            return results;
        }
    }; // namespace Vision
}; // namespace ofx

#endif
