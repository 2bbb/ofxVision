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
            Request *createRequest(const TargetRequest::Settings &settings)
            {
                auto request = [[VNGenerateOpticalFlowRequest alloc] initWithTargetedCIImage:(CIImage *)settings.baseImage
                                                                                                              options:@{}];
                request.computationAccuracy = conv(settings.accuracyLevel);
                OFX_VISION_AUTORELEASE(request);
                return request;
            }
            
            TargetRequest::ResultType detectWithCIImage(void *handler_impl,
                                                        const TargetRequest::Settings &settings,
                                                        CIImage *image)
            {
                auto handler = (VNSequenceRequestHandler *)handler_impl;
                auto request = createRequest(settings);
                
                NSError *err = nil;
                auto res = [handler performRequests:@[ request ]
                                          onCIImage:image
                                        orientation:kCGImagePropertyOrientationUp
                                              error:&err];
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
        
#include "details/detect_impl.inl"

        void TargetRequest::releaseImage() {
            CIImage *baseImage = (CIImage *)settings.baseImage;
            if(settings.baseImage) OFX_VISION_RELEASE(baseImage);
        }
        void TargetRequest::setBaseImage(const ofBaseHasPixels &pix) {
            releaseImage();
            settings.baseImage = OFX_VISION_RETAIN(toCIImage(pix));
        }
        void TargetRequest::setBaseImage(IOSurfaceRef surface) {
            releaseImage();
            settings.baseImage = OFX_VISION_RETAIN(toCIImage(surface));
        }
        void TargetRequest::setBaseImage(CVPixelBufferRef pix) {
            releaseImage();
            settings.baseImage = OFX_VISION_RETAIN(toCIImage(pix));
        }
        
        Request *TargetRequest::createRequest() const
        { return ofx::Vision::createRequest(settings); }
        
        TargetRequest::ResultType TargetRequest::createResult(Request *request) const {
            CVPixelBufferRef pixelBuffer = request.results.firstObject.pixelBuffer;
#if OFX_VISION_USE_TEXTURE
            return pixelBufferToOfFloatTexture(pixelBuffer);
#else
            return pixelBuffer2fToOfFloatImage(pixelBuffer);
#endif
        }
    }; // namespace Vision
}; // namespace ofx
