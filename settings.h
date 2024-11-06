#ifndef SETTINGS_H
#define SETTINGS_H

#include <Preferences.h>

class Settings {
public:
    Settings() {}

    void setMode(const String& mode) {
        preferences.begin("settings", false);
        preferences.putString("mode", mode);
        Serial.print("Modo salvo: ");
        Serial.println(mode);
        preferences.end();  // Garante o salvamento
    }

    String getMode() {
        preferences.begin("settings", true);
        String mode = preferences.getString("mode", "google");
        preferences.end();
        return mode;
    }

    void setSSID(const char* ssid) {
        preferences.begin("settings", false);
        preferences.putString("ssid", ssid);
        Serial.print("SSID salvo: ");
        Serial.println(ssid);
        preferences.end();  // Garante o salvamento
    }

    String getSSID() {
        preferences.begin("settings", true);
        String ssid = preferences.getString("ssid", "Wifi Free");
        preferences.end();
        return ssid;
    }

    void setPassword(const char* password) {
        preferences.begin("settings", false);
        preferences.putString("password", password);
        Serial.print("Senha salva: ");
        Serial.println(password);
        preferences.end();  // Garante o salvamento
    }

    String getPassword() {
        preferences.begin("settings", true);
        String password = preferences.getString("password", "");
        preferences.end();
        return password;
    }

private:
    Preferences preferences;
};

#endif  // SETTINGS_H
