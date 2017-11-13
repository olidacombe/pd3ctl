#pragma once

#include "Ps3Controller.h"
#include "ofMain.h"

class Ps3Component {

    protected:
        std::shared_ptr<Ps3Controller> controller;

        virtual const ofColor&& getColor(const float& val) {
            ofColor c(val, val, val);
            return std::move(c);
        }

    public:
        Ps3Component() {
            controller = Ps3Controller::getOne();
        }
        virtual ~Ps3Component() = default;
        virtual void draw() = 0;
};
