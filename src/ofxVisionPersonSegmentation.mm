//
//  ofxVisionPersonSegmentation.mm
//
//  Created by 2bit on 2023/06/29.
//

#include "ofxVisionPersonSegmentation.h"

#include "ofLog.h"

#import <CoreImage/CoreImage.h>
#import <Vision/Vision.h>

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

namespace ofx {
    namespace Vision {
        void PersonSegmentation::setup() {
            VNSequenceRequestHandler *handler = [[VNSequenceRequestHandler alloc] init];
            handler_impl = (OFX_VISION_BRIDGE_RETAINED void *)handler;
        }
        std::shared_ptr<ofImage> PersonSegmentation::detect(IOSurfaceRef surface) {
            VNSequenceRequestHandler *handler = (VNSequenceRequestHandler *)handler_impl;
            
            VNGeneratePersonSegmentationRequest *request = [[VNGeneratePersonSegmentationRequest alloc] init];
            request.qualityLevel = VNGeneratePersonSegmentationRequestQualityLevelFast;
            request.outputPixelFormat = kCVPixelFormatType_OneComponent8;
            OFX_VISION_AUTORELEASE(request);
            
            NSError *err = nil;
            CIImage *image = [CIImage imageWithIOSurface:surface];
            [handler performRequests:@[request]
                           onCIImage:image
                         orientation:kCGImagePropertyOrientationUp
                               error:&err];
            if(err) {
                ofLogError("ofxVisionPersonSegmentation") << err.description.UTF8String;
                return nullptr;
            }
            CVPixelBufferRef pixelBuffer = request.results.firstObject.pixelBuffer;
            std::shared_ptr<ofImage> result = std::make_shared<ofImage>();
            if(CVPixelBufferLockBaseAddress(pixelBuffer, 0) == kCVReturnSuccess) {
                auto base = (uint8_t *)CVPixelBufferGetBaseAddress(pixelBuffer);
                auto width = CVPixelBufferGetWidth(pixelBuffer);
                auto height = CVPixelBufferGetHeight(pixelBuffer);
                result->setFromPixels(base, width, height, OF_IMAGE_GRAYSCALE);
                CVPixelBufferUnlockBaseAddress(pixelBuffer, 0);
            }
            return result;
        }
        
        PersonSegmentation::~PersonSegmentation() {
            VNSequenceRequestHandler *handler = (OFX_VISION_BRIDGE_TRANSFER VNSequenceRequestHandler *)handler_impl;
            OFX_VISION_RELEASE(handler);
        }
    }; // namespace Vision
}; // namespace ofx
