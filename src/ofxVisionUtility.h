//
//  ofxVisionUtility.h
//
//  Created by 2bit on 2023/06/29.
//

#pragma once

#include "ofImage.h"
#include "ofVectorMath.h"
#include "ofRectangle.h"

#import <CoreGraphics/CoreGraphics.h>
#import <CoreVideo/CoreVideo.h>

#include <memory>

#include <Availability.h>

#ifdef MAC_OS_X_VERSION_MIN_REQUIRED
#   define OFX_VISION_MAC_OS_VERSION_MIN_REQUIRED MAC_OS_X_VERSION_MIN_REQUIRED
#elif MAC_OS_VERSION_MIN_REQUIRED
#   define OFX_VISION_MAC_OS_VERSION_MIN_REQUIRED MAC_OS_VERSION_MIN_REQUIRED
#else
#   error what?
#endif

#define OFX_VISION_VERSION_CHECK_X(MAJOR, MINOR) (MAC_OS_X_VERSION_##MAJOR##_##MINOR <= OFX_VISION_MAC_OS_VERSION_MIN_REQUIRED)
#define OFX_VISION_VERSION_CHECK(MAJOR, MINOR) (MAC_OS_VERSION_##MAJOR##_##MINOR <= OFX_VISION_MAC_OS_VERSION_MIN_REQUIRED)

namespace ofx {
    namespace Vision {
        std::shared_ptr<ofImage> pixelBufferToOfImage(CVPixelBufferRef pixelBuffer);
        std::shared_ptr<ofFloatImage> pixelBufferToOfFloatImage(CVPixelBufferRef pixelBuffer);
        ofTexture pixelBufferToOfFloatTexture(CVPixelBufferRef pixelBuffer);
        std::shared_ptr<ofFloatImage> pixelBuffer2fToOfFloatImage(CVPixelBufferRef pixelBuffer);
        
        CGImageRef ofBaseHasPixelsToCGImageRef(const ofBaseHasPixels &pix);
        namespace {
            glm::vec2 toOF(CGPoint p) { return { p.x, 1.0f - p.y }; }
            ofRectangle toOF(CGRect r) {
                return ofRectangle(r.origin.x,
                                   1.0f - r.origin.y - r.size.height,
                                   r.size.width,
                                   r.size.height);
            };
        };
    }; // namespace Vision
}; // namespace Vision
