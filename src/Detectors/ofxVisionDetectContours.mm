//
//  ofxVisionDetectContours.mm
//
//  Created by 2bit on 2023/07/01.
//

#import "ofxVisionDetectContours.h"

#if OFX_VISION_VERSION_CHECK(11, 0)

#import "ofxVisionObjCUtility.h"

#include "ofLog.h"

namespace ofx {
    namespace Vision {
        using Target = DetectContours;

        BaseRequest *Target::createRequest() const {
            auto request = OFX_VISION_AUTORELEASE([[VNDetectContoursRequest alloc] init]);
            request.contrastAdjustment = settings.contrastAdjustment;
            request.contrastPivot = settings.contrastPivot < 0.0f ? nil : @(settings.contrastPivot);
            request.detectsDarkOnLight = settings.detectsDarkOnLight;
            return request;
        }
        
        Target::ResultType Target::createResult(void *req) const {
            Target::Request *request = (Target::Request *)req;
            Target::ResultType result;
            if(request.results.firstObject) result = toOF(request.results.firstObject);
            return result;
        }
    }; // namespace Vision
}; // namespace ofx

#endif
