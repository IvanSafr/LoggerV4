#ifndef STATE_WORK_H
#define STATE_WORK_H

#include "FSM_Definitions.h"
#include "FSMManager.h"
#include "AppConfig.h"
#include "DataLogger.h"
#include "Pinout.h" // <<< ИЗМЕНЕНО: Используем Pinout.h для пина батареи
#include "Adafruit_AHTX0.h"

extern Adafruit_AHTX0 aht;
extern DataLoggerClass dataLogger;

class State_Work : public BaseState {
public:
    void onEnter() override {
        Serial.println("FSM: -> WORK (Measuring)");

        sensors_event_t humidity, temp;
        aht.getEvent(&humidity, &temp);

        // <<< ИЗМЕНЕНО: Используем константу из Pinout.h >>>
        float v_bat = (analogRead(BATTERY_ADC_PIN) * 3.3 / 4095.0) * 2.0;
        int bat_pct = map(v_bat * 100, 330, 420, 0, 100);
        bat_pct = constrain(bat_pct, 0, 100);

        DataEntry entry;
        entry.timestamp = time(nullptr);
        entry.temperature = temp.temperature;
        entry.humidity = humidity.relative_humidity;
        entry.battery = bat_pct;

        dataLogger.write(entry);

        AppConfig.wakeCount++;
        AppConfig.save();

        // <<< НОВОЕ: Ключевая логика принятия решений о следующем состоянии >>>
        // Проверяем, нужно ли синхронизировать время (самый высокий приоритет)
        if (AppConfig.wakeCount % AppConfig.D_netSync == 0) {
            Serial.println("WORK: Time to sync network time.");
            fsm.handleEvent(FSMEvent::TIME_TO_SYNC);
        }
        // Иначе проверяем, нужно ли отправлять данные
        else if (AppConfig.wakeCount % AppConfig.N_sendData == 0) {
            Serial.println("WORK: Time to send data.");
            fsm.handleEvent(FSMEvent::TIME_TO_SEND);
        }
        // Если ничего из вышеперечисленного не требуется, просто завершаем работу
        else {
            Serial.println("WORK: Measurement complete, going back to sleep.");
            fsm.handleEvent(FSMEvent::WORK_COMPLETE);
        }
    }

    void onUpdate() override {}
    void onExit() override {
        Serial.println("FSM: <- WORK"); // <<< НОВОЕ: Добавлен лог выхода
    }
    StateType getType() const override { return StateType::WORK; }
};

#endif