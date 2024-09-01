#include <BLEConfig.h>
#include <Arduino.h>

Preferences BLEConfig::preferences;

BLEConfig::BLEConfig(const char* appName, bool uniqueName)
  : _service(SERVICE_UUID)
{
  _appName = appName;
  _uniqueName = uniqueName;
  _active = false;
}

void BLEConfig::addItem(BLEConfigItem& item)
{
  item.loadPreferences();
  _service.addCharacteristic(*item.getCharacteristic());
}

void BLEConfig::pollAndHandleConnected()
{
  BLE.poll();
  while (BLE.connected()) {
    BLE.poll(10);
  }
}

bool BLEConfig::begin()
{
  preferences.begin("BLEConfig", false);
  if (!BLE.begin()) {
    return false;
  }

  // Get our own MAC address and construct unique name
  String address = BLE.address();
  char addressArray[18];
  address.toCharArray(addressArray, sizeof(addressArray));
  _appNameWithMac = (char*)malloc(sizeof(_appName)+9);
  sprintf(_appNameWithMac, "%s-%c%c%c%c%c%c", _appName, addressArray[9], addressArray[10], addressArray[12], addressArray[13], addressArray[15], addressArray[16]);

  if (_uniqueName) {
    BLE.setLocalName(_appNameWithMac);
    BLE.setDeviceName(_appNameWithMac);
  } else {
    BLE.setLocalName(_appName);
    BLE.setDeviceName(_appName);    
  }
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
    _service.addCharacteristic(*(item->getCharacteristic()));
  }
  return true;
}

void BLEConfig::loop() {
  poll();
}

void BLEConfig::poll()
{
  if (_active) {
    BLE.poll();
  }
}

void BLEConfig::handleConnected()
{
  while (BLE.connected()) {
    BLE.poll(10);
  }
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
}