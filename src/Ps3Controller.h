#pragma once

#include <hidapi/hidapi.h>

class Ps3Controller
{
    hid_device *device;

public:
    Ps3Controller();
    ~Ps3Controller();
};
