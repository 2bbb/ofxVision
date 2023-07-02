//
//  ofxVisionAttentionBasedSaliencyImage.m
//  example
//
//  Created by 2bit on 2023/06/30.
//

#import "ofxVisionObjCUtility.h"

#import "ofxVisionAttentionBasedSaliencyImage.h"
#include "ofxVisionUtility.h"

#include "ofLog.h"

#if OFX_VISION_VERSION_CHECK_X(10, 15)

namespace ofx {
    namespace Vision {
        using Target = AttentionBasedSaliencyImage;
        
        Target::ResultType Target::detectWithCIImage(CIImage *image) {
            auto request = createRequest();
            NSError *err = nil;
            [handler performRequests:@[request]
                           onCIImage:image
                         orientation:kCGImagePropertyOrientationUp
                               error:&err];
            return createResult(request);
        }
        
#include "details/detect_impl.inl"
        
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

#endif // OFX_VISION_VERSION_CHECK(10, 15)
