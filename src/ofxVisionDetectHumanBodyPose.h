//
//  ofxVisionDetectHumanBodyPose.h
//
//  Created by 2bit on 2023/06/30.
//

#pragma once

#include "ofxVisionBase.h"

#ifdef Request
#   undef Request
#endif

#if OFX_VISION_VERSION_CHECK(11, 0)

namespace ofx {
    namespace Vision {
        struct DetectHumanBodyPose : Base {
            using ResultType = std::vector<Observation::BodyPose>;
            using Request = OFX_VISION_OBJC_CLASS(VNDetectHumanBodyPoseRequest);

            enum class QualityLevel {
                Fast,
                Balanced,
                Accurate,
            };
            
            struct Settings {};
            
#include "details/detect_header.inl"
            
        protected:
#include "details/create_req_res_header.inl"
            Settings settings;
        };
    }; // namespace Vision
}; // namespace ofx

using ofxVisionDetectHumanBodyPose = ofx::Vision::DetectHumanBodyPose;

#else
#   warning ofxVisionDetectHumanBodyPose need macOS 11.0~
#endif
