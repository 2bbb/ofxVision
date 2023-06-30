//
//  ofxVisionObservation.h
//  example
//
//  Created by 2bit on 2023/06/30.
//

#ifndef ofxVisionObservation_h
#define ofxVisionObservation_h

#include "ofVectorMath.h"
#include "ofRectangle.h"
#include "ofImage.h"

#include <memory>

namespace ofx {
    namespace Vision {
        namespace Observation {
            struct Base {
                float confidence;
                std::string uuid;
            };
            struct PixelBuffer : Base {
                std::shared_ptr<ofImage> pixels;
                std::string featureName;
            };
            struct FloatPixelBuffer : Base {
                std::shared_ptr<ofFloatImage> pixels;
                std::string featureName;
            };
            struct Object : Base {
                ofRectangle boundingBox;
                FloatPixelBuffer segmentationMask;
            };
            struct Rectangle : Object {
                glm::vec2 topLeft;
                glm::vec2 topRight;
                glm::vec2 bottomRight;
                glm::vec2 bottomLeft;
            };
            struct SaliencyImage : FloatPixelBuffer {
                std::vector<Observation::Rectangle> salientObjects;
            };
        }
    };
};

#endif /* ofxVisionObservation_h */
