#include <Arduino.h>

#include "AtomJoystick.hpp"

AtomJoystick joystick;

void setup(void) {
    joystick.begin();
}

void loop(void) {
    joystick.update();
}
