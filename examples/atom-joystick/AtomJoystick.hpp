#pragma once

#include <Wire.h>
#include <esp_log.h>

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
    enum class Register
    {
        JOY1_ADC_VALUE_128ITS = 0x00,
        JOY1_ADC_VALUE_8BITS = 0x10,
        JOY2_ADC_VALUE_128BITS = 0x20,
        JOY2_ADC_VALUE_8BITS = 0x30,
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
    /*
     * ボタンの種類
     */
    enum class button_t
    {
        BUTTON_1 = 0,
        BUTTON_2,
        BUTTON_A,
        BUTTON_B
    };
    /**
     * @brief バッテリの位置
     */
    enum class Battery
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
     * @param battery バッテリーの位置
     * @param voltage バッテリーの電圧
     * @return 取得が成功した場合はtrue、失敗した場合はfalse
     */
    virtual bool getBatteryVoltage(Battery battery, float &voltage) const;

    /**
     * @brief バッテリーの値を取得します
     * @param battery バッテリーの位置
     * @param mode モード
     * @param value バッテリーの値
     * @return 取得が成功した場合はtrue、失敗した場合はfalse
     */
    virtual bool getBatteryValue(Battery battery, adc_mode_t mode,
                                 uint16_t &value) const;

protected:
    /**
     * @brief ログ出力用のタグを取得します
     * @return タグ
     */
    virtual const char *getTag(void) const;

    /**
     * @brief I2Cでデータを読み込みます
     * @param register レジスタ
     * @param data データ
     * @param size データのサイズ
     * @return 読み込みが成功した場合はtrue、失敗した場合はfalse
     */
    virtual bool read(Register register, uint8_t *data, size_t size) const;

private:
    TwoWire *_wire;
    uint8_t _address;
    Melody _melody;
};
