//
//  ofxVisionBase.h
//
//  Created by 2bit on 2023/06/30.
//

#pragma once

#include "ofxVisionObservation.h"

#include "ofBaseTypes.h"

#include <IOSurface/IOSurface.h>
#include <CoreVideo/CVPixelBuffer.h>

#include <memory>
#include <vector>

namespace ofx {
    namespace Vision {
        struct Base {
            virtual ~Base();
            virtual void setup();
        protected:
            void *handler_impl;
        };
    };
};
