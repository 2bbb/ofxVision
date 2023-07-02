//
//  ofxVisionObjectnessBasedSaliencyImage.h
//
//  Created by 2bit on 2023/06/30.
//

#include "ofxVisionBase.h"

#include "ofImage.h"
#include "ofVectorMath.h"

#if OFX_VISION_VERSION_CHECK_X(10, 15)

#ifdef Request
#   undef Request
#endif

namespace ofx {
    namespace Vision {
        struct ObjectnessBasedSaliencyImage : Base {
            using ResultType = Observation::SaliencyImage;
            using Request = OFX_VISION_OBJC_CLASS(VNGenerateObjectnessBasedSaliencyImageRequest);

            struct Settings {};
            
#include "details/detect_header.inl"

        protected:
#include "details/create_req_res_header.inl"
            
            Settings settings;
        };
    }; // namespace Vision
}; // namespace ofx

using ofxVisionObjectnessBasedSaliencyImage = ofx::Vision::ObjectnessBasedSaliencyImage;

#else
#   warning ofxVisionObjectnessBasedSaliencyImage need macOS 10.15~
#endif
