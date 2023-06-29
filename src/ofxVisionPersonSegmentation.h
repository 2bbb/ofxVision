//
//  ofxVisionPersonSegmentation.h
//
//  Created by 2bit on 2023/06/29.
//

#pragma once

#include "ofImage.h"

#include <IOSurface/IOSurface.h>
#include <CoreVideo/CVPixelBuffer.h>

#include <memory>
#include <vector>

namespace ofx {
    namespace Vision {
        struct PersonSegmentation {
            void setup();
            std::shared_ptr<ofImage> detect(const ofBaseHasPixels &pix);
            std::shared_ptr<ofImage> detect(IOSurfaceRef surace);
            std::shared_ptr<ofImage> detect(CVPixelBufferRef pix);
            ~PersonSegmentation();
        protected:
            void *handler_impl;
        };
    }; // namespace Vision
}; // namespace ofx
