//
//  ofxVisionMultipleDetector.m
//
//  Created by 2bit on 2023/07/03.
//

#import "ofxVisionMultipleDetector.h"
#import "ofxVisionObjCUtility.h"

namespace ofx {
    namespace Vision {
        bool detectMultiple(void *handler,
                            std::vector<void *> &request_vec,
                            void *image)
        {
            NSMutableArray<VNImageBasedRequest *> *requests = [NSMutableArray array];
            for(auto i = 0; i < request_vec.size(); ++i) {
                [requests addObject:(VNImageBasedRequest *)request_vec[i]];
            }
            NSError *err = nil;
            BOOL success= [(Handler *)handler performRequests:requests
                                                    onCIImage:(CIImage *)image
                                                  orientation:kCGImagePropertyOrientationUp
                                                        error:&err];
            if(err) {
                ofLogError("ofxVisionMultipleDetector::detect") << err.description.UTF8String;
                return false;
            }
            if(!success) {
                ofLogError("ofxVisionMultipleDetector::detect") << "what";
                return false;
            }

            return true;
        }
    };
};
