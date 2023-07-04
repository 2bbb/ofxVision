//
//  ofxVisionObservation.h
//
//  Created by 2bit on 2023/06/30.
//

#ifndef ofxVisionObservation_h
#define ofxVisionObservation_h

#include "ofVectorMath.h"
#include "ofRectangle.h"
#include "ofImage.h"

#include <memory>
#include <array>
#include <vector>
#include <string>
#include <functional>

namespace ofx {
    namespace Vision {
        namespace Observation {
            namespace detail {
                template <typename type>
                struct inline_static_variable {
                    static type &get() {
                        static type _;
                        return _;
                    }
                };
                
                struct dummy_ref {
                    template <typename type>
                    operator type &() {
                        return inline_static_variable<type>::get();
                    }
                };
            };
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
            struct DetectedObject : Base {
                ofRectangle boundingBox;
                FloatPixelBuffer segmentationMask;
            };
            struct Rectangle : DetectedObject {
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
                        switch(n) {
                            case 0: return cmc;
                            case 1: return mp;
                            case 2: return ip;
                            case 3: return tip;
                            default: return detail::dummy_ref{};
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
                        switch(n) {
                            case 0: return mcp;
                            case 1: return pip;
                            case 2: return dip;
                            case 3: return tip;
                            default: return detail::dummy_ref{};
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
                    switch(n) {
                        case 0: return index;
                        case 1: return middle;
                        case 2: return ring;
                        case 3: return little;
                        default: return detail::dummy_ref{};
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
                        switch(i) {
                            case 0: return leftEar;
                            case 1: return leftEye;
                            case 2: return nose;
                            case 3: return rightEye;
                            case 4: return rightEar;
                            case 5: return neck;
                            default: return detail::dummy_ref{};
                        }
                    }
                    std::size_t size() const { return 6; }
                } head;
                struct Arm {
                    RecognizedPoint shoulder;
                    RecognizedPoint elbow;
                    RecognizedPoint wrist;
                    const RecognizedPoint &operator[](std::size_t i) const {
                        switch(i) {
                            case 0: return shoulder;
                            case 1: return elbow;
                            case 2: return wrist;
                            default: return detail::dummy_ref{};
                        }
                    }
                    std::size_t size() const { return 3; }
                };
                struct Arms {
                    Arm left;
                    Arm right;
                    const Arm &operator[](std::size_t i) {
                        switch(i) {
                            case 0: return left;
                            case 1: return right;
                            default: return detail::dummy_ref{};
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
                        switch(i) {
                            case 0: return hip;
                            case 1: return knee;
                            case 2: return ankle;
                            default: return detail::dummy_ref{};
                        }
                    }
                    std::size_t size() const { return 3; }
                };
                struct Legs {
                    Leg left;
                    Leg right;
                    const Leg &operator[](std::size_t i) const {
                        switch(i) {
                            case 0: return left;
                            case 1: return right;
                            default: return detail::dummy_ref{};
                        }
                    }
                    std::size_t size() const { return 2; }
                } legs;
            }; // struct BodyPose
            
            using IndexPath = std::vector<std::size_t>;
            
            struct Contour {
                float aspectRatio;
                IndexPath indexPath;
                std::vector<glm::vec2> normalizedPoints;
                std::vector<std::shared_ptr<Contour>> childContours;
            };
            
            struct Contours : Base {
                std::size_t contourCount;
                std::vector<std::shared_ptr<Contour>> topLevelContours;
            }; // struct Contours
            
            enum class PointsClassification {
                Disconnected,
                OpenPath,
                ClosedPath,
                NotImplemented
            };
            
            struct FaceLandmarks {
                float confidence{-1.0f};
            };
            
            struct FaceLandmarkRegion2D {
                PointsClassification pointsClassification;
                std::vector<glm::vec2> normalizedPoints;
            };
            
            struct FaceLandmarks2D : FaceLandmarks {
                FaceLandmarkRegion2D allPoints;
                FaceLandmarkRegion2D faceContour;
                
                FaceLandmarkRegion2D leftEye;
                FaceLandmarkRegion2D leftEyebrow;
                FaceLandmarkRegion2D leftPupil;

                FaceLandmarkRegion2D rightEye;
                FaceLandmarkRegion2D rightEyebrow;
                FaceLandmarkRegion2D rightPupil;
                
                FaceLandmarkRegion2D nose;
                FaceLandmarkRegion2D noseCrest;
                
                FaceLandmarkRegion2D medianLine;
                FaceLandmarkRegion2D outerLips;
                FaceLandmarkRegion2D innerLips;
                const FaceLandmarkRegion2D &operator[](std::size_t n) const {
                    static FaceLandmarkRegion2D dummy;
                    switch(n) {
                        case 0: return allPoints;
                        case 1: return faceContour;
                        case 2: return leftEye;
                        case 3: return leftEyebrow;
                        case 4: return leftPupil;
                        case 5: return rightEye;
                        case 6: return rightEyebrow;
                        case 7: return rightPupil;
                        case 8: return nose;
                        case 9: return noseCrest;
                        case 10: return medianLine;
                        case 11: return outerLips;
                        case 12: return innerLips;
                        default: return dummy;
                    }
                }
                std::size_t size() const { return 13; };
            };
            
            struct Face : DetectedObject {
                FaceLandmarks2D landmarks;
                
                float roll;
                float pitch;
                float yaw;
                
                float faceCaptureQuality;
            };
        }
    };
};

#endif /* ofxVisionObservation_h */
