//
//  ofxVisionPersonSegmentation.h
//
//  Created by 2bit on 2023/06/29.
//

#pragma once

#include "ofxVisionBase.h"

namespace ofx {
    namespace Vision {
        struct PersonSegmentation : Base {
            using ResultType = std::shared_ptr<ofImage>;
            enum class QualityLevel {
                Fast,
                Balanced,
                Accurate,
            };
            
            void setup(QualityLevel qualityLevel = QualityLevel::Fast) {
                Base::setup();
                this->qualityLevel = qualityLevel;
            }
            ResultType detect(const ofBaseHasPixels &pix);
            ResultType detect(IOSurfaceRef surace);
            ResultType detect(CVPixelBufferRef pix);
            
            QualityLevel getQualityLevel() const
            { return qualityLevel; };
            void setQualityLevel(QualityLevel level)
            { qualityLevel = level; };

        protected:
            QualityLevel qualityLevel{QualityLevel::Fast};
        };
    }; // namespace Vision
}; // namespace ofx

using ofxVisionPersonSegmentation = ofx::Vision::PersonSegmentation;
