#include "ofApp.h"
#include <limits>

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);

    showDebug = false;
    joyMute = false;
    ccMute = false;
    noteMute = false;

    controller = Ps3Controller::getOne();
    udlr = std::make_unique<UDLR>();
    xotrisq = std::make_unique<XOTriSq>();
    lr12 = std::make_unique<LR12>();
    
    midiOut = std::make_shared<ofxMidiOut>();
    if(!midiOut->openVirtualPort("ps3ctl")) {
        ofLogError("failed to open virtual midi output port");
        ofExit();
    }

    ofSetBackgroundAuto(true);
    ofColor colorOne(15);
    ofColor colorTwo(35);
    ofBackgroundGradient(colorOne, colorTwo, OF_GRADIENT_CIRCULAR);
    ofBackground(100);
}

//--------------------------------------------------------------
void ofApp::update(){
    // all below may be put in a separate thread, but for now it is fine here

    constexpr unsigned char channel = 1;

    constexpr unsigned char maxCC = 127;
    constexpr float midpoint = UCHAR_MAX/2;
    static const float maxRadius = std::sqrt(2) * midpoint;

    using v = Ps3Controller::CVal;

    static const ofVec2f origin(midpoint, midpoint);
    static const ofVec2f xAxis(-1, 0);

    unsigned char ccNum = 0;

    static ofxMidiCCSender x1Sender{midiOut, ccNum++};
    static ofxMidiCCSender y1Sender{midiOut, ccNum++};
    static ofxMidiCCSender rad1Sender{midiOut, ccNum++, 0, maxRadius}; // radius 1
    static ofxMidiCCSender t1Sender{midiOut, ccNum++, -180, 180}; // argument (theta - 't') 1

    static ofxMidiCCSender x2Sender{midiOut, ccNum++};
    static ofxMidiCCSender y2Sender{midiOut, ccNum++};
    static ofxMidiCCSender rad2Sender{midiOut, ccNum++, 0, maxRadius}; // radius 1
    static ofxMidiCCSender t2Sender{midiOut, ccNum++, -180, 180}; // argument (theta - 't') 1

    static ofxMidiNoteSender uNoteSender{midiOut, ccNum};
    static ofxMidiCCSender uSender{midiOut, ccNum++};
    static ofxMidiNoteSender dNoteSender{midiOut, ccNum};
    static ofxMidiCCSender dSender{midiOut, ccNum++};
    static ofxMidiNoteSender lNoteSender{midiOut, ccNum};
    static ofxMidiCCSender lSender{midiOut, ccNum++};
    static ofxMidiNoteSender rNoteSender{midiOut, ccNum};
    static ofxMidiCCSender rSender{midiOut, ccNum++};

    static ofxMidiNoteSender l1NoteSender{midiOut, ccNum};
    static ofxMidiCCSender l1Sender{midiOut, ccNum++};
    static ofxMidiNoteSender r1NoteSender{midiOut, ccNum};
    static ofxMidiCCSender r1Sender{midiOut, ccNum++};
    static ofxMidiNoteSender l2NoteSender{midiOut, ccNum};
    static ofxMidiCCSender l2Sender{midiOut, ccNum++};
    static ofxMidiNoteSender r2NoteSender{midiOut, ccNum};
    static ofxMidiCCSender r2Sender{midiOut, ccNum++};

    static ofxMidiNoteSender xNoteSender{midiOut, ccNum};
    static ofxMidiCCSender xSender{midiOut, ccNum++};
    static ofxMidiNoteSender oNoteSender{midiOut, ccNum};
    static ofxMidiCCSender oSender{midiOut, ccNum++};
    static ofxMidiNoteSender sqNoteSender{midiOut, ccNum};
    static ofxMidiCCSender sqSender{midiOut, ccNum++};
    static ofxMidiNoteSender triNoteSender{midiOut, ccNum};
    static ofxMidiCCSender triSender{midiOut, ccNum++};


    static ofVec2f joy1;
    static ofVec2f joy1relative;

    if(!joyMute) {
        const auto x1 = controller->getCVal(v::L_x);
        const auto y1 = controller->getCVal(v::L_y);
        joy1.set(x1, y1);
        joy1relative = joy1 - origin;
        x1Sender(x1);
        y1Sender(y1);

        rad1Sender(joy1relative.length());
        t1Sender(joy1relative.angle(xAxis));
    }

    const auto u = controller->getCVal(v::U);
    const auto d = controller->getCVal(v::D);
    const auto l = controller->getCVal(v::L);
    const auto r = controller->getCVal(v::R);

    const auto l1 = controller->getCVal(v::L1);
    const auto r1 = controller->getCVal(v::R1);
    const auto l2 = controller->getCVal(v::L2);
    const auto r2 = controller->getCVal(v::R2);

    const auto x = controller->getCVal(v::X);
    const auto o = controller->getCVal(v::O);
    const auto sq = controller->getCVal(v::Sq);
    const auto tri = controller->getCVal(v::Tri);

    if(!noteMute) {
        uNoteSender(u);
        dNoteSender(d);
        lNoteSender(l);
        rNoteSender(r);

        l1NoteSender(l1);
        r1NoteSender(r1);
        l2NoteSender(l2);
        r2NoteSender(r2);

        xNoteSender(x);
        oNoteSender(o);
        sqNoteSender(sq);
        triNoteSender(tri);
    }

    if(!ccMute) {
        uSender(u);
        dSender(d);
        lSender(l);
        rSender(r);

        l1Sender(l1);
        r1Sender(r1);
        l2Sender(l2);
        r2Sender(r2);

        xSender(x);
        oSender(o);
        sqSender(sq);
        triSender(tri);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    const float w = ofGetWidth();
    const float h = ofGetHeight();

    //ofClear(0);
    if(showDebug) drawDebug();

    using v = Ps3Controller::CVal;

    drawJoystick(controller->getCVal(v::L_x), controller->getCVal(v::L_y), 200, 400);
    drawJoystick(controller->getCVal(v::R_x), controller->getCVal(v::R_y), w-200, 400);

    ofPushMatrix();
    ofTranslate(w/2, 50);
    lr12->draw();
    ofPopMatrix();

    ofPushMatrix();
    ofTranslate(200, 220);
    udlr->draw();
    ofPopMatrix();
    ofPushMatrix();
    ofTranslate(w - 200, 220);
    xotrisq->draw();
    ofPopMatrix();
}


template <class T>
void ofApp::drawJoystick(const T& xVal, const T& yVal, const float &x, const float &y) {
    constexpr float joyDrawRadius = 100;
    constexpr int joyDrawSize = 2;
    constexpr T maxValue= std::numeric_limits<T>::max();

    ofPushMatrix();
    ofTranslate(x, y);
    
    ofDrawCircle(
        ofMap(xVal, 0, maxValue, -1* joyDrawRadius, joyDrawRadius),
        ofMap(yVal, 0, maxValue, -1* joyDrawRadius, joyDrawRadius),
    joyDrawSize);

    ofPopMatrix();
}

void ofApp::drawDebug() {
    // get raw data from Ps3Controller and print as text
    // fixed-width, same place (so we can see changes easily)
    auto data = controller->getData();
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

void ofApp::exit() {
    midiOut->closePort();
}

