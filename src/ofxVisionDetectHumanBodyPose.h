//
//  ofxVisionDetectHumanBodyPose.h
//
//  Created by 2bit on 2023/06/30.
//

#pragma once

#include "ofxVisionBase.h"

#if OFX_VISION_VERSION_CHECK(11, 0)

namespace ofx {
    namespace Vision {
        struct DetectHumanBodyPose : Base<std::vector<Observation::BodyPose>> {
            using Request = OFX_VISION_OBJC_CLASS(VNDetectHumanBodyPoseRequest);

            enum class QualityLevel {
                Fast,
                Balanced,
                Accurate,
            };
            
            struct Settings {};
            
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

using ofxVisionDetectHumanBodyPose = ofx::Vision::DetectHumanBodyPose;

#else
#   warning ofxVisionDetectHumanBodyPose need macOS 11.0~
#endif
