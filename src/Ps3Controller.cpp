#include <stdio.h>
#include "Ps3Controller.h"

//#define find_arse_elbow

Ps3Controller::Ps3Controller()
{
    struct hid_device_info *devs, *dev_i;;

#ifdef find_arse_elbow
    devs = hid_enumerate(0x0, 0x0);

    dev_i = devs;
    while(dev_i) {
        printf("======= Dev ========\n");
        printf("vendor: %04hx, prod: %04hx\npath: %s, serial: %ls\n",
                dev_i->vendor_id, dev_i->product_id, dev_i->path, dev_i->serial_number);
        printf("====================\n");
        dev_i = dev_i->next;
    }

    hid_free_enumeration(devs);
    /* found ps3 controller:
     * vendor: 054c, prod: 0268
     * path: Bluetooth_054c_0268_417dbc5a, serial: 00-19-c1-7d-bc-5a
     */
#endif

    device = hid_open(0x54c, 0x0268, NULL); // device = NULL on failure
}

Ps3Controller::~Ps3Controller()
{
    if(device!=nullptr) {
        hid_close(device);
    }
    hid_exit(); // maybe there should be one manager which does this last
}
