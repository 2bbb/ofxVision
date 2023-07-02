//
//  ofxVisionDetectContours.h
//
//  Created by 2bit on 2023/07/01.
//

#pragma once

#include "ofxVisionBase.h"

#if OFX_VISION_VERSION_CHECK(11, 0)

namespace ofx {
    namespace Vision {
        struct DetectContours : Base<Observation::Contours> {
            using Request = OFX_VISION_OBJC_CLASS(VNDetectContoursRequest);
            
            struct Settings {
                float contrastAdjustment{2.0f}; // 0.0 - 3.0
                float contrastPivot{0.5}; // 0.0 - 1.0, nil (automatically)
                bool detectsDarkOnLight{true};
            };
            
            void setContrastAdjustment(float adjustment) {
                settings.contrastAdjustment = ofClamp(adjustment, 0.0f, 3.0f);
            }
            float getContrastAdjustment() const
            { return settings.contrastAdjustment; };
            
            void setContrastPivot(float pivot) {
                settings.contrastPivot = pivot < 0.0f ? -1.0f : ofClamp(pivot, 0.0f, 1.0f);
            }
            float getContrastPivot() const
            { return settings.contrastPivot; }
            
            void setDetectsDarkOnLight(bool detectsDarkOnLight)
            { settings.detectsDarkOnLight = detectsDarkOnLight; }
            bool getDetectsDarkOnLight() const
            { return settings.detectsDarkOnLight; }
            
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

using ofxVisionDetectContours = ofx::Vision::DetectContours;

#else
#   warning ofxVisionDetectContours need macOS 11.0~
#endif
