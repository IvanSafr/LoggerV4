#include "DataLogger.h"

DataLoggerClass dataLogger;

bool DataLoggerClass::begin() {
    // Монтируем файловую систему. Если не форматирована — форматируем.
    if (!LittleFS.begin(true)) {
        Serial.println("DataLogger: LittleFS Mount Failed");
        return false;
    }
    return true;
}

bool DataLoggerClass::write(const DataEntry& entry) {
    // Открываем файл для ДОЗАПИСИ ("a" - append)
    File file = LittleFS.open(_filename, "a");
    if (!file) {
        Serial.println("DataLogger: Failed to open file for appending");
        return false;
    }

    // Пишем сырые байты структуры
    size_t written = file.write((uint8_t*)&entry, sizeof(DataEntry));
    file.close();

    if (written != sizeof(DataEntry)) {
        Serial.println("DataLogger: Write failed/incomplete");
        return false;
    }
    
    Serial.printf("DataLogger: Saved entry. TS: %u, Temp: %.2f\n", entry.timestamp, entry.temperature);
    return true;
}

size_t DataLoggerClass::getCount() {
    if (!LittleFS.exists(_filename)) return 0;

    File file = LittleFS.open(_filename, "r");
    if (!file) return 0;

    size_t size = file.size();
    file.close();

    // Количество записей = размер файла / размер одной структуры
    return size / sizeof(DataEntry);
}

bool DataLoggerClass::read(size_t index, DataEntry& outEntry) {
    File file = LittleFS.open(_filename, "r");
    if (!file) return false;

    // Смещаем курсор на нужную позицию: index * 13 байт
    size_t position = index * sizeof(DataEntry);
    
    if (position >= file.size()) {
        file.close();
        return false; // Индекс за пределами файла
    }

    file.seek(position);
    size_t readBytes = file.read((uint8_t*)&outEntry, sizeof(DataEntry));
    file.close();

    return (readBytes == sizeof(DataEntry));
}

size_t DataLoggerClass::getFileSize() {
    if (!LittleFS.exists(_filename)) return 0;
    File file = LittleFS.open(_filename, "r");
    size_t size = file.size();
    file.close();
    return size;
}

void DataLoggerClass::clear() {
    if (LittleFS.exists(_filename)) {
        LittleFS.remove(_filename);
        Serial.println("DataLogger: Log file cleared");
    }
}