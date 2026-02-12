#include <Arduino.h>
#include "FSMManager.h"
#include "Button2.h"
#include "AppConfig.h"
#include "DataLogger.h"

Button2 btn(0); // Кнопка на GPIO 0 (ESP32-C3)

void setup() {
    Serial.begin(115200);
    AppConfig.begin(); 
    dataLogger.begin();
    // Настройка кнопки через библиотеку Button2
    btn.setClickHandler([](Button2& b){
        fsm.handleEvent(FSMEvent::BTN_SHORT);
    });
    
    btn.setLongClickHandler([](Button2& b){
        fsm.handleEvent(FSMEvent::BTN_LONG);
    });

    // Инициализация FSM
    fsm.init(StateType::WORK); 
}

void loop() {
    btn.loop();     // Проверка кнопки
    fsm.update();   // Тик текущего состояния (onUpdate)
}