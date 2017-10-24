#include <iostream>
#include "Ps3Controller.h"


Ps3Controller::Ps3Controller() : device(nullptr), stopControllerSearch(true), stopRead(true)
{
    inputBuffer.fill(0);

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
                readLoop();
            } else if(!stopControllerSearch) { // search
                searchForController();
            }
        }
    });
}

// I think this doesn't work :?
void Ps3Controller::stopWorkThread() {
    stopWork = true;
    stopControllerSearch = true;
    stopRead = true;
    if(workThread.joinable()) workThread.join();
}


void Ps3Controller::searchForController() {

    static std::chrono::seconds waitTime{1};

    while(!stopControllerSearch && device == nullptr) {
        std::this_thread::sleep_for(waitTime);

        device = hid_open(0x054c, 0x0268, nullptr); // device = nullptr on failure
        if(device != nullptr) {
            std::cout << "controller found" << std::endl;
            hid_set_nonblocking(device, 1); // nonblocking - but give generous timeout
            stopControllerSearch = true;
            stopRead = false;
        }
    }
}

void Ps3Controller::readLoop() {
    while(!stopRead && readData()); // too aggressive?
    stopRead = true;
    stopControllerSearch = false;
}


bool Ps3Controller::readData()
{
  if(device==nullptr) return false;

  // thread safety?
  //const int readResult = hid_read(device, inputBuffer.data(), inputBuffer.size());
  const int readResult = hid_read_timeout(device, inputBuffer.data(), inputBuffer.size(), 10);
  
  if(readResult==-1) return false; // error, may have lost device and need to slow scan
  if(readResult==0) { /* we're non-blocking and there's no data available at the mo */ }

  return true;
}


// no c++14 yet :(
//const auto& Ps3Controller::getData()
const decltype(Ps3Controller::inputBuffer)& Ps3Controller::getData()
{
    return inputBuffer;
}

Ps3Controller::~Ps3Controller()
{
    stopWorkThread();

    if(device!=nullptr) {
        hid_close(device);
    }
    hid_exit(); // maybe there should be one manager which does this last
}

