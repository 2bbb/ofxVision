//
//  ofxVisionDetectHumanBodyPose.mm
//
//  Created by 2bit on 2023/06/30.
//

#import "ofxVisionObjCUtility.h"

#import "ofxVisionDetectHumanBodyPose.h"
#include "ofxVisionUtility.h"

#include "ofLog.h"

namespace ofx {
    namespace Vision {
        using TargetRequest = DetectHumanBodyPose;
        
        namespace {
            VNDetectHumanBodyPoseRequest *createRequest() {
                VNDetectHumanBodyPoseRequest *request = [[VNDetectHumanBodyPoseRequest alloc] init];
                OFX_VISION_AUTORELEASE(request);
                return request;
            }
            
            TargetRequest::ResultType detectWithCIImage(void *handler_impl,
                                                        CIImage *image)
            {
                auto handler = (VNSequenceRequestHandler *)handler_impl;
                auto request = createRequest();
                NSError *err = nil;
                [handler performRequests:@[request]
                               onCIImage:image
                             orientation:kCGImagePropertyOrientationUp
                                   error:&err];
                if(err) {
                    ofLogError("ofxVisionDetectBodyPose") << err.description.UTF8String;
                    return {};
                }
                TargetRequest::ResultType results;
                for(VNHumanBodyPoseObservation *body in request.results) {
                    results.push_back(toOF(body));
                }
                return results;
            }
        };
        
        TargetRequest::ResultType TargetRequest::detect(const ofBaseHasPixels &pix) {
            CGImageRef cgImage = ofBaseHasPixelsToCGImageRef(pix);
            return detectWithCIImage(handler_impl, [CIImage imageWithCGImage:cgImage]);
        }
        
        TargetRequest::ResultType TargetRequest::detect(IOSurfaceRef surface) {
            return detectWithCIImage(handler_impl, [CIImage imageWithIOSurface:surface]);
        }
        
        TargetRequest::ResultType TargetRequest::detect(CVPixelBufferRef pix) {
            return detectWithCIImage(handler_impl, [CIImage imageWithCVPixelBuffer:pix]);
        }
    }; // namespace Vision
}; // namespace ofx
