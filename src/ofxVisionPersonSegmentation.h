//
//  ofxVisionPersonSegmentation.h
//
//  Created by 2bit on 2023/06/29.
//

#pragma once

#include "ofImage.h"

#include <IOSurface/IOSurface.h>
#include <CoreVideo/CVPixelBuffer.h>

#include <memory>
#include <vector>

namespace ofx {
    namespace Vision {
        struct PersonSegmentation {
            enum class QualityLevel {
                Fast,
                Balanced,
                Accurate, // TODO: fix result will be broken when accurate
            };
            
            void setup(QualityLevel qualityLevel = QualityLevel::Fast);
            std::shared_ptr<ofImage> detect(const ofBaseHasPixels &pix);
            std::shared_ptr<ofImage> detect(IOSurfaceRef surace);
            std::shared_ptr<ofImage> detect(CVPixelBufferRef pix);
            ~PersonSegmentation();
            
            QualityLevel getQualityLevel() const
            { return qualityLevel; };
            void setQualityLevel(QualityLevel level)
            { qualityLevel = level; };

        protected:
            QualityLevel qualityLevel{QualityLevel::Fast};
            void *handler_impl;
        };
    }; // namespace Vision
}; // namespace ofx
