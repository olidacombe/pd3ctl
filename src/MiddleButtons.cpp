#include "MiddleButtons.h"

MiddleButtons::MiddleButtons() {

    SelectButton.addVertex(ofVec3f(0, -1*buttonHeight /2));
    SelectButton.addVertex(ofVec3f(0, buttonHeight /2));
    SelectButton.addVertex(ofVec3f(-1*buttonWidth, buttonHeight /2));
    SelectButton.addVertex(ofVec3f(-1*buttonWidth, -1*buttonHeight /2));

    SelectButton.addIndex(0);
    SelectButton.addIndex(1);
    SelectButton.addIndex(2);

    SelectButton.addIndex(2);
    SelectButton.addIndex(3);
    SelectButton.addIndex(0);

    StartButton.addVertex(ofVec3f(0, -1*buttonHeight /2));
    StartButton.addVertex(ofVec3f(0, buttonHeight /2));
    StartButton.addVertex(ofVec3f(buttonWidth, 0));
    
    StartButton.addIndex(0);
    StartButton.addIndex(1);
    StartButton.addIndex(2);

}

void MiddleButtons::draw() {
    static constexpr float hOffset = 50;
    static constexpr float vOffset = 40;

    ofPushStyle();
    ofPushMatrix();

    const auto sw_buts = controller->getCVal(v::SW_buttons);
    // Start Button
    ofTranslate(hOffset, 0);
    ofSetColor(getColor(sw_buts & bmask::Start));
    StartButton.draw();

    // Select Button
    ofTranslate(-2* hOffset, 0);
    ofSetColor(getColor(sw_buts & bmask::Select));
    SelectButton.draw();
    
    // PS button
    ofTranslate(hOffset, vOffset);
    ofSetColor(getColor(controller->getCVal(v::PS)));
    ofDrawCircle(0, 0, buttonWidth / 2);


    ofPopMatrix();
    ofPopStyle();
}
