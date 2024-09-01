#include "BLEConfigItem.h"
#include "BLEConfig.h"

unsigned int BLEConfigItem::itemNumber = 0;

std::map<String, BLEConfigItem*> BLEConfigItem::_byName;
std::map<String, BLEConfigItem*> BLEConfigItem::_byUUID;

BLEConfigItem::BLEConfigItem(const char* name, unsigned int size)
{
  _name = name;
  itemNumber++;

  sprintf(_uuid, BLEConfig::UUID_TEMPLATE, itemNumber);

  _characteristic = new BLECharacteristic(_uuid, BLERead | BLEWrite, size);
  _characteristic->setEventHandler(BLEWritten, characteristicWritten);

  _byName[name] = this;
  _byUUID[_characteristic->uuid()] = this;

  // https://infineon.github.io/bless/ble_api_reference_manual/html/group__group__ble__common__api__macros__gatt__uuid__char__desc.html#ga824b567b10a51351ca9c4cb05e9570c5
  BLEDescriptor desc("2901", name);
  _characteristic->addDescriptor(desc);
}

void BLEConfigItem::characteristicWritten(BLEDevice central, BLECharacteristic characteristic) {
  char* value = new char[characteristic.valueSize()];
  memset(value, 0x00, sizeof(value));

  characteristic.readValue(value, sizeof(value));
  _byUUID[characteristic.uuid()]->writeHandler(value, sizeof(value));

  delete[] value;
}

BLEUIntConfigItem::BLEUIntConfigItem(const char* name, unsigned int defaultValue) : BLEConfigItem(name, sizeof(unsigned int))
{
  _defaultValue = defaultValue;
  _value = defaultValue;

  _characteristic->writeValue(uint32_t(_value));
}

void BLEUIntConfigItem::writeHandler(const char* value, unsigned int size)
{
  _value = *(unsigned int*) value;
  BLEConfig::preferences.putUInt(_name, _value);
}

void BLEUIntConfigItem::loadPreferences()
{
  _value = BLEConfig::preferences.getUInt(_name, _defaultValue);
  BLEConfig::preferences.putUInt(_name, _value);

  _characteristic->writeValue(uint32_t(_value));
}