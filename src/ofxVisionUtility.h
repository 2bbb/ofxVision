//
//  ofxVisionUtility.h
//
//  Created by 2bit on 2023/06/29.
//

#pragma once

#include "ofImage.h"

#import <CoreGraphics/CoreGraphics.h>

#include <memory>

namespace ofx {
    namespace Vision {
        std::shared_ptr<ofImage> pixelBufferToOfImage(CVPixelBufferRef pixelBuffer);
        CGImageRef ofBaseHasPixelsToCGImageRef(const ofBaseHasPixels &pix);
    }; // namespace Vision
}; // namespace Vision
