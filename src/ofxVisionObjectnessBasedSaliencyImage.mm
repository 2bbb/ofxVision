//
//  ofxVisionObjectnessBasedSaliencyImage.m
//  example
//
//  Created by 2bit on 2023/06/30.
//

#import "ofxVisionObjCUtility.h"

#import "ofxVisionObjectnessBasedSaliencyImage.h"
#include "ofxVisionUtility.h"

#include "ofLog.h"

namespace ofx {
    namespace Vision {
        using RequestObject = ObjectnessBasedSaliencyImage;
        namespace {
            VNGenerateObjectnessBasedSaliencyImageRequest *createRequest() {
                VNGenerateObjectnessBasedSaliencyImageRequest *request = [[VNGenerateObjectnessBasedSaliencyImageRequest alloc] init];
                OFX_VISION_AUTORELEASE(request);
                return request;
            }
            
            RequestObject::ResultType detectWithCIImage(void *handler_impl, CIImage *image) {
                VNSequenceRequestHandler *handler = (VNSequenceRequestHandler *)handler_impl;
                auto request = createRequest();
                
                NSError *err = nil;
                [handler performRequests:@[request]
                               onCIImage:image
                             orientation:kCGImagePropertyOrientationUp
                                   error:&err];
                RequestObject::ResultType result;
                if(err) {
                    ofLogError("AttentionBasedSaliencyImage") << err.description.UTF8String;
                    return result;
                }
                result = toOF(request.results.firstObject);
                return result;
            }
        };
        
        RequestObject::ResultType RequestObject::detect(const ofBaseHasPixels &pix) {
            CGImageRef cgImage = ofBaseHasPixelsToCGImageRef(pix);
            return detectWithCIImage(handler_impl, [CIImage imageWithCGImage:cgImage]);
        }
        
        RequestObject::ResultType RequestObject::detect(IOSurfaceRef surface) {
            return detectWithCIImage(handler_impl, [CIImage imageWithIOSurface:surface]);
        }
        
        RequestObject::ResultType RequestObject::detect(CVPixelBufferRef pix) {
            return detectWithCIImage(handler_impl, [CIImage imageWithCVPixelBuffer:pix]);
        }
    }; // namespace Vision
}; // namespace ofx

