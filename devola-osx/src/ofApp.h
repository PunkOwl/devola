#pragma once

#include "ofMain.h"
#include "ofxPostGlitch.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        ofVideoGrabber      vidGrabber;
        ofPixels            videoInverted;
        ofTexture           videoTexture;
        ofFbo               myFbo;
        ofxPostGlitch       myGlitch;
        ofEasyCam           camera;
        ofTexture           texture;
        ofImage             lenna;
        bool                bDrawLenna;
        bool                bShowHelp;
    
        int camWidth;
        int camHeight;
        
        bool isBlankMode;
        bool isGlitchMode;
    
    private:
        
        void initMode();
        void switchMode(int key);
        
		
};
