#include "DisplayUI.h"
#include "AppConfig.h"
#include "DataLogger.h"
#include "Adafruit_AHTX0.h"
#include <WiFi.h>

// Объявляем глобальный экземпляр
DisplayUIClass DisplayUI;
extern Adafruit_AHTX0 aht; // Получаем доступ к датчику для живых данных

DisplayUIClass::DisplayUIClass() {}

void DisplayUIClass::begin(Adafruit_SSD1306* display) {
    _display = display;
}

void DisplayUIClass::show() {
    if (!_display) return;
    drawCurrentScreen();
}

void DisplayUIClass::hide() {
    if (!_display) return;
    _display->clearDisplay();
    _display->display();
}

void DisplayUIClass::nextScreen() {
    _currentScreen = (_currentScreen + 1) % SCREEN_COUNT;
    drawCurrentScreen();
}

void DisplayUIClass::drawCurrentScreen() {
    if (!_display) return;
    _display->clearDisplay();
    _display->setTextSize(1);
    _display->setTextColor(SSD1306_WHITE);
    _display->setCursor(0, 0);

    switch (_currentScreen) {
        case 0: _drawMainScreen(); break;
        case 1: _drawStatusScreen(); break;
        case 2: _drawNetworkScreen(); break;
    }

    _display->display();
}


// --- Реализация конкретных экранов ---

void DisplayUIClass::_drawMainScreen() {
    // Получаем свежие данные с датчиков
    sensors_event_t humidity, temp;
    aht.getEvent(&humidity, &temp);

    char buf[32];

    _display->println("--- MAIN DATA ---");

    snprintf(buf, sizeof(buf), "Temp: %.2f C", temp.temperature);
    _display->println(buf);

    snprintf(buf, sizeof(buf), "Humi: %.2f %%", humidity.relative_humidity);
    _display->println(buf);

    // Батарею возьмем из последней записи в логе для простоты
    DataEntry lastEntry;
    if (dataLogger.getCount() > 0 && dataLogger.read(dataLogger.getCount() - 1, lastEntry)) {
        snprintf(buf, sizeof(buf), "Batt: %d %%", lastEntry.battery);
        _display->println(buf);
    } else {
        _display->println("Batt: N/A");
    }
}

void DisplayUIClass::_drawStatusScreen() {
    char buf[32];
    const char* modes[] = {"ECO", "NORMAL", "STD", "SCHED"};
    const char* modeName = (AppConfig.mode < 4) ? modes[AppConfig.mode] : "UNK";

    _display->println("---- STATUS ----");

    snprintf(buf, sizeof(buf), "Mode: %s", modeName);
    _display->println(buf);

    snprintf(buf, sizeof(buf), "Wakes: %u", AppConfig.wakeCount);
    _display->println(buf);

    snprintf(buf, sizeof(buf), "Logs: %u", dataLogger.getCount());
    _display->println(buf);
}

void DisplayUIClass::_drawNetworkScreen() {
    _display->println("---- NETWORK ----");
    if (WiFi.status() == WL_CONNECTED) {
        _display->printf("SSID: %s\n", AppConfig.ssid.c_str());
        _display->printf("IP: %s\n", WiFi.localIP().toString().c_str());
    } else {
        _display->println("WiFi: Disconnected");
        _display->println("SSID: " + AppConfig.ssid);
    }
     _display->printf("LoRa: %d Mhz\n", (int)(AppConfig.loraFrequency / 1E6));
}