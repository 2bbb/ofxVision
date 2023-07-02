//
//  ofxVisionGenerateOpticalFlow.m
//
//  Created by 2bit on 2023/06/30.
//

#import "ofxVisionObjCUtility.h"

#import "ofxVisionGenerateOpticalFlow.h"
#include "ofxVisionUtility.h"

#include "ofLog.h"

#if OFX_VISION_VERSION_CHECK(11, 0)

namespace ofx {
    namespace Vision {
        using Target = GenerateOpticalFlow;
        
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
        
        Target::ResultType Target::detectWithCIImage(ofxVisionCIImage *image) {
            auto request = createRequest();
            
            NSError *err = nil;
            auto res = [handler performRequests:@[ request ]
                                      onCIImage:(CIImage *)image
                                    orientation:kCGImagePropertyOrientationUp
                                          error:&err];
            if(err) {
                ofLogError("ofxGenerateOpticalFlow") << err.description.UTF8String;
                return {};
            }
            return createResult(request);
        }
        
        void Target::releaseImage() {
            CIImage *baseImage = (CIImage *)settings.baseImage;
            if(settings.baseImage) OFX_VISION_RELEASE((CIImage *)baseImage);
        }
        
        void Target::setBaseImage(ofxVisionCIImage *image) {
            releaseImage();
            settings.baseImage = OFX_VISION_RETAIN((CIImage *)image);
        }

        
        Target::Request *Target::createRequest() const {
            auto request = [[Target::Request alloc] initWithTargetedCIImage:(CIImage *)settings.baseImage
                                                                                                          options:@{}];
            request.computationAccuracy = conv(settings.accuracyLevel);
            OFX_VISION_AUTORELEASE(request);
            return request;
        }
        
        Target::ResultType Target::createResult(void *req) const {
            Target::Request *request = (Target::Request *)req;
            CVPixelBufferRef pixelBuffer = request.results.firstObject.pixelBuffer;
            return pixelBufferToOfFloatTexture(pixelBuffer);
        }
    }; // namespace Vision
}; // namespace ofx

#endif
