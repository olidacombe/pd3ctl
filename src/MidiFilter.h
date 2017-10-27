#pragma once

class ofxMidiCCSender {

    unsigned char ch, cc, lastValue;
    ofxMidiOut* midiOut;


public:
    ofxMidiCCSender(ofxMidiOut& out, const unsigned char &channel=0, const unsigned char &ccnum=0) :
        ch(channel), cc(ccnum), midiOut(&out), lastValue(0)
    {
        assert(cc<128);
        assert(ch<16);
    }

    void operator() (unsigned char v) {
        if(v>127) return; // keep it valid, people
        if(lastValue == v) return; // don't spam
        
        midiOut->sendControlChange(ch, cc, v);
        lastValue=v;
    }
};

