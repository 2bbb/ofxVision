//
//  ofxVisionMultipleDetector.h
//  example
//
//  Created by 2bit on 2023/07/03.
//

#pragma once

#include "ofxVisionObservation.h"
#include "ofxVisionUtility.h"

#include "details/type_traits.hpp"

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
        bool detectMultiple(void *handler,
                            std::vector<void *> &request_vec,
                            void *image);
        
        template <typename ... Detectors>
        struct MultipleDetector {
            using DetectorsTuple = std::tuple<Detectors ...>;
            using Requests = std::tuple<typename Detectors::Request * ...>;
            using Results = std::tuple<typename Detectors::ResultType ...>;

            void setup() {
                handler = createHandler();
            }
            
            template <std::size_t ... is>
            bool detectWithCIImage(ofxVisionCIImage *image, index_sequence<is ...> &&)
            {
                Requests requests = { std::get<is>(detectors).createRequest() ... };
                std::vector<BaseRequest *> requests_vec = { (BaseRequest *)std::get<is>(requests) ... };
                
                auto success = detectMultiple(handler, requests_vec, image);
                if(!success) {
                    ofLogError(__func__) << "What";
                    return false;
                }
                auto && _ = { (std::get<is>(results) = std::get<is>(detectors).createResult(std::get<is>(requests)), 0) ... };
                return true;
            }
            bool detectWithCIImage(ofxVisionCIImage *image) {
                return detectWithCIImage(image,
                                         make_index_sequence<sizeof...(Detectors)>{});
            }
            bool detect(const ofBaseHasPixels &pix) {
                return detectWithCIImage(toCIImage(pix));
            }
            bool detect(IOSurfaceRef surface) {
                return detectWithCIImage(toCIImage(surface));
            }
            bool detect(CVPixelBufferRef pix) {
                return detectWithCIImage(toCIImage(pix));
            }
            
            template <std::size_t n>
            auto getResult() const
                -> typename std::enable_if<
                    n < sizeof...(Detectors),
                    const typename std::tuple_element<
                        n,
                        Results
                    >::type &
                >::type
            { return std::get<n>(results); }
                                    
        protected:
            
            DetectorsTuple detectors;
            Results results;
            Handler *handler;
        };
    };
};

template <typename ... Detectors>
using ofxVisionMultipleDetector = ofx::Vision::MultipleDetector<Detectors ...>;
