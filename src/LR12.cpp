#include "LR12.h"

LR12::LR12() {

    Button.addVertex(ofVec3f(0, 0));
    Button.addVertex(ofVec3f(buttonWidth, 0));
    Button.addVertex(ofVec3f(buttonWidth, buttonHeight));
    Button.addVertex(ofVec3f(0, buttonHeight));
    
    Button.addIndex(0);
    Button.addIndex(1);
    Button.addIndex(2);

    Button.addIndex(2);
    Button.addIndex(3);
    Button.addIndex(0);

}

void LR12::draw() {
    static constexpr float hOffset = 100;
    static constexpr float padding = 10;

    ofPushStyle();
    ofPushMatrix();

    ofTranslate(hOffset, 0);
    ofSetColor(getColor(controller->getCVal(v::R2)));
    Button.draw();

    ofPopMatrix();
    ofPopStyle();
}
