#ifndef BLEConfig_h
#define BLEConfig_h

#include "Arduino.h"
#include <Preferences.h>
#include <ArduinoBLE.h>
#include <map>

#include "BLEConfigItem.h"

class BLEConfig
{
  public:
    BLEConfig(const char* appName);

    void addItem(BLEConfigItem& item);

    bool begin();
    bool begin(BLEConfigItemList items);
    void advertise();
    void end();

    static Preferences preferences;

  private:
    BLEService _service;
    const char* _appName;
};

#endif