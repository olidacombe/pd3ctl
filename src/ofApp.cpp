#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);

    showDebug = false;

    ofSetBackgroundAuto(true);
    ofColor colorOne(15);
    ofColor colorTwo(35);
    ofBackgroundGradient(colorOne, colorTwo, OF_GRADIENT_CIRCULAR);
    ofBackground(100);
}

//--------------------------------------------------------------
void ofApp::update(){
    //controller.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    //ofClear(0);
    if(showDebug) drawDebug();

    using v = Ps3Controller::CVal;
    // latency for days
    //ofDrawBitmapString(ofToHex(controller.getCVal(v::L_x)), 100, 100);
    //ofDrawCircle(mouseX, mouseY, 2);
    ofDrawCircle(
        ofMap(controller.getCVal(v::L_x), 0, 0xff, 0, ofGetWidth()),
        ofMap(controller.getCVal(v::L_y), 0, 0xff, 0, ofGetHeight()),
    2);
    ofDrawCircle(
        ofMap(controller.getCVal(v::R_x), 0, 0xff, 0, ofGetWidth()),
        ofMap(controller.getCVal(v::R_y), 0, 0xff, 0, ofGetHeight()),
    2);
}

void ofApp::drawDebug() {
    // get raw data from Ps3Controller and print as text
    // fixed-width, same place (so we can see changes easily)
    auto data = controller.getData();
    ofPushMatrix();
    ofPushStyle();
    ofSetColor(255);
    ofTranslate(10, 20);
    for(int i=0; i<data.size(); i++) {
        ofDrawBitmapString(ofToHex(static_cast<unsigned char>(i)) + ":", 2, i*11);
        ofDrawBitmapString(ofToHex(data[i]), 30, i*11);
    }
    ofPopStyle();
    ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch(key) {
        case 'd':
            showDebug = !showDebug;
            break;
        default:
            break;
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

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

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
