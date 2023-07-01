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
            Request *createRequest(const TargetRequest::Settings &settings) {
                auto request = [[VNDetectHumanBodyPoseRequest alloc] init];
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
        
#include "details/detect_impl.inl"

        Request *TargetRequest::createRequest() const
        { return ofx::Vision::createRequest(settings); }
        
        TargetRequest::ResultType TargetRequest::createResult(Request *request) const {
            TargetRequest::ResultType results;
            for(VNHumanBodyPoseObservation *body in request.results) {
                results.push_back(toOF(body));
            }
            return results;
        }
    }; // namespace Vision
}; // namespace ofx
