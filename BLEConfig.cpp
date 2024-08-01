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

byte data[7] = { 0xff, 0xff, 0x01, 0x02, 0x03, 0x04, 0x05};


bool BLEConfig::begin()
{
  preferences.begin("BLEConfig", false);
  if (!BLE.begin()) {
    return false;
  }
  BLE.setLocalName(_appName);
  BLE.setDeviceName("Test Device Name");
  BLE.setAdvertisedService(_service);

  String address = BLE.address();
  char address2[18];
  address.toCharArray(address2, sizeof(address2));
  Serial.printf("Address: %x\n", address);
  Serial.printf("Address: %s\n", address);
  Serial.print("Local address is: ");
  Serial.println(address);
  Serial.printf("In char array format: %s\n", address2);
  Serial.printf("In char array format: %c%c%c%c%c%c\n", address2[9], address2[10], address2[12], address2[13], address2[15], address2[16]);

  BLE.setManufacturerData((unsigned char*)address2, sizeof(address2));
  BLE.setManufacturerData(data, 7);

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
  Serial.println("Bluetooth® device active, waiting for connections...");
}