//
//  ofxVisionDetectFaceLandmarks.h
//  example
//
//  Created by 2bit on 2023/07/01.
//

#include "ofxVisionBase.h"

#if OFX_VISION_VERSION_CHECK_X(10, 13)

#ifdef Request
#   undef Request
#endif

#if __OBJC__
#   define Request VNDetectFaceLandmarksRequest
#else
#   define Request void
#endif

namespace ofx {
    namespace Vision {
        struct DetectFaceLandmarks : Base {
            using ResultType = std::vector<Observation::Face>;
            
            struct Settings {
                
            };
            
#include "details/detect_header.inl"
            
        protected:
#include "details/create_req_res_header.inl"
            Settings settings;
        };
    }; // namespace Vision
}; // namespace ofx

using ofxVisionDetectFaceLandmarks = ofx::Vision::DetectFaceLandmarks;

#else
#   error ofxVisionDetectFaceLandmarks need macOS 10.13~
#endif
