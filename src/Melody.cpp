#include "Melody.hpp"

const melody_tone_t Melody::BEEP_TONE = {BEEP_FREQUENCY, BEEP_DURATION_MS};

Melody::Melody(uint8_t pin) : _pin(pin) {
}

bool Melody::play(const melody_tone_t& tone) const {
    ::tone(this->_pin, tone.frequency, tone.duration);
    return true;
}

bool Melody::play(const melody_tone_t tones[], size_t size) const {
    for (size_t i = 0; i < size; i++) {
        play(tones[i]);
    }
    return true;
}

void Melody::beep(void) const {
    play(BEEP_TONE);
}
