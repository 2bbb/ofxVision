//
//  ofxVisionDetectFaceLandmarks.h
//  example
//
//  Created by 2bit on 2023/07/01.
//

#include "ofxVisionBase.h"

namespace ofx {
    namespace Vision {
        struct DetectFaceLandmarks : Base {
            using ResultType = std::vector<Observation::Face>;
            
            struct Settings {
                
            };
            
            ResultType detect(const ofBaseHasPixels &pix);
            ResultType detect(IOSurfaceRef surace);
            ResultType detect(CVPixelBufferRef pix);
        protected:
            Settings settings;
        };
    }; // namespace Vision
}; // namespace ofx

using ofxVisionDetectFaceLandmarks = ofx::Vision::DetectFaceLandmarks;
