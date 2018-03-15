#include "ofApp.h"
#include <limits>

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);

    gui.setup("panel");
#ifdef debug
    gui.add(showDebug.set("show debug [D]", false));
#endif
    gui.add(showGui.set("show gui [g]", true));
    gui.add(joyMute.set("mute joysticks [m]", false));
    gui.add(ccMute.set("mute CC [c]", false));
    gui.add(noteMute.set("mute notes [n]", false));
    gui.add(maxX1Speed.set("X1 speed", defaultSpeed, minSpeed, maxSpeed));
    gui.add(maxY1Speed.set("Y1 speed", defaultSpeed, minSpeed, maxSpeed));
    gui.add(maxX2Speed.set("X2 speed", defaultSpeed, minSpeed, maxSpeed));
    gui.add(maxY2Speed.set("Y2 speed", defaultSpeed, minSpeed, maxSpeed));
    gui.loadFromFile(settingsFilename);

    controller = Ps3Controller::getOne();
    udlr = std::make_unique<UDLR>();
    xotrisq = std::make_unique<XOTriSq>();
    lr12 = std::make_unique<LR12>();
    middleButtons = std::make_unique<MiddleButtons>();
    
    midiOut = std::make_shared<ofxMidiOut>();
    if(!midiOut->openVirtualPort("ps3ctl")) {
        ofLogError("failed to open virtual midi output port");
        ofExit();
    }

    ccNumInitializer = 0;
    static const float maxRadius = std::sqrt(2) * (UCHAR_MAX/2);

    x1Sender = std::make_unique<ofxMidiCCSender>(midiOut, ccNumInitializer++);
    x1TrackSender = std::make_unique<ofxMidiCCSender>(midiOut, ccNumInitializer++);
    x1Hemi1Sender = std::make_unique<ofxMidiCCSender>(midiOut, ccNumInitializer++);
    x1Hemi2Sender = std::make_unique<ofxMidiCCSender>(midiOut, ccNumInitializer++);
    y1Sender = std::make_unique<ofxMidiCCSender>(midiOut, ccNumInitializer++);
    y1TrackSender = std::make_unique<ofxMidiCCSender>(midiOut, ccNumInitializer++);
    y1Hemi1Sender = std::make_unique<ofxMidiCCSender>(midiOut, ccNumInitializer++);
    y1Hemi2Sender = std::make_unique<ofxMidiCCSender>(midiOut, ccNumInitializer++);
    rad1Sender = std::make_unique<ofxMidiCCSender>(midiOut, ccNumInitializer++, 0, maxRadius); // radius 1
    t1Sender = std::make_unique<ofxMidiCCSender>(midiOut, ccNumInitializer++, -180, 180); // argument (theta - 't') 1

    x2Sender = std::make_unique<ofxMidiCCSender>(midiOut, ccNumInitializer++);
    x2TrackSender = std::make_unique<ofxMidiCCSender>(midiOut, ccNumInitializer++);
    x2Hemi1Sender = std::make_unique<ofxMidiCCSender>(midiOut, ccNumInitializer++);
    x2Hemi2Sender = std::make_unique<ofxMidiCCSender>(midiOut, ccNumInitializer++);
    y2Sender = std::make_unique<ofxMidiCCSender>(midiOut, ccNumInitializer++);
    y2TrackSender = std::make_unique<ofxMidiCCSender>(midiOut, ccNumInitializer++);
    y2Hemi1Sender = std::make_unique<ofxMidiCCSender>(midiOut, ccNumInitializer++);
    y2Hemi2Sender = std::make_unique<ofxMidiCCSender>(midiOut, ccNumInitializer++);
    rad2Sender = std::make_unique<ofxMidiCCSender>(midiOut, ccNumInitializer++, 0, maxRadius); // radius 2
    t2Sender = std::make_unique<ofxMidiCCSender>(midiOut, ccNumInitializer++, -180, 180); // argument (theta - 't') 2

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

    using v = Ps3Controller::CVal;
    using bmask = Ps3Controller::BMask;

    static const ofVec2f origin(midpoint, midpoint);
    static const ofVec2f xAxis(-1, 0);

    static ofxMidiNoteSender uNoteSender{midiOut, ccNumInitializer};
    static ofxMidiCCSender uSender{midiOut, ccNumInitializer++};
    static ofxMidiNoteSender dNoteSender{midiOut, ccNumInitializer};
    static ofxMidiCCSender dSender{midiOut, ccNumInitializer++};
    static ofxMidiNoteSender lNoteSender{midiOut, ccNumInitializer};
    static ofxMidiCCSender lSender{midiOut, ccNumInitializer++};
    static ofxMidiNoteSender rNoteSender{midiOut, ccNumInitializer};
    static ofxMidiCCSender rSender{midiOut, ccNumInitializer++};

    static ofxMidiNoteSender l1NoteSender{midiOut, ccNumInitializer};
    static ofxMidiCCSender l1Sender{midiOut, ccNumInitializer++};
    static ofxMidiNoteSender r1NoteSender{midiOut, ccNumInitializer};
    static ofxMidiCCSender r1Sender{midiOut, ccNumInitializer++};
    static ofxMidiNoteSender l2NoteSender{midiOut, ccNumInitializer};
    static ofxMidiCCSender l2Sender{midiOut, ccNumInitializer++};
    static ofxMidiNoteSender r2NoteSender{midiOut, ccNumInitializer};
    static ofxMidiCCSender r2Sender{midiOut, ccNumInitializer++};

    static ofxMidiNoteSender xNoteSender{midiOut, ccNumInitializer};
    static ofxMidiCCSender xSender{midiOut, ccNumInitializer++};
    static ofxMidiNoteSender oNoteSender{midiOut, ccNumInitializer};
    static ofxMidiCCSender oSender{midiOut, ccNumInitializer++};
    static ofxMidiNoteSender sqNoteSender{midiOut, ccNumInitializer};
    static ofxMidiCCSender sqSender{midiOut, ccNumInitializer++};
    static ofxMidiNoteSender triNoteSender{midiOut, ccNumInitializer};
    static ofxMidiCCSender triSender{midiOut, ccNumInitializer++};

    static ofxMidiNoteSender JoyLSender{midiOut, ccNumInitializer++};
    static ofxMidiNoteSender JoyRSender{midiOut, ccNumInitializer++};
    static ofxMidiNoteSender StartSender{midiOut, ccNumInitializer++};
    static ofxMidiNoteSender SelectSender{midiOut, ccNumInitializer++};
    static ofxMidiNoteSender PSSender{midiOut, ccNumInitializer++};

    static ofVec2f joy1;
    static ofVec2f joy1relative;
    static ofVec2f joy2;
    static ofVec2f joy2relative;

    if(!joyMute) {
        const auto x1 = controller->getCVal(v::L_x);
        const auto y1 = controller->getCVal(v::L_y);
        using T = decltype(x1);
        static auto LowerHemi = [](const T &v) {
            return static_cast<T>(ofMap(midpoint - v, 0, midpoint, 0, UCHAR_MAX, true));
        };
        static auto UpperHemi = [](const T &v) {
            return static_cast<T>(ofMap(v - midpoint, 0, midpoint, 0, UCHAR_MAX, true));
        };
        joy1.set(x1, y1);
        joy1relative = joy1 - origin;
        (*x1Sender)(x1);
        (*x1Hemi1Sender)(LowerHemi(x1));
        (*x1Hemi2Sender)(UpperHemi(x1));
        (*y1Sender)(y1);
        (*y1Hemi1Sender)(LowerHemi(y1));
        (*y1Hemi2Sender)(UpperHemi(y1));
        (*rad1Sender)(joy1relative.length());
        (*t1Sender)(joy1relative.angle(xAxis));

        const auto x2 = controller->getCVal(v::R_x);
        const auto y2 = controller->getCVal(v::R_y);
        joy2.set(x2, y2);
        joy2relative = joy2 - origin;
        (*x2Sender)(x2);
        (*x2Hemi1Sender)(LowerHemi(x2));
        (*x2Hemi2Sender)(UpperHemi(x2));
        (*y2Sender)(y2);
        (*y2Hemi1Sender)(LowerHemi(y2));
        (*y2Hemi2Sender)(UpperHemi(y2));
        (*rad2Sender)(joy2relative.length());
        (*t2Sender)(joy2relative.angle(xAxis));
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

        const auto sw_buts = controller->getCVal(v::SW_buttons);
        constexpr unsigned char digitalButtonVelocity = 127;
        const unsigned char joyL = (sw_buts & bmask::JoyL) ? digitalButtonVelocity : 0;
        const unsigned char select = (sw_buts & bmask::Select) ? digitalButtonVelocity : 0;
        const unsigned char ps = controller->getCVal(v::PS) ? digitalButtonVelocity : 0;
        const unsigned char start = (sw_buts & bmask::Start) ? digitalButtonVelocity : 0;
        const unsigned char joyR = (sw_buts & bmask::JoyR) ? digitalButtonVelocity : 0;

        JoyLSender(joyL);
        SelectSender(select);
        PSSender(ps);
        StartSender(start);
        JoyRSender(joyR);
        
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
    using bmask = Ps3Controller::BMask;

    const auto sw_buts = controller->getCVal(v::SW_buttons);

    const bool lj = sw_buts & bmask::JoyL;
    const bool rj = sw_buts & bmask::JoyR;
    drawJoystick(controller->getCVal(v::L_x), controller->getCVal(v::L_y), lj, 200, 400);
    drawJoystick(controller->getCVal(v::R_x), controller->getCVal(v::R_y), rj, w-200, 400);

    constexpr int midline = 220;

    ofPushMatrix();
    ofTranslate(w/2, 50);
    lr12->draw();
    ofPopMatrix();

    ofPushMatrix();
    ofTranslate(200, midline);
    udlr->draw();
    ofPopMatrix();

    ofPushMatrix();
    ofTranslate(w/2, midline);
    middleButtons->draw();
    ofPopMatrix();

    ofPushMatrix();
    ofTranslate(w - 200, midline);
    xotrisq->draw();
    ofPopMatrix();

    ofPushMatrix();
    ofTranslate(10, h - 20);
    showStatus();
    ofPopMatrix();

    if(showGui) {
        gui.draw();
    }

}

void ofApp::showStatus() {
    constexpr int lineStep = -20;
    //ofPushMatrix();
    ofPushStyle();

    ofSetColor(255, 0, 0);
    if(joyMute) {
        ofDrawBitmapString("Joystick mute", 0, 0);
        ofTranslate(0, lineStep);
    }
    if(ccMute) {
        ofDrawBitmapString("CC mute", 0, 0);
        ofTranslate(0, lineStep);
    }
    if(noteMute) {
        ofDrawBitmapString("Note mute", 0, 0);
    }

    ofPopStyle();
    //ofPopMatrix();
}


template <class T>
void ofApp::drawJoystick(const T& xVal, const T& yVal, const bool pressed, const float &x, const float &y) {
    constexpr float joyDrawRadius = 100;
    constexpr int joyDrawSize = 2;
    constexpr T maxValue= std::numeric_limits<T>::max();

    const auto plotX = ofMap(xVal, 0, maxValue, -1* joyDrawRadius, joyDrawRadius);
    const auto plotY = ofMap(yVal, 0, maxValue, -1* joyDrawRadius, joyDrawRadius);

    ofPushMatrix();
    ofTranslate(x, y);
    
    if(pressed) {
        ofPushStyle();
        ofSetColor(0xff, 0xff, 0xff, 0x7f);
        ofDrawCircle(0, 0, joyDrawRadius / 2);
        ofPopStyle();
        ofDrawLine(0, 0, plotX, plotY);
    }

    ofDrawCircle(plotX, plotY, joyDrawSize);

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
        case 'D':
            showDebug = !showDebug;
            break;
        case 'g':
            showGui = !showGui;
            break;
        case 'm':
            joyMute = !joyMute;
            break;
        case 'c':
            ccMute = !ccMute;
            break;
        case 'n':
            noteMute = !noteMute;
            break;
        default:
            break;
    }

    /* mapping sender keys:
     *
     * q -> yl
     * w -> yl bottom hemi clamp
     * e -> yl top hemi clamp
     * r -> radius l
     * t -> theta l
     * a -> xl
     * s -> xl left hemi
     * d -> xl right hemi
     *
     * y -> yr
     * u -> yr bottom hemi clamp
     * i -> yr top hemi clamp
     * o -> radius r
     * p -> theta r
     * h -> xr
     * j -> xr left hemi
     * k -> xr right hemi
     */
    if(joyMute) {
        switch(key) {
            // left
            case 'q':
                y1Sender->bang();
                break;
            case 'Q':
                y1TrackSender->bang();
                break;
            case 'w':
                y1Hemi1Sender->bang();
                break;
            case 'e':
                y1Hemi2Sender->bang();
                break;
            case 'r':
                rad1Sender->bang();
                break;
            case 't':
                t1Sender->bang();
                break;
            case 'a':
                x1Sender->bang();
                break;
            case 'A':
                x1TrackSender->bang();
                break;
            case 's':
                x1Hemi1Sender->bang();
                break;
            case 'd':
                x1Hemi2Sender->bang();
                break;
            case '6': // select
                break;

            //right
            case '7': // start
                break;
            case 'y':
                y2Sender->bang();
                break;
            case 'Y':
                y2TrackSender->bang();
                break;
            case 'u':
                y2Hemi1Sender->bang();
                break;
            case 'i':
                y2Hemi2Sender->bang();
                break;
            case 'o':
                rad2Sender->bang();
                break;
            case 'p':
                t2Sender->bang();
                break;
            case 'h':
                x2Sender->bang();
                break;
            case 'H':
                x2TrackSender->bang();
                break;
            case 'j':
                x2Hemi1Sender->bang();
                break;
            case 'k':
                x2Hemi2Sender->bang();
                break;
                
            default:
                break;
        }
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

void ofApp::saveSettings() {
    gui.saveToFile(settingsFilename);
}

void ofApp::exit() {
    saveSettings();
    midiOut->closePort();
}

