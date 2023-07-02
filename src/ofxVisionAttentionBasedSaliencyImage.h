//
//  ofxVisionAttentionBasedSaliencyImage.h
//
//  Created by 2bit on 2023/06/30.
//

#include "ofxVisionBase.h"

#if OFX_VISION_VERSION_CHECK_X(10, 15)

namespace ofx {
    namespace Vision {
        struct AttentionBasedSaliencyImage : Base<Observation::SaliencyImage> {
            using Request = OFX_VISION_OBJC_CLASS(VNGenerateAttentionBasedSaliencyImageRequest);

            struct Settings {};
            
        protected:
            Base::ResultType detectWithCIImage(ofxVisionCIImage *image) override;
            Request *createRequest() const;
            ResultType createResult(void *result) const;

            template <typename ... Detectors>
            friend struct MultipleDetector;

            Settings settings;
        };
    }; // namespace Vision
}; // namespace ofx

using ofxVisionAttentionBasedSaliencyImage = ofx::Vision::AttentionBasedSaliencyImage;

#else
#   warning ofxVisionAttentionBasedSaliencyImage need macOS 10.15~
#endif
