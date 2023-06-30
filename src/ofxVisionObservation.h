//
//  ofxVisionObservation.h
//  example
//
//  Created by 2bit on 2023/06/30.
//

#ifndef ofxVisionObservation_h
#define ofxVisionObservation_h

#include "ofVectorMath.h"
#include "ofRectangle.h"
#include "ofImage.h"

#include <memory>

namespace ofx {
    namespace Vision {
        namespace Observation {
            struct Base {
                float confidence;
                std::string uuid;
            };
            struct PixelBuffer : Base {
                std::shared_ptr<ofImage> pixels;
                std::string featureName;
            };
            struct FloatPixelBuffer : Base {
                std::shared_ptr<ofFloatImage> pixels;
                std::string featureName;
            };
            struct Object : Base {
                ofRectangle boundingBox;
                FloatPixelBuffer segmentationMask;
            };
            struct Rectangle : Object {
                glm::vec2 topLeft;
                glm::vec2 topRight;
                glm::vec2 bottomRight;
                glm::vec2 bottomLeft;
            };
            struct SaliencyImage : FloatPixelBuffer {
                std::vector<Observation::Rectangle> salientObjects;
            };
            
            struct DetectedPoint {
                glm::vec2 position;
                float confidence{-1.0f};
                operator bool() const { return 0.0f < confidence; }
            };
            struct RecognizedPoint : DetectedPoint {
                std::string identifier;
            };
            
            struct HandPose {
                enum class Chirality {
                    Left = -1,
                    Right = 1,
                    Unknown = 0
                } chirality;
                
                struct ThumbJointGroup {
                    RecognizedPoint tip;
                    RecognizedPoint ip;
                    RecognizedPoint mp;
                    RecognizedPoint cmc;
                    const RecognizedPoint &operator[](std::size_t n) const {
                        static RecognizedPoint dummy;
                        switch(n) {
                            case 0: return cmc;
                            case 1: return mp;
                            case 2: return ip;
                            case 3: return tip;
                            default: return dummy;
                        }
                    }
                    std::size_t size() const { return 4; }
                };
                struct JointGroup {
                    RecognizedPoint tip;
                    RecognizedPoint dip;
                    RecognizedPoint pip;
                    RecognizedPoint mcp;
                    const RecognizedPoint &operator[](std::size_t n) const {
                        static RecognizedPoint dummy;
                        switch(n) {
                            case 0: return mcp;
                            case 1: return pip;
                            case 2: return dip;
                            case 3: return tip;
                            default: return dummy;
                        }
                    }
                    std::size_t size() const { return 4; }
                };
                ThumbJointGroup thumb;
                JointGroup index;
                JointGroup middle;
                JointGroup ring;
                JointGroup little;
                const JointGroup &operator[](std::size_t n) const {
                    static JointGroup dummy;
                    switch(n) {
                        case 0: return index;
                        case 1: return middle;
                        case 2: return ring;
                        case 3: return little;
                        default: return dummy;
                    }
                }
                std::size_t size() const { return 4; }
                RecognizedPoint wrist;
            }; // struct HandPose
            
            struct BodyPose {
                struct Head {
                    RecognizedPoint leftEar;
                    RecognizedPoint leftEye;
                    RecognizedPoint nose;
                    RecognizedPoint rightEye;
                    RecognizedPoint rightEar;
                    RecognizedPoint neck;
                    const RecognizedPoint &operator[](std::size_t i) const {
                        static RecognizedPoint dummy;
                        switch(i) {
                            case 0: return leftEar;
                            case 1: return leftEye;
                            case 2: return nose;
                            case 3: return rightEye;
                            case 4: return rightEar;
                            case 5: return neck;
                            default: return dummy;
                        }
                    }
                    std::size_t size() const { return 6; }
                } head;
                struct Arm {
                    RecognizedPoint shoulder;
                    RecognizedPoint elbow;
                    RecognizedPoint wrist;
                    const RecognizedPoint &operator[](std::size_t i) const {
                        static RecognizedPoint dummy;
                        switch(i) {
                            case 0: return shoulder;
                            case 1: return elbow;
                            case 2: return wrist;
                            default: return dummy;
                        }
                    }
                    std::size_t size() const { return 3; }
                };
                struct Arms {
                    Arm left;
                    Arm right;
                    const Arm &operator[](std::size_t i) {
                        static Arm dummy;
                        switch(i) {
                            case 0: return left;
                            case 1: return right;
                            default: return dummy;
                        }
                    }
                    std::size_t size() const { return 2; }
                } arms;
                RecognizedPoint waist;
                struct Leg {
                    RecognizedPoint hip;
                    RecognizedPoint knee;
                    RecognizedPoint ankle;
                    const RecognizedPoint &operator[](std::size_t i) const {
                        static RecognizedPoint dummy;
                        switch(i) {
                            case 0: return hip;
                            case 1: return knee;
                            case 2: return ankle;
                            default: return dummy;
                        }
                    }
                    std::size_t size() const { return 3; }
                };
                struct Legs {
                    Leg left;
                    Leg right;
                    const Leg &operator[](std::size_t i) const {
                        static Leg dummy;
                        switch(i) {
                            case 0: return left;
                            case 1: return right;
                            default: return dummy;
                        }
                    }
                    std::size_t size() const { return 2; }
                } legs;
            };
        }
    };
};

#endif /* ofxVisionObservation_h */
