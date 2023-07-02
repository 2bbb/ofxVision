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
            Base::ResultType detectWithCIImage(ofxVisionCIImage *image) override;
            Request *createRequest() const;
            ResultType createResult(void *result) const;

            template <typename ... Detectors>
            friend struct MultipleDetector;

            Settings settings;
        };
    }; // namespace Vision
}; // namespace ofx

using ofxVisionDetectHumanBodyPose = ofx::Vision::DetectHumanBodyPose;

#else
#   warning ofxVisionDetectHumanBodyPose need macOS 11.0~
#endif
