#pragma once

#include "ofMain.h"
#include "Ps3Controller.h"

class UDLR {
    
    std::shared_ptr<Ps3Controller> controller;

    ofVboMesh UButton; // up button

    const ofColor& getColor(const float& val);
    
	public:
        UDLR(std::shared_ptr<Ps3Controller>&);
        ~UDLR();
        void draw();
		
};
