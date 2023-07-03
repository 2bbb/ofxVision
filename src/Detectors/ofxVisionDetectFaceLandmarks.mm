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
            auto request = [[VNDetectFaceLandmarksRequest alloc] init];
            
            return OFX_VISION_AUTORELEASE(request);
        }
        
        Target::ResultType Target::createResult(void *req) const {
            Target::Request *request = (Target::Request *)req;
            
            Target::ResultType result;
            result.resize(request.results.count);
            for(auto i = 0; i < result.size(); ++i) {
                result[i] = toOF(request.results[i]);
            }
            return result;
        }
    }; // namespace Vision
}; // namespace ofx
