#include <BLEConfig.h>
#include <Arduino.h>

Preferences BLEConfig::preferences;

BLEConfig::BLEConfig(const char* appName)
  : _service("19B10000-E8F2-537E-4F6C-D104768A1214")
{
  _appName = appName;
}

void BLEConfig::addItem(const char* name, unsigned int defaultValue, unsigned int *variable)
{
  BLEConfigItem* item = new BLEConfigItem(name, defaultValue, variable);

  _service.addCharacteristic(*item->getCharacteristic());
  _items[name] = item;
}

void BLEConfig::begin()
{
  preferences.begin("BLEConfig", false);
  if (!BLE.begin()) {
    Serial.println("starting Bluetooth® Low Energy module failed!");
    while (1);
  }
  BLE.setLocalName(_appName);
  BLE.setAdvertisedService(_service);
}

void BLEConfig::advertise()
{
  BLE.addService(_service);
  BLE.advertise();
  Serial.println(("Bluetooth® device active, waiting for connections..."));
}

unsigned int BLEConfig::test1() {
  unsigned int v;
  _items["dmx_address"]->getCharacteristic()->readValue(v);

  return(v);
}