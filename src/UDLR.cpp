#include "UDLR.h"

UDLR::UDLR(std::shared_ptr<Ps3Controller>& _controller) : controller(_controller) {
    constexpr int width = 40;
    constexpr int height = 60;
    constexpr int padding = 15;

    UButton.addVertex(ofVec3f(0, padding));
    UButton.addVertex(ofVec3f(width/2, padding + height - width));
    UButton.addVertex(ofVec3f(width/2, padding + height));
    UButton.addVertex(ofVec3f(-1*width/2, padding + height));
    UButton.addVertex(ofVec3f(-1*width/2, padding + height - width));
}

UDLR::~UDLR() {

}

const ofColor& UDLR::getColor(const float& val) {
    static ofColor c;

    c.set(val, val, val);

    return c;
}

void UDLR::draw() {

    using v = Ps3Controller::CVal;

    ofPushStyle();
    ofPushMatrix();
    ofFill();

    ofSetColor(getColor(controller->getCVal(v::D)));
    UButton.draw();

    ofRotateDeg(90);
    ofSetColor(getColor(controller->getCVal(v::L)));
    UButton.draw();

    ofRotateDeg(90);
    ofSetColor(getColor(controller->getCVal(v::U)));
    UButton.draw();

    ofRotateDeg(90);
    ofSetColor(getColor(controller->getCVal(v::R)));
    UButton.draw();

    ofPopMatrix();
    ofPopStyle();
}
