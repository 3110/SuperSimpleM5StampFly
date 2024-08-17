#pragma once

#include "Melody.hpp"

class AtomJoystick {
public:
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
     * @return 初期化が成功した場合はtrue、失敗した場合はfalse
     */
    virtual bool begin(void);
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

private:
    Melody _melody;
};
