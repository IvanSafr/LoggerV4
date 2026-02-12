#ifndef DATA_LOGGER_H
#define DATA_LOGGER_H

#include <Arduino.h>
#include <FS.h>
#include <LittleFS.h> // Рекомендуется вместо SPIFFS для ESP32
#include <time.h>

// Имя файла для логирования
#define LOG_FILENAME "/log.bin"

// Структура данных. 
// __attribute__((packed)) гарантирует, что компилятор не добавит пустых байтов для выравнивания.
// Размер записи будет жестко фиксирован: 4+4+4+1 = 13 байт.
struct __attribute__((packed)) DataEntry {
    uint32_t timestamp; // Unix время
    float temperature;
    float humidity;
    uint8_t battery;    // Заряд в %
};

class DataLoggerClass {
public:
    // Инициализация файловой системы
    bool begin();

    // Запись одной структуры в конец файла
    bool write(const DataEntry& entry);

    // Чтение записи по индексу (0 - самая старая)
    bool read(size_t index, DataEntry& outEntry);

    // Получить количество записей в файле
    size_t getCount();

    // Удалить файл лога (вызывается после успешной отправки)
    void clear();

    // Получить размер файла в байтах
    size_t getFileSize();

private:
    const char* _filename = LOG_FILENAME;
};

// Объявляем глобальный экземпляр, чтобы он был виден в State_Work и других файлах
extern DataLoggerClass dataLogger;

#endif