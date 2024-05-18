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

    void addItem(const char* name, unsigned int defaultValue, unsigned int *variable);

    void begin();
    void advertise();
    void end();

    static Preferences preferences;

    unsigned int test1();

  private:
    std::map<String, BLEConfigItem*> _items;

    BLEService _service;
    const char* _appName;
};

#endif