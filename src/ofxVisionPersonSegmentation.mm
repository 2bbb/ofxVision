//
//  ofxVisionPersonSegmentation.mm
//
//  Created by 2bit on 2023/06/29.
//

#include "ofxVisionPersonSegmentation.h"
#include "ofxVisionUtility.h"

#import "ofxVisionObjC_ARCMacro.h"

#include "ofLog.h"

#import <CoreImage/CoreImage.h>
#import <Vision/Vision.h>

namespace ofx {
    namespace Vision {
        VNGeneratePersonSegmentationRequestQualityLevel conv(PersonSegmentation::QualityLevel level) {
            switch(level) {
                case PersonSegmentation::QualityLevel::Fast:
                    return VNGeneratePersonSegmentationRequestQualityLevelFast;
                case PersonSegmentation::QualityLevel::Balanced:
                    return VNGeneratePersonSegmentationRequestQualityLevelBalanced;
                case PersonSegmentation::QualityLevel::Accurate:
                    return VNGeneratePersonSegmentationRequestQualityLevelAccurate;
            }
        }
        
        VNGeneratePersonSegmentationRequest *createRequest(PersonSegmentation::QualityLevel qualityLevel) {
            VNGeneratePersonSegmentationRequest *request = [[VNGeneratePersonSegmentationRequest alloc] init];
            request.qualityLevel = conv(qualityLevel);
            request.outputPixelFormat = kCVPixelFormatType_OneComponent8;
            OFX_VISION_AUTORELEASE(request);
            return request;
        }
        
        std::shared_ptr<ofImage> detectWithCIImage(void *handler_impl,
                                                   PersonSegmentation::QualityLevel qualityLevel,
                                                   CIImage *image)
        {
            VNSequenceRequestHandler *handler = (VNSequenceRequestHandler *)handler_impl;
            VNGeneratePersonSegmentationRequest *request = createRequest(qualityLevel);
            
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
        
        void PersonSegmentation::setup(QualityLevel level) {
            qualityLevel = level;
            VNSequenceRequestHandler *handler = [[VNSequenceRequestHandler alloc] init];
            handler_impl = (OFX_VISION_BRIDGE_RETAINED void *)handler;
        }
        
        std::shared_ptr<ofImage> PersonSegmentation::detect(const ofBaseHasPixels &pix) {
            CGImageRef cgImage = ofBaseHasPixelsToCGImageRef(pix);
            return detectWithCIImage(handler_impl, qualityLevel, [CIImage imageWithCGImage:cgImage]);
        }
        
        std::shared_ptr<ofImage> PersonSegmentation::detect(IOSurfaceRef surface) {
            return detectWithCIImage(handler_impl, qualityLevel, [CIImage imageWithIOSurface:surface]);
        }
        
        std::shared_ptr<ofImage> PersonSegmentation::detect(CVPixelBufferRef pix) {
            return detectWithCIImage(handler_impl, qualityLevel, [CIImage imageWithCVPixelBuffer:pix]);
        }
        
        PersonSegmentation::~PersonSegmentation() {
            VNSequenceRequestHandler *handler = (OFX_VISION_BRIDGE_TRANSFER VNSequenceRequestHandler *)handler_impl;
            OFX_VISION_RELEASE(handler);
        }
    }; // namespace Vision
}; // namespace ofx
