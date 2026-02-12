#ifndef STATE_NET_SETUP_H
#define STATE_NET_SETUP_H

#include "FSM_Definitions.h"
#include <WiFi.h>
#include "time.h"
#include "AppConfig.h" // Предполагаем, что тут хранятся SSID и PASS

class State_NetSetup : public BaseState {
private:
    uint32_t _startAttemptTime;
    const uint32_t _timeout = 15000; // 15 секунд на попытку подключения
    bool _isSyncing = false;

public:
    void onEnter() override {
        Serial.println("FSM: -> NET_SETUP (WiFi connecting...)");
        _startAttemptTime = millis();
        _isSyncing = false;

        WiFi.begin(AppConfig.ssid, AppConfig.pass);
    }

    void onUpdate() override {
        // 1. Проверка статуса подключения
        if (WiFi.status() == WL_CONNECTED) {
            if (!_isSyncing) {
                Serial.println("WiFi Connected. Syncing NTP...");
                configTime(0, 0, "pool.ntp.org", "time.nist.gov");
                _isSyncing = true;
            }

            // Проверяем, получено ли системное время (год > 1970)
            struct tm timeinfo;
            if (getLocalTime(&timeinfo)) {
                Serial.println("Time Synchronized!");
                // Сообщаем менеджеру об успехе
                extern class FSMManager fsm; 
                fsm.handleEvent(FSMEvent::SYNC_SUCCESS);
            }
        }

        // 2. Проверка тайм-аута (если WiFi не подключился за 15 сек)
        if (millis() - _startAttemptTime > _timeout) {
            Serial.println("WiFi Timeout!");
            extern class FSMManager fsm;
            fsm.handleEvent(FSMEvent::SYNC_FAIL);
        }
    }

    void onExit() override {
        Serial.println("FSM: <- NET_SETUP (Turning WiFi off)");
        WiFi.disconnect(true);
        WiFi.mode(WIFI_OFF); // Важно для энергосбережения
    }

    StateType getType() const override { return StateType::NET_SETUP; }
};

#endif