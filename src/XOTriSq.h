#pragma once

#include "ofMain.h"
#include "Ps3Component.h"

class XOTriSq : public Ps3Component {
    
    const ofColor& getColor(const float& val);
    
	public:
        XOTriSq();
        void draw() override;
		
};
