//
//  ofxVisionBase.mm
//
//  Created by 2bit on 2023/06/30.
//

#include "ofxVisionBase.h"
#import "ofxVisionObjCUtility.h"

namespace ofx {
    namespace Vision {
        bool runRequest(void *handler,
                        void *request,
                        ofxVisionCIImage *image,
                        const std::string &name)
        {
            NSError *err = nil;
            [(Handler *)handler performRequests:@[(BaseRequest *)request]
                                      onCIImage:(CIImage *)image
                                    orientation:kCGImagePropertyOrientationUp
                                          error:&err];
            if(err) {
                ofLogError(name) << "got error: " << err.description.UTF8String;
                return false;
            }
            return true;
        }

    };
};
