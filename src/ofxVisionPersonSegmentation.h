//
//  ofxVisionPersonSegmentation.h
//
//  Created by 2bit on 2023/06/29.
//

#pragma once

#include "ofxVisionBase.h"

#ifdef Request
#   undef Request
#endif

#if __OBJC__
#   define Request VNGeneratePersonSegmentationRequest
#else
#   define Request void
#endif

namespace ofx {
    namespace Vision {
        struct PersonSegmentation : Base {
            using ResultType = std::shared_ptr<ofImage>;
            enum class QualityLevel {
                Fast,
                Balanced,
                Accurate,
            };
            
            struct Settings {
                QualityLevel qualityLevel{QualityLevel::Fast};
            };
            
            void setup(QualityLevel qualityLevel = QualityLevel::Fast) {
                Base::setup();
                settings.qualityLevel = qualityLevel;
            }
            
            QualityLevel getQualityLevel() const
            { return settings.qualityLevel; };
            void setQualityLevel(QualityLevel level)
            { settings.qualityLevel = level; };

#include "details/detect_header.inl"
            
        protected:
#include "details/create_req_res_header.inl"
            Settings settings;
        };
    }; // namespace Vision
}; // namespace ofx

using ofxVisionPersonSegmentation = ofx::Vision::PersonSegmentation;
