//
//  ofxVisionObjectnessBasedSaliencyImage.h
//
//  Created by 2bit on 2023/06/30.
//

#include "ofxVisionBase.h"

#include "ofImage.h"
#include "ofVectorMath.h"

#ifdef Request
#   undef Request
#endif

#if __OBJC__
#   define Request VNGenerateObjectnessBasedSaliencyImageRequest
#else
#   define Request void
#endif

namespace ofx {
    namespace Vision {
        struct ObjectnessBasedSaliencyImage : Base {
            using ResultType = Observation::SaliencyImage;
            
            struct Settings {};
            
            ResultType detect(const ofBaseHasPixels &pix);
            ResultType detect(IOSurfaceRef surace);
            ResultType detect(CVPixelBufferRef pix);
            
        protected:
            Request *createRequest() const;
            ResultType createResult(Request *result) const;
            Settings settings;
        };
    }; // namespace Vision
}; // namespace ofx

using ofxVisionObjectnessBasedSaliencyImage = ofx::Vision::ObjectnessBasedSaliencyImage;
