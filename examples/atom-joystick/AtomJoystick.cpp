#include "AtomJoystick.hpp"

const melody_tone_t AtomJoystick::STARTUP_MELODY[] = {
    {NOTE_D4, 200},
    {NOTE_A4, 200},
    {NOTE_F4, 200},
    {NOTE_G4, 200},
};

AtomJoystick::AtomJoystick(uint8_t buzzerPin) : _melody(buzzerPin) {
}

AtomJoystick::~AtomJoystick(void) {
}

bool AtomJoystick::begin(void) {
    return _melody.play(STARTUP_MELODY, _countof(STARTUP_MELODY));
}
