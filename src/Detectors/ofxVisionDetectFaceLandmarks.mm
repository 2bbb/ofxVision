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
        
        BaseRequest *Target::createRequest() const {
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
