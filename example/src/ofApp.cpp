#include "ofxVision.h"
#include "ofMain.h"

class ofApp : public ofBaseApp {
    ofVideoGrabber grabber;
    ofImage prev;
    
    template <typename RequestType>
    struct RequestAndObservation : RequestType {
        typename RequestType::ResultType result;
        template <typename ... arguments>
        void detect(arguments && ... args) {
            result = RequestType::detect(std::forward<arguments>(args) ...);
        }
    };
    
#if OFX_VISION_VERSION_CHECK(12, 0)
    RequestAndObservation<ofxVisionPersonSegmentation> person;
#endif
    
#if OFX_VISION_VERSION_CHECK_X(10, 15)
    RequestAndObservation<ofxVisionAttentionBasedSaliencyImage> att_saliency;
    RequestAndObservation<ofxVisionObjectnessBasedSaliencyImage> obj_saliency;
#endif

#if OFX_VISION_VERSION_CHECK(11, 0)
    RequestAndObservation<ofxVisionDetectHumanHandPose> handpose;
    RequestAndObservation<ofxVisionDetectHumanBodyPose> bodypose;
    RequestAndObservation<ofxVisionGenerateOpticalFlow> optical;
    RequestAndObservation<ofxVisionDetectContours> contours;
#endif

    RequestAndObservation<ofxVisionDetectRectangles> rectangles;
    RequestAndObservation<ofxVisionDetectFaceLandmarks> face_landmarks;
#if OFX_VISION_VERSION_CHECK(11, 0)
    ofxVisionMultipleDetector<
        ofxVisionDetectHumanHandPose,
        ofxVisionDetectFaceLandmarks
    > multiple;
#elif OFX_VISION_VERSION_CHECK_X(10, 13)
    ofxVisionMultipleDetector<
        ofxVisionDetectRectangles,
        ofxVisionDetectFaceLandmarks
    > multiple;
#endif
    
    ofShader opticalShader;
    ofShader segmentationShader;
    
    const int num_mode = 10;
//    int mode = num_mode - 1;
    int mode = 0;
public:
	void setup() {
        grabber.setDeviceID(1);
        grabber.setup(1280, 720);
        
#if OFX_VISION_VERSION_CHECK(12, 0)
        person.setup();
        person.setQualityLevel(ofxVisionPersonSegmentationQualityLevel::Balanced);
        segmentationShader.load("shaders/segmentation");
#endif
#if OFX_VISION_VERSION_CHECK_X(10, 15)
        att_saliency.setup();
        obj_saliency.setup();
#endif
        
#if OFX_VISION_VERSION_CHECK(11, 0)
        contours.setup();
#endif
        rectangles.setup();
        face_landmarks.setup();
        
#if OFX_VISION_VERSION_CHECK(11, 0)
        optical.setup();
        opticalShader.load("shaders/optical");
        
        handpose.setup(16ul);
        bodypose.setup();
#endif
        
        multiple.setup();
        
        ofEnableAlphaBlending();
        ofSetBackgroundColor(0);
	}
    void update() {
        ofSetWindowTitle(ofToString(ofGetFrameRate(), 3));
        grabber.update();
        if(grabber.isFrameNew()) {
            switch(mode) {
                case 0:
#if OFX_VISION_VERSION_CHECK(12, 0)
                    person.detect(grabber);
#endif
                    break;
                case 1:
#if OFX_VISION_VERSION_CHECK_X(10, 15)
                    att_saliency.detect(grabber);
#endif
                    break;
                case 2:
#if OFX_VISION_VERSION_CHECK_X(10, 15)
                    obj_saliency.detect(grabber);
#endif
                    break;
                case 3:
#if OFX_VISION_VERSION_CHECK(11, 0)
                    handpose.detect(grabber);
#endif
                    break;
                case 4:
#if OFX_VISION_VERSION_CHECK(11, 0)
                    bodypose.detect(grabber);
#endif
                    break;
                case 5:
#if OFX_VISION_VERSION_CHECK(11, 0)
                    contours.detect(grabber);
#endif
                    break;
                case 6:
#if OFX_VISION_VERSION_CHECK(11, 0)
                    if(prev.isAllocated()) {
                        if(ofGetFrameNum() % 6 == 0) optical.setBaseImage(prev);
                        optical.detect(grabber);
                    }
#endif
                    break;
                case 7:
                    rectangles.detect(grabber);
                    break;
                case 8:
                    face_landmarks.detect(grabber);
                    break;
                case 9:
                    if(!multiple.detect(grabber)) {
                        ofLogError("multiple.detect") << "failed";
                    }
                    break;
            }
        }
    }
    
#if OFX_VISION_VERSION_CHECK(12, 0)
    void drawPersonSegmentation() {
        ofEnableAlphaBlending();
        if(person.result) {
            ofSetColor(255, 255, 255);
            if(prev.isAllocated()) prev.draw(0, 0, ofGetWidth(), ofGetHeight());
            segmentationShader.begin();
            segmentationShader.setUniform2f("size", ofGetWidth(), ofGetHeight());
            segmentationShader.setUniformTexture("base", grabber.getTexture(), 2);
            person.result->draw(0, 0, ofGetWidth(), ofGetHeight());
            segmentationShader.end();
        }
    }
#endif
    
#if OFX_VISION_VERSION_CHECK_X(10, 15)
    void drawObjectnessSaliency() {
        const glm::vec2 s{ofGetWidth(), ofGetHeight()};
        auto &attention = obj_saliency.result;
        if(attention.pixels) {
            ofSetColor(255, 0, 0, 128);
            attention.pixels->draw(0, 0, ofGetWidth(), ofGetHeight());
            ofSetColor(0, 255, 0);
            for(auto &&o : attention.salientObjects) {
                ofDrawLine(o.topLeft * s, o.topRight * s);
                ofDrawLine(o.topRight * s, o.bottomRight * s);
                ofDrawLine(o.bottomRight * s, o.bottomLeft * s);
                ofDrawLine(o.bottomLeft * s, o.topLeft * s);
            }
        }
    }
#endif
    
#if OFX_VISION_VERSION_CHECK_X(10, 15)
    void drawAttentionSaliency() {
        const glm::vec2 s{ofGetWidth(), ofGetHeight()};
        auto &attention = att_saliency.result;
        if(attention.pixels) {
            ofSetColor(0, 255, 0, 128);
            attention.pixels->draw(0, 0, ofGetWidth(), ofGetHeight());
            ofSetColor(0, 0, 255);
            for(auto &&o : attention.salientObjects) {
                ofDrawLine(o.topLeft * s, o.topRight * s);
                ofDrawLine(o.topRight * s, o.bottomRight * s);
                ofDrawLine(o.bottomRight * s, o.bottomLeft * s);
                ofDrawLine(o.bottomLeft * s, o.topLeft * s);
            }
        }
    }
#endif

#if OFX_VISION_VERSION_CHECK(11, 0)
    void drawOpticalFlow() {
        auto &flow = optical.result;
        if(flow.isAllocated()) {
            ofSetColor(255, 255, 255, 128);
            opticalShader.begin();
            opticalShader.setUniformTexture("base", grabber.getTexture(), 1);
            flow.draw(0, 0, ofGetWidth(), ofGetHeight());
            opticalShader.end();
        }
    }
#endif
        
#if OFX_VISION_VERSION_CHECK(11, 0)
    void drawHandTracking() {
        const glm::vec2 s{ofGetWidth(), ofGetHeight()};
        auto &hands = handpose.result;
        for(auto &&hand : hands) {
            const float size = 3.0f;
            ofSetColor(255, 0, (int)hand.chirality == 1 ? 255 : 0);
            if(hand.wrist) {
                ofDrawCircle(hand.wrist.position * s, size);
            }
            for(auto i = 0; i < 4; ++i) {
                if(hand.thumb[i]) {
                    ofDrawCircle(hand.thumb[i].position * s, size);
                }
                for(auto j = 0; j < 4; ++j) {
                    if(hand[j][i]) {
                        ofDrawCircle(hand[j][i].position * s, size);
                    }
                }
            }
            
            ofSetColor(0, 255, (int)hand.chirality == 1 ? 255 : 0);
            if(hand.wrist) {
                if(hand.thumb[0]) {
                    ofDrawLine(hand.wrist.position * s, hand.thumb[0].position * s);
                }
                for(auto j = 0; j < 4; ++j) {
                    if(hand[j][0]) {
                        ofDrawLine(hand.wrist.position * s, hand[j][0].position * s);
                    }
                }
            }
            for(auto i = 0; i < 3; ++i) {
                if(hand.thumb[i] && hand.thumb[i + 1]) {
                    ofDrawLine(hand.thumb[i].position * s, hand.thumb[i + 1].position * s);
                }
                for(auto j = 0; j < 4; ++j) {
                    if(hand[j][i]) {
                        ofDrawLine(hand[j][i].position * s, hand[j][i + 1].position * s);
                    }
                }
            }
        }
    } // void drawHandTracking()
#endif
        
#if OFX_VISION_VERSION_CHECK(11, 0)
    void drawBodyTracking() {
        const glm::vec2 s{ofGetWidth(), ofGetHeight()};
        auto &bodies = bodypose.result;
        ofSetColor(255, 0, 0);
        for(auto &&body : bodies) {
            const float size = 10.0f;
            for(auto i = 0; i < body.head.size(); ++i) {
                if(0.0 < body.head[i].confidence) {
                    ofDrawCircle(body.head[i].position * s, size);
                }
            }
            if(0.0 < body.waist.confidence) ofDrawCircle(body.waist.position * s, size);
            for(auto i = 0; i < body.arms.size(); ++i) {
                for(auto j = 0; j < body.arms[i].size(); ++j) {
                    if(0.0 < body.arms[i][j].confidence) {
                        ofDrawCircle(body.arms[i][j].position * s, size);
                    }
                }
            }
            for(auto i = 0; i < body.legs.size(); ++i) {
                for(auto j = 0; j < body.legs[i].size(); ++j) {
                    if(0.0 < body.legs[i][j].confidence) {
                        ofDrawCircle(body.legs[i][j].position * s, size);
                    }
                }
            }
        }
    } // void drawHandTracking()
#endif
        
    void drawContour(std::shared_ptr<ofxVision::Observation::Contour> contour) {
        ofMesh mesh;
        mesh.setMode(OF_PRIMITIVE_LINE_STRIP);
        for(const auto &p : contour->normalizedPoints) {
            mesh.addVertex({p.x, p.y, 0.0f});
        }
        mesh.draw();
        for(auto child : contour->childContours) {
            drawContour(child);
        }
    }
    
#if OFX_VISION_VERSION_CHECK(11, 0)
    void drawContours() {
        ofPushMatrix();
        ofScale(ofGetWidth(), ofGetHeight());
        ofSetColor(255, 0, 0);
        const auto &contours = this->contours.result;
        for(auto contour : contours.topLevelContours) {
            drawContour(contour);
        }
        ofPopMatrix();
    } // void drawContours()
#endif
        
    void drawRectangles() {
        ofPushMatrix();
        ofScale(ofGetWidth(), ofGetHeight());
        const auto &rectangles = this->rectangles.result;
        ofSetColor(255, 0, 0);
        for(const auto &rect : rectangles) {
            ofDrawLine(rect.topLeft, rect.topRight);
            ofDrawLine(rect.topRight, rect.bottomRight);
            ofDrawLine(rect.bottomRight, rect.bottomLeft);
            ofDrawLine(rect.bottomLeft, rect.topLeft);
        }
        ofPopMatrix();
    } // void drawRectangles()
        
        
    void drawLandmarksRegion(const ofxVision::Observation::FaceLandmarkRegion2D &landmark,
                             glm::vec2 scale,
                             float size)
    {
        for(const auto &p : landmark.normalizedPoints) {
            ofDrawCircle(p * scale, size);
        }
    }
    void drawFaceLandmarks() {
        const auto &faces = face_landmarks.result;
        auto w = ofGetWidth();
        auto h = ofGetHeight();
        for(auto &face : faces) {
            ofPushMatrix();
            const auto scale = glm::vec2{face.boundingBox.width * w, face.boundingBox.height * h};
            ofTranslate(face.boundingBox.x * w, face.boundingBox.y * h);
            ofSetColor(255, 0, 0);
            for(auto i = 1ul; i < face.landmarks.size(); ++i) {
                const auto &landmarks = face.landmarks[i];
                drawLandmarksRegion(landmarks, scale, 5.0f);
            }
            ofPopMatrix();
        }
    } // void drawFaceLandmarks()
        
    void drawMultiple() {
#if OFX_VISION_VERSION_CHECK(11, 0)
        // handpose
        {
            const glm::vec2 s{ofGetWidth(), ofGetHeight()};
            auto &hands = multiple.getResult<0>();
            for(auto &&hand : hands) {
                const float size = 3.0f;
                ofSetColor(255, 0, (int)hand.chirality == 1 ? 255 : 0);
                if(hand.wrist) {
                    ofDrawCircle(hand.wrist.position * s, size);
                }
                for(auto i = 0; i < 4; ++i) {
                    if(hand.thumb[i]) {
                        ofDrawCircle(hand.thumb[i].position * s, size);
                    }
                    for(auto j = 0; j < 4; ++j) {
                        if(hand[j][i]) {
                            ofDrawCircle(hand[j][i].position * s, size);
                        }
                    }
                }
                
                ofSetColor(0, 255, (int)hand.chirality == 1 ? 255 : 0);
                if(hand.wrist) {
                    if(hand.thumb[0]) {
                        ofDrawLine(hand.wrist.position * s, hand.thumb[0].position * s);
                    }
                    for(auto j = 0; j < 4; ++j) {
                        if(hand[j][0]) {
                            ofDrawLine(hand.wrist.position * s, hand[j][0].position * s);
                        }
                    }
                }
                for(auto i = 0; i < 3; ++i) {
                    if(hand.thumb[i] && hand.thumb[i + 1]) {
                        ofDrawLine(hand.thumb[i].position * s, hand.thumb[i + 1].position * s);
                    }
                    for(auto j = 0; j < 4; ++j) {
                        if(hand[j][i]) {
                            ofDrawLine(hand[j][i].position * s, hand[j][i + 1].position * s);
                        }
                    }
                }
            }
        }
#else
        // rectangles
        {
            ofPushMatrix();
            ofScale(ofGetWidth(), ofGetHeight());
            const auto &rectangles = multiple.getResult<0>();
            ofSetColor(255, 0, 0);
            for(const auto &rect : rectangles) {
                ofDrawLine(rect.topLeft, rect.topRight);
                ofDrawLine(rect.topRight, rect.bottomRight);
                ofDrawLine(rect.bottomRight, rect.bottomLeft);
                ofDrawLine(rect.bottomLeft, rect.topLeft);
            }
            ofPopMatrix();
        }
#endif
        // face landmarks
        {
            const auto &faces = multiple.getResult<1>();
            auto w = ofGetWidth();
            auto h = ofGetHeight();
            for(auto &face : faces) {
                ofPushMatrix();
                const auto scale = glm::vec2{face.boundingBox.width * w, face.boundingBox.height * h};
                ofTranslate(face.boundingBox.x * w, face.boundingBox.y * h);
                ofSetColor(255, 0, 0);
                for(auto i = 1ul; i < face.landmarks.size(); ++i) {
                    const auto &landmarks = face.landmarks[i];
                    drawLandmarksRegion(landmarks, scale, 5.0f);
                }
                ofPopMatrix();
            }
        }
    } // void drawMultiple

	void draw() {
        const glm::vec2 s = { ofGetWidth(), ofGetHeight() };
        ofSetColor(255, 255, 255);
        grabber.draw(0, 0, ofGetWidth(), ofGetHeight());
        
        switch(mode) {
            case 0:
#if OFX_VISION_VERSION_CHECK(12, 0)
                drawPersonSegmentation();
                ofDrawBitmapStringHighlight("person segmentation", 20, 20);
                ofDrawBitmapStringHighlight("'a': Accurate", 20, 40);
                ofDrawBitmapStringHighlight("'b': Balanced", 20, 60);
                ofDrawBitmapStringHighlight("'f': Fast", 20, 80);
#else
                ofDrawBitmapStringHighlight("person segmentation is NOT Available on this macOS version need 12.0~", 20, 20);
#endif
                break;
            case 1:
#if OFX_VISION_VERSION_CHECK_X(10, 15)
                drawAttentionSaliency();
                ofDrawBitmapStringHighlight("attention based saliency", 20, 20);
#else
                ofDrawBitmapStringHighlight("attention based saliency is NOT Available on this macOS version need 10.15~", 20, 20);
#endif
                break;
            case 2:
#if OFX_VISION_VERSION_CHECK_X(10, 15)
                drawObjectnessSaliency();
                ofDrawBitmapStringHighlight("objectness based saliency", 20, 20);
#else
                ofDrawBitmapStringHighlight("objectness based saliency is NOT Available on this macOS version need 10.15~", 20, 20);
#endif
                break;
            case 3:
#if OFX_VISION_VERSION_CHECK(11, 0)
                drawHandTracking();
                ofDrawBitmapStringHighlight("hand pose detection", 20, 20);
#else
                ofDrawBitmapStringHighlight("hand pose detection is NOT Available on this macOS version need 11.0~", 20, 20);
#endif
                break;
            case 4:
#if OFX_VISION_VERSION_CHECK(11, 0)
                drawBodyTracking();
                ofDrawBitmapStringHighlight("body pose detection", 20, 20);
#else
                ofDrawBitmapStringHighlight("body pose detection is NOT Available on this macOS version need 11.0~", 20, 20);
#endif
                break;
            case 5:
#if OFX_VISION_VERSION_CHECK(11, 0)
                drawContours();
                ofDrawBitmapStringHighlight("contours detection", 20, 20);
#else
                ofDrawBitmapStringHighlight("contours detection is NOT Available on this macOS version need 11.0~", 20, 20);
#endif
                break;
            case 6:
#if OFX_VISION_VERSION_CHECK(11, 0)
                drawOpticalFlow();
                ofDrawBitmapStringHighlight("optical flow", 20, 20);
#else
                ofDrawBitmapStringHighlight("optical flow is NOT Available on this macOS version, need 11.0~", 20, 20);
#endif
                break;
            case 7:
                drawRectangles();
                ofDrawBitmapStringHighlight("rectangles detection", 20, 20);
                break;
            case 8:
                drawFaceLandmarks();
                ofDrawBitmapStringHighlight("face landmark detection", 20, 20);
                break;
            case 9:
                drawMultiple();
#if OFX_VISION_VERSION_CHECK(11, 0)
                ofDrawBitmapStringHighlight("hand tracking & face landmark detection", 20, 20);
#else
                ofDrawBitmapStringHighlight("rectangles & face landmark detection", 20, 20);
#endif
                break;
        }

        if(grabber.isFrameNew()) {
            prev.setFromPixels(grabber.getPixels());
        }
	} // draw

	void keyPressed(int key) {
        switch(key) {
            case 'R':
                opticalShader.load("shaders/optical");
                segmentationShader.load("shaders/segmentation");
                break;
            case OF_KEY_LEFT:
                mode = (mode + num_mode - 1) % num_mode;
                break;
            case OF_KEY_RIGHT:
                mode = (mode + 1) % num_mode;
                break;
            case 'a':
                person.setQualityLevel(ofxVision::PersonSegmentationQualityLevel::Accurate);
                break;
            case 'b':
                person.setQualityLevel(ofxVision::PersonSegmentationQualityLevel::Balanced);
                break;
            case 'f':
                person.setQualityLevel(ofxVision::PersonSegmentationQualityLevel::Fast);
                break;
            default: break;
        }
    }
};

int main() {
    ofGLFWWindowSettings settings;
    settings.setGLVersion(4, 1);
    settings.resizable = false;
    
    settings.setSize(1280, 720);
    std::shared_ptr<ofAppBaseWindow> window = ofCreateWindow(settings);
    std::shared_ptr<ofApp> app(new ofApp());
    ofRunApp(window, app);
    
    ofRunMainLoop();
}
