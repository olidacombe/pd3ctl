#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "Ps3Controller.h"
#include "JoystickComponent.h"
#include "UDLR.h"
#include "XOTriSq.h"
#include "LR12.h"
#include "MiddleButtons.h"
#include "ofxMidi.h"
#include "MidiFilter.h"

namespace midiNum {
    enum num {
        up, down, left, right,
        l1, r1, l2, r2,
        x, o, sq, tri,
        joyL, joyR, start, select, PS,
        // keep these ones 'last'
        jx, jxTrack, jxHemi1, jxHemi2,
        jy, jyTrack, jyHemi1, jyHemi2,
        r, t
    };
    static constexpr int pressureSensitiveSize = num::joyL;
    static constexpr int directSize = num::jx;
    static constexpr int j1j2offset = num::t - num::jx + 1;
    static constexpr int size = num::t + j1j2offset + 1;
    
    using v = Ps3Controller::CVal;
    static constexpr v directSource[] = {
        v::U, v::D, v::L, v::R,
        v::L1, v::R1, v::L2, v::R2,
        v::X, v::O, v::Sq, v::Tri
    }; // this has to be kept in the same order as num
}

class ofApp : public ofBaseApp{

    const string settingsFilename = "settings.xml";
    const float defaultSpeed = 0.07;
    const float maxSpeed = 0.1;
    const float minSpeed = 0.01;

    std::shared_ptr<Ps3Controller> controller;
    std::shared_ptr<ofxMidiOut> midiOut;
    std::unique_ptr<UDLR> udlr;
    std::unique_ptr<XOTriSq> xotrisq;
    std::unique_ptr<LR12> lr12;
    std::unique_ptr<MiddleButtons> middleButtons;
    std::unique_ptr<JoystickComponent> joyLComponent, joyRComponent;

    std::array<std::unique_ptr<ofxMidiCCSender>, midiNum::size> CCSenders;
    std::array<std::unique_ptr<ofxMidiNoteSender>, midiNum::size> NoteSenders;
    /*
    std::vector<std::unique_ptr<ofxMidiCCSender>> jxSender, jxTrackSender, jxHemi1Sender, jxHemi2Sender,
        jySender, jyTrackSender, jyHemi1Sender, jyHemi2Sender, radSender, tSender;
     */
    std::array<ofVec2f, 2> joyPoints, trackers;
    std::array<bool, 2> joyPressed;

    ofParameter<bool> showGui, showDebug, mappingMode, joyMute, ccMute, noteMute;
    ofParameter<float> joyThreshold, speed, mappingScanTime, mappingClearTime;
    ofxPanel gui;

    void drawDebug();
    void showStatus();
    template <class T>
    void drawJoystick(const T& xVal, const T& yVal, const bool pressed=false, const float &x=0, const float &y=0);

    void saveSettings();
    void exit();

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
		
};
