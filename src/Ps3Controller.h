#pragma once

#include <array>
#include <chrono>
#include <thread>
#include <hidapi/hidapi.h>

//#define discern_arse_elbow

class Ps3Controller
{
    std::atomic<hid_device*> device;
    static const size_t inputBufferSize = 49;
    //unsigned char[50] buf; // received 49 bytes in test, +1 for report num
    //unsigned char inputBuffer[inputBufferSize]; // let's see how much data we can get before trimming
    std::array<unsigned char, inputBufferSize> inputBuffer;

    //std::atomic_bool available;
    std::thread controllerPoller, dataReader;
    std::atomic_bool stopControllerSearch, stopRead;

    bool readData();

    /* Button bits
     * ===========
     * 0x02 & 0x0f : Start | JoyR | JoyL | Select
     * 0x02 & 0xf0 : L | D | R | U
     * 0x03 & 0x0f : R1 | L1 | R2 | L2
     * 0x03 & 0xf0 : Square | X | O | Triangle
    */

    void searchForController();

    void startReadThread();
    void stopReadThread();
    void startControllerSearchThread();
    void stopControllerSearchThread();

public:
    Ps3Controller();
    ~Ps3Controller();


    /*
     * Get a thread system going, where we loop slowly looking for a controller when there is none
     * and we loop hid_read (blocking) when there is one (no waiting between hid_read's return and next call).
     *
     * To get safety, look into std::array.allocate with swap, or similar
     *
     */

    const bool available() { return !(device==nullptr); } // eventually this will return true when we've got a controller

    void update();
    
    //enum class CVal: decltype(inputBuffer)::size_type {
    enum CVal: decltype(inputBuffer)::size_type {
        L_x = 0x06,
        L_y = 0x07,
        R_x = 0x08,
        R_y = 0x09,
        U = 0x0e,
        R = 0x0f,
        D = 0x10,
        L = 0x11,
        L2 = 0x12,
        R2 = 0x13,
        L1 = 0x14,
        R1 = 0x15,
        Tri = 0x16,
        O = 0x17,
        X = 0x18,
        Sq = 0x19,
        Roll = 0x2a,
        Pitch = 0x2c
    };

    auto getCVal(CVal item) -> decltype(inputBuffer[item]) {
        return inputBuffer[item];
    }

    // no c++14 yet :(
    //const auto& getData();
    const decltype(inputBuffer)& getData();
};
