#include "FSM_Definitions.h"

class State_Sleep : public BaseState {
public:
    void onEnter() override { 
        Serial.println("FSM: -> SLEEP (Entering Light Sleep)"); 
        // Настройка пробуждения по таймеру T и по кнопке
        // esp_light_sleep_start();
    }
    void onUpdate() override {} 
    void onExit() override { Serial.println("FSM: <- SLEEP (Waking up)"); }
    StateType getType() const override { return StateType::SLEEP; }
};