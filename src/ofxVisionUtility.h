//
//  ofxVisionUtility.h
//
//  Created by 2bit on 2023/06/29.
//

#pragma once

#include "ofImage.h"

#import <CoreImage/CoreImage.h>

#include <memory>

std::shared_ptr<ofImage> pixelBufferToOfImage(CVPixelBufferRef pixelBuffer);
CGImageRef ofBaseHasPixelsToCGImageRef(const ofBaseHasPixels &pix);
