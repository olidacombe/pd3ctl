#pragma once

#include <climits>

constexpr unsigned char maxCC = 127;

class ofxMidiCCSender {

    unsigned char ch, cc, lastValue;
    float min, max;
    std::shared_ptr<ofxMidiOut> midiOut;

    void sendIfChanged(const unsigned char& v) {
        const unsigned char newValue = ofMap(v, 0, UCHAR_MAX, 0, maxCC);

        if(lastValue == newValue) return; // don't spam

        midiOut->sendControlChange(ch, cc, newValue);
        lastValue=newValue;
    }

public:
    ofxMidiCCSender(const std::shared_ptr<ofxMidiOut>& out, const unsigned char &ccnum=0, const unsigned char &channel=0, const float& minimum=0.0, const float& maximum=1.0) :
        ch(channel), cc(ccnum), midiOut(out), min(minimum), max(maximum), lastValue(0)
    {
        assert(cc<128);
        assert(ch<16);
    }

    void operator() (const unsigned char& v) {
        sendIfChanged(v);
    }

    void operator() (const float& v) {
        const unsigned char newValue = ofMap(v, min, max, 0, UCHAR_MAX, true);
        sendIfChanged(newValue);
    }
};

