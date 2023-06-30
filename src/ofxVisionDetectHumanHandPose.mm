//
//  ofxVisionDetectHumanHandPose.m
//
//  Created by 2bit on 2023/06/30.
//

#import "ofxVisionObjCUtility.h"

#import "ofxVisionDetectHumanHandPose.h"
#include "ofxVisionUtility.h"

#include "ofLog.h"

namespace ofx {
    namespace Vision {
        using TargetRequest = DetectHumanHandPose;
        
        namespace {
            VNDetectHumanHandPoseRequest *createRequest(std::size_t maximumHandCount) {
                VNDetectHumanHandPoseRequest *request = [[VNDetectHumanHandPoseRequest alloc] init];
                request.maximumHandCount = maximumHandCount;
                OFX_VISION_AUTORELEASE(request);
                return request;
            }
            
            TargetRequest::ResultType detectWithCIImage(void *handler_impl,
                                                        std::size_t maximumHandCount,
                                                        CIImage *image)
            {
                auto handler = (VNSequenceRequestHandler *)handler_impl;
                auto request = createRequest(maximumHandCount);
                NSError *err = nil;
                [handler performRequests:@[request]
                               onCIImage:image
                             orientation:kCGImagePropertyOrientationUp
                                   error:&err];
                if(err) {
                    ofLogError("ofxVisionPersonSegmentation") << err.description.UTF8String;
                    return {};
                }
                TargetRequest::ResultType results;
                for(VNHumanHandPoseObservation *hand in request.results) {
                    results.push_back(toOF(hand));
                }
                return results;
            }
        };
        
        TargetRequest::ResultType TargetRequest::detect(const ofBaseHasPixels &pix) {
            CGImageRef cgImage = ofBaseHasPixelsToCGImageRef(pix);
            return detectWithCIImage(handler_impl, maximumHandCount, [CIImage imageWithCGImage:cgImage]);
        }
        
        TargetRequest::ResultType TargetRequest::detect(IOSurfaceRef surface) {
            return detectWithCIImage(handler_impl, maximumHandCount, [CIImage imageWithIOSurface:surface]);
        }
        
        TargetRequest::ResultType TargetRequest::detect(CVPixelBufferRef pix) {
            return detectWithCIImage(handler_impl, maximumHandCount, [CIImage imageWithCVPixelBuffer:pix]);
        }
    }; // namespace Vision
}; // namespace ofx
