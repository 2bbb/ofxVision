//
//  ofxVisionAttentionBasedSaliencyImage.h
//
//  Created by 2bit on 2023/06/30.
//

#include "ofxVisionBase.h"

#if OFX_VISION_VERSION_CHECK_X(10, 15)

#include "ofImage.h"
#include "ofVectorMath.h"

#ifdef Request
#   undef Request
#endif

#if __OBJC__
#   define Request VNGenerateAttentionBasedSaliencyImageRequest
#else
#   define Request void
#endif

namespace ofx {
    namespace Vision {
        struct AttentionBasedSaliencyImage : Base {
            using ResultType = Observation::SaliencyImage;
            
            struct Settings {};
            
#include "details/detect_header.inl"
            
        protected:
#include "details/create_req_res_header.inl"
            
            Settings settings;
        };
    }; // namespace Vision
}; // namespace ofx

using ofxVisionAttentionBasedSaliencyImage = ofx::Vision::AttentionBasedSaliencyImage;

#else
#   warning ofxVisionAttentionBasedSaliencyImage need macOS 10.15~
#endif
