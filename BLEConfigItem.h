#ifndef BLEConfigItem_h
#define BLEConfigItem_h

#include <ArduinoBLE.h>
#include <map>

class BLEConfigItem
{
  public:
    BLEConfigItem(const char* name, unsigned int size);
    inline BLECharacteristic* getCharacteristic() const { return _characteristic; };
    inline const char* getName() const { return _name; };
    static void characteristicWritten(BLEDevice central, BLECharacteristic characteristic);

    static unsigned int itemNumber;

  protected:
    BLECharacteristic* _characteristic;
    const char* _name;

    static std::map<String, BLEConfigItem*> _byName;
    static std::map<BLECharacteristic, BLEConfigItem*> _byCharacteristic;

    virtual void writeHandler(const char* value, unsigned int size) = 0;
    virtual void loadPreferences() = 0;

  friend class BLEConfig;
};

using BLEConfigItemList = std::initializer_list<BLEConfigItem>;

class BLEUIntConfigItem : public BLEConfigItem
{
  public:
    BLEUIntConfigItem(const char* name, unsigned int defaultValue);
    inline const unsigned int value() const { return(_value); };

  private:
    unsigned int _value;
    unsigned int _defaultValue;

  protected:
    void writeHandler(const char* value, unsigned int size);
    void loadPreferences();
};
#endif