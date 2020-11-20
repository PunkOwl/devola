#pragma once

#include "ofMain.h"
#include "ofxPostGlitch.h"
#include "ofxNetwork.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
        
        ofxUDPManager       UDPConnection;
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
        float              cameraScaleFactor = 1;
        float              cameraScaleUnit = 0.01;
        
        int camWidth;
        int camHeight;
        
        bool isBlankMode;
        bool isGlitchMode;
    
    private:
        
        void initMode();
        void switchMode(int key);
        void cameraScaleKey(int key);
        void UDPHandler();
        
		
};
