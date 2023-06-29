//
//  ofxVisionUtility.h
//
//  Created by 2bit on 2023/06/29.
//

#pragma once

#include "ofImage.h"

#import <CoreImage/CoreImage.h>

#include <memory>

#if __has_feature(objc_arc)
#   define OFX_VISION_BRIDGE_TRANSFER __bridge_transfer
#   define OFX_VISION_BRIDGE_RETAINED __bridge_retained
#   define OFX_VISION_RETAIN(obj) obj
#   define OFX_VISION_RELEASE(obj) obj
#   define OFX_VISION_AUTORELEASE(obj) obj
#else
#   define OFX_VISION_BRIDGE_TRANSFER
#   define OFX_VISION_BRIDGE_RETAINED
#   define OFX_VISION_RETAIN(obj) [obj retain]
#   define OFX_VISION_RELEASE(obj) [obj release]
#   define OFX_VISION_AUTORELEASE(obj) [obj autorelease]
#endif

std::shared_ptr<ofImage> pixelBufferToOfImage(CVPixelBufferRef pixelBuffer);
CGImageRef ofBaseHasPixelsToCGImageRef(const ofBaseHasPixels &pix);
