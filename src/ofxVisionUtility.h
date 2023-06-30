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

namespace ofx {
    namespace Vision {
        std::shared_ptr<ofImage> pixelBufferToOfImage(CVPixelBufferRef pixelBuffer);
        std::shared_ptr<ofFloatImage> pixelBufferToOfFloatImage(CVPixelBufferRef pixelBuffer);
        ofTexture pixelBufferToOfFloatTexture(CVPixelBufferRef pixelBuffer);
        std::shared_ptr<ofFloatImage> pixelBuffer2fToOfFloatImage(CVPixelBufferRef pixelBuffer);
        
        CGImageRef ofBaseHasPixelsToCGImageRef(const ofBaseHasPixels &pix);
        namespace {
            glm::vec2 toOF(CGPoint p) { return { p.x, p.y }; }
            ofRectangle toOF(CGRect r) {
                return ofRectangle(r.origin.x, r.origin.y, r.size.width, r.size.height);
            };
        };
    }; // namespace Vision
}; // namespace Vision
