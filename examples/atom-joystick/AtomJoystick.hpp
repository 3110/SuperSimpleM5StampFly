#pragma once

#include <Wire.h>

#if !__has_include(<M5Unified.h>)
#include <utility/Log_Class.hpp>
namespace m5gfx {
__attribute__((unused)) static inline unsigned long millis(void) {
    return (unsigned long)(esp_timer_get_time() / 1000ULL);
}
}  // namespace m5gfx
#undef M5_LOGE
#define M5_LOGE(format, ...)                                          \
    AtomJoystick::Log(ESP_LOG_ERROR, M5UNIFIED_LOG_FORMAT(E, format), \
                      ##__VA_ARGS__)
#undef M5_LOGW
#define M5_LOGW(format, ...)                                         \
    AtomJoystick::Log(ESP_LOG_WARN, M5UNIFIED_LOG_FORMAT(W, format), \
                      ##__VA_ARGS__)
#undef M5_LOGI
#define M5_LOGI(format, ...)                                         \
    AtomJoystick::Log(ESP_LOG_INFO, M5UNIFIED_LOG_FORMAT(I, format), \
                      ##__VA_ARGS__)
#undef M5_LOGD
#define M5_LOGD(format, ...)                                          \
    AtomJoystick::Log(ESP_LOG_DEBUG, M5UNIFIED_LOG_FORMAT(D, format), \
                      ##__VA_ARGS__)
#undef M5_LOGV
#define M5_LOGV(format, ...)                                            \
    AtomJoystick::Log(ESP_LOG_VERBOSE, M5UNIFIED_LOG_FORMAT(V, format), \
                      ##__VA_ARGS__)
#endif
#include "Melody.hpp"

class AtomJoystick {
public:
    /**
     * @brief I2Cアドレス
     */
    static constexpr uint8_t I2C_ADDRESS = 0x59;
    /**
     * @brief I2C周波数
     */
    static constexpr uint32_t I2C_FREQUENCY = 400000UL;
    /**
     * @brief レジスタ
     */
    enum class register_t
    {
        JOY1_X_ADC_VALUE_12BITS = 0x00,
        JOY1_X_ADC_VALUE_8BITS = 0x10,
        JOY1_Y_ADC_VALUE_12BITS = 0x02,
        JOY1_Y_ADC_VALUE_8BITS = 0x11,
        JOY2_X_ADC_VALUE_12BITS = 0x20,
        JOY2_X_ADC_VALUE_8BITS = 0x30,
        JOY2_Y_ADC_VALUE_12BITS = 0x22,
        JOY2_Y_ADC_VALUE_8BITS = 0x31,
        BATTERY1_ADC_12BITS = 0x40,
        BATTERY1_ADC_8BITS = 0x50,
        BATTERY2_ADC_12BITS = 0x42,
        BATTERY2_ADC_8BITS = 0x51,
        BATTERY1_VOLTAGE = 0x60,
        BATTERY2_VOLTAGE = 0x62,
        BUTTON = 0x70,
        BOOTLOADER_VERSION = 0xFC,
        FIRMWARE_VERSION = 0xFE,
        I2C_ADDRESS = 0xFF,
    };
    /**
     * @brief ADCモード。
     * @note 値はバイト数と合わせている
     */
    enum class adc_mode_t
    {
        ADC_8BITS = 1,
        ADC_12BITS = 2,
    };
    /**
     * @brief ジョイスティックの位置
     */
    enum class joystick_position_t
    {
        JOYSTICK_1 = 1,
        JOYSTICK_2,
    };
    /*
     * ボタンの種類
     */
    enum class button_position_t
    {
        BUTTON_1 = 0,
        BUTTON_2,
        BUTTON_A,
        BUTTON_B
    };
    /**
     * @brief バッテリの位置
     */
    enum class battery_position_t
    {
        BATTERY_1 = 0,
        BATTERY_2,
    };
    /**
     * @brief ブザーのピン番号
     */
    static constexpr uint8_t BUZZER_PIN = 5;
    /**
     * @brief 起動時に再生するメロディ
     */
    static const melody_tone_t STARTUP_MELODY[];
    /**
     * @brief ログ出力用インスタンス
     */
    static m5::Log_Class Log;

    /**
     * @brief コンストラクタ
     * @param buzzerPin ブザーのピン番号
     */
    AtomJoystick(uint8_t buzzerPin = BUZZER_PIN);

    /**
     * @brief デストラクタ
     */
    virtual ~AtomJoystick(void);

    /**
     * @brief 初期化を行います
     * @param wire Wireオブジェクト
     * @param address I2Cアドレス
     * @param sda SDAピン番号
     * @param scl SCLピン番号
     * @param frequency I2C周波数
     * @return 初期化が成功した場合はtrue、失敗した場合はfalse
     */
    virtual bool begin(TwoWire &wire = Wire, uint8_t address = I2C_ADDRESS,
                       uint8_t sda = SDA, uint8_t scl = SCL,
                       uint32_t frequency = I2C_FREQUENCY);
    /**
     * @brief 状態の更新を行います
     * @return 状態が更新されたらtrue、更新されなかったらfalse
     */
    virtual bool update(void);

    /**
     * @brief ビープ音を鳴らします
     */
    virtual void beep(void) const;

    /**
     * @brief メロディを再生します
     * @param melody メロディ
     * @param size メロディのサイズ
     * @return 再生が成功した場合はtrue、失敗した場合はfalse
     */
    virtual bool play(const melody_tone_t melody[], size_t size) const;

    /**
     * @brief ファームウェアのバージョンを取得します
     * @param version ファームウェアのバージョン
     * @return 取得が成功した場合はtrue、失敗した場合はfalse
     */
    virtual bool getFirmwareVersion(uint8_t &version) const;

    /**
     * @brief ブートローダーのバージョンを取得します
     * @param version ブートローダーのバージョン
     * @return 取得が成功した場合はtrue、失敗した場合はfalse
     */
    virtual bool getBootLoaderVersion(uint8_t &version) const;

    /**
     * @brief バッテリーの電圧を取得します
     * @param pos バッテリーの位置
     * @param voltage バッテリーの電圧
     * @return 取得が成功した場合はtrue、失敗した場合はfalse
     */
    virtual bool getBatteryVoltage(battery_position_t pos,
                                   float &voltage) const;

    /**
     * @brief バッテリーの値を取得します
     * @param pos バッテリーの位置
     * @param mode モード
     * @param value バッテリーの値
     * @return 取得が成功した場合はtrue、失敗した場合はfalse
     */
    virtual bool getBatteryValue(battery_position_t pos, adc_mode_t mode,
                                 uint16_t &value) const;

    /**
     * @brief ジョイスティックのX軸の値を取得します
     * @param pos ジョイスティックの位置
     * @param mode モード
     * @param value ジョイスティックのX軸の値
     * @return 取得が成功した場合はtrue、失敗した場合はfalse
     */
    virtual bool getJoystickX(joystick_position_t pos, adc_mode_t mode,
                              uint16_t &value) const;

    /**
     * @brief ジョイスティックのY軸の値を取得します
     * @param pos ジョイスティックの位置
     * @param mode モード
     * @param value ジョイスティックのY軸の値
     * @return 取得が成功した場合はtrue、失敗した場合はfalse
     */
    virtual bool getJoystickY(joystick_position_t pos, adc_mode_t mode,
                              uint16_t &value) const;

protected:
    /**
     * @brief I2Cでデータを読み込みます
     * @param register レジスタ
     * @param data データ
     * @param size データのサイズ
     * @return 読み込みが成功した場合はtrue、失敗した場合はfalse
     */
    virtual bool read(register_t register, uint8_t *data, size_t size) const;

private:
    TwoWire *_wire;
    uint8_t _address;
    Melody _melody;
};
