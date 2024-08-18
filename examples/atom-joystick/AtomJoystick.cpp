#include "AtomJoystick.hpp"

static constexpr char TAG[] = "AtomJoystick";

const melody_tone_t AtomJoystick::STARTUP_MELODY[] = {
    {NOTE_D4, 200},
    {NOTE_A4, 200},
    {NOTE_F4, 200},
    {NOTE_G4, 200},
};

AtomJoystick::AtomJoystick(uint8_t buzzerPin)
    : _wire(nullptr), _address(0), _melody(buzzerPin) {
}

AtomJoystick::~AtomJoystick(void) {
}

bool AtomJoystick::begin(TwoWire &wire, uint8_t address, uint8_t sda,
                         uint8_t scl, uint32_t frequency) {
    this->_wire = &wire;
    this->_address = address;
    this->_wire->begin(sda, scl, frequency);
    delay(10);
    uint8_t firmwareVersion;
    if (!getFirmwareVersion(firmwareVersion)) {
        ESP_LOGE(TAG, "Failed to initialize I2C");
        return false;
    }
#if defined(CORE_DEBUG_LEVEL) && (CORE_DEBUG_LEVEL >= ESP_LOG_DEBUG)
    uint8_t bootloaderVersion;
    if (!getBootLoaderVersion(bootloaderVersion)) {
        ESP_LOGE(TAG, "Failed to get bootloader version");
        return false;
    }
    ESP_LOGD(TAG, "Version: Firmware %d, Bootloader %d", firmwareVersion,
             bootloaderVersion);
    float battery1Voltage = 0.0f;
    if (!getBatteryVoltage(battery_t::BATTERY_1, battery1Voltage)) {
        ESP_LOGE(TAG, "Failed to get battery 1 voltage");
        return false;
    }
    float battery2Voltage = 0.0f;
    if (!getBatteryVoltage(battery_t::BATTERY_2, battery2Voltage)) {
        ESP_LOGE(TAG, "Failed to get battery 2 voltage");
        return false;
    }
    ESP_LOGD(TAG, "Battery Voltage: (1) %3.2fV, (2) %3.2fV", battery1Voltage,
             battery2Voltage);

    uint16_t battery1Value = 0;
    if (!getBatteryValue(battery_t::BATTERY_1, adc_mode_t::ADC_12BITS,
                         battery1Value)) {
        ESP_LOGE(TAG, "Failed to get battery 1 value");
        return false;
    }
    uint16_t battery2Value = 0;
    if (!getBatteryValue(battery_t::BATTERY_2, adc_mode_t::ADC_12BITS,
                         battery2Value)) {
        ESP_LOGE(TAG, "Failed to get battery 2 value");
        return false;
    }
    ESP_LOGD(TAG, "Battery Value: (1) %d, (2) %d", battery1Value,
             battery2Value);
#endif
    return play(STARTUP_MELODY, _countof(STARTUP_MELODY));
}

bool AtomJoystick::update(void) {
    return false;
}

void AtomJoystick::beep(void) const {
    _melody.beep();
}

bool AtomJoystick::play(const melody_tone_t melody[], size_t size) const {
    return _melody.play(melody, size);
}

bool AtomJoystick::getFirmwareVersion(uint8_t &version) const {
    return read(register_t::FIRMWARE_VERSION, &version, sizeof(version));
}

bool AtomJoystick::getBootLoaderVersion(uint8_t &version) const {
    return read(register_t::BOOTLOADER_VERSION, &version, sizeof(version));
}

bool AtomJoystick::getBatteryVoltage(battery_t battery, float &voltage) const {
    uint8_t data[sizeof(voltage)];
    if (!read(battery == battery_t::BATTERY_1 ? register_t::BATTERY1_VOLTAGE
                                              : register_t::BATTERY2_VOLTAGE,
              data, sizeof(data))) {
        ESP_LOGE(TAG, "Failed to read battery voltage");
        return false;
    }
    voltage = ((data[1] << 8) | data[0]) * 0.001f;
    return true;
}

bool AtomJoystick::getBatteryValue(battery_t battery, adc_mode_t mode,
                                   uint16_t &value) const {
    uint8_t data[static_cast<uint8_t>(mode)];
    if (!read(battery == battery_t::BATTERY_1
                  ? (mode == adc_mode_t::ADC_12BITS
                         ? register_t::BATTERY1_ADC_12BITS
                         : register_t::BATTERY1_ADC_8BITS)
                  : (mode == adc_mode_t::ADC_12BITS
                         ? register_t::BATTERY2_ADC_12BITS
                         : register_t::BATTERY2_ADC_8BITS),
              data, sizeof(data))) {
        ESP_LOGE(TAG, "Failed to read battery value");
        return false;
    }
    if (mode == adc_mode_t::ADC_12BITS) {
        value = ((data[1] << 8) | data[0]);
    } else {
        value = data[0];
    }
    return true;
}

const char *AtomJoystick::getTag(void) const {
    return TAG;
}

bool AtomJoystick::read(register_t reg, uint8_t *data, size_t size) const {
    this->_wire->beginTransmission(this->_address);
    this->_wire->write(static_cast<uint8_t>(reg));
    if (this->_wire->endTransmission() != 0) {
        ESP_LOGE(TAG, "Failed to write to register 0x%02X",
                 static_cast<uint8_t>(reg));
        return false;
    }
    if (this->_wire->requestFrom(this->_address, size) != size) {
        ESP_LOGE(TAG, "Failed to read data from register 0x%02X",
                 static_cast<uint8_t>(reg));
        return false;
    }
    for (size_t i = 0; i < size; i++) {
        data[i] = this->_wire->read();
    }
    return true;
}
