#include "EZHaptic.h"


EZHaptic::EZHaptic(uint8_t motorPin, timer_group_t group, timer_idx_t idx)
    : _motorPin(motorPin), _timerGroup(group), _timerIdx(idx),
      _index(0), _isPlaying(false), _frequencyMs(250) { // Standardwert
}

void EZHaptic::begin() {
    // PWM initialisieren
    ledcSetup(_pwmChannel, _pwmFrequency, _pwmResolution);
    ledcAttachPin(_motorPin, _pwmChannel);
    ledcWrite(_pwmChannel, 0);

    // Timer vorbereiten (aber noch nicht starten)
    initTimer();
}

void EZHaptic::playRepeated(const String& pattern, int frequencyMs, int repeatCount) {
    String repeated;
    for (int i = 0; i < repeatCount; ++i) {
        repeated += pattern;
    }
    play(repeated, frequencyMs);
}

void EZHaptic::play(const String& pattern, int frequencyMs) {
    _pattern = pattern;
    _index = 0;
    _isPlaying = true;

    if (frequencyMs == 0) {
        frequencyMs = TIMER_INTERVAL_US / 1000;
    }

    if (frequencyMs > 0 && frequencyMs != _frequencyMs) {
        _frequencyMs = frequencyMs;
        timer_pause(_timerGroup, _timerIdx);
        timer_set_alarm_value(_timerGroup, _timerIdx, _frequencyMs * 1000);
        timer_set_counter_value(_timerGroup, _timerIdx, 0);
        timer_enable_intr(_timerGroup, _timerIdx);
    } else {
        timer_set_counter_value(_timerGroup, _timerIdx, 0);
    }

    timer_start(_timerGroup, _timerIdx);
}

void EZHaptic::stop() {
    timer_pause(_timerGroup, _timerIdx);
    ledcWrite(_pwmChannel, 0);
    _isPlaying = false;
}

void EZHaptic::initTimer() {
    timer_config_t config = {
        .alarm_en = TIMER_ALARM_EN,
        .counter_en = TIMER_PAUSE,
        .intr_type = TIMER_INTR_LEVEL,
        .counter_dir = TIMER_COUNT_UP,
        .auto_reload = TIMER_AUTORELOAD_EN,
        .divider = 80  // 80 MHz / 80 = 1 µs Takt
    };
    timer_init(_timerGroup, _timerIdx, &config);
    timer_set_counter_value(_timerGroup, _timerIdx, 0);
    timer_set_alarm_value(_timerGroup, _timerIdx, TIMER_INTERVAL_US);
    timer_enable_intr(_timerGroup, _timerIdx);
    timer_isr_register(_timerGroup, _timerIdx, onTimer,
                       this, ESP_INTR_FLAG_IRAM, nullptr);
}

void IRAM_ATTR EZHaptic::onTimer(void* arg) {
    auto* self = static_cast<EZHaptic*>(arg);

    // ❗ Korrekt plattformübergreifend Interrupt quittieren
    timer_group_clr_intr_status_in_isr(self->_timerGroup, self->_timerIdx);

    self->handleStep();

    // ❗ Falls notwendig (bei manchen SDKs):
    timer_group_enable_alarm_in_isr(self->_timerGroup, self->_timerIdx);
}

void EZHaptic::handleStep() {
    if (!_isPlaying || _index >= _pattern.length()) {
        stop();
        return;
    }

    const char current = _pattern[_index++];
    executeChar(current);
}

void EZHaptic::executeChar(char c) {
    switch (c) {
        case 'x': ledcWrite(_pwmChannel, 255); break;   // volle Kraft
        case 'o': ledcWrite(_pwmChannel, 200); break;   // halbe Kraft
        case '.': ledcWrite(_pwmChannel, 150); break;   // 1/4 Kraft
        case '_':
        case ' ': ledcWrite(_pwmChannel, 0); break;
        case '!': stop(); break;
        default: break;
    }
}
