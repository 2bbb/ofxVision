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
    
    RequestAndObservation<ofxVisionPersonSegmentation> person;
    RequestAndObservation<ofxVisionAttentionBasedSaliencyImage> att_saliency;
    RequestAndObservation<ofxVisionObjectnessBasedSaliencyImage> obj_saliency;
    RequestAndObservation<ofxVisionDetectHumanHandPose> handpose;
    RequestAndObservation<ofxVisionDetectHumanBodyPose> bodypose;
    RequestAndObservation<ofxVisionGenerateOpticalFlow> optical;
    RequestAndObservation<ofxVisionDetectContours> contours;
    RequestAndObservation<ofxVisionDetectRectangles> rectangles;
    RequestAndObservation<ofxVisionDetectFaceLandmarks> face_landmarks;

    ofShader opticalShader;
    
    const int num_mode = 9;
    int mode = num_mode - 1;
public:
	void setup() {
        grabber.setDeviceID(1);
        grabber.setup(1280, 720);
        
        person.setup();
        att_saliency.setup();
        obj_saliency.setup();
        contours.setup();
        rectangles.setup();
        face_landmarks.setup();
        optical.setup();
        opticalShader.load("shaders/optical");
        
        handpose.setup(16ul);
        bodypose.setup();
        ofEnableAlphaBlending();
        ofSetBackgroundColor(0);
	}
    void update() {
        ofSetWindowTitle(ofToString(ofGetFrameRate(), 3));
        grabber.update();
        if(grabber.isFrameNew()) {
            switch(mode) {
                case 0:
                    person.detect(grabber);
                    break;
                case 1:
                    att_saliency.detect(grabber);
                    break;
                case 2:
                    obj_saliency.detect(grabber);
                    break;
                case 3:
                    handpose.detect(grabber);
                    break;
                case 4:
                    bodypose.detect(grabber);
                    break;
                case 5:
                    contours.detect(grabber);
                    break;
                case 6:
                    if(prev.isAllocated()) optical.detect(prev, grabber);
                    break;
                case 7:
                    rectangles.detect(grabber);
                    break;
                case 8:
                    face_landmarks.detect(grabber);
                    break;
            }
            prev.setFromPixels(grabber.getPixels());
        }
    }
    
    void drawPersonSegmentation() {
        if(person.result) {
            ofSetColor(0, 0, 255, 128);
            person.result->draw(0, 0, 1280, 720);
        }
    }
    
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
    
    void drawOpticalFlow() {
        auto &flow = optical.result;
#if OFX_VISION_USE_TEXTURE
        if(flow.isAllocated()) {
#else
        if(flow->isAllocated()) {
#endif
            ofSetColor(255, 255, 255, 128);
            opticalShader.begin();
            opticalShader.setUniformTexture("base", grabber.getTexture(), 1);
#if OFX_VISION_USE_TEXTURE
            flow.draw(0, 0, ofGetWidth(), ofGetHeight());
#else
            flow->draw(0, 0, ofGetWidth(), ofGetHeight());
#endif
            opticalShader.end();
        }
    }
    
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
        
    void drawBodyTracking() {
        const glm::vec2 s{ofGetWidth(), ofGetHeight()};
        auto &bodies = bodypose.result;
        for(auto &&body : bodies) {
            const float size = 3.0f;
            for(auto i = 0; i < body.head.size(); ++i) {
                ofDrawCircle(body.head[i].position * s, size);
            }
            ofDrawCircle(body.waist.position * s, size);
            for(auto i = 0; i < body.arms.size(); ++i) {
                for(auto j = 0; j < body.arms[i].size(); ++j) {
                    ofDrawCircle(body.arms[i][j].position * s, size);
                }
            }
            for(auto i = 0; i < body.legs.size(); ++i) {
                for(auto j = 0; j < body.legs[i].size(); ++j) {
                    ofDrawCircle(body.legs[i][j].position * s, size);
                }
            }
        }
    } // void drawHandTracking()

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
	void draw() {
        const glm::vec2 s = { ofGetWidth(), ofGetHeight() };
        ofSetColor(255, 255, 255);
        grabber.draw(0, 0, ofGetWidth(), ofGetHeight());
        
        switch(mode) {
            case 0:
                drawPersonSegmentation();
                ofDrawBitmapStringHighlight("person segmentation", 20, 20);
                break;
            case 1:
                drawAttentionSaliency();
                ofDrawBitmapStringHighlight("attention based saliency", 20, 20);
                break;
            case 2:
                drawObjectnessSaliency();
                ofDrawBitmapStringHighlight("objectness based saliency", 20, 20);
                break;
            case 3:
                drawHandTracking();
                ofDrawBitmapStringHighlight("hand pose detection", 20, 20);
                break;
            case 4:
                drawBodyTracking();
                ofDrawBitmapStringHighlight("body pose detection", 20, 20);
                break;
            case 5:
                drawContours();
                ofDrawBitmapStringHighlight("contours detection", 20, 20);
                break;
            case 6:
                drawOpticalFlow();
                ofDrawBitmapStringHighlight("optical flow", 20, 20);
                break;
            case 7:
                drawRectangles();
                ofDrawBitmapStringHighlight("rectangles detection", 20, 20);
                break;
            case 8:
                drawFaceLandmarks();
                ofDrawBitmapStringHighlight("face landmark detection", 20, 20);
                break;
        }

	}

	void keyPressed(int key) {
        switch(key) {
            case 'R':
                opticalShader.load("shaders/optical");
                break;
            case OF_KEY_LEFT:
                mode = (mode + num_mode - 1) % num_mode;
                break;
            case OF_KEY_RIGHT:
                mode = (mode + 1) % num_mode;
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
