#ifndef APP_CONFIG_H
#define APP_CONFIG_H

#include <Arduino.h>
#include <Preferences.h>

// Operating modes
enum WorkMode {
    MODE_ECO = 0,      // WiFi only via button, data collection only
    MODE_NORMAL = 1,   // Immediate transmission (WiFi -> LoRa backup)
    MODE_STANDARD = 2, // LoRa only (periodic)
    MODE_SCHEDULE = 3  // Collection -> Bulk transmission via schedule
};

class AppConfigClass {
private:
    Preferences prefs;
    const char* NAMESPACE = "sensor_conf";

public:
    // === Configuration Variables ===

    // Device Identification
    String deviceId;          
    uint8_t mode;             // Operating mode (previously workMode)
    uint32_t wakeCount;       // Counter for wake-up cycles (Added for State_Work logic)

    // Timing and Intervals
    uint32_t T_sleepSeconds;  // T: Sleep duration in seconds
    uint32_t D_netSync;       // D: Sync time every D wake-ups
    uint32_t N_sendData;      // N: Send data every N wake-ups
    uint32_t A_screenTime;    // A: Display active time in seconds

    // Schedule Mode Settings
    uint32_t X_schedInterval; // X: Bulk upload interval (seconds)
    uint32_t Y_burstCount;    // Y: Number of transmissions per hour
    uint32_t Z_burstDuration; // Z: Duration of communication session

    // Network Settings
    String ssid;              // WiFi SSID (Renamed to match State usage)
    String pass;              // WiFi Password (Renamed to match State usage)

    // LoRa Settings
    long loraFrequency; 

    // === Methods ===

    void begin();             // Initialize and load from NVS
    void load();              // Load variables from NVS
    void save();              // Save current variables to NVS
    void resetToFactory();    // Reset to default values

    // Diagnostics
    void printConfig();       

    // Utility
    void setWifi(String newSsid, String newPass);
};

extern AppConfigClass AppConfig;

#endif