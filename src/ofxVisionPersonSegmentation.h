//
//  ofxVisionPersonSegmentation.h
//
//  Created by 2bit on 2023/06/29.
//

#include "ofImage.h"

#include <IOSurface/IOSurface.h>
#include <memory>
#include <vector>

namespace ofx {
    namespace Vision {
        struct PersonSegmentation {
            void setup();
            std::shared_ptr<ofImage> detect(IOSurfaceRef surace);
            ~PersonSegmentation();
        protected:
            void *handler_impl;
        };
    }; // namespace Vision
}; // namespace ofx
