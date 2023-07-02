//
//  ofxVisionPersonSegmentation.h
//
//  Created by 2bit on 2023/06/29.
//

#pragma once

#include "ofxVisionBase.h"

#if OFX_VISION_VERSION_CHECK(12, 0)

namespace ofx {
    namespace Vision {
        struct PersonSegmentation : Base<std::shared_ptr<ofImage>> {
            using Request = OFX_VISION_OBJC_CLASS(VNGeneratePersonSegmentationRequest);

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

using ofxVisionPersonSegmentation = ofx::Vision::PersonSegmentation;

#else
#   warning ofxVisionPersonSegmentation need macOS 12.0~
#endif