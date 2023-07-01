//
//  ofxVisionPersonSegmentation.mm
//
//  Created by 2bit on 2023/06/29.
//

#include "ofxVisionPersonSegmentation.h"

#if OFX_VISION_VERSION_CHECK(12, 0)

#import "ofxVisionObjCUtility.h"

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
            Request *createRequest(const PersonSegmentation::Settings &settings)
            {
                auto request = [[Request alloc] init];
                request.qualityLevel = conv(settings.qualityLevel);
                request.outputPixelFormat = kCVPixelFormatType_OneComponent8;
                OFX_VISION_AUTORELEASE(request);
                return request;
            }
            
            TargetRequest::ResultType detectWithCIImage(void *handler_impl,
                                                        const PersonSegmentation::Settings &settings,
                                                        CIImage *image)
            {
                VNSequenceRequestHandler *handler = (VNSequenceRequestHandler *)handler_impl;
                VNGeneratePersonSegmentationRequest *request = createRequest(settings);
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
        
#include "details/detect_impl.inl"

        Request *TargetRequest::createRequest() const
        { return ofx::Vision::createRequest(settings); }
        
        TargetRequest::ResultType TargetRequest::createResult(Request *request) const {
            CVPixelBufferRef pixelBuffer = request.results.firstObject.pixelBuffer;
            return pixelBufferToOfImage(pixelBuffer);
        }
    }; // namespace Vision
}; // namespace ofx

#endif
