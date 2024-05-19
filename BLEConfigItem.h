#ifndef BLEConfigItem_h
#define BLEConfigItem_h

#include <ArduinoBLE.h>
#include <map>

class BLEConfigItem
{
  public:
    BLEConfigItem(const char* name, void* variable, unsigned int size);
    inline BLECharacteristic* getCharacteristic() { return _characteristic; };
    inline const char* getName() { return _name; };

    static void characteristicWritten(BLEDevice central, BLECharacteristic characteristic);

    static unsigned int itemNumber;

  protected:
    BLECharacteristic* _characteristic;
    const char* _name;
    void* _variable;

    static std::map<String, BLEConfigItem*> _byName;
    static std::map<BLECharacteristic, BLEConfigItem*> _byCharacteristic;
};

class BLEUIntConfigItem : public BLEConfigItem
{
  public:
    BLEUIntConfigItem(const char* name, unsigned int* variable, unsigned int defaultValue);
};
#endif