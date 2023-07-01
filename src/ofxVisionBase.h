//
//  ofxVisionBase.h
//
//  Created by 2bit on 2023/06/30.
//

#pragma once

#include "ofxVisionObservation.h"
#include "ofxVisionUtility.h"

#include "ofBaseTypes.h"

#include <IOSurface/IOSurface.h>
#include <CoreVideo/CVPixelBuffer.h>

#if __OBJC__
#import <Vision/Vision.h>
#endif

#include <memory>
#include <vector>
#include <tuple>

namespace ofx {
    namespace Vision {
        struct MultipleDetectorBase {
            
        };
        
        struct Base {
            virtual ~Base();
            virtual void setup();
        protected:
            void *handler_impl;
            MultipleDetectorBase *multipleDetector{nullptr};
            friend struct MultipleDetectorBase;
        };
        
        template <typename ... Requests>
        struct MultipleDetector : MultipleDetectorBase {
            std::tuple<typename Requests::ResultType ...> results;
        };
    };
};
