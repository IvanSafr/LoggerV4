#ifndef STATE_SCREEN_H
#define STATE_NET_SETUP_H

#include "FSM_Definitions.h"

class State_ScreenOn : public BaseState {
    uint32_t _startTime;
public:
    void onEnter() override {
        _startTime = millis();
        Serial.println("FSM: -> SCREEN_ON");
        // Display.show();
    }
    void onUpdate() override {
        // Если прошло больше А секунд (берем из конфига)
        // if (millis() - _startTime > AppConfig.A * 1000) { ... }
    }
    void onExit() override { /* Display.hide(); */ }
    StateType getType() const override { return StateType::SCREEN_ON; }
};

#endif