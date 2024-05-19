#include <BLEConfig.h>
#include <Arduino.h>

Preferences BLEConfig::preferences;

BLEConfig::BLEConfig(const char* appName)
  : _service("19B10000-E8F2-537E-4F6C-D104768A1214")
{
  _appName = appName;
  _active = false;
}

void BLEConfig::addItem(BLEConfigItem& item)
{
  item.loadPreferences();
  _service.addCharacteristic(*item.getCharacteristic());
}

bool BLEConfig::begin()
{
  preferences.begin("BLEConfig", false);
  if (!BLE.begin()) {
    return false;
  }
  BLE.setLocalName(_appName);
  BLE.setAdvertisedService(_service);

  _active = true;
  return true;
}

bool BLEConfig::begin(BLEConfigItemList items)
{
  if (!begin())
  {
    return(false);
  }

  for (std::initializer_list<BLEConfigItem>::iterator item = items.begin(); item != items.end(); ++item){
    Serial.printf("Adding characteristic for item %s\n", item->getName());
    _service.addCharacteristic(*(item->getCharacteristic()));
  }
  return true;
}

void BLEConfig::end()
{
  preferences.end();
  BLE.end();
  _active = false;
}

void BLEConfig::advertise()
{
  BLE.addService(_service);
  BLE.advertise();
  Serial.println(("Bluetooth® device active, waiting for connections..."));
}