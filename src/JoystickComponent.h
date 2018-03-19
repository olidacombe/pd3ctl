#pragma once

#include "Ps3Component.h"
#include "Ps3Controller.h"

class JoystickComponent : public Ps3Component {
    
    const ofVec2f &point, &tracker;
    const bool &pressed;
    
    static constexpr float defaultRadius = 100.0;
    static constexpr int pointSize = 2;
    static constexpr int crossHairArmSize = 3;

	public:
        JoystickComponent(const ofVec2f& point, const ofVec2f& tracker, const bool& pressed);
        void draw() override {
            draw(defaultRadius);
        }
        void draw(const float radius);
		
};
