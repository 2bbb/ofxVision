//
//  ofxVisionDetectFaceLandmarks.h
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
            Settings settings;
            
            // common
            std::string getName() const override
            { return "ofxVisionDetectFaceLandmarks"; }
            BaseRequest *createRequest() const override;
            ResultType createResult(void *result) const override;

            template <typename ... Detectors>
            friend struct MultipleDetector;
        };
    }; // namespace Vision
}; // namespace ofx

using ofxVisionDetectFaceLandmarks = ofx::Vision::DetectFaceLandmarks;

#else
#   warning ofxVisionDetectFaceLandmarks need macOS 10.13~
#endif
