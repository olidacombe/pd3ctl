#include <stdio.h>
#include "Ps3Controller.h"


Ps3Controller::Ps3Controller() : device(nullptr)
{
    inputBuffer.fill(0);

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

void Ps3Controller::searchForController() {
	if(device != nullptr) { // shouldn't get here
		hid_close(device);
		device = nullptr;
		//available = false;
		return;
	}	
	device = hid_open(0x054c, 0x0268, nullptr); // device = nullptr on failure
}

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
    const int readStatus = readData();
    // do stuff with readStatus
}

// no c++14 yet :(
//const auto& Ps3Controller::getData()
const decltype(Ps3Controller::inputBuffer)& Ps3Controller::getData()
{
    return inputBuffer;
}

Ps3Controller::~Ps3Controller()
{
    if(device!=nullptr) {
        hid_close(device);
    }
    hid_exit(); // maybe there should be one manager which does this last
}

