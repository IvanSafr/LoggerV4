#pragma once

#include <Arduino.h>

// I2C для AHT10 и OLED
#define I2C_SDA          4
#define I2C_SCL          5

// SPI для Ra-02 (LoRa)
#define LORA_SCK         6
#define LORA_MISO        4
#define LORA_MOSI        5
#define LORA_SS          3
#define LORA_RST         7
#define LORA_DIO0        8
#define LORA_DIO1        0

// Периферия
#define BUTTON_PIN       10    // Подтяжка "вверх" (INPUT_PULLUP)
#define BATTERY_ADC_PIN  9    // Делитель напряжения
//#define SENSOR_POWER_PIN 20   // MOSFET для питания датчиков (опционально)

// Параметры дисплея
#define OLED_RESET       -1
#define OLED_ADDR        0x3C