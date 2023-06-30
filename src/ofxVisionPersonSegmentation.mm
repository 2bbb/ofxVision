//
//  ofxVisionPersonSegmentation.mm
//
//  Created by 2bit on 2023/06/29.
//

#import "ofxVisionObjCUtility.h"

#include "ofxVisionPersonSegmentation.h"
#include "ofxVisionUtility.h"

#include "ofLog.h"

namespace ofx {
    namespace Vision {
        using TargetRequest = PersonSegmentation;
        
        VNGeneratePersonSegmentationRequestQualityLevel conv(PersonSegmentation::QualityLevel level)
        {
            switch(level) {
                case PersonSegmentation::QualityLevel::Fast:
                    return VNGeneratePersonSegmentationRequestQualityLevelFast;
                case PersonSegmentation::QualityLevel::Balanced:
                    return VNGeneratePersonSegmentationRequestQualityLevelBalanced;
                case PersonSegmentation::QualityLevel::Accurate:
                    return VNGeneratePersonSegmentationRequestQualityLevelAccurate;
            }
        }
        
        namespace {
            VNGeneratePersonSegmentationRequest *createRequest(PersonSegmentation::QualityLevel qualityLevel) {
                VNGeneratePersonSegmentationRequest *request = [[VNGeneratePersonSegmentationRequest alloc] init];
                request.qualityLevel = conv(qualityLevel);
                request.outputPixelFormat = kCVPixelFormatType_OneComponent8;
                OFX_VISION_AUTORELEASE(request);
                return request;
            }
            
            TargetRequest::ResultType detectWithCIImage(void *handler_impl,
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
        };
        
        void TargetRequest::setup(QualityLevel level) {
            Base::setup();
            qualityLevel = level;
        }
        
        TargetRequest::ResultType TargetRequest::detect(const ofBaseHasPixels &pix) {
            CGImageRef cgImage = ofBaseHasPixelsToCGImageRef(pix);
            return detectWithCIImage(handler_impl, qualityLevel, [CIImage imageWithCGImage:cgImage]);
        }
        
        TargetRequest::ResultType TargetRequest::detect(IOSurfaceRef surface) {
            return detectWithCIImage(handler_impl, qualityLevel, [CIImage imageWithIOSurface:surface]);
        }
        
        TargetRequest::ResultType TargetRequest::detect(CVPixelBufferRef pix) {
            return detectWithCIImage(handler_impl, qualityLevel, [CIImage imageWithCVPixelBuffer:pix]);
        }
    }; // namespace Vision
}; // namespace ofx
