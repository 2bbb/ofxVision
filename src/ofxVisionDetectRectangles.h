//
//  ofxVisionDetectRectangles.h
//  example
//
//  Created by 2bit on 2023/07/01.
//

#include "ofxVisionBase.h"

#if OFX_VISION_VERSION_CHECK_X(10, 13)

#ifdef Request
#   undef Request
#endif

#if __OBJC__
#   define Request VNDetectRectanglesRequest
#else
#   define Request void
#endif

namespace ofx {
    namespace Vision {
        struct DetectRectangles : Base {
            using ResultType = std::vector<Observation::Rectangle>;
            struct Settings {
                float minimumAspectRatio{0.5f}; // 0.0 - 1.0
                float maximumAspectRatio{1.0f}; // 0.0 - 1.0
                float quadratureTolerance{30.0f}; // 0.0 - 45.0
                float minimumSize{0.2f}; // 0.0 - 1.0
                float minimumConfidence{0.0f}; // 0.0 - 1.0
                std::size_t maximumObservations{0ul};
            };
            
            void setMinimumAspectRatio(float v) {
                settings.minimumAspectRatio = ofClamp(v, 0.0f, 1.0f);
            }
            float getMinimumAspectRatio() const
            { return settings.minimumAspectRatio; };
            
            void setMaximumAspectRatio(float v) {
                settings.maximumAspectRatio = ofClamp(v, 0.0f, 1.0f);
            }
            float getMaximumAspectRatio() const
            { return settings.maximumAspectRatio; };
            
            void setQuadratureTolerance(float degree) {
                settings.maximumAspectRatio = ofClamp(degree, 0.0f, 45.0f);
            }
            float getQuadratureTolerance() const
            { return settings.quadratureTolerance; };

            void setMinimumSize(float v) {
                settings.minimumSize = ofClamp(v, 0.0f, 1.0f);
            }
            float getMinimumSize() const
            { return settings.minimumSize; };

            void setMinimumConfidence(float v) {
                settings.minimumConfidence = ofClamp(v, 0.0f, 1.0f);
            }
            float getMinimumConfidence() const
            { return settings.minimumConfidence; };

            void setMaximumObservations(std::size_t num) {
                settings.maximumObservations = num;
            }
            std::size_t getMaximumObservations() const
            { return settings.maximumObservations; };

#include "details/detect_header.inl"
            
        protected:
#include "details/create_req_res_header.inl"
            Settings settings;
        };
    }; // namespace Vision
}; // namespace ofx

using ofxVisionDetectRectangles = ofx::Vision::DetectRectangles;

#else
#   error ofxVisionDetectRectangles need macOS 10.13~
#endif
