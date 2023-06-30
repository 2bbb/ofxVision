//
//  ofxVisionDetectHumanBodyPose.h
//
//  Created by 2bit on 2023/06/30.
//

#pragma once

#include "ofxVisionBase.h"

namespace ofx {
    namespace Vision {
        struct DetectHumanBodyPose : Base {
            using ResultType = std::vector<Observation::BodyPose>;
            enum class QualityLevel {
                Fast,
                Balanced,
                Accurate,
            };
            
            ResultType detect(const ofBaseHasPixels &pix);
            ResultType detect(IOSurfaceRef surace);
            ResultType detect(CVPixelBufferRef pix);
            
        protected:
            
        };
    }; // namespace Vision
}; // namespace ofx

using ofxVisionDetectHumanBodyPose = ofx::Vision::DetectHumanBodyPose;
