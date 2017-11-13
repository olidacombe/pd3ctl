#pragma once

#include "ofMain.h"
#include "Ps3Component.h"

class UDLR : public Ps3Component {
    
    ofVboMesh UButton; // up button

    //const ofColor& getColor(const float& val);
    
	public:
        UDLR();
        void draw() override;
		
};
