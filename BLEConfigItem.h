#ifndef BLEConfigItem_h
#define BLEConfigItem_h

#include <ArduinoBLE.h>

class BLEConfigItem
{
  public:
    BLEConfigItem(const char* name, unsigned int defaultValue, unsigned int* variable);
    inline BLECharacteristic* getCharacteristic() { return _characteristic; };

    static unsigned int item_count;

    const char* _name;

  private:
    BLECharacteristic* _characteristic;


    void* _variable;
};

#endif