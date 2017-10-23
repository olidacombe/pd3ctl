#include <iostream>
#include "Ps3Controller.h"


Ps3Controller::Ps3Controller() : device(nullptr), stopControllerSearch(true), stopRead(true)
{
    inputBuffer.fill(0);

    hid_init();

    /*
    device = hid_open(0x54c, 0x0268, NULL); // device = NULL on failure

    if(device != nullptr) {
        available = true;
        //hid_set_nonblocking(device, 1); // we will want to set it blocking in thread
        startReadThread();
    } else {
        startControllerSearchThread();
    }
    */

    startControllerSearchThread();
}

void Ps3Controller::startControllerSearchThread()
{
    stopReadThread();
    stopControllerSearch = false;
    controllerPoller = std::thread(&Ps3Controller::searchForController, this);
}

void Ps3Controller::stopControllerSearchThread()
{
    // check out this type of thing:
    // https://stackoverflow.com/questions/32206611/how-to-exit-from-a-background-thread-loop
    stopControllerSearch = true;
    if(controllerPoller.joinable()) controllerPoller.join();
}

void Ps3Controller::searchForController() {

    static std::chrono::seconds waitTime{1};

    while(!stopControllerSearch && device == nullptr) {
        device = hid_open(0x054c, 0x0268, nullptr); // device = nullptr on failure
        if(device != nullptr) {
            std::cout << "controller found" << std::endl;
        }
        std::this_thread::sleep_for(waitTime);
    }
}

void Ps3Controller::startReadThread() {
    stopControllerSearchThread();
    stopRead = false;
    dataReader = std::thread( [this] {
        while(!stopRead && readData()); // too aggressive?
    });
}

void Ps3Controller::stopReadThread() {
    stopRead = true;
    if(dataReader.joinable()) dataReader.join();
}

// replace with lambda?
/*
void Ps3Controller::readLoop() {
    while(!(stopRead=readData()));
}
*/

bool Ps3Controller::readData()
{
  if(device==nullptr) return false;

  // thread safety?
  //const int readResult = hid_read(device, inputBuffer.data(), inputBuffer.size());
  const int readResult = hid_read_timeout(device, inputBuffer.data(), inputBuffer.size(), 10);
  
  if(readResult==-1) return false; // error, may have lost device and need to slow scan
  if(readResult==0) { /* we're non-blocking and there's no data available at the mo */ }

  else {
    // debug dump
    /*
    for(int i=0; i<readResult; i++) {
      printf("%02hx ", inputBuffer[i]);
    }
    printf("\n");
    */
  }

  return true;
}

void Ps3Controller::update()
{
    if(!readData()) {
        // lost device?
    }
}

// no c++14 yet :(
//const auto& Ps3Controller::getData()
const decltype(Ps3Controller::inputBuffer)& Ps3Controller::getData()
{
    return inputBuffer;
}

Ps3Controller::~Ps3Controller()
{
    stopControllerSearchThread();
    if(device!=nullptr) {
        hid_close(device);
    }
    hid_exit(); // maybe there should be one manager which does this last
}

