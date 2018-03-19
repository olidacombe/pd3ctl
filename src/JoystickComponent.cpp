#include "JoystickComponent.h"

JoystickComponent::JoystickComponent(const ofVec2f& _point, const ofVec2f& _tracker, const bool& _pressed) : point(_point), tracker(_tracker), pressed(_pressed)
{}

void JoystickComponent::draw(const float radius) {

    using v = Ps3Controller::CVal;

    ofPushStyle();
    ofPushMatrix();
    
    if(pressed) {
        ofPushStyle();
        ofSetColor(0xff, 0xff, 0xff, 0x7f);
        ofDrawCircle(0, 0, radius);
        ofDrawLine(0, 0, point.x * radius, point.y * radius);
        ofPopStyle();
    }

    ofSetColor(0xff, 0xff, 0xff, 0x5f);
    ofDrawCircle(0, 0, pointSize);
    ofSetColor(0xff, 0xff, 0xff, 0xff);
    ofDrawCircle(point * radius, pointSize);

    ofPopMatrix();
    ofPopStyle();
}
