#include <BLEConfig.h>
#include <Arduino.h>

#include "esp_mac.h"

Preferences BLEConfig::preferences;

BLEConfig::BLEConfig()
{
  _active = false;
}

void BLEConfig::addItem(BLEConfigItem& item)
{
  item.addToService(_pService);
}

bool BLEConfig::begin(const char* appName, bool uniqueName)
{
  unsigned char mac[6];

  _appName = appName;
  _uniqueName = uniqueName;

  preferences.begin("BLEConfig", false);

  // Get our own MAC address and construct unique name
  esp_read_mac((uint8_t*) &mac, ESP_MAC_BT);
  _appNameWithMac = (char*)malloc(sizeof(_appName)+9);
  sprintf(_appNameWithMac, "%s-%02x%02x%02x", _appName, mac[3], mac[4], mac[5]);

  // Initialize
  BLEDevice::init(_uniqueName ? _appNameWithMac : _appName);
  _pServer = BLEDevice::createServer();
  _pServer->setCallbacks(new ServerCallbacks(this));
  _pService = _pServer->createService(SERVICE_UUID);

  _active = true;
  return true;
}

bool BLEConfig::begin(const char* appName, bool uniqueName, BLEConfigItemList items)
{
  if (!begin(appName, uniqueName))
  {
    return(false);
  }

  for (std::initializer_list<BLEConfigItem>::iterator item = items.begin(); item != items.end(); ++item){
// FIXME
//    item->addToService(_pService);
  }
  return true;
}

void BLEConfig::loop() {
  // Currently does... nothing.
}

void BLEConfig::end()
{
  preferences.end();
  BLEDevice::deinit();
  _active = false;
}

void BLEConfig::advertise()
{
  _pService->start();

  _pAdvertising = BLEDevice::getAdvertising();
  _pAdvertising->addServiceUUID(SERVICE_UUID);
  _pAdvertising->setScanResponse(true);
  // Below is copied from https://github.com/espressif/arduino-esp32/blob/master/libraries/BLE/examples/Server/Server.ino
  _pAdvertising->setMinPreferred(0x06);
  _pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
}


BLEConfig::ServerCallbacks::ServerCallbacks(BLEConfig* pBLEConfig)
{
  _pBLEConfig = pBLEConfig;
}

void BLEConfig::ServerCallbacks::onConnect(BLEServer *pServer)
{
  _pBLEConfig->_connected = true;
  _pBLEConfig->_connectionCount++;
  BLEDevice::startAdvertising();
};

void BLEConfig::ServerCallbacks::onDisconnect(BLEServer *pServer) {
  _pBLEConfig->_connected = false;
  pServer->startAdvertising();  // restart advertising
};