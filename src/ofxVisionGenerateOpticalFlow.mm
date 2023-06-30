//
//  ofxVisionGenerateOpticalFlow.m
//
//  Created by 2bit on 2023/06/30.
//

#import "ofxVisionObjCUtility.h"

#import "ofxVisionGenerateOpticalFlow.h"
#include "ofxVisionUtility.h"

#include "ofLog.h"

namespace ofx {
    namespace Vision {
        using TargetRequest = GenerateOpticalFlow;
        
        VNGenerateOpticalFlowRequestComputationAccuracy conv(GenerateOpticalFlow::ComputationAccuracyLevel level)
        {
            using Level = GenerateOpticalFlow::ComputationAccuracyLevel;
            switch(level) {
                case Level::Low:
                    return VNGenerateOpticalFlowRequestComputationAccuracyLow;
                case Level::Medium:
                    return VNGenerateOpticalFlowRequestComputationAccuracyMedium;
                case Level::High:
                    return VNGenerateOpticalFlowRequestComputationAccuracyHigh;
                case Level::VeryHigh:
                    return VNGenerateOpticalFlowRequestComputationAccuracyVeryHigh;
            }
        }
        
        namespace {
            VNGenerateOpticalFlowRequest *createRequest(CIImage *image,
                                                        GenerateOpticalFlow::ComputationAccuracyLevel accuracyLevel)
            {
                VNGenerateOpticalFlowRequest *request = [[VNGenerateOpticalFlowRequest alloc] initWithTargetedCIImage:image
                                                                                                              options:@{}];
                request.computationAccuracy = conv(accuracyLevel);
//                request.outputPixelFormat = kCVPixelFormatType_OneComponent32Float;
                OFX_VISION_AUTORELEASE(request);
                return request;
            }
            
            TargetRequest::ResultType detectWithCIImage(void *handler_impl,
                                                        GenerateOpticalFlow::ComputationAccuracyLevel accuracyLevel,
                                                        CIImage *base_image,
                                                        CIImage *image)
            {
                auto handler = (VNSequenceRequestHandler *)handler_impl;
                auto request = createRequest(image, accuracyLevel);
                
                NSError *err = nil;
                auto res = [handler performRequests:@[ request ]
                                          onCIImage:base_image
                                        orientation:kCGImagePropertyOrientationUp
                                              error:&err];
                if(!res) {
                    if(err) {
                        ofLogError("ofxGenerateOpticalFlow") << err.description.UTF8String;
                    } else {
                        ofLogError("ofxGenerateOpticalFlow") << "unknown error...";
                    }
                    return {};
                }
                if(err) {
                    ofLogError("ofxGenerateOpticalFlow") << err.description.UTF8String;
                    return {};
                }
                CVPixelBufferRef pixelBuffer = request.results.firstObject.pixelBuffer;
#if OFX_VISION_USE_TEXTURE
                return pixelBufferToOfFloatTexture(pixelBuffer);
#else
                return pixelBuffer2fToOfFloatImage(pixelBuffer);
#endif
            }
        };
        
        GenerateOpticalFlow::ResultType TargetRequest::detect(const ofBaseHasPixels &base,
                                                              const ofBaseHasPixels &pix)
        {
            CGImageRef cgImage = ofBaseHasPixelsToCGImageRef(pix);
            return detectWithCIImage(handler_impl,
                                     accuracyLevel,
                                     toCIImage(base),
                                     [CIImage imageWithCGImage:cgImage]);
        }
        
        GenerateOpticalFlow::ResultType TargetRequest::detect(const ofBaseHasPixels &base,
                                                              IOSurfaceRef surface)
        {
            return detectWithCIImage(handler_impl,
                                     accuracyLevel,
                                     toCIImage(base),
                                     [CIImage imageWithIOSurface:surface]);
        }
        
        GenerateOpticalFlow::ResultType TargetRequest::detect(const ofBaseHasPixels &base,
                                                              CVPixelBufferRef pix)
        {
            return detectWithCIImage(handler_impl,
                                     accuracyLevel,
                                     toCIImage(base),
                                     [CIImage imageWithCVPixelBuffer:pix]);
        }
    }; // namespace Vision
}; // namespace ofx
