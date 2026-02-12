#include <Arduino.h>
#include "FSMManager.h"
#include "Button2.h"
#include "AppConfig.h"
#include "DataLogger.h"

#include <Wire.h>
#include <LoRa.h>
#include "Pinout.h"
#include "Adafruit_AHTX0.h"
#include "Adafruit_SSD1306.h"
#include <esp_sleep.h> // <<< НОВОЕ: Подключаем для определения причины пробуждения

Button2 btn(BUTTON_PIN);
Adafruit_AHTX0 aht;
Adafruit_SSD1306 display(128, 32, &Wire, OLED_RESET);


// <<< НОВОЕ: Функция для вывода причины пробуждения (для отладки) >>>
void print_wakeup_reason(){
  esp_sleep_wakeup_cause_t wakeup_reason;
  wakeup_reason = esp_sleep_get_wakeup_cause();
  switch(wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break;
    default : Serial.printf("Wakeup was not caused by deep sleep: %d\n",wakeup_reason); break;
  }
}


void setup() {
    Serial.begin(115200);
    Serial.println("\n\n--- Logger V4 WAKE UP ---");

    // <<< НОВОЕ: Определяем причину пробуждения и выбираем начальное состояние >>>
    print_wakeup_reason();
    esp_sleep_wakeup_cause_t reason = esp_sleep_get_wakeup_cause();
    StateType initial_state;

    if (reason == ESP_SLEEP_WAKEUP_EXT0) {
        initial_state = StateType::SCREEN_ON; // Проснулись от кнопки -> включаем экран
    } else {
        initial_state = StateType::WORK; // Проснулись от таймера или первого включения -> измеряем
    }
    // <<< КОНЕЦ НОВОГО БЛОКА >>>


    Wire.begin(I2C_SDA, I2C_SCL);

    if(!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
        Serial.println(F("SSD1306 allocation failed"));
    } else {
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(0,0);
        display.println("Display OK");
        display.display();
    }

    if (!aht.begin()) {
        Serial.println("Could not find AHT. Check wiring!");
    } else {
         Serial.println("AHT10 OK");
    }

    LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);
    if (!LoRa.begin(AppConfig.loraFrequency)) {
        Serial.println("Starting LoRa failed!");
    } else {
        Serial.println("LoRa OK");
    }

    AppConfig.begin();
    dataLogger.begin();

    btn.setClickHandler([](Button2& b){
        fsm.handleEvent(FSMEvent::BTN_SHORT);
    });

    btn.setLongClickHandler([](Button2& b){
        fsm.handleEvent(FSMEvent::BTN_LONG);
    });

    // <<< ИЗМЕНЕНО: Инициализируем FSM с динамически выбранным состоянием >>>
    fsm.init(initial_state);
}

void loop() {
    btn.loop();
    fsm.update();
}