#include "ofxVision.h"
#include "ofMain.h"

class ofApp : public ofBaseApp {
    ofVideoGrabber grabber;
    ofx::Vision::PersonSegmentation person;
    std::shared_ptr<ofImage> image;
public:
	void setup() {
        grabber.setDeviceID(1);
        grabber.setup(1280, 720);
        person.setup();
	}
	void update() {
        ofSetWindowTitle(ofToString(ofGetFrameRate(), 3));
        grabber.update();
        if(grabber.isFrameNew()) {
            image = person.detect(grabber);
        }
	}
	void draw() {
        if(image) image->draw(0, 0, 1280, 720);
	}

	void keyPressed(int key) {
        switch(key) {
            case '0': person.setQualityLevel(ofx::Vision::PersonSegmentation::QualityLevel::Fast);
                break;
            case '1': person.setQualityLevel(ofx::Vision::PersonSegmentation::QualityLevel::Balanced);
                break;
            case '2': person.setQualityLevel(ofx::Vision::PersonSegmentation::QualityLevel::Accurate);
                break;
        }
    }
};

int main() {
    ofGLFWWindowSettings settings;
    settings.setGLVersion(3, 3);
    settings.resizable = false;
    
    settings.setSize(1280, 720);
    std::shared_ptr<ofAppBaseWindow> window = ofCreateWindow(settings);
    std::shared_ptr<ofApp> app(new ofApp());
    ofRunApp(window, app);
    
    ofRunMainLoop();
}
