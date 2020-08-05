#include "ofApp.h"

ofPolyline line;

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowTitle("PunkOwl Devola v0.1 - vonqo");
    ofBackground(0,0,0);

    // Camera Setup
    camWidth = 320;  // try to grab at this size.
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
    
    // Glitch Setup
    ofSetVerticalSync(true);
    camera.setDistance(400);
    ofSetCircleResolution(3);

    lenna.loadImage("lenna.png");
    bDrawLenna = false;
    bShowHelp  = true;
    myFbo.allocate(1024, 768);

    myGlitch.setup(&myFbo);
}

//--------------------------------------------------------------
void ofApp::update(){
    if(isGlitchMode) {
//        myFbo.begin();
//        ofClear(0, 0, 0,255);
//        if (!bDrawLenna){
//            camera.begin();
//
//            for (int i = 0;i < 100;i++){
//                if        (i % 5 == 0)ofSetColor(50 , 255, 100);
//                else if (i % 9 == 0)ofSetColor(255, 50, 100);
//                else                ofSetColor(255, 255, 255);
//
//                ofPushMatrix();
//                ofRotate(ofGetFrameNum(), 1.0, 1.0, 1.0);
//                ofTranslate((ofNoise(i/2.4)-0.5)*1000,
//                            (ofNoise(i/5.6)-0.5)*1000,
//                            (ofNoise(i/8.2)-0.5)*1000);
//                ofCircle(0, 0, (ofNoise(i/3.4)-0.5)*100+ofRandom(3));
//                ofPopMatrix();
//            }
//
//            camera.end();
//        }else{
//            ofSetColor(255);
//            lenna.draw(0, 0);
//        }
//        myFbo.end();
        
        myFbo.begin();
        vidGrabber.update();
        if(vidGrabber.isFrameNew()){
            ofPixels & pixels = vidGrabber.getPixels();
            for(size_t i = 0; i < pixels.size(); i++){
                // invert the color of the pixel
                // videoInverted[i] = 255 - pixels[i];
                videoInverted[i] = pixels[i];
            }
            //load the inverted pixels
            videoTexture.loadData(videoInverted);
        }
        
        ofSetHexColor(0xffffff);
        videoTexture.draw(0, 0, 1024, 768);
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
//        ofSetHexColor(0xffffff);
//        vidGrabber.draw(0, 0);
//        videoTexture.draw(0, 0, 1024, 768);
//        myGlitch.generateFx();
//        ofSetColor(255);
//        myFbo.draw(100, 100);
        
        /* draw normal view */
//        ofSetColor(255);
//        myFbo.draw(0, 0);

        /* Apply effects */
        myGlitch.generateFx();

        /* draw effected view */
        ofSetColor(255);
        myFbo.draw(0, 0);


        /* show information*/
        string info = "";
        info += "1 - 0 : Apply glitch effects.\n";
        info += "q - u : Apply color remap effects.\n";
        info += "L key : Switch 3Dview / 2DImage.\n";
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

    if (key == 'l') bDrawLenna ^= true;
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
//--------------------------------------------------------------

void ofApp::initMode() {
    isBlankMode = false;
    isGlitchMode = false;
}

void ofApp::switchMode(int key) {
    if(key == 'q' || key == 'Q') {
        isBlankMode = true;
        isGlitchMode = false;
    } else if(key == 'w' || key == 'W') {
        isBlankMode = false;
        isGlitchMode = true;
    }
}
