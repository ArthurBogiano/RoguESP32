#include "Arduino.h"



bool progmemToSPIFFS(const char* adr, int len, String path) {
    File f = SPIFFS.open(path, "w+");

    if (!f) {
        return false;
    }

    for (int i = 0; i < len; i++) {
        f.write(pgm_read_byte_near(adr + i));
    }
    f.close();
    return true;
}
