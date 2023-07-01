#pragma once

#include "ofxVisionPersonSegmentation.h"

#include "ofxVisionDetectHumanHandPose.h"
#include "ofxVisionDetectHumanBodyPose.h"
#include "ofxVisionDetectContours.h"
#include "ofxVisionDetectRectangles.h"
#include "ofxVisionDetectFaceLandmarks.h"

#include "ofxVisionAttentionBasedSaliencyImage.h"
#include "ofxVisionObjectnessBasedSaliencyImage.h"

#include "ofxVisionGenerateOpticalFlow.h"

namespace ofxVision = ofx::Vision;

#if !OFX_VISION_VERSION_CHECK_X(10, 13)
#   error ofxVision needs macOS 10.13 or newer.
#endif
