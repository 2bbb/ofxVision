//
//  ofxVisionDetectHumanHandPose.m
//
//  Created by 2bit on 2023/06/30.
//

#import "ofxVisionObjCUtility.h"

#import "ofxVisionDetectHumanHandPose.h"
#include "ofxVisionUtility.h"

#include "ofLog.h"

#if OFX_VISION_VERSION_CHECK(11, 0)

namespace ofx {
    namespace Vision {
        using TargetRequest = DetectHumanHandPose;
        
        namespace {
            VNDetectHumanHandPoseRequest *createRequest(const TargetRequest::Settings &settings)
            {
                VNDetectHumanHandPoseRequest *request = [[VNDetectHumanHandPoseRequest alloc] init];
                request.maximumHandCount = settings.maximumHandCount;
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
        
#include "details/detect_impl.inl"

        Request *TargetRequest::createRequest() const
        { return ofx::Vision::createRequest(settings); }
        
        TargetRequest::ResultType TargetRequest::createResult(Request *request) const {
            TargetRequest::ResultType results;
            for(VNHumanHandPoseObservation *hand in request.results) {
                results.push_back(toOF(hand));
            }
            return results;
        }
    }; // namespace Vision
}; // namespace ofx

#endif
