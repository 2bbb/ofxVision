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
        
        Target::ResultType Target::detectWithCIImage(CIImage *image) {
            auto request = createRequest();
            
            NSError *err = nil;
            auto res = [handler performRequests:@[ request ]
                                      onCIImage:image
                                    orientation:kCGImagePropertyOrientationUp
                                          error:&err];
            if(err) {
                ofLogError("ofxGenerateOpticalFlow") << err.description.UTF8String;
                return {};
            }
            return createResult(request);
        }
        
#include "details/detect_impl.inl"

        void Target::releaseImage() {
            CIImage *baseImage = (CIImage *)settings.baseImage;
            if(settings.baseImage) OFX_VISION_RELEASE(baseImage);
        }
        void Target::setBaseImage(const ofBaseHasPixels &pix) {
            releaseImage();
            settings.baseImage = OFX_VISION_RETAIN(toCIImage(pix));
        }
        void Target::setBaseImage(IOSurfaceRef surface) {
            releaseImage();
            settings.baseImage = OFX_VISION_RETAIN(toCIImage(surface));
        }
        void Target::setBaseImage(CVPixelBufferRef pix) {
            releaseImage();
            settings.baseImage = OFX_VISION_RETAIN(toCIImage(pix));
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
#if OFX_VISION_USE_TEXTURE
            return pixelBufferToOfFloatTexture(pixelBuffer);
#else
            return pixelBuffer2fToOfFloatImage(pixelBuffer);
#endif
        }
    }; // namespace Vision
}; // namespace ofx

#endif
