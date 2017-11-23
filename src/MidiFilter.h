#pragma once

#include <climits>

constexpr unsigned char maxCC = 127;
constexpr unsigned char maxVelocity = 127;

class ofxMidiSender {

protected:
    std::shared_ptr<ofxMidiOut> midiOut;
    unsigned char ch; // channel

public:
    ofxMidiSender(const std::shared_ptr<ofxMidiOut>& out, const unsigned char channel=1) :
        ch(channel), midiOut(out)
    {
        assert(ch<16);
    }
};

class ofxMidiCCSender : public ofxMidiSender {

    unsigned char cc, lastValue;
    float min, max;

    void sendIfChanged(const unsigned char& v) {
        const unsigned char newValue = ofMap(v, 0, UCHAR_MAX, 0, maxCC);

        if(lastValue == newValue) return; // don't spam

        midiOut->sendControlChange(ch, cc, newValue);
        lastValue=newValue;
    }

public:
    ofxMidiCCSender(const std::shared_ptr<ofxMidiOut>& out, const unsigned char &ccnum=0, const float& minimum=0.0, const float& maximum=1.0, const unsigned char channel=1) :
        ofxMidiSender(out, channel), cc(ccnum), min(minimum), max(maximum), lastValue(0)
    {
        assert(cc<128);
    }

    void operator() (const unsigned char& v) {
        sendIfChanged(v);
    }

    void operator() (const float& v) {
        const unsigned char newValue = ofMap(v, min, max, 0, UCHAR_MAX, true);
        sendIfChanged(newValue);
    }
};


class ofxMidiNoteSender : public ofxMidiSender {
    
    unsigned char n, velocity;

    void sendNoteIfTriggered(const unsigned char& v) {
        const unsigned char newValue = ofMap(v, 0, UCHAR_MAX, 0, maxVelocity);

        if(newValue > 0 && velocity == 0) {
            midiOut->sendNoteOn(ch, n, newValue);
        } else if(velocity > 0 && newValue == 0) {
            midiOut->sendNoteOff(ch, n, newValue);
        }

        velocity = newValue;
    }
    
public:
    ofxMidiNoteSender(const std::shared_ptr<ofxMidiOut>& out, const unsigned char noteNum=0, const unsigned char channel=1) :
        ofxMidiSender(out, channel), n(noteNum), velocity(0)
    {
        assert(n<128);
    }

    void operator() (const unsigned char& v) {
        sendNoteIfTriggered(v);
    }
};
