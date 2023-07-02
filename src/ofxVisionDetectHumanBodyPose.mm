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
        
        Target::ResultType Target::detectWithCIImage(ofxVisionCIImage *image) {
            auto request = createRequest();
            NSError *err = nil;
            [handler performRequests:@[request]
                           onCIImage:(CIImage *)image
                         orientation:kCGImagePropertyOrientationUp
                               error:&err];
            if(err) {
                ofLogError("ofxVisionDetectBodyPose") << err.description.UTF8String;
                return {};
            }
            return createResult(request);
        }
        
        Target::Request *Target::createRequest() const
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
