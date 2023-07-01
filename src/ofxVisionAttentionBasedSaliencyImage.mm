//
//  ofxVisionAttentionBasedSaliencyImage.m
//  example
//
//  Created by 2bit on 2023/06/30.
//

#import "ofxVisionObjCUtility.h"

#import "ofxVisionAttentionBasedSaliencyImage.h"
#include "ofxVisionUtility.h"

#include "ofLog.h"

namespace ofx {
    namespace Vision {
        using TargetRequest = AttentionBasedSaliencyImage;
        
        namespace {
            Request *createRequest(const TargetRequest::Settings &settings)
            {
                auto request = [[VNGenerateAttentionBasedSaliencyImageRequest alloc] init];
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
                [handler performRequests:@[request]
                               onCIImage:image
                             orientation:kCGImagePropertyOrientationUp
                                   error:&err];
                AttentionBasedSaliencyImage::ResultType result;
                if(err) {
                    ofLogError("AttentionBasedSaliencyImage") << err.description.UTF8String;
                    return result;
                }
                result = toOF(request.results.firstObject);
                return result;
            }
        };
        
#include "details/detect_impl.inl"
        
        Request *TargetRequest::createRequest() const
        { return ofx::Vision::createRequest(settings); }
        
        TargetRequest::ResultType TargetRequest::createResult(Request *request) const
        { return toOF(request.results.firstObject); }
    }; // namespace Vision
}; // namespace ofx

