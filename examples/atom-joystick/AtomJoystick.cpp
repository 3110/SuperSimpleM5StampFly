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
    return play(STARTUP_MELODY, _countof(STARTUP_MELODY));
}

void AtomJoystick::beep(void) const {
    _melody.beep();
}

bool AtomJoystick::play(const melody_tone_t melody[], size_t size) const {
    return _melody.play(melody, size);
}