//
//  ofxVisionDetectContours.h
//
//  Created by 2bit on 2023/07/01.
//

#pragma once

#include "ofxVisionBase.h"

#ifdef Request
#   undef Request
#endif

#if __OBJC__
#   define Request VNDetectContoursRequest
#else
#   define Request void
#endif

namespace ofx {
    namespace Vision {
        struct DetectContours : Base {
            using ResultType = Observation::Contours;
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

#include "details/detect_header.inl"
            
        protected:
#include "details/create_req_res_header.inl"
            Settings settings;
        };
    }; // namespace Vision
}; // namespace ofx

using ofxVisionDetectContours = ofx::Vision::DetectContours;
