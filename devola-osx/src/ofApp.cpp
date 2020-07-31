#include "ofApp.h"

ofPolyline line;

//--------------------------------------------------------------
void ofApp::setup(){
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
    ofSetVerticalSync(true);
    
    // Glitch Setup
    myFbo.allocate(640,480);
    myGlitch.setup(&myFbo);
}

//--------------------------------------------------------------
void ofApp::update(){
    ofBackground(0, 0, 0);
    
    if(isGlitchMode) {
        vidGrabber.update();
        
        if(vidGrabber.isFrameNew()){
            ofPixels & pixels = vidGrabber.getPixels();
            for(size_t i = 0; i < pixels.size(); i++){
                //invert the color of the pixel
                videoInverted[i] = 255 - pixels[i];
            }
            //load the inverted pixels
            videoTexture.loadData(videoInverted);
        }
    } else if(isBlankMode) {
        
    }
    
    for (auto &vert : line.getVertices()){
        vert.x += ofRandom(-1,1);
        vert.y += ofRandom(-1,1);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    if(isGlitchMode) {
        ofSetHexColor(0xffffff);
        // vidGrabber.draw(0, 0);
        videoTexture.draw(0, 0, 1024, 768);
        // videoTexture.draw(20,20, camWidth, camHeight);
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
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

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
    if(key == '1') {
        isBlankMode = true;
        isGlitchMode = false;
    } else if(key == '2') {
        isBlankMode = false;
        isGlitchMode = true;
    }
}
