//
//  ofxVisionDetectRectangles.mm
//
//  Created by 2bit on 2023/07/01.
//

#import "ofxVisionObjCUtility.h"

#import "ofxVisionDetectRectangles.h"
#include "ofxVisionUtility.h"

#include "ofLog.h"

namespace ofx {
    namespace Vision {
        using Target = DetectRectangles;
        
        Target::ResultType Target::detectWithCIImage(ofxVisionCIImage *image) {
            auto request = createRequest();
            NSError *err = nil;
            [handler performRequests:@[request]
                           onCIImage:(CIImage *)image
                         orientation:kCGImagePropertyOrientationUp
                               error:&err];
            if(err) {
                ofLogError("ofxVisionDetectRectangles") << err.description.UTF8String;
                return {};
            }
            return createResult(request);
        }
        
        Target::Request *Target::createRequest() const {
            auto request = OFX_VISION_AUTORELEASE([[Target::Request alloc] init]);
            
            request.minimumAspectRatio = settings.minimumAspectRatio;
            request.maximumAspectRatio = settings.maximumAspectRatio;
            request.quadratureTolerance = settings.quadratureTolerance;
            request.minimumSize = settings.minimumSize;
            request.minimumConfidence = settings.minimumConfidence;
            request.maximumObservations = settings.maximumObservations;
            
            return request;
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
