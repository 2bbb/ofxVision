#include "ofxVision.h"
#include "ofxIOSurfaceTexture.h"
#include "ofMain.h"

class ofApp : public ofBaseApp {
    ofVideoGrabber grabber;
    ofxIOSurfaceFbo fbo;
    ofx::Vision::PersonSegmentation person;
    std::shared_ptr<ofImage> image;
public:
	void setup() {
        grabber.setDeviceID(1);
        grabber.setup(1280, 720);
        fbo.allocate(640, 360, GL_RGBA);
        person.setup();
	}
	void update() {
        ofSetWindowTitle(ofToString(ofGetFrameRate(), 3));
        grabber.update();
        if(grabber.isFrameNew()) {
            fbo.begin();
            ofSetColor(255);
            grabber.draw(0, 0, fbo.getWidth(), fbo.getHeight());
            fbo.end();
            image = person.detect(fbo.surfaceRef());
        }
	}
	void draw() {
        if(image) image->draw(0, 0, 1280, 720);
	}

	void keyPressed(int key) {}
	void keyReleased(int key) {}
	void mouseMoved(int x, int y) {}
	void mouseDragged(int x, int y, int button) {}
	void mousePressed(int x, int y, int button) {}
	void mouseReleased(int x, int y, int button) {}
	void mouseEntered(int x, int y) {}
	void mouseExited(int x, int y) {}
	void windowResized(int w, int h) {}
	void dragEvent(ofDragInfo dragInfo) {}
	void gotMessage(ofMessage msg) {}
};

// class subApp : public ofBaseApp {
// public:
//     subApp(std::shared_ptr<ofApp> mainApp) : mainApp{mainApp}
//     {};
    
//     void draw() {

//     }
//     void keyPressed(int key) {
//         mainApp->keyPressed(key);
//     }

//     std::shared_ptr<ofApp> mainApp;
// };

int main() {
    ofGLFWWindowSettings settings;
    settings.setGLVersion(3, 3);
    settings.resizable = false;
    
    settings.setSize(1280, 720);
    std::shared_ptr<ofAppBaseWindow> window = ofCreateWindow(settings);
    std::shared_ptr<ofApp> app(new ofApp());
    ofRunApp(window, app);
    
    // settings.setSize(640, 360);
    // settings.shareContextWith = window;
    // std::shared_ptr<ofAppBaseWindow> sub_window = ofCreateWindow(settings);
    // std::shared_ptr<subApp> sub_window_app(new subApp(app));
    // ofRunApp(sub_window, sub_window_app);

    ofRunMainLoop();
}
