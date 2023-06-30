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
    
    ofShader opticalShader;
    
    int mode = 0;
public:
	void setup() {
        grabber.setDeviceID(1);
        grabber.setup(1280, 720);
        person.setup();
        att_saliency.setup();
        obj_saliency.setup();
        
        optical.setup();
        opticalShader.load("shaders/optical");
        
        handpose.setup(16ul);
        bodypose.setup();
        ofEnableAlphaBlending();
	}
    void update() {
        ofSetWindowTitle(ofToString(ofGetFrameRate(), 3));
        grabber.update();
        if(grabber.isFrameNew()) {
            person.detect(grabber);
            att_saliency.detect(grabber);
            obj_saliency.detect(grabber);
            handpose.detect(grabber);
            bodypose.detect(grabber);
            if(prev.isAllocated()) {
                optical.detect(prev, grabber);
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

    
	void draw() {
        const glm::vec2 s = { ofGetWidth(), ofGetHeight() };
        ofSetColor(255, 255, 255);
        grabber.draw(0, 0, ofGetWidth(), ofGetHeight());
        
//        drawOpticalFlow();
//        drawPersonSegmentation();
//        
//        drawAttentionSaliency();
//        drawObjectnessSaliency();
//        
        drawHandTracking();
        drawBodyTracking();
	}

	void keyPressed(int key) {
        switch(key) {
            case 'R':
                opticalShader.load("shaders/optical");
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
