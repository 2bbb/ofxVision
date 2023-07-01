//
//  ofxVisionGenerateOpticalFlow.h
//
//  Created by 2bit on 2023/06/30.
//

#include "ofxVisionBase.h"

#ifdef Request
#   undef Request
#endif

#if OFX_VISION_VERSION_CHECK(11, 0)

#if __OBJC__
#   define Request VNGenerateOpticalFlowRequest
#else
#   define Request void
#endif

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
            
            void setBaseImage(const ofBaseHasPixels &pix);
            void setBaseImage(IOSurfaceRef surface);
            void setBaseImage(CVPixelBufferRef pix);

#include "details/detect_header.inl"
            
        protected:
#include "details/create_req_res_header.inl"
            void releaseImage();
            Settings settings;
        };
    }; // namespace Vision
}; // namespace ofx

using ofxVisionGenerateOpticalFlow = ofx::Vision::GenerateOpticalFlow;

#else
#   warning ofxVisionGenerateOpticalFlow need macOS 11.0~
#endif
