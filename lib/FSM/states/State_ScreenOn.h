#ifndef STATE_SCREEN_H
#define STATE_SCREEN_H

#include "FSM_Definitions.h"
#include "FSMManager.h"    // Для fsm
#include "DisplayUI.h"     // Наш новый UI модуль
#include "AppConfig.h"     // Для времени A_screenTime

class State_ScreenOn : public BaseState {
private:
    uint32_t _startTime;
public:
    void onEnter() override {
        Serial.println("FSM: -> SCREEN_ON");
        _startTime = millis();
        DisplayUI.show(); // Включаем дисплей и рисуем экран
    }

    void onUpdate() override {
        // Если прошло больше А секунд (берем из конфига)
        if (millis() - _startTime > AppConfig.A_screenTime * 1000) {
            Serial.println("Screen timeout. Going to sleep.");
            fsm.handleEvent(FSMEvent::TIMEOUT);
        }
    }

    void onExit() override {
        Serial.println("FSM: <- SCREEN_ON");
        DisplayUI.hide(); // Выключаем дисплей при выходе
    }

    StateType getType() const override { return StateType::SCREEN_ON; }

    // <<< НОВОЕ: Метод для сброса таймера извне >>>
    void resetTimer() {
        _startTime = millis();
        Serial.println("Screen timer reset.");
    }
};

#endif