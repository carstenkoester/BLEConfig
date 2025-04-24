#include "BLEConfigItem.h"
#include "BLEConfig.h"

unsigned int BLEConfigItem::itemCount = 0;

BLEConfigItem::BLEConfigItem(const char* name, unsigned int size)
{
  // Store name and UUID
  _name = name;
  _size = size;
  sprintf(_uuid, BLEConfig::UUID_TEMPLATE, ++itemCount);
}

void BLEConfigItem::CharacteristicCallbacks::onWrite(BLECharacteristic *pCharacteristic)
{
  _pConfigItem->writeHandler(pCharacteristic->getValue());
}

BLEConfigItem::CharacteristicCallbacks::CharacteristicCallbacks(BLEConfigItem* pConfigItem)
{
  _pConfigItem = pConfigItem;
}


void BLEConfigItem::addToService(BLEService* pService)
{
  _pCharacteristic = pService->createCharacteristic(_uuid, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
  _pCharacteristic->setCallbacks(new CharacteristicCallbacks(this));

  _pBLE2901 = new BLE2901;
  _pBLE2901->setDescription(_name);
  _pCharacteristic->addDescriptor(_pBLE2901);

  Serial.printf("Added BLE configuration item: UUID %s, Name %s\n", _uuid, _name);

  // Load from preferences and set initial value
  load();
}

/*
 * UInt
 */
BLEUIntConfigItem::BLEUIntConfigItem(const char* name, unsigned int defaultValue) : BLEConfigItem(name, sizeof(unsigned int))
{
  _defaultValue = defaultValue;
  _value = defaultValue;
}

void BLEUIntConfigItem::writeHandler(String value)
{
  unsigned int length = value.length();
  if (length == 1) {
    _value = value[0];
  } else if (length == 2) {
    _value = (value[0]) + (value[1] << 8);
  } else if (length == 3) {
    _value = (value[0]) + (value[1] << 8) + (value[2] << 16);
  } else {
    _value = (value[0]) + (value[1] << 8) + (value[2] << 16) + (value[3] << 24);
  }

  BLEConfig::preferences.putUInt(_name, _value);
  Serial.printf("Configuration item \"%s\" set to  %d\n", _name, _value);
}

void BLEUIntConfigItem::load()
{
  _value = BLEConfig::preferences.getUInt(_name, _defaultValue);
  BLEConfig::preferences.putUInt(_name, _value);

  _pBLE2904 = new BLE2904;
  _pBLE2904->setFormat(BLE2904::FORMAT_UINT32);
  _pCharacteristic->addDescriptor(_pBLE2904);

  _pCharacteristic->setValue((uint8_t *)&_value, sizeof(_value));
}


/*
 * String
 */
BLEStringConfigItem::BLEStringConfigItem(const char* name, std::string defaultValue, unsigned int max_length) : BLEConfigItem(name, max_length + 1)
{
  _defaultValue = defaultValue;
  _value = defaultValue;
  _max_length = max_length;
}

void BLEStringConfigItem::writeHandler(String value)
{
  _value = value.c_str();
  BLEConfig::preferences.putString(_name, _value.c_str());
  Serial.printf("Configuration item \"%s\" set to \"%s\" (len %d/%d)\n", _name, _value.c_str(), _value.length(), _size);
}

void BLEStringConfigItem::load()
{
  char value[512];
  size_t read;

  read = BLEConfig::preferences.getString(_name, (char *) &value, (size_t) sizeof(value));
  if (read != 0) {
    _value = std::string(value, min(read, _max_length-1));
  } else {
    _value = _defaultValue;
  }

  _pBLE2904 = new BLE2904;
  _pBLE2904->setFormat(BLE2904::FORMAT_UTF8);
  _pCharacteristic->addDescriptor(_pBLE2904);

  _pCharacteristic->setValue(_value.c_str());
}