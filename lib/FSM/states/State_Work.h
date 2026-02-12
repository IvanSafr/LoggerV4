#ifndef STATE_WORK_H
#define STATE_WORK_H

#include "FSM_Definitions.h"
#include "FSMManager.h" // Required for fsm.handleEvent
#include "AppConfig.h"
#include "DataLogger.h"    // Required for DataEntry and dataLogger
#include "Adafruit_AHTX0.h"

// Define or include Pinout
#ifndef ADC_BAT_PIN
#define ADC_BAT_PIN 1 // Example pin, change to actual
#endif

extern Adafruit_AHTX0 aht; 
extern DataLoggerClass dataLogger; 

class State_Work : public BaseState {
public:
    void onEnter() override {
        Serial.println("FSM: -> WORK (Measuring)");

        sensors_event_t humidity, temp;
        // Ensure 'aht' is initialized in main setup
        aht.getEvent(&humidity, &temp);

        float v_bat = (analogRead(ADC_BAT_PIN) * 3.3 / 4095.0) * 2.0; 
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

        if (AppConfig.wakeCount % AppConfig.D_netSync == 0) {
            fsm.handleEvent(FSMEvent::SENSOR_DONE); 
        } else if (AppConfig.wakeCount % AppConfig.N_sendData == 0) {
            fsm.handleEvent(FSMEvent::SENSOR_DONE); 
        } else {
            fsm.handleEvent(FSMEvent::SENSOR_DONE); 
        }
    }

    void onUpdate() override {}
    void onExit() override {}
    StateType getType() const override { return StateType::WORK; }
};

#endif