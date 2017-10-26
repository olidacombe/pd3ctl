#include <iostream>
#include "Ps3Controller.h"


Ps3Controller::Ps3Controller() : device(nullptr), stopControllerSearch(true), stopRead(true), input{&buffers[0]}, output{&buffers[1]}
{
    output->fill(0);

    hid_init();

    runWorkThread();
}

void Ps3Controller::runWorkThread() {
    stopWorkThread();
    stopWork = false;
    stopRead = true;
    stopControllerSearch = false;
    workThread = std::thread( [this] {
        while(!stopWork) {
            if(!stopRead) { // read
                logMessage("read loop");
                readLoop();
            } else if(!stopControllerSearch) { // search
                logMessage("search loop");
                searchForController();
            }
        }
    });
}

void Ps3Controller::stopWorkThread() {
    stopWork = true;
    stopControllerSearch = true;
    stopRead = true;
    if(workThread.joinable()) workThread.join();
}


void Ps3Controller::searchForController() {

    static std::chrono::seconds waitTime{1};

    while(!stopControllerSearch) { // && device == nullptr) {
        std::this_thread::sleep_for(waitTime);

        device = hid_open(0x054c, 0x0268, nullptr); // device = nullptr on failure
        if(device != nullptr) {
            logMessage("controller found");
            hid_set_nonblocking(device, 1); // nonblocking - but give generous timeout
            stopControllerSearch = true;
            stopRead = false;
        }
    }
}

void Ps3Controller::readLoop() {

    while(!stopRead && readData());

    if(!stopWork) logMessage("controller lost?");

    hid_close(device); // bad idea?  Try wondering far away and see what happens
    stopRead = true;
    stopControllerSearch = false;
}


bool Ps3Controller::readData()
{
  if(device==nullptr) return false;

  // thread safety?
  const int readResult = hid_read_timeout(device, input->data(), input->size(), 10);
  
  if(readResult == -1) return false; // error, may have lost device and need to slow scan
  if(readResult != 0) { /* 0 means we're non-blocking and there's no data available at the mo */
      // do swap
      std::swap(input, output);
  }

  return true;
}


// no c++14 yet :(
//const auto& Ps3Controller::getData()
const Ps3Controller::bufferType Ps3Controller::getData()
{
    return *output;
}

Ps3Controller::~Ps3Controller()
{
    stopWorkThread();

    hid_exit(); // maybe there should be one manager which does this last
}

