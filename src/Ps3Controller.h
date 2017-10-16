#pragma once

#include <hidapi/hidapi.h>

//#define discern_arse_elbow

class Ps3Controller
{
    hid_device *device;
    static const size_t inputBufferSize = 1024;
    //unsigned char[50] buf; // received 49 bytes in test, +1 for report num
    unsigned char inputBuffer[inputBufferSize]; // let's see how much data we can get before trimming

#ifdef discern_arse_elbow
    void getFeatureReport();
#endif

    bool readData();

public:
    Ps3Controller();
    ~Ps3Controller();
};
