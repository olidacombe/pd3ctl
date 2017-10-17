#pragma once

#include <array>
#include <hidapi/hidapi.h>

//#define discern_arse_elbow

class Ps3Controller
{
    hid_device *device;
    static const size_t inputBufferSize = 49;
    //unsigned char[50] buf; // received 49 bytes in test, +1 for report num
    //unsigned char inputBuffer[inputBufferSize]; // let's see how much data we can get before trimming
    std::array<unsigned char, inputBufferSize> inputBuffer;

    bool readData();

public:
    Ps3Controller();
    ~Ps3Controller();

    void update();

    // no c++14 yet :(
    //const auto& getData();
    const decltype(inputBuffer)& getData();
};
