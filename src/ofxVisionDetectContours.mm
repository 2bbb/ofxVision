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

        Target::ResultType Target::detectWithCIImage(CIImage *image) {
            auto request = createRequest();
            NSError *err = nil;
            [handler performRequests:@[request]
                           onCIImage:image
                         orientation:kCGImagePropertyOrientationUp
                               error:&err];
            if(err) {
                ofLogError("ofxVisionDetectContours") << err.description.UTF8String;
                return {};
            }
            return createResult(request);
        }
        
#include "details/detect_impl.inl"

        Target::Request *Target::createRequest() const {
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
