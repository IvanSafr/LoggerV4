#ifndef DISPLAY_UI_H
#define DISPLAY_UI_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DataLogger.h" // Для получения количества записей

// <<< НОВОЕ: Полноценный класс для управления UI >>>
class DisplayUIClass {
private:
    Adafruit_SSD1306* _display = nullptr;
    uint8_t _currentScreen = 0;
    const uint8_t SCREEN_COUNT = 3; // 0=Main, 1=Status, 2=Network

    // Приватные методы для отрисовки каждого экрана
    void _drawMainScreen();
    void _drawStatusScreen();
    void _drawNetworkScreen();

public:
    DisplayUIClass();
    void begin(Adafruit_SSD1306* display); // Передаем указатель на объект дисплея
    void show();                          // Включает дисплей и рисует текущий экран
    void hide();                          // Очищает и выключает дисплей
    void nextScreen();                    // Переключает на следующий экран и перерисовывает его
    void drawCurrentScreen();             // Рисует текущий экран
};

extern DisplayUIClass DisplayUI;

#endif