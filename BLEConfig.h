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
    void loop();
    void poll();
    void advertise();
    void end();

    inline const bool active() const { return(_active); };

    static Preferences preferences;

    static constexpr const char* UUID_TEMPLATE = "84fdef37-%04x-41e6-adbd-f87ced6f2e5b";

  private:
    BLEService _service;
    const char* _appName;
    const char* _appNameWithMac;
    bool _active;
};

#endif