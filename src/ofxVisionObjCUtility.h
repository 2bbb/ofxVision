//
//  ofxVisionObjC_ARCMacro.h
//
//  Created by 2bit on 2023/06/29.
//

#ifdef __OBJC__
#ifndef ofxVisionObjC_ARCMacro_h
#define ofxVisionObjC_ARCMacro_h

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

#include "ofxVisionUtility.h"

#include "ofBaseTypes.h"

#import <IOSurface/IOSurface.h>
#import <CoreVideo/CoreVideo.h>
#import <CoreImage/CoreImage.h>

namespace ofx {
    namespace Vision {
        namespace {
            CIImage *toCIImage(const ofBaseHasPixels &pix) {
                return [CIImage imageWithCGImage:ofBaseHasPixelsToCGImageRef(pix)];
            }
            CIImage *toCIImage(CVPixelBufferRef pixelBuffer) {
                return [CIImage imageWithCVPixelBuffer:pixelBuffer];
            }
            CIImage *toCIImage(IOSurfaceRef surface) {
                return [CIImage imageWithIOSurface:surface];
            }
        }
    };
};

#endif /* ofxVisionObjC_ARCMacro_h */
#endif /* __OBJC__ */
