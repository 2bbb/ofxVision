//
//  ofxVisionGenerateOpticalFlow.h
//
//  Created by 2bit on 2023/06/30.
//

#include "ofxVisionBase.h"

#define OFX_VISION_USE_TEXTURE 1

namespace ofx {
    namespace Vision {
        struct GenerateOpticalFlow : Base {
#if OFX_VISION_USE_TEXTURE
            using ResultType = ofTexture;
#else
            using ResultType = std::shared_ptr<ofFloatImage>;
#endif
            enum class ComputationAccuracyLevel {
                Low,
                Medium,
                High,
                VeryHigh
            };
            
            void setup(ComputationAccuracyLevel accuracyLevel = ComputationAccuracyLevel::Medium)
            {
                Base::setup();
                this->accuracyLevel = accuracyLevel;
            }
            ResultType detect(const ofBaseHasPixels &base,
                              const ofBaseHasPixels &pix);
            ResultType detect(const ofBaseHasPixels &base,
                              IOSurfaceRef surace);
            ResultType detect(const ofBaseHasPixels &base,
                              CVPixelBufferRef pix);
            
            ComputationAccuracyLevel getAccuracyLevel() const
            { return accuracyLevel; };
            void setAccuracyLevel(ComputationAccuracyLevel level)
            { accuracyLevel = level; };

        protected:
            ComputationAccuracyLevel accuracyLevel{ComputationAccuracyLevel::Medium};
        };
    }; // namespace Vision
}; // namespace ofx

using ofxVisionGenerateOpticalFlow = ofx::Vision::GenerateOpticalFlow;
