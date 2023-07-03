//
//  ofxVisionDetectHumanHandPose.h
//
//  Created by 2bit on 2023/06/30.
//

#pragma once

#include "ofxVisionBase.h"

#if OFX_VISION_VERSION_CHECK(11, 0)

namespace ofx {
    namespace Vision {
        struct DetectHumanHandPose : Base<std::vector<Observation::HandPose>> {
            using Request = OFX_VISION_OBJC_CLASS(VNDetectHumanHandPoseRequest);

            enum class QualityLevel {
                Fast,
                Balanced,
                Accurate,
            };
            
            struct Settings {
                std::size_t maximumHandCount{2ul};
            };
            
            void setup(std::size_t maximumHandCount = 2ul) {
                Base::setup();
                settings.maximumHandCount = maximumHandCount;
            }
            
            std::size_t getMaximumHandCount() const
            { return settings.maximumHandCount; };
            void setMaximumHandCount(std::size_t count)
            { settings.maximumHandCount = count; };

        protected:
            Settings settings;
            
            // common
            std::string getName() const override
            { return "ofxVisionDetectHumanHandPose"; }
            BaseRequest *createRequest() const override;
            ResultType createResult(void *result) const override;

            template <typename ... Detectors>
            friend struct MultipleDetector;
        };
    }; // namespace Vision
}; // namespace ofx

using ofxVisionDetectHumanHandPose = ofx::Vision::DetectHumanHandPose;
#else
#   warning ofxVisionDetectHumanHandPose need macOS 11.0~
#endif
