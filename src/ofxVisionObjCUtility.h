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
#include "ofxVisionObservation.h"

#include "ofBaseTypes.h"

#import <IOSurface/IOSurface.h>
#import <CoreVideo/CoreVideo.h>
#import <CoreImage/CoreImage.h>
#import <Vision/Vision.h>


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
            
            Observation::SaliencyImage toOF(VNSaliencyImageObservation *saliencyResult) {
                Observation::SaliencyImage result;
                CVPixelBufferRef pixelBuffer = saliencyResult.pixelBuffer;
                result.pixels = pixelBufferToOfFloatImage(pixelBuffer);
                auto &salients = result.salientObjects;
                for(VNRectangleObservation *rect in saliencyResult.salientObjects) {
                    salients.emplace_back();
                    auto &s = salients.back();
                    s.uuid = rect.uuid.UUIDString.UTF8String;
                    s.segmentationMask.pixels = pixelBufferToOfFloatImage(rect.globalSegmentationMask.pixelBuffer);
                    if(rect.globalSegmentationMask.featureName) s.segmentationMask.featureName =  rect.globalSegmentationMask.featureName.UTF8String;
                    s.topLeft = toOF(rect.topLeft);
                    s.topRight = toOF(rect.topRight);
                    s.bottomLeft = toOF(rect.bottomLeft);
                    s.bottomRight = toOF(rect.bottomRight);
                    s.boundingBox = toOF(rect.boundingBox);
                }
                return result;
            }
        }
    };
};

#endif /* ofxVisionObjC_ARCMacro_h */
#endif /* __OBJC__ */
