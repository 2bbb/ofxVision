//
//  ofxVisionObjectnessBasedSaliencyImage.h
//
//  Created by 2bit on 2023/06/30.
//

#include "ofxVisionBase.h"

#if OFX_VISION_VERSION_CHECK_X(10, 15)

namespace ofx {
    namespace Vision {
        struct ObjectnessBasedSaliencyImage : Base<Observation::SaliencyImage> {
            using Request = OFX_VISION_OBJC_CLASS(VNGenerateObjectnessBasedSaliencyImageRequest);

            struct Settings {};
            
        protected:
            Settings settings;
            
            // common
            Request *createRequest() const;
            ResultType createResult(void *result) const override;
            ResultType detectWithCIImage(ofxVisionCIImage *image) override;

            template <typename ... Detectors>
            friend struct MultipleDetector;
        };
    }; // namespace Vision
}; // namespace ofx

using ofxVisionObjectnessBasedSaliencyImage = ofx::Vision::ObjectnessBasedSaliencyImage;

#else
#   warning ofxVisionObjectnessBasedSaliencyImage need macOS 10.15~
#endif
