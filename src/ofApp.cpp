#include "ofApp.h"
#include <limits>

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);

    showDebug = false;
    
    gui.setup("panel");
#ifdef debug
    gui.add(showDebug.set("show debug [D]", false));
#endif
    gui.add(showGui.set("show gui [g]", true));
    gui.add(mappingMode.set("mapping mode [M]", false));
    gui.add(mappingScanTime.set("mapping scan time", 0.1, 0.001, 0.5));
    gui.add(mappingClearTime.set("mapping clear time", 0.5, 0.2, 1.0));
    gui.add(joyMute.set("mute joysticks [m]", false));
    gui.add(ccMute.set("mute CC [c]", false));
    gui.add(noteMute.set("mute notes [n]", false));
    gui.add(joyThreshold.set("joystick noise floor", 5., 0., 100.));
    gui.add(speed.set("speed", defaultSpeed, minSpeed, maxSpeed));
    gui.loadFromFile(settingsFilename);

    controller = Ps3Controller::getOne();
    udlr = std::make_unique<UDLR>();
    xotrisq = std::make_unique<XOTriSq>();
    lr12 = std::make_unique<LR12>();
    middleButtons = std::make_unique<MiddleButtons>();
    joyLComponent = std::make_unique<JoystickComponent>(joyPoints[0], trackers[0], joyPressed[0]);
    joyRComponent = std::make_unique<JoystickComponent>(joyPoints[1], trackers[1], joyPressed[1]);
    
    midiOut = std::make_shared<ofxMidiOut>();
    if(!midiOut->openVirtualPort("ps3ctl")) {
        ofLogError("failed to open virtual midi output port");
        ofExit();
    }

    static const float maxRadius = std::sqrt(2) * (UCHAR_MAX/2);

    for(int i=0; i <= midiNum::j1j2offset; i += midiNum::j1j2offset) {
        using namespace midiNum;
        //jxSender.emplace_back(new ofxMidiCCSender(midiOut, jx + i));
        CCSenders[jx + i] = std::make_unique<ofxMidiCCSender>(midiOut, jx + i);
        //jxTrackSender.emplace_back(new ofxMidiCCSender(midiOut, jxTrack + i, -1, 1));
        CCSenders[jxTrack + i] = std::make_unique<ofxMidiCCSender>(midiOut, jxTrack + i, -1, 1);
        //jxHemi1Sender.emplace_back(new ofxMidiCCSender(midiOut, jxHemi1 + i));
        CCSenders[jxHemi1 + i] = std::make_unique<ofxMidiCCSender>(midiOut, jxHemi1 + i);
        //jxHemi2Sender.emplace_back(new ofxMidiCCSender(midiOut, jxHemi2 + i));
        CCSenders[jxHemi2 + i] = std::make_unique<ofxMidiCCSender>(midiOut, jxHemi2 + i);
        //jySender.emplace_back(new ofxMidiCCSender(midiOut, jy + i));
        CCSenders[jy + i] = std::make_unique<ofxMidiCCSender>(midiOut, jy + i);
        //jyTrackSender.emplace_back(new ofxMidiCCSender(midiOut, jyTrack + i, -1, 1));
        CCSenders[jyTrack + i] = std::make_unique<ofxMidiCCSender>(midiOut, jyTrack + i, -1, 1);
        //jyHemi1Sender.emplace_back(new ofxMidiCCSender(midiOut, jyHemi1 + i));
        CCSenders[jyHemi1 + i] = std::make_unique<ofxMidiCCSender>(midiOut, jyHemi1 + i);
        //jyHemi2Sender.emplace_back(new ofxMidiCCSender(midiOut, jyHemi2 + i));
        CCSenders[jyHemi2 + i] = std::make_unique<ofxMidiCCSender>(midiOut, jyHemi2 + i);
        //radSender.emplace_back(new ofxMidiCCSender(midiOut, r + i, 0, maxRadius)); // radius 1
        CCSenders[num::r + i] = std::make_unique<ofxMidiCCSender>(midiOut, num::r + i, 0, maxRadius);
        //tSender.emplace_back(new ofxMidiCCSender(midiOut, t + i, -180, 180)); // argument (theta - 't') 1
        CCSenders[num::t + i] = std::make_unique<ofxMidiCCSender>(midiOut, num::t + i, -180, 180);
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

    using v = Ps3Controller::CVal;
    using bmask = Ps3Controller::BMask;

    static const ofVec2f origin(midpoint, midpoint);
    static const ofVec2f xAxis(-1, 0);

    static ofxMidiNoteSender uNoteSender{midiOut, midiNum::up};
    static ofxMidiCCSender uSender{midiOut, midiNum::up};
    static ofxMidiNoteSender dNoteSender{midiOut, midiNum::down};
    static ofxMidiCCSender dSender{midiOut, midiNum::down};
    static ofxMidiNoteSender lNoteSender{midiOut, midiNum::left};
    static ofxMidiCCSender lSender{midiOut, midiNum::left};
    static ofxMidiNoteSender rNoteSender{midiOut, midiNum::right};
    static ofxMidiCCSender rSender{midiOut, midiNum::right};

    static ofxMidiNoteSender l1NoteSender{midiOut, midiNum::l1};
    static ofxMidiCCSender l1Sender{midiOut, midiNum::l1};
    static ofxMidiNoteSender r1NoteSender{midiOut, midiNum::r1};
    static ofxMidiCCSender r1Sender{midiOut, midiNum::r1};
    static ofxMidiNoteSender l2NoteSender{midiOut, midiNum::l2};
    static ofxMidiCCSender l2Sender{midiOut, midiNum::l2};
    static ofxMidiNoteSender r2NoteSender{midiOut, midiNum::r2};
    static ofxMidiCCSender r2Sender{midiOut, midiNum::r2};

    static ofxMidiNoteSender xNoteSender{midiOut, midiNum::x};
    static ofxMidiCCSender xSender{midiOut, midiNum::x};
    static ofxMidiNoteSender oNoteSender{midiOut, midiNum::o};
    static ofxMidiCCSender oSender{midiOut, midiNum::o};
    static ofxMidiNoteSender sqNoteSender{midiOut, midiNum::sq};
    static ofxMidiCCSender sqSender{midiOut, midiNum::sq};
    static ofxMidiNoteSender triNoteSender{midiOut, midiNum::tri};
    static ofxMidiCCSender triSender{midiOut, midiNum::tri};

    static ofxMidiNoteSender JoyLSender{midiOut, midiNum::joyL};
    static ofxMidiNoteSender JoyRSender{midiOut, midiNum::joyR};
    static ofxMidiNoteSender StartSender{midiOut, midiNum::start};
    static ofxMidiNoteSender SelectSender{midiOut, midiNum::select};
    static ofxMidiNoteSender PSSender{midiOut, midiNum::PS};


    if(!joyMute && !ccMute && !mappingMode) {
        using T = unsigned char; // ugh, failed to determine from Ps3Controller using result_of<decltype(....
        const std::array<T, 2> xs = {controller->getCVal(v::L_x), controller->getCVal(v::R_x)};
        const std::array<T, 2> ys = {controller->getCVal(v::L_y), controller->getCVal(v::R_y)};
        static auto LowerHemi = [](const T &v) {
            return static_cast<T>(ofMap(midpoint - v, 0, midpoint, 0, UCHAR_MAX, true));
        };
        static auto UpperHemi = [](const T &v) {
            return static_cast<T>(ofMap(v - midpoint, 0, midpoint, 0, UCHAR_MAX, true));
        };
        
        for(int i=0; i<2; i++) {
            static ofVec2f joy;
            static ofVec2f joyRelative;
            
            auto senderOffset = i*midiNum::j1j2offset;

            auto x=xs[i];
            auto y=ys[i];

            joy.set(x, y);
            joyRelative.x = ofMap(joy.x, 0, UCHAR_MAX, -1, 1, true);
            joyRelative.y = ofMap(joy.y, 0, UCHAR_MAX, -1, 1, true);
            auto r = joyRelative.length() * midpoint;
            if(r < joyThreshold) { // de-noise
                r=0; x=0; y=0;
                joy = origin;
                joyRelative = ofVec2f::zero();
            }

            joyPoints[i] = joyRelative;
            trackers[i].x = ofClamp(trackers[i].x + speed*joyRelative.x, -1, 1);
            trackers[i].y = ofClamp(trackers[i].y + speed*joyRelative.y, -1, 1);

            using namespace midiNum;

            (*CCSenders[jx + senderOffset])(x);
            (*CCSenders[jxTrack + senderOffset])(trackers[i].x);
            (*CCSenders[jxHemi1 + senderOffset])(LowerHemi(x));
            (*CCSenders[jxHemi2 + senderOffset])(UpperHemi(x));

            (*CCSenders[jy + senderOffset])(y);
            (*CCSenders[jyTrack + senderOffset])(trackers[i].y);
            (*CCSenders[jyHemi1 + senderOffset])(LowerHemi(y));
            (*CCSenders[jyHemi2 + senderOffset])(UpperHemi(y));

            (*CCSenders[num::r + senderOffset])(r);
            (*CCSenders[num::t + senderOffset])(joyRelative.angle(xAxis));
        }

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

    if(mappingMode) {
        static std::array<std::weak_ptr<ofxMidiSender>, 3> triggerSequence;
        static bool scanning = true;
    } else { // not in mapping mode
        
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
    joyPressed[0] = sw_buts & bmask::JoyL;
    joyPressed[1] = sw_buts & bmask::JoyR;

    ofPushMatrix();
    ofTranslate(200, 400);
    joyLComponent->draw();
    ofPopMatrix();
    ofPushMatrix();
    ofTranslate(w-200, 400);
    joyRComponent->draw();
    ofPopMatrix();

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
        case 'M':
            mappingMode = !mappingMode;
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
     * Q -> yl track
     * w -> yl bottom hemi clamp
     * e -> yl top hemi clamp
     * r -> radius l
     * t -> theta l
     * a -> xl
     * A -> xl track
     * s -> xl left hemi
     * d -> xl right hemi
     *
     * y -> yr
     * Y -> yr track
     * u -> yr bottom hemi clamp
     * i -> yr top hemi clamp
     * o -> radius r
     * p -> theta r
     * h -> xr
     * H -> xr track
     * j -> xr left hemi
     * k -> xr right hemi
     */
    
    /*
    if(joyMute) {
        switch(key) {
            // left
            case 'q':
                jySender[0]->bang();
                break;
            case 'Q':
                jyTrackSender[0]->bang();
                break;
            case 'w':
                jyHemi1Sender[0]->bang();
                break;
            case 'e':
                jyHemi2Sender[0]->bang();
                break;
            case 'r':
                radSender[0]->bang();
                break;
            case 't':
                tSender[0]->bang();
                break;
            case 'a':
                jxSender[0]->bang();
                break;
            case 'A':
                jxTrackSender[0]->bang();
                break;
            case 's':
                jxHemi1Sender[0]->bang();
                break;
            case 'd':
                jxHemi2Sender[0]->bang();
                break;
            case '6': // select
                break;

            //right
            case '7': // start
                break;
            case 'y':
                jySender[1]->bang();
                break;
            case 'Y':
                jyTrackSender[1]->bang();
                break;
            case 'u':
                jyHemi1Sender[1]->bang();
                break;
            case 'i':
                jyHemi2Sender[1]->bang();
                break;
            case 'o':
                radSender[1]->bang();
                break;
            case 'p':
                tSender[1]->bang();
                break;
            case 'h':
                jxSender[1]->bang();
                break;
            case 'H':
                jxTrackSender[1]->bang();
                break;
            case 'j':
                jxHemi1Sender[1]->bang();
                break;
            case 'k':
                jxHemi2Sender[1]->bang();
                break;
                
            default:
                break;
        }
    }
    */
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

