//
//  ofxVisionPersonSegmentation.mm
//
//  Created by 2bit on 2023/06/29.
//

#include "ofxVisionPersonSegmentation.h"

#if OFX_VISION_VERSION_CHECK(12, 0)

#import "ofxVisionObjCUtility.h"

#include "ofLog.h"

namespace ofx {
    namespace Vision {
        using Target = PersonSegmentation;
        
        VNGeneratePersonSegmentationRequestQualityLevel conv(PersonSegmentation::QualityLevel level)
        {
            switch(level) {
                case PersonSegmentation::QualityLevel::Fast:
                    return VNGeneratePersonSegmentationRequestQualityLevelFast;
                case PersonSegmentation::QualityLevel::Balanced:
                    return VNGeneratePersonSegmentationRequestQualityLevelBalanced;
                case PersonSegmentation::QualityLevel::Accurate:
                    return VNGeneratePersonSegmentationRequestQualityLevelAccurate;
            }
        }
        
        BaseRequest *Target::createRequest() const {
            auto request = [[Target::Request alloc] init];
            
            request.qualityLevel = conv(settings.qualityLevel);
            request.outputPixelFormat = kCVPixelFormatType_OneComponent8;
            
            return OFX_VISION_AUTORELEASE(request);
        }
        
        Target::ResultType Target::createResult(void *req) const {
            Target::Request *request = (Target::Request *)req;
            CVPixelBufferRef pixelBuffer = request.results.firstObject.pixelBuffer;
            return pixelBufferToOfImage(pixelBuffer);
        }
    }; // namespace Vision
}; // namespace ofx

#endif
