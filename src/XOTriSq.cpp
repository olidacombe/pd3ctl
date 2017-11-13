#include "XOTriSq.h"

XOTriSq::XOTriSq() {
}

void XOTriSq::draw() {

    using v = Ps3Controller::CVal;

    ofPushStyle();
    ofPushMatrix();
    ofFill();

    ofSetColor(getColor(controller->getCVal(v::X)));
    ofDrawCircle(0, padding + buttonRadius, buttonRadius);

    ofSetColor(getColor(controller->getCVal(v::O)));
    ofDrawCircle(padding + buttonRadius, 0, buttonRadius);

    ofSetColor(getColor(controller->getCVal(v::Tri)));
    ofDrawCircle(0, -1 * (padding + buttonRadius), buttonRadius);

    ofSetColor(getColor(controller->getCVal(v::Sq)));
    ofDrawCircle(-1 * (padding + buttonRadius), 0, buttonRadius);

    ofPopMatrix();
    ofPopStyle();
}
