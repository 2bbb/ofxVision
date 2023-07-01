//
//  ofxVisionObjectnessBasedSaliencyImage.mm
//
//  Created by 2bit on 2023/06/30.
//

#import "ofxVisionObjectnessBasedSaliencyImage.h"

#if OFX_VISION_VERSION_CHECK_X(10, 15)

#import "ofxVisionObjCUtility.h"

#include "ofLog.h"

namespace ofx {
    namespace Vision {
        using TargetRequest = ObjectnessBasedSaliencyImage;
        namespace {
            Request *createRequest(const TargetRequest::Settings &settings) {
                auto request = [[VNGenerateObjectnessBasedSaliencyImageRequest alloc] init];
                OFX_VISION_AUTORELEASE(request);
                return request;
            }
            
            TargetRequest::ResultType detectWithCIImage(void *handler_impl,
                                                        const TargetRequest::Settings &settings,
                                                        CIImage *image)
            {
                VNSequenceRequestHandler *handler = (VNSequenceRequestHandler *)handler_impl;
                auto request = createRequest(settings);
                
                NSError *err = nil;
                [handler performRequests:@[request]
                               onCIImage:image
                             orientation:kCGImagePropertyOrientationUp
                                   error:&err];
                TargetRequest::ResultType result;
                if(err) {
                    ofLogError("ObjectnessBasedSaliencyImage") << err.description.UTF8String;
                    return result;
                }
                return toOF(request.results.firstObject);
            }
        };
        
#include "details/detect_impl.inl"

        Request *TargetRequest::createRequest() const
        { return ofx::Vision::createRequest(settings); }
        
        TargetRequest::ResultType TargetRequest::createResult(Request *request) const {
            return toOF(request.results.firstObject);
        }
    }; // namespace Vision
}; // namespace ofx

#endif
