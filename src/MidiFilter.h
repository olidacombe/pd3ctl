#pragma once

#include <climits>

constexpr unsigned char maxCC = 127;

class ofxMidiCCSender {

    unsigned char ch, cc, lastValue;
    std::shared_ptr<ofxMidiOut> midiOut;


public:
    ofxMidiCCSender(const std::shared_ptr<ofxMidiOut>& out, const unsigned char &ccnum=0, const unsigned char &channel=0) :
        ch(channel), cc(ccnum), midiOut(out), lastValue(0)
    {
        assert(cc<128);
        assert(ch<16);
    }

    void operator() (const unsigned char& v) {

        const unsigned char newValue = ofMap(v, 0, UCHAR_MAX, 0, maxCC);
        if(lastValue == newValue) return; // don't spam
        
        midiOut->sendControlChange(ch, cc, newValue);
        lastValue=newValue;
    }
};

