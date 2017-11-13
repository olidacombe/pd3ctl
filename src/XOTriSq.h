#pragma once

#include "Ps3Component.h"

class XOTriSq : public Ps3Component {
    
    static constexpr float buttonRadius = 25;
    static constexpr float padding = 25;
    
	public:
        XOTriSq();
        void draw() override;
		
};
