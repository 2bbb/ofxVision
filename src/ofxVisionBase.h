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
        struct Base {
            virtual ~Base();
            virtual void setup();
        protected:
            Handler *handler;
        };
        
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
            
            bool detectWithCIImage(ofxVisionCIImage *image)
            {
                Requests requests;
                std::vector<BaseRequest *> requests_vec;
                create_requests(requests, requests_vec);
                
                auto success = detectMultiple(handler, requests_vec, image);
                if(!success) {
                    ofLogError(__func__) << "What";
                    return false;
                }
                set_results(requests);
                return true;
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
            {
                return std::get<n>(results);
            }
                                    
        protected:
            template <std::size_t i = 0>
            auto create_requests(Requests &requests,
                                 std::vector<BaseRequest *> &vs)
                -> typename std::enable_if<
                    i < sizeof...(Detectors) - 1,
                    void
                >::type
            {
                std::get<i>(requests) = std::get<i>(detectors).createRequest();
                vs.push_back((BaseRequest *)std::get<i>(requests));
                create_requests<i + 1>(requests, vs);
            }

            template <std::size_t i>
            auto create_requests(Requests &requests,
                                 std::vector<BaseRequest *> &vs)
                -> typename std::enable_if<
                    i == sizeof...(Detectors) - 1,
                    void
                >::type
            {
                std::get<i>(requests) = std::get<i>(detectors).createRequest();
                vs.push_back((BaseRequest *)std::get<i>(requests));
            }

            template <std::size_t i = 0>
            auto set_results(Requests &requests)
                -> typename std::enable_if<
                    i < sizeof...(Detectors) - 1,
                    void
                >::type
            {
                std::get<i>(results) = std::get<i>(detectors).createResult(std::get<i>(requests));
                set_results<i + 1>(requests);
            }

            template <std::size_t i>
            auto set_results(Requests &requests)
                -> typename std::enable_if<
                    i == sizeof...(Detectors) - 1,
                    void
                >::type
            {
                std::get<i>(results) = std::get<i>(detectors).createResult(std::get<i>(requests));
            }
            
            DetectorsTuple detectors;
            Results results;
            Handler *handler;
        };
    };
};

template <typename ... Detectors>
using ofxVisionMultipleDetector = ofx::Vision::MultipleDetector<Detectors ...>;
