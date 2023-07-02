//
//  ofxVisionGenerateOpticalFlow.h
//
//  Created by 2bit on 2023/06/30.
//

#include "ofxVisionBase.h"

#if OFX_VISION_VERSION_CHECK(11, 0)

namespace ofx {
    namespace Vision {
        struct GenerateOpticalFlow : Base<ofTexture> {
            using Request = OFX_VISION_OBJC_CLASS(VNGenerateOpticalFlowRequest);

            
            enum class ComputationAccuracyLevel {
                Low,
                Medium,
                High,
                VeryHigh
            };
            
            struct Settings {
                ComputationAccuracyLevel accuracyLevel{ComputationAccuracyLevel::Medium};
                void *baseImage{nullptr};
            };
            
            void setup(ComputationAccuracyLevel accuracyLevel = ComputationAccuracyLevel::Medium)
            {
                Base::setup();
                settings.accuracyLevel = accuracyLevel;
            }
            
            ComputationAccuracyLevel getAccuracyLevel() const
            { return settings.accuracyLevel; };
            void setAccuracyLevel(ComputationAccuracyLevel level)
            { settings.accuracyLevel = level; };
            
            void setBaseImage(ofxVisionCIImage *image);
            void setBaseImage(const ofPixels &pix)
            { setBaseImage(toCIImage(pix)); };
            void setBaseImage(const ofBaseHasPixels &pix)
            { setBaseImage(toCIImage(pix)); }
            void setBaseImage(IOSurfaceRef surface)
            { setBaseImage(toCIImage(surface)); }
            void setBaseImage(CVPixelBufferRef pix)
            { setBaseImage(toCIImage(pix)); };

        protected:
            Base::ResultType detectWithCIImage(ofxVisionCIImage *image) override;
            Request *createRequest() const;
            ResultType createResult(void *result) const;

            template <typename ... Detectors>
            friend struct MultipleDetector;

            void releaseImage();
            Settings settings;
        };
    }; // namespace Vision
}; // namespace ofx

using ofxVisionGenerateOpticalFlow = ofx::Vision::GenerateOpticalFlow;

#else
#   warning ofxVisionGenerateOpticalFlow need macOS 11.0~
#endif
