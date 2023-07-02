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
        using Target = DetectFaceLandmarks;
        
        Target::ResultType Target::detectWithCIImage(ofxVisionCIImage *image)
        {
            auto request = createRequest();
            NSError *err = nil;
            [handler performRequests:@[request]
                           onCIImage:(CIImage *)image
                         orientation:kCGImagePropertyOrientationUp
                               error:&err];
            if(err) {
                ofLogError("ofxVisionDetectFaceLandmarks") << err.description.UTF8String;
                return {};
            }
            return createResult(request);
        }
        
#include "details/detect_impl.inl"

        Target::Request *Target::createRequest() const {
            auto request = OFX_VISION_AUTORELEASE([[VNDetectFaceLandmarksRequest alloc] init]);
            return request;
        }
        
        Target::ResultType Target::createResult(void *req) const {
            Target::Request *request = (Target::Request *)req;
            ofLogNotice(__func__) << request;
            
            Target::ResultType result;
            result.resize(request.results.count);
            for(auto i = 0; i < result.size(); ++i) {
                result[i] = toOF(request.results[i]);
            }
            return result;
        }
    }; // namespace Vision
}; // namespace ofx
