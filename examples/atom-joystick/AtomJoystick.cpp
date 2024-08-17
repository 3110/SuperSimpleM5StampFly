#include "AtomJoystick.hpp"

const melody_tone_t AtomJoystick::STARTUP_MELODY[] = {
    {294, 200},
    {441, 200},
    {350, 200},
    {393, 200},
};

AtomJoystick::AtomJoystick(uint8_t buzzerPin) : _melody(buzzerPin) {
}

AtomJoystick::~AtomJoystick(void) {
}

bool AtomJoystick::begin(void) {
    return _melody.play(STARTUP_MELODY, _countof(STARTUP_MELODY));
}
