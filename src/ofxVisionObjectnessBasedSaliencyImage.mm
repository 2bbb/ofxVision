//
//  ofxVisionObjectnessBasedSaliencyImage.mm
//
//  Created by 2bit on 2023/06/30.
//

#import "ofxVisionObjectnessBasedSaliencyImage.h"

#if OFX_VISION_VERSION_CHECK_X(10, 15)

#import "ofxVisionObjCUtility.h"

#include "ofLog.h"

namespace ofx {
    namespace Vision {
        using Target = ObjectnessBasedSaliencyImage;
        
        Target::ResultType Target::detectWithCIImage(ofxVisionCIImage *image) {
            auto request = createRequest();
            
            NSError *err = nil;
            [handler performRequests:@[request]
                           onCIImage:(CIImage *)image
                         orientation:kCGImagePropertyOrientationUp
                               error:&err];
            Target::ResultType result;
            if(err) {
                ofLogError("ObjectnessBasedSaliencyImage") << err.description.UTF8String;
                return result;
            }
            return createResult(request);
        }
        
        Target::Request *Target::createRequest() const {
            auto request = OFX_VISION_AUTORELEASE([[Target::Request alloc] init]);
            return request;
        }
        
        Target::ResultType Target::createResult(void *req) const {
            Target::Request *request = (Target::Request *)req;
            return toOF(request.results.firstObject);
        }
    }; // namespace Vision
}; // namespace ofx

#endif
