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

        std::shared_ptr<ofFloatImage> pixelBufferToOfFloatImage(CVPixelBufferRef pixelBuffer) {
            std::shared_ptr<ofFloatImage> result = std::make_shared<ofFloatImage>();
            if(CVPixelBufferLockBaseAddress(pixelBuffer, 0) == kCVReturnSuccess) {
                auto base = (float *)CVPixelBufferGetBaseAddress(pixelBuffer);
                auto real_width = CVPixelBufferGetWidth(pixelBuffer);
                auto width = CVPixelBufferGetBytesPerRow(pixelBuffer) / sizeof(float);
                auto height = CVPixelBufferGetHeight(pixelBuffer);
                result->setFromPixels(base, width, height, OF_IMAGE_GRAYSCALE);
                if(width != real_width) result->crop(0, 0, real_width, height);
                CVPixelBufferUnlockBaseAddress(pixelBuffer, 0);
            }
            return result;
        }

        ofTexture pixelBufferToOfFloatTexture(CVPixelBufferRef pixelBuffer) {
            if(CVPixelBufferLockBaseAddress(pixelBuffer, 0) == kCVReturnSuccess) {
                auto base = (float *)CVPixelBufferGetBaseAddress(pixelBuffer);
                auto width = CVPixelBufferGetWidth(pixelBuffer);
                auto height = CVPixelBufferGetHeight(pixelBuffer);
                ofTexture texture;
                bool b = ofGetUsingArbTex();
//                texture.loadData(base, width, height, GL_RG32F);
                texture.allocate(width, height, GL_RG32F, true, GL_RG, GL_FLOAT);
                texture.loadData(base, width, height, GL_RG, GL_FLOAT);
//                FILE *fp = fopen("/Users/2bit/Downloads/test.bin", "wb");
//                fwrite(base, sizeof(float), width * height * 2, fp);
//                fclose(fp);
                
                CVPixelBufferUnlockBaseAddress(pixelBuffer, 0);
                return texture;
            }
            return {};
        }
        
        std::shared_ptr<ofFloatImage> pixelBuffer2fToOfFloatImage(CVPixelBufferRef pixelBuffer) {
            if(CVPixelBufferLockBaseAddress(pixelBuffer, 0) == kCVReturnSuccess) {
                auto base = (float *)CVPixelBufferGetBaseAddress(pixelBuffer);
                auto width = CVPixelBufferGetWidth(pixelBuffer);
                auto height = CVPixelBufferGetHeight(pixelBuffer);
                auto image = std::make_shared<ofFloatImage>();
                image->allocate(width, height, OF_IMAGE_COLOR_ALPHA);
                for(auto j = 0; j < height; ++j) {
                    for(auto i = 0; i < width; ++i) {
                        image->setColor(i, j, ofFloatColor(base[2 * (i + j * width)], base[2 * (i + j * width) + 1], 0.0f, 0.0f));
                    }
                }
                image->update();
                CVPixelBufferUnlockBaseAddress(pixelBuffer, 0);
                return image;
            }
            return {};
        }

        
        CGImageRef ofPixelsToCGImageRef(const ofPixels &pix) {
            auto width = pix.getWidth();
            auto height = pix.getHeight();
            auto numComponents = pix.getNumChannels();
            int bitsPerColorComponent = 8;
            auto rawImageDataLength = width * height * numComponents;
            
            CGColorSpaceRef colorSpaceRef;
            if(numComponents == 1) {
                colorSpaceRef = CGColorSpaceCreateDeviceGray();
            } else {
                colorSpaceRef = CGColorSpaceCreateDeviceRGB();
            }
            
            GLubyte *imageDataBuffer = (GLubyte *)(pix.getData());
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
            OFX_VISION_CFAUTORELEASE(imageRef);
            return imageRef;
        }
        
        ofxVisionCIImage *toCIImage(const ofPixels &pix)
        { return [CIImage imageWithCGImage:ofPixelsToCGImageRef(pix)]; };
        ofxVisionCIImage *toCIImage(const ofBaseHasPixels &pix)
        { return toCIImage(pix.getPixels()); };
        ofxVisionCIImage *toCIImage(CVPixelBufferRef pixelBuffer)
        { return [CIImage imageWithCVPixelBuffer:pixelBuffer]; };
        ofxVisionCIImage *toCIImage(IOSurfaceRef surface)
        { return [CIImage imageWithIOSurface:surface]; };
        
        Handler *createHandler() {
            return [[Handler alloc] init];
        }
    }; // namespace Vision
}; // namespace Vision
