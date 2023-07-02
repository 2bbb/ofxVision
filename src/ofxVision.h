#pragma once

#include "./Detectors/ofxVisionPersonSegmentation.h"

#include "./Detectors/ofxVisionDetectHumanHandPose.h"
#include "./Detectors/ofxVisionDetectHumanBodyPose.h"
#include "./Detectors/ofxVisionDetectContours.h"
#include "./Detectors/ofxVisionDetectRectangles.h"
#include "./Detectors/ofxVisionDetectFaceLandmarks.h"

#include "./Detectors/ofxVisionAttentionBasedSaliencyImage.h"
#include "./Detectors/ofxVisionObjectnessBasedSaliencyImage.h"

#include "./Detectors/ofxVisionGenerateOpticalFlow.h"

#include "ofxVisionMultipleDetector.h"

namespace ofxVision = ofx::Vision;

#if !OFX_VISION_VERSION_CHECK_X(10, 13)
#   error ofxVision needs macOS 10.13 or newer.
#endif
