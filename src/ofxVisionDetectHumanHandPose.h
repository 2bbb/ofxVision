//
//  ofxVisionDetectHumanHandPose.h
//
//  Created by 2bit on 2023/06/30.
//

#pragma once

#include "ofxVisionBase.h"

namespace ofx {
    namespace Vision {
        struct DetectHumanHandPose : Base {
            using ResultType = std::vector<Observation::HandPose>;
            enum class QualityLevel {
                Fast,
                Balanced,
                Accurate,
            };
            
            void setup(std::size_t maximumHandCount = 2ul) {
                Base::setup();
                this->maximumHandCount = maximumHandCount;
            }
            ResultType detect(const ofBaseHasPixels &pix);
            ResultType detect(IOSurfaceRef surace);
            ResultType detect(CVPixelBufferRef pix);
            
            std::size_t getMaximumHandCount() const
            { return maximumHandCount; };
            void setMaximumHandCount(std::size_t count)
            { maximumHandCount = count; };

        protected:
            std::size_t maximumHandCount{2ul};
        };
    }; // namespace Vision
}; // namespace ofx

using ofxVisionDetectHumanHandPose = ofx::Vision::DetectHumanHandPose;
