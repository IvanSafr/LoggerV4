#ifndef STATE_DATA_SEND_H
#define STATE_DATA_SEND_H

#include "FSM_Definitions.h"
#include "AppConfig.h"
#include <WiFi.h>

// Подключаем ваши библиотеки (предполагаемые имена методов)
#include "DataLogger.h" // Нужно будет добавить методы отправки в DataLogger
#include "LoRa.h"       // Ваш класс работы с LoRa
#include "FSMManager.h"

class State_DataSend : public BaseState {
private:
    uint32_t _startTime;
    bool _wifiAttempted = false;
    bool _sentSuccess = false;

    // Вспомогательный метод для попытки подключения WiFi (короткий таймаут)
    bool connectWiFi() {
        if (WiFi.status() == WL_CONNECTED) return true;
        
        Serial.println("DataSend: Connecting WiFi...");
        WiFi.begin(AppConfig.ssid.c_str(), AppConfig.pass.c_str());
        
        uint32_t start = millis();
        while (millis() - start < 10000) { // 10 секунд на подключение
            if (WiFi.status() == WL_CONNECTED) return true;
            delay(100);
        }
        return false;
    }

    // Логика отправки через WiFi (HTTP/MQTT)
    bool sendViaWiFi() {
        if (!connectWiFi()) return false;
        
        Serial.println("DataSend: Sending via WiFi (HTTP)...");
        // Здесь вызов вашего метода из DataLogger или HTTPClient
        // bool result = DataLogger.uploadToDB(); 
        
        // ЗАГЛУШКА для примера:
        bool result = true; // Предположим, успешно
        delay(500); // Симуляция отправки
        
        return result;
    }

    // Логика отправки через LoRa
    void sendViaLoRa() {
        Serial.println("DataSend: Sending via LoRa...");
        // Инициализация LoRa, если выключена
        // LoRa.begin(...);
        
        // Отправка пакета
        // String data = DataLogger.getLastPacket();
        // LoRa.sendPacket(data);
        
        delay(200); // Время на передачу
    }

public:
    void onEnter() override {
        Serial.println("FSM: -> DATA_SEND");
        _startTime = millis();
        _wifiAttempted = false;
        _sentSuccess = false;

        // Выбор стратегии в зависимости от режима
        switch (AppConfig.mode) {
            
            // === NORMAL: WiFi, если нет -> LoRa ===
            case MODE_NORMAL:
                if (sendViaWiFi()) {
                    Serial.println("DataSend: WiFi Success");
                    _sentSuccess = true;
                } else {
                    Serial.println("DataSend: WiFi Failed -> Fallback to LoRa");
                    sendViaLoRa();
                    // Статус все равно считаем "успехом" для выхода, 
                    // так как данные ушли хотя бы по LoRa
                    _sentSuccess = true; 
                }
                break;

            // === STANDARD: Только LoRa ===
            case MODE_STANDARD:
                WiFi.mode(WIFI_OFF); // Гарантируем, что WiFi выключен
                sendViaLoRa();
                _sentSuccess = true;
                break;

            // === ECO: Отправка по кнопке (обычно WiFi) ===
            case MODE_ECO:
                // Если мы попали сюда, значит пользователь инициировал отправку
                if (sendViaWiFi()) {
                     Serial.println("DataSend: ECO Upload Success");
                     _sentSuccess = true;
                } else {
                     Serial.println("DataSend: ECO Upload Failed");
                     _sentSuccess = false;
                }
                break;

            // === SCHEDULE: Пакетная отправка ===
            case MODE_SCHEDULE:
                // Логика массовой отправки
                // sendViaLoRaBurst();
                _sentSuccess = true;
                break;
        }
    }

    void onUpdate() override {
        // Поскольку в onEnter мы использовали блокирующие вызовы (для простоты),
        // здесь мы просто проверяем завершение и выходим.
        // В более сложной системе здесь был бы конечный автомат процесса отправки.
        
        extern class FSMManager fsm;
        
        if (_sentSuccess) {
            fsm.handleEvent(FSMEvent::SEND_SUCCESS);
        } else {
            fsm.handleEvent(FSMEvent::SEND_FAIL);
        }
    }

    void onExit() override {
        Serial.println("FSM: <- DATA_SEND");
        
        // Отключаем WiFi для экономии, если не нужен
        WiFi.disconnect(true);
        WiFi.mode(WIFI_OFF);
        
        // LoRa в режим сна
        LoRa.sleep();
    }

    StateType getType() const override { return StateType::DATA_SEND; }
};

#endif