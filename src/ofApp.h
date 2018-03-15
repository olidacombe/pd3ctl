#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "Ps3Controller.h"
#include "UDLR.h"
#include "XOTriSq.h"
#include "LR12.h"
#include "MiddleButtons.h"
#include "ofxMidi.h"
#include "MidiFilter.h"

class ofApp : public ofBaseApp{
    
    const string settingsFilename = "settings.xml";
    const float defaultSpeed = 10.0;
    const float maxSpeed = 100.0;
    const float minSpeed = 0.01;

    std::shared_ptr<Ps3Controller> controller;
    std::shared_ptr<ofxMidiOut> midiOut;
    std::unique_ptr<UDLR> udlr;
    std::unique_ptr<XOTriSq> xotrisq;
    std::unique_ptr<LR12> lr12;
    std::unique_ptr<MiddleButtons> middleButtons;

    std::unique_ptr<ofxMidiCCSender> x1Sender, x1Hemi1Sender, x1Hemi2Sender,
        y1Sender, y1Hemi1Sender, y1Hemi2Sender, rad1Sender, t1Sender;
    std::unique_ptr<ofxMidiCCSender> x2Sender, x2Hemi1Sender, x2Hemi2Sender,
        y2Sender, y2Hemi1Sender, y2Hemi2Sender, rad2Sender, t2Sender;

    ofParameter<bool> showGui, showDebug, joyMute, ccMute, noteMute;
    ofParameter<float> maxX1Speed, maxY1Speed, maxX2Speed, maxY2Speed;
    ofxPanel gui;

    void drawDebug();
    void showStatus();
    template <class T>
    void drawJoystick(const T& xVal, const T& yVal, const bool pressed=false, const float &x=0, const float &y=0);

    void saveSettings();
    void exit();

    unsigned char ccNumInitializer;

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
