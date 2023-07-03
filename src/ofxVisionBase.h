//
//  ofxVisionBase.h
//
//  Created by 2bit on 2023/06/30.
//

#pragma once

#include "ofxVisionUtility.h"
#include "ofxVisionObservation.h"

#include "ofBaseTypes.h"

#include <IOSurface/IOSurface.h>
#include <CoreVideo/CVPixelBuffer.h>

#if __OBJC__
#   import <Vision/Vision.h>
#endif

namespace ofx {
    namespace Vision {
        bool runRequest(void *handler,
                        void *request,
                        ofxVisionCIImage *image,
                        const std::string &name);
        
        template <typename Result>
        struct Base {
            using ResultType = Result;
            
            ResultType detect(const ofPixels &pix) {
                return detectWithCIImage(toCIImage(pix));
            }
            ResultType detect(const ofBaseHasPixels &pix) {
                return detectWithCIImage(toCIImage(pix));
            }
            ResultType detect(IOSurfaceRef surface) {
                return detectWithCIImage(toCIImage(surface));
            }
            ResultType detect(CVPixelBufferRef pix) {
                return detectWithCIImage(toCIImage(pix));
            }
            
            virtual ~Base() {
                objc_release(handler);
            }
            virtual void setup() {
                handler = createHandler();
            }
        protected:
            virtual std::string getName() const = 0;
            virtual BaseRequest *createRequest() const = 0;
            virtual ResultType createResult(void *result) const = 0;
            
            ResultType detectWithCIImage(ofxVisionCIImage *image) {
                auto request = createRequest();
                runRequest(handler, request, image, getName());
                return createResult(request);
            };
            Handler *handler;
        };
        
    };
};
