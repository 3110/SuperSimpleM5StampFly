#pragma once

#include <Arduino.h>

#include "Pitches.hpp"

#if !defined(_countof)
#define _countof(array) (sizeof(array) / sizeof(array[0]))
#endif

/**
 * @brief メロディ音を表す構造体
 * @details メロディ音は周波数と持続時間で表されます
 * @param frequency 周波数
 * @param duration 持続時間
 */
struct melody_tone_t
{
    unsigned int frequency;
    unsigned long duration;
};

class Melody {
public:
    /**
     * @brief ビープの周波数
     */
    static constexpr unsigned int BEEP_FREQUENCY = 4000U;
    /**
     * @brief ビープの持続時間
     */
    static constexpr unsigned long BEEP_DURATION_MS = 100UL;
    /**
     * @brief ビープ音
     */
    static const melody_tone_t BEEP_TONE;

    /**
     * @brief コンストラクタ
     * @param pin ブザーのピン番号
     */
    Melody(uint8_t pin);
    /**
     * @brief デストラクタ
     */
    virtual ~Melody(void) = default;
    /**
     * @brief メロディを再生します
     * @param tone メロディ音
     * @return 再生が成功した場合はtrue、失敗した場合はfalse
     */
    virtual bool play(const melody_tone_t& tone) const;
    /**
     * @brief メロディを再生します
     * @param tones メロディ音の配列
     * @param size メロディ音の配列のサイズ
     * @return 再生が成功した場合はtrue、失敗した場合はfalse
     */
    virtual bool play(const melody_tone_t tones[], size_t size) const;

    /**
     * @brief ビープ音を再生します
     */
    virtual void beep(void) const;

private:
    const uint8_t _pin;
};
