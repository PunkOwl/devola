#include "ofApp.h"

ofPolyline line;

//--------------------------------------------------------------
void ofApp::setup(){
    /* Printing the initial screen sizes */
    ofLogNotice() << ofGetWindowPositionX();
    ofLogNotice() << ofGetWindowPositionY();
    ofLogNotice() << ofGetScreenWidth();
    ofLogNotice() << ofGetScreenHeight();
    
    int screenWidth = ofGetScreenWidth();
    int screenHeight = ofGetScreenHeight();
    
    /* Window Title */
    ofSetWindowTitle("PunkOwl Devola v0.1 - vonqo");
    ofBackground(0,0,0);

    /* Camera Setup */
    /* 320 x 240 is constant! */
    camWidth = 320;
    camHeight = 240;
    
    // get back a list of devices.
    vector<ofVideoDevice> devices = vidGrabber.listDevices();

    for(size_t i = 0; i < devices.size(); i++){
        if(devices[i].bAvailable){
            //log the device
            ofLogNotice() << devices[i].id << ": " << devices[i].deviceName;
        } else {
            //log the device and note it as unavailable
            ofLogNotice() << devices[i].id << ": " << devices[i].deviceName << " - unavailable ";
        }
    }
    vidGrabber.setDeviceID(0);
    vidGrabber.setDesiredFrameRate(60);
    vidGrabber.initGrabber(camWidth, camHeight);

    videoInverted.allocate(camWidth, camHeight, OF_PIXELS_RGB);
    videoTexture.allocate(videoInverted);
    
    /* Glitch Setup */
    ofSetVerticalSync(true);
    camera.setDistance(400);
    ofSetCircleResolution(3);
    
    bShowHelp  = true;
    myFbo.allocate(screenWidth, screenHeight);
    myGlitch.setup(&myFbo);
}

//--------------------------------------------------------------
void ofApp::update() {
    UDPHandler();
    
    if(isGlitchMode) {
        myFbo.begin();
        vidGrabber.update();
        if(vidGrabber.isFrameNew()) {
            ofPixels &pixels = vidGrabber.getPixels();
            pixels.mirror(false, true);
            videoTexture.loadData(pixels);
        }
        
        ofSetHexColor(0xffffff);
        int screenWidth = ofGetScreenWidth();
        int screenHeight = ofGetScreenHeight();
        
        float widthScale = (screenWidth * cameraScaleFactor);
        float heightScale = (screenHeight * cameraScaleFactor);
        
        float xStart = (screenWidth - widthScale) / 2;
        float yStart = (screenHeight - heightScale) / 2;
        
        ofLogNotice() << xStart;
        ofLogNotice() << yStart;
        ofLogNotice() << "=====================";
        
        videoTexture.draw(xStart, yStart, widthScale, heightScale);
        myFbo.end();
    } else if(isBlankMode) {
        
        ofBackground(0, 0, 0);
    }
    for (auto &vert : line.getVertices()){
        vert.x += ofRandom(-1,1);
        vert.y += ofRandom(-1,1);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    if(isGlitchMode) {
        /* Apply effects */
        myGlitch.generateFx();

        /* draw effected view */
        ofSetColor(255);
        myFbo.draw(0, 0);


        /* show information*/
        string info = "";
        info += "1 - 0 : Apply glitch effects.\n";
        info += "q - u : Apply color remap effects.\n";
        info += "A key : Credit scene.\n";
        info += "S key : Camera/Glitch scene.\n";
        info += "z - x : Scale Camera.\n";
        info += "H key : Hide or show this information.";

        if (bShowHelp){
            ofSetColor(0, 200);
            ofRectangle(25, 17, 320, 60);
            ofSetColor(255);
            ofDrawBitmapString(info, 30,30);
        }
        
    } else if(isBlankMode) {
        ofClear(0,255);
    }
    
    line.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switchMode(key);
    cameraScaleKey(key);
    if(key == 's' || key == 'S'){
        vidGrabber.videoSettings();
    }
    
    if (key == '1') myGlitch.setFx(OFXPOSTGLITCH_CONVERGENCE    , true);
    if (key == '2') myGlitch.setFx(OFXPOSTGLITCH_GLOW            , true);
    if (key == '3') myGlitch.setFx(OFXPOSTGLITCH_SHAKER            , true);
    if (key == '4') myGlitch.setFx(OFXPOSTGLITCH_CUTSLIDER        , true);
    if (key == '5') myGlitch.setFx(OFXPOSTGLITCH_TWIST            , true);
    if (key == '6') myGlitch.setFx(OFXPOSTGLITCH_OUTLINE        , true);
    if (key == '7') myGlitch.setFx(OFXPOSTGLITCH_NOISE            , true);
    if (key == '8') myGlitch.setFx(OFXPOSTGLITCH_SLITSCAN        , true);
    if (key == '9') myGlitch.setFx(OFXPOSTGLITCH_SWELL            , true);
    if (key == '0') myGlitch.setFx(OFXPOSTGLITCH_INVERT            , true);

    if (key == 'q') myGlitch.setFx(OFXPOSTGLITCH_CR_HIGHCONTRAST, true);
    if (key == 'w') myGlitch.setFx(OFXPOSTGLITCH_CR_BLUERAISE    , true);
    if (key == 'e') myGlitch.setFx(OFXPOSTGLITCH_CR_REDRAISE    , true);
    if (key == 'r') myGlitch.setFx(OFXPOSTGLITCH_CR_GREENRAISE    , true);
    if (key == 't') myGlitch.setFx(OFXPOSTGLITCH_CR_BLUEINVERT    , true);
    if (key == 'y') myGlitch.setFx(OFXPOSTGLITCH_CR_REDINVERT    , true);
    if (key == 'u') myGlitch.setFx(OFXPOSTGLITCH_CR_GREENINVERT    , true);
    
    if (key == 'h') bShowHelp ^= true;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if (key == '1') myGlitch.setFx(OFXPOSTGLITCH_CONVERGENCE    , false);
    if (key == '2') myGlitch.setFx(OFXPOSTGLITCH_GLOW            , false);
    if (key == '3') myGlitch.setFx(OFXPOSTGLITCH_SHAKER            , false);
    if (key == '4') myGlitch.setFx(OFXPOSTGLITCH_CUTSLIDER        , false);
    if (key == '5') myGlitch.setFx(OFXPOSTGLITCH_TWIST            , false);
    if (key == '6') myGlitch.setFx(OFXPOSTGLITCH_OUTLINE        , false);
    if (key == '7') myGlitch.setFx(OFXPOSTGLITCH_NOISE            , false);
    if (key == '8') myGlitch.setFx(OFXPOSTGLITCH_SLITSCAN        , false);
    if (key == '9') myGlitch.setFx(OFXPOSTGLITCH_SWELL            , false);
    if (key == '0') myGlitch.setFx(OFXPOSTGLITCH_INVERT            , false);

    if (key == 'q') myGlitch.setFx(OFXPOSTGLITCH_CR_HIGHCONTRAST, false);
    if (key == 'w') myGlitch.setFx(OFXPOSTGLITCH_CR_BLUERAISE    , false);
    if (key == 'e') myGlitch.setFx(OFXPOSTGLITCH_CR_REDRAISE    , false);
    if (key == 'r') myGlitch.setFx(OFXPOSTGLITCH_CR_GREENRAISE    , false);
    if (key == 't') myGlitch.setFx(OFXPOSTGLITCH_CR_BLUEINVERT    , false);
    if (key == 'y') myGlitch.setFx(OFXPOSTGLITCH_CR_REDINVERT    , false);
    if (key == 'u') myGlitch.setFx(OFXPOSTGLITCH_CR_GREENINVERT    , false);
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    ofPoint pt;
    pt.set(x,y);
    line.addVertex(pt);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    line.clear();
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
//--------------------------------------------------------------

void ofApp::initMode() {
    /* Initial States */
    isBlankMode = false;
    isGlitchMode = false;
    
    /* UDP Listener */
    UDPConnection.Create();
    UDPConnection.Bind(11999);
    UDPConnection.SetNonBlocking(true);
}

void ofApp::switchMode(int key) {
    if(key == 'a' || key == 'A') {
        isBlankMode = true;
        isGlitchMode = false;
    } else if(key == 's' || key == 'S') {
        isBlankMode = false;
        isGlitchMode = true;
    }
}

void ofApp::cameraScaleKey(int key) {
    if(key == 'z') {
        if(cameraScaleFactor > 0.5) {
            cameraScaleFactor = cameraScaleFactor - cameraScaleUnit;
        }
    } else if(key == 'x') {
        if(cameraScaleFactor < 1) {
            cameraScaleFactor = cameraScaleFactor + cameraScaleUnit;
        }
    }
}

void ofApp::UDPHandler() {
    char udpMessage[1000];
    UDPConnection.Receive(udpMessage,100000);
    string message = udpMessage;
    ofLogVerbose() << message;
}


