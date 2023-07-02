//
//  ofxVisionDetectFaceLandmarks.h
//  example
//
//  Created by 2bit on 2023/07/01.
//

#include "ofxVisionBase.h"

#if OFX_VISION_VERSION_CHECK_X(10, 13)

namespace ofx {
    namespace Vision {
        struct DetectFaceLandmarks : Base<std::vector<Observation::Face>> {
            using Request = OFX_VISION_OBJC_CLASS(VNDetectFaceLandmarksRequest);

            struct Settings {
                
            };
            
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

using ofxVisionDetectFaceLandmarks = ofx::Vision::DetectFaceLandmarks;

#else
#   error ofxVisionDetectFaceLandmarks need macOS 10.13~
#endif
