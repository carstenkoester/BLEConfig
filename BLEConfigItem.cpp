#include "BLEConfigItem.h"
#include "BLEConfig.h"

unsigned int BLEConfigItem::itemNumber = 0;

std::map<String, BLEConfigItem*> BLEConfigItem::_byName;
std::map<BLECharacteristic, BLEConfigItem*> BLEConfigItem::_byCharacteristic;

BLEConfigItem::BLEConfigItem(const char* name, void* variable, unsigned int size)
{
  _name = name;
  _variable = variable;
  itemNumber++;

  _characteristic = new BLECharacteristic("19B10001-ABCD-537E-4F6C-D104768A1214", BLERead | BLEWrite, size);
  _characteristic->setEventHandler(BLEWritten, characteristicWritten);

  _byName[name] = this;
  _byCharacteristic[*_characteristic] = this;
}


void BLEConfigItem::characteristicWritten(BLEDevice central, BLECharacteristic characteristic) {
  // central wrote new value to characteristic, update LED
  unsigned int v;
  characteristic.readValue(v);

  Serial.printf("Characteristic event, written value %d\n", v);
  Serial.printf("ch %lx\n", characteristic);
  Serial.printf("&ch %lx\n", &characteristic);

  void* var = _byCharacteristic[characteristic]->_variable;

  Serial.printf("Going to print pointer...\n");
  Serial.printf("Variable pointer: %llx\n", var);
  Serial.printf("Current variable value: %d\n", *(unsigned int *)var);
  Serial.printf("Name: %s\n", _byCharacteristic[characteristic]->_name);
  *(unsigned int *)var = v;

  char* value[characteristic.valueSize()];
  
}

BLEUIntConfigItem::BLEUIntConfigItem(const char* name, unsigned int* variable, unsigned int defaultValue) : BLEConfigItem(name, variable, sizeof(unsigned int))
{
  *variable = BLEConfig::preferences.getUInt(name, defaultValue);
  BLEConfig::preferences.putUInt(name, *variable);
  BLEConfig::preferences.end();

  _characteristic->writeValue(*variable);
}