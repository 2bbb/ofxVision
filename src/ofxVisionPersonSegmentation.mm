//
//  ofxVisionPersonSegmentation.mm
//
//  Created by 2bit on 2023/06/29.
//

#include "ofxVisionPersonSegmentation.h"
#include "ofxVisionUtility.h"

#include "ofLog.h"

#import <CoreImage/CoreImage.h>
#import <Vision/Vision.h>

namespace ofx {
    namespace Vision {
        VNGeneratePersonSegmentationRequest *createRequest() {
            VNGeneratePersonSegmentationRequest *request = [[VNGeneratePersonSegmentationRequest alloc] init];
            request.qualityLevel = VNGeneratePersonSegmentationRequestQualityLevelFast;
            request.outputPixelFormat = kCVPixelFormatType_OneComponent8;
            OFX_VISION_AUTORELEASE(request);
            return request;
        }
        
        std::shared_ptr<ofImage> detectWithCIImage(void *handler_impl, CIImage *image) {
            VNSequenceRequestHandler *handler = (VNSequenceRequestHandler *)handler_impl;
            VNGeneratePersonSegmentationRequest *request = createRequest();
            
            NSError *err = nil;
            [handler performRequests:@[request]
                           onCIImage:image
                         orientation:kCGImagePropertyOrientationUp
                               error:&err];
            if(err) {
                ofLogError("ofxVisionPersonSegmentation") << err.description.UTF8String;
                return std::make_shared<ofImage>();
            }
            CVPixelBufferRef pixelBuffer = request.results.firstObject.pixelBuffer;
            return pixelBufferToOfImage(pixelBuffer);
        }
        
        void PersonSegmentation::setup() {
            VNSequenceRequestHandler *handler = [[VNSequenceRequestHandler alloc] init];
            handler_impl = (OFX_VISION_BRIDGE_RETAINED void *)handler;
        }
        
        std::shared_ptr<ofImage> PersonSegmentation::detect(const ofBaseHasPixels &pix) {
            CGImageRef cgImage = ofBaseHasPixelsToCGImageRef(pix);
            return detectWithCIImage(handler_impl, [CIImage imageWithCGImage:cgImage]);
        }
        
        std::shared_ptr<ofImage> PersonSegmentation::detect(IOSurfaceRef surface) {
            return detectWithCIImage(handler_impl, [CIImage imageWithIOSurface:surface]);
        }
        
        std::shared_ptr<ofImage> PersonSegmentation::detect(CVPixelBufferRef pix) {
            return detectWithCIImage(handler_impl, [CIImage imageWithCVPixelBuffer:pix]);
        }
        
        PersonSegmentation::~PersonSegmentation() {
            VNSequenceRequestHandler *handler = (OFX_VISION_BRIDGE_TRANSFER VNSequenceRequestHandler *)handler_impl;
            OFX_VISION_RELEASE(handler);
        }
    }; // namespace Vision
}; // namespace ofx
