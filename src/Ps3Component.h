#pragma once

#include "ofMain.h"
#include "Ps3Controller.h"

class Ps3Component {

    protected:
        using v = Ps3Controller::CVal;

        std::shared_ptr<Ps3Controller> controller;

        //virtual const ofColor&& getColor(const float& val) {
        virtual const ofColor getColor(const float& val) {
            ofColor c(val, val, val);
            return std::move(c);
            //return c;
        }

    public:
        Ps3Component() {
            controller = Ps3Controller::getOne();
        }
        virtual ~Ps3Component() = default;
        virtual void draw() = 0;
};
