#include "BLEConfigItem.h"
#include "BLEConfig.h"

unsigned int BLEConfigItem::itemNumber = 0;

std::map<String, BLEConfigItem*> BLEConfigItem::_byName;
std::map<BLECharacteristic, BLEConfigItem*> BLEConfigItem::_byCharacteristic;

BLEConfigItem::BLEConfigItem(const char* name, unsigned int size)
{
  _name = name;
  itemNumber++;

  sprintf(_uuid, BLEConfig::UUID_TEMPLATE, itemNumber);

  _characteristic = new BLECharacteristic(_uuid, BLERead | BLEWrite, size);
  _characteristic->setEventHandler(BLEWritten, characteristicWritten);

  _byName[name] = this;
  _byCharacteristic[*_characteristic] = this;

  BLEDescriptor desc("19B10000-E8D2-537E-4F6C-D104768A1214", name);
  _characteristic->addDescriptor(desc);
}

void BLEConfigItem::characteristicWritten(BLEDevice central, BLECharacteristic characteristic) {
  char* value = new char[characteristic.valueSize()];

  characteristic.readValue(value, sizeof(value));
  _byCharacteristic[characteristic]->writeHandler(value, sizeof(value));

  delete[] value;
}

BLEUIntConfigItem::BLEUIntConfigItem(const char* name, unsigned int defaultValue) : BLEConfigItem(name, sizeof(unsigned int))
{
  _defaultValue = defaultValue;
  _value = defaultValue;

  _characteristic->writeValue(_value);
}

void BLEUIntConfigItem::writeHandler(const char* value, unsigned int size)
{
  _value = (unsigned int) *value;
  BLEConfig::preferences.putUInt(_name, _value);
}

void BLEUIntConfigItem::loadPreferences()
{
  _value = BLEConfig::preferences.getUInt(_name, _defaultValue);
  BLEConfig::preferences.putUInt(_name, _value);

  _characteristic->writeValue(_value);
}