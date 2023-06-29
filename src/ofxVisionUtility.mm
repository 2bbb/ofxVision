//
//  ofxVisionUtility.mm
//
//  Created by 2bit on 2023/06/29.
//

#import "ofxVisionObjCUtility.h"
#import "ofxVisionUtility.h"

#import <CoreVideo/CoreVideo.h>

namespace ofx {
    namespace Vision {
        std::shared_ptr<ofImage> pixelBufferToOfImage(CVPixelBufferRef pixelBuffer) {
            std::shared_ptr<ofImage> result = std::make_shared<ofImage>();
            if(CVPixelBufferLockBaseAddress(pixelBuffer, 0) == kCVReturnSuccess) {
                auto base = (uint8_t *)CVPixelBufferGetBaseAddress(pixelBuffer);
                auto real_width = CVPixelBufferGetWidth(pixelBuffer);
                auto width = CVPixelBufferGetBytesPerRow(pixelBuffer);
                auto height = CVPixelBufferGetHeight(pixelBuffer);
                result->setFromPixels(base, width, height, OF_IMAGE_GRAYSCALE);
                if(width != real_width) result->crop(0, 0, real_width, height);
                CVPixelBufferUnlockBaseAddress(pixelBuffer, 0);
            }
            return result;
        }

        CGImageRef ofBaseHasPixelsToCGImageRef(const ofBaseHasPixels &pix) {
            auto width = pix.getPixels().getWidth();
            auto height = pix.getPixels().getHeight();
            auto numComponents = pix.getPixels().getNumChannels();
            int bitsPerColorComponent = 8;
            auto rawImageDataLength = width * height * numComponents;
            
            CGColorSpaceRef colorSpaceRef;
            if(numComponents == 1) {
                colorSpaceRef = CGColorSpaceCreateDeviceGray();
            } else {
                colorSpaceRef = CGColorSpaceCreateDeviceRGB();
            }
            
            GLubyte *imageDataBuffer = (GLubyte *)(pix.getPixels().getData());
            CGDataProviderRef dataProviderRef = CGDataProviderCreateWithData(nullptr, imageDataBuffer, rawImageDataLength, nil);
            
            CGBitmapInfo bitmapInfo = kCGBitmapByteOrderDefault;
            CGColorRenderingIntent renderingIntent = kCGRenderingIntentDefault;
            BOOL interpolateAndSmoothPixels = NO;
            CGImageRef imageRef = CGImageCreate(width, height,
                                                bitsPerColorComponent,
                                                bitsPerColorComponent * numComponents,
                                                width * numComponents,
                                                colorSpaceRef,
                                                bitmapInfo,
                                                dataProviderRef,
                                                NULL,
                                                interpolateAndSmoothPixels,
                                                renderingIntent);
            
            CGDataProviderRelease(dataProviderRef);
            return imageRef;
        }
    }; // namespace Vision
}; // namespace Vision
