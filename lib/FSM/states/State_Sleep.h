#ifndef STATE_SLEEP_H // <<< НОВОЕ: Добавлены защитные директивы
#define STATE_SLEEP_H

#include "FSM_Definitions.h"
#include "AppConfig.h" // <<< НОВОЕ: Нужен для доступа к T_sleepSeconds
#include "Pinout.h"    // <<< НОВОЕ: Нужен для доступа к BUTTON_PIN
#include <esp_sleep.h> // <<< НОВОЕ: Основная библиотека для управления сном

class State_Sleep : public BaseState {
public:
    void onEnter() override {
        Serial.printf("FSM: -> SLEEP (for %u seconds)\n", AppConfig.T_sleepSeconds);

        // <<< НОВОЕ: Логика ухода в сон >>>
        // 1. Настройка пробуждения по таймеру
        esp_sleep_enable_timer_wakeup(AppConfig.T_sleepSeconds * 1000000ULL); // Время в микросекундах

        // 2. Настройка пробуждения по кнопке (GPIO 0).
        // Пробуждение по низкому уровню сигнала (кнопка нажата)
        gpio_wakeup_enable(GPIO_NUM_10, GPIO_INTR_LOW_LEVEL);

        Serial.println("Entering Light Sleep...");
        Serial.flush(); // Убедимся, что все сообщения успели отправиться в порт

        // 3. Уход в сон
        esp_light_sleep_start();
    }
    void onUpdate() override {}
    void onExit() override { Serial.println("FSM: <- SLEEP (Waking up)"); }
    StateType getType() const override { return StateType::SLEEP; }
};

#endif // STATE_SLEEP_H