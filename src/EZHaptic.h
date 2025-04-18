#pragma once

#include <Arduino.h>
#include <driver/timer.h>
#include <EZLog.h>

#define TIMER_INTERVAL_US       125000  // 1250ms

const static String BzzBzz = "xxx.xx..";    // short
const static String LongRing = "xxxxxxxx        ";
const static String LongRingUpDown = "xxxxxxxx        oooooooo        ";
const static String WeeOoo = "xxxxxxxxoooooooo";
const static String Pulse = "x x x x x x x x x x x x ";

class EZHaptic : public Loggable {
public:
    String fileName() const override { return __FILE__; }

    EZHaptic(uint8_t motorPin, timer_group_t group = TIMER_GROUP_0, timer_idx_t idx = TIMER_0);

    void begin();
    void playRepeated(const String& pattern, int frequencyMs, int repeatCount);
    void play(const String& pattern, int frequencyMs = 0);
    void stop();


private:
    void initTimer();
    static void IRAM_ATTR onTimer(void* arg);
    void handleStep();
    void executeChar(char c);

    uint8_t _motorPin;

    // PWM
    static constexpr uint8_t _pwmChannel = 0;
    static constexpr uint16_t _pwmFrequency = 200; // Hz
    static constexpr uint8_t _pwmResolution = 8;

    // Timer
    timer_group_t _timerGroup;
    timer_idx_t _timerIdx;

    String _pattern;
    int _frequencyMs;
    size_t _index;
    volatile bool _isPlaying;
};
