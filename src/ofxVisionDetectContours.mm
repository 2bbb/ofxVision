//
//  ofxVisionDetectContours.mm
//
//  Created by 2bit on 2023/07/01.
//

#import "ofxVisionObjCUtility.h"

#import "ofxVisionDetectContours.h"
#include "ofxVisionUtility.h"

#include "ofLog.h"

namespace ofx {
    namespace Vision {
        using TargetRequest = DetectContours;
        
        namespace {
            VNDetectContoursRequest *createRequest(const TargetRequest::Settings &settings) {
                VNDetectContoursRequest *request = [[VNDetectContoursRequest alloc] init];
                request.contrastAdjustment = settings.contrastAdjustment;
                request.contrastPivot = settings.contrastPivot < 0.0f ? nil : @(settings.contrastPivot);
                request.detectsDarkOnLight = settings.detectsDarkOnLight;
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
                    ofLogError("ofxVisionDetectContours") << err.description.UTF8String;
                    return {};
                }
                TargetRequest::ResultType result;
                if(request.results.firstObject) result = toOF(request.results.firstObject);
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
