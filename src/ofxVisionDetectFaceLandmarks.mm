//
//  ofxVisionDetectFaceLandmarks.mm
//
//  Created by 2bit on 2023/07/01.
//

#import "ofxVisionObjCUtility.h"

#import "ofxVisionDetectFaceLandmarks.h"
#include "ofxVisionUtility.h"

#include "ofLog.h"

namespace ofx {
    namespace Vision {
        using TargetRequest = DetectFaceLandmarks;
        
        namespace {
            VNDetectFaceLandmarksRequest *createRequest(const TargetRequest::Settings &settings) {
                auto request = [[VNDetectFaceLandmarksRequest alloc] init];
                
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
                if(err) {
                    ofLogError("ofxVisionDetectFaceLandmarks") << err.description.UTF8String;
                    return {};
                }
                TargetRequest::ResultType result;
                result.resize(request.results.count);
                for(auto i = 0; i < result.size(); ++i) {
                    result[i] = toOF(request.results[i]);
                }
                return result;
            }
        };
        
        TargetRequest::ResultType TargetRequest::detect(const ofBaseHasPixels &pix) {
            CGImageRef cgImage = ofBaseHasPixelsToCGImageRef(pix);
            return detectWithCIImage(handler_impl, settings, [CIImage imageWithCGImage:cgImage]);
        }
        
        TargetRequest::ResultType TargetRequest::detect(IOSurfaceRef surface) {
            return detectWithCIImage(handler_impl, settings, [CIImage imageWithIOSurface:surface]);
        }
        
        TargetRequest::ResultType TargetRequest::detect(CVPixelBufferRef pix) {
            return detectWithCIImage(handler_impl, settings, [CIImage imageWithCVPixelBuffer:pix]);
        }
    }; // namespace Vision
}; // namespace ofx
