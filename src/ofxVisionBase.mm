//
//  ofxVisionBase.mm
//
//  Created by 2bit on 2023/06/30.
//

#include "ofxVisionBase.h"

#import "ofxVisionObjCUtility.h"

namespace ofx {
    namespace Vision {
        void Base::setup() {
            VNSequenceRequestHandler *handler = [[VNSequenceRequestHandler alloc] init];
            handler_impl = (OFX_VISION_BRIDGE_RETAINED void *)handler;
        }
        Base::~Base() {
            id handler = (OFX_VISION_BRIDGE_TRANSFER VNSequenceRequestHandler *)handler_impl;
            OFX_VISION_RELEASE(handler);
        };
    };
};
