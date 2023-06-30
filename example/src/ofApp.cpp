#include "ofxVision.h"
#include "ofMain.h"

class ofApp : public ofBaseApp {
    ofVideoGrabber grabber;
    ofImage prev;
    
    ofxVisionPersonSegmentation person;
    ofxVisionPersonSegmentation::ResultType segmentation;
    
    ofxVisionAttentionBasedSaliencyImage saliency;
    ofxVisionAttentionBasedSaliencyImage::ResultType attention;
    
    ofxVisionGenerateOpticalFlow optical;
    ofxVisionGenerateOpticalFlow::ResultType flow;
    ofShader opticalShader;
public:
	void setup() {
        grabber.setDeviceID(1);
        grabber.setup(1280, 720);
        person.setup();
        saliency.setup();
        optical.setup();
        opticalShader.load("shaders/optical");
        ofEnableAlphaBlending();
	}
	void update() {
        ofSetWindowTitle(ofToString(ofGetFrameRate(), 3));
        grabber.update();
        if(grabber.isFrameNew()) {
//            segmentation = person.detect(grabber);
//            attention = saliency.detect(grabber);
            if(prev.isAllocated()) {
                flow = optical.detect(prev, grabber);
            }
            prev.setFromPixels(grabber.getPixels());
        }
	}
	void draw() {
        ofSetColor(255, 255, 255);
        grabber.draw(0, 0, ofGetWidth(), ofGetHeight());
        if(segmentation) segmentation->draw(0, 0, 1280, 720);
//        if(attention.pixels) {
//            ofSetColor(255, 0, 0, 128);
//            attention.pixels->draw(0, 0, ofGetWidth(), ofGetHeight());
//            ofSetColor(0, 255, 0);
//            ofLogNotice() << attention.salientObjects.size();
//            glm::vec2 s = { ofGetWidth(), ofGetHeight() };
//            for(auto &&o : attention.salientObjects) {
//                ofDrawLine(o.topLeft * s, o.topRight * s);
//                ofDrawLine(o.topRight * s, o.bottomRight * s);
//                ofDrawLine(o.bottomRight * s, o.bottomLeft * s);
//                ofDrawLine(o.bottomLeft * s, o.topLeft * s);
//            }
//        }
#if OFX_VISION_USE_TEXTURE
        if(flow.isAllocated()) {
            ofSetColor(255, 255, 255, 255);
            opticalShader.begin();
            opticalShader.setUniformTexture("base", grabber.getTexture(), 1);
            flow.draw(0, 0, ofGetWidth(), ofGetHeight());
            opticalShader.end();
        }
#else
        if(flow && flow->isAllocated()) {
            ofSetColor(255, 255, 255, 255);
            opticalShader.begin();
            opticalShader.setUniformTexture("base", grabber.getTexture(), 1);
            flow->draw(0, 0, ofGetWidth(), ofGetHeight());
            opticalShader.end();
        }
#endif
	}

	void keyPressed(int key) {
        switch(key) {
            case '0': person.setQualityLevel(ofxVisionPersonSegmentation::QualityLevel::Fast);
                break;
            case '1': person.setQualityLevel(ofxVisionPersonSegmentation::QualityLevel::Balanced);
                break;
            case '2': person.setQualityLevel(ofxVisionPersonSegmentation::QualityLevel::Accurate);
                break;
            case 'R':
                opticalShader.load("shaders/optical");
                break;
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
