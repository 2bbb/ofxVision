//
//  ofxVisionObjectnessBasedSaliencyImage.h
//
//  Created by 2bit on 2023/06/30.
//

#include "ofxVisionBase.h"

#include "ofImage.h"
#include "ofVectorMath.h"

namespace ofx {
    namespace Vision {
        struct ObjectnessBasedSaliencyImage : Base {
            using ResultType = Observation::SaliencyImage;
            
            ResultType detect(const ofBaseHasPixels &pix);
            ResultType detect(IOSurfaceRef surace);
            ResultType detect(CVPixelBufferRef pix);
            
        protected:
            
        };
    }; // namespace Vision
}; // namespace ofx

using ofxVisionObjectnessBasedSaliencyImage = ofx::Vision::ObjectnessBasedSaliencyImage;
