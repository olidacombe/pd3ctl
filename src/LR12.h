#pragma once

#include "Ps3Component.h"

class LR12 : public Ps3Component {

    static constexpr float buttonHeight = 40;
    static constexpr float buttonWidth = 100;

    ofVboMesh Button;

    public:
        LR12();
        void draw() override;
};
