#pragma once

#include "Ps3Component.h"

class MiddleButtons : public Ps3Component {

    static constexpr float buttonHeight = 20;
    static constexpr float buttonWidth = 40;

    ofVboMesh SelectButton;
    ofVboMesh StartButton;;

    const ofColor getColor(unsigned char pressed) {
        return Ps3Component::getColor(pressed ? 255.0 : 0);
    }
    
    public:
        MiddleButtons();
        void draw() override;
};
