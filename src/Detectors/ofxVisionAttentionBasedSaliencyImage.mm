//
//  ofxVisionAttentionBasedSaliencyImage.m
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
        
        BaseRequest *Target::createRequest() const {
            auto request = [[Target::Request alloc] init];
            
            return OFX_VISION_AUTORELEASE(request);
        }
        
        Target::ResultType Target::createResult(void *req) const {
            Target::Request *request = (Target::Request *)req;
            return toOF(request.results.firstObject);
        }
    }; // namespace Vision
}; // namespace ofx

#endif // OFX_VISION_VERSION_CHECK(10, 15)
