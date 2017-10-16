#include <stdio.h>
#include "Ps3Controller.h"


Ps3Controller::Ps3Controller()
{
    struct hid_device_info *devs, *dev_i;;

    device = hid_open(0x54c, 0x0268, NULL); // device = NULL on failure

#ifdef discern_arse_elbow
    getFeatureReport();
#endif

    readData();
}

bool Ps3Controller::readData()
{
  if(device==nullptr) return false;

  const int readResult = hid_read(device, inputBuffer, inputBufferSize);
  if(readResult==-1) return false; // error, may have lost device and need to slow scan
  if(readResult==0) { /* we're non-blocking and there's no data available at the mo */ }

  // debug dump
  else {
    for(int i=0; i<readResult; i++) {
      printf("%02hx ", inputBuffer[i]);
    }
    printf("\n");
  }

  return true;
}

#ifdef discern_arse_elbow
void Ps3Controller::getFeatureReport()
{
  if(device==nullptr) return;

  inputBuffer[0] = 0x0;
  const int report0n = hid_get_feature_report(device, inputBuffer, inputBufferSize);
  printf("report0n: %i\n", report0n);
  inputBuffer[0] = 0x1;
  const int report1n = hid_get_feature_report(device, inputBuffer, inputBufferSize);
  printf("report1n: %i\n", report1n);
  inputBuffer[0] = 0x2;
  const int report2n = hid_get_feature_report(device, inputBuffer, inputBufferSize);
  printf("report2n: %i\n", report2n);
}
#endif

Ps3Controller::~Ps3Controller()
{
    if(device!=nullptr) {
        hid_close(device);
    }
    hid_exit(); // maybe there should be one manager which does this last
}
