#include "AppConfig.h"

AppConfigClass AppConfig;

void AppConfigClass::begin() {
    prefs.begin(NAMESPACE, false); // false = read/write

    // Check for initialization key. If missing, apply factory defaults.
    if (!prefs.isKey("init_done")) {
        Serial.println("Config: First run detected, applying defaults...");
        resetToFactory();
        prefs.putBool("init_done", true);
    } else {
        load();
    }
}

void AppConfigClass::resetToFactory() {
    // Default values assignment
    deviceId = "ESP32_SENSOR_001";
    mode = MODE_NORMAL;
    wakeCount = 0;

    T_sleepSeconds = 600;  // 10 minutes
    D_netSync = 6;         // Every hour (if T=600s)
    N_sendData = 1;        // Every wake-up
    A_screenTime = 10;     // 10 seconds

    X_schedInterval = 86400; // 24 hours
    Y_burstCount = 5;
    Z_burstDuration = 600;

    ssid = "Your_SSID";
    pass = "Your_PASS";
    loraFrequency = 433E6;

    save(); // Persist defaults immediately
}

void AppConfigClass::load() {
    // Load variables with fallback defaults
    deviceId = prefs.getString("dev_id", "ESP_DEF");
    mode = prefs.getUChar("mode", MODE_NORMAL);
    wakeCount = prefs.getUInt("wake_cnt", 0);

    T_sleepSeconds = prefs.getUInt("T_sleep", 600);
    D_netSync = prefs.getUInt("D_sync", 6);
    N_sendData = prefs.getUInt("N_send", 1);
    A_screenTime = prefs.getUInt("A_scr", 10);

    X_schedInterval = prefs.getUInt("X_sched", 86400);
    Y_burstCount = prefs.getUInt("Y_burst", 5);
    Z_burstDuration = prefs.getUInt("Z_dur", 600);

    ssid = prefs.getString("ssid", "");
    pass = prefs.getString("pass", "");
    loraFrequency = prefs.getLong("lora_freq", 433E6);

    Serial.println("Config: Loaded from NVS");
}

void AppConfigClass::save() {
    prefs.putString("dev_id", deviceId);
    prefs.putUChar("mode", mode);
    prefs.putUInt("wake_cnt", wakeCount);

    prefs.putUInt("T_sleep", T_sleepSeconds);
    prefs.putUInt("D_sync", D_netSync);
    prefs.putUInt("N_send", N_sendData);
    prefs.putUInt("A_scr", A_screenTime);

    prefs.putUInt("X_sched", X_schedInterval);
    prefs.putUInt("Y_burst", Y_burstCount);
    prefs.putUInt("Z_dur", Z_burstDuration);

    prefs.putString("ssid", ssid);
    prefs.putString("pass", pass);
    prefs.putLong("lora_freq", loraFrequency);

    Serial.println("Config: Saved to NVS");
}

void AppConfigClass::setWifi(String newSsid, String newPass) {
    ssid = newSsid;
    pass = newPass;
    save();
}

void AppConfigClass::printConfig() {
    Serial.println("=== Configuration ===");
    Serial.printf("Device ID: %s\n", deviceId.c_str());
    Serial.printf("Mode: %d (0=ECO, 1=NORM, 2=STD, 3=SCH)\n", mode);
    Serial.printf("Wake Count: %u\n", wakeCount);
    Serial.printf("T (Sleep): %u s\n", T_sleepSeconds);
    Serial.printf("D (Sync every): %u wakes\n", D_netSync);
    Serial.printf("N (Send every): %u wakes\n", N_sendData);
    Serial.printf("A (Screen): %u s\n", A_screenTime);
    Serial.printf("WiFi SSID: %s\n", ssid.c_str());
    Serial.println("=====================");
}