//
//  ofxVisionDetectHumanHandPose.h
//
//  Created by 2bit on 2023/06/30.
//

#pragma once

#include "ofxVisionBase.h"

#ifdef Request
#   undef Request
#endif

#if OFX_VISION_VERSION_CHECK(11, 0)

#if __OBJC__
#   define Request VNDetectHumanHandPoseRequest
#else
#   define Request void
#endif

namespace ofx {
    namespace Vision {
        struct DetectHumanHandPose : Base {
            using ResultType = std::vector<Observation::HandPose>;
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

#include "details/detect_header.inl"
            
        protected:
#include "details/create_req_res_header.inl"
            Settings settings;
        };
    }; // namespace Vision
}; // namespace ofx

using ofxVisionDetectHumanHandPose = ofx::Vision::DetectHumanHandPose;
#else
#   warning ofxVisionDetectHumanHandPose need macOS 11.0~
#endif
