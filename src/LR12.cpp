#include "LR12.h"

LR12::LR12() {

    Button.addVertex(ofVec3f(-1 * buttonWidth/2, -1*buttonHeight /2));
    Button.addVertex(ofVec3f(buttonWidth/2, -1*buttonHeight/2));
    Button.addVertex(ofVec3f(buttonWidth/2, buttonHeight/2));
    Button.addVertex(ofVec3f(-1 * buttonWidth/2, buttonHeight/2));
    
    Button.addIndex(0);
    Button.addIndex(1);
    Button.addIndex(2);

    Button.addIndex(2);
    Button.addIndex(3);
    Button.addIndex(0);

}

void LR12::draw() {
    static constexpr float hOffset = 200;
    static constexpr float padding = 10;

    ofPushStyle();
    ofPushMatrix();

    ofTranslate(hOffset, 0);
    ofSetColor(getColor(controller->getCVal(v::R2)));
    Button.draw();

    ofTranslate(0, buttonHeight + padding);
    ofSetColor(getColor(controller->getCVal(v::R1)));
    Button.draw();

    ofPopMatrix();
    ofPushMatrix();

    ofTranslate(-1 * hOffset, 0);
    ofSetColor(getColor(controller->getCVal(v::L2)));
    Button.draw();

    ofTranslate(0, buttonHeight + padding);
    ofSetColor(getColor(controller->getCVal(v::L1)));
    Button.draw();

    ofPopMatrix();
    ofPopStyle();
}
