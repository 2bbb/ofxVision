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

#if __OBJC__
#   import <Vision/Vision.h>
#   define OFX_VISION_OBJC_CLASS(Klass) Klass
#else
#   define OFX_VISION_OBJC_CLASS(Klass) void
#endif

namespace ofx {
    namespace Vision {
        void *objc_retain(void *obj);
        void *objc_release(void *obj);

        std::shared_ptr<ofImage> pixelBufferToOfImage(CVPixelBufferRef pixelBuffer);
        std::shared_ptr<ofFloatImage> pixelBufferToOfFloatImage(CVPixelBufferRef pixelBuffer);
        ofTexture pixelBufferToOfFloatTexture(CVPixelBufferRef pixelBuffer);
        std::shared_ptr<ofFloatImage> pixelBuffer2fToOfFloatImage(CVPixelBufferRef pixelBuffer);
        
        CGImageRef ofPixelsToCGImageRef(const ofPixels &pix);
        inline CGImageRef ofBaseHasPixelsToCGImageRef(const ofBaseHasPixels &pix) {
            return ofPixelsToCGImageRef(pix.getPixels());
        }
        namespace {
            glm::vec2 toOF(CGPoint p) { return { p.x, 1.0f - p.y }; }
            ofRectangle toOF(CGRect r) {
                return ofRectangle(r.origin.x,
                                   1.0f - r.origin.y - r.size.height,
                                   r.size.width,
                                   r.size.height);
            };
        };
        
        using ofxVisionCIImage = void;
        
        using Handler = OFX_VISION_OBJC_CLASS(VNSequenceRequestHandler);
        using BaseRequest = OFX_VISION_OBJC_CLASS(VNRequest);
        using BaseObservation = OFX_VISION_OBJC_CLASS(VNObservation);
        
        ofxVisionCIImage *toCIImage(const ofPixels &pix);
        ofxVisionCIImage *toCIImage(const ofBaseHasPixels &pix);
        ofxVisionCIImage *toCIImage(CVPixelBufferRef pixelBuffer);
        ofxVisionCIImage *toCIImage(IOSurfaceRef surface);
        
        Handler *createHandler();
    }; // namespace Vision
}; // namespace Vision
