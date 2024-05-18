#include "BLEConfigItem.h"
#include "BLEConfig.h"

unsigned int BLEConfigItem::item_count = 0;

BLEConfigItem::BLEConfigItem(const char* name, unsigned int defaultValue, unsigned int *variable)
{
  _name = name;
  _variable = variable;
  item_count++;

  *variable = BLEConfig::preferences.getUInt(name, defaultValue);
  BLEConfig::preferences.putUInt(name, *variable);
  BLEConfig::preferences.end();

  _characteristic = new BLECharacteristic("19B10001-ABCD-537E-4F6C-D104768A1214", BLERead | BLEWrite, sizeof(unsigned int));
  _characteristic->writeValue(*variable);
}