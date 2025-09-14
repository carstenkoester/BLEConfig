#ifndef BLEConfigItem_h
#define BLEConfigItem_h

#include <map>
#include <string>

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2901.h>  // User Description descriptor
#include <BLE2904.h>  // Presentation Format descriptor

class BLEConfigItem
{
  public:
    BLEConfigItem(const char* name, unsigned int size);
    void addToService(BLEService* pService);

    inline const BLECharacteristic* getCharacteristic() const { return _pCharacteristic; };
    inline const char* getName() const { return _name; };

    static unsigned int itemCount;

  protected:
    BLECharacteristic* _pCharacteristic;
    BLE2901* _pBLE2901;

    const char* _name;
    unsigned int _size;

    static std::map<std::string, BLEConfigItem*> _byName;
    static std::map<std::string, BLEConfigItem*> _byUUID;

    virtual void writeHandler(String value) = 0;
    virtual void load() = 0;

    char _uuid[37];

    class CharacteristicCallbacks : public BLECharacteristicCallbacks {
      public:
        CharacteristicCallbacks(BLEConfigItem* pConfigItem);
        void onWrite(BLECharacteristic *pCharacteristic);

      protected:
        BLEConfigItem* _pConfigItem;
    };

  friend class BLEConfig;
};

/*
 * Unsigned Integer
 */
class BLEUIntConfigItem : public BLEConfigItem
{
  public:
    BLEUIntConfigItem(const char* name, unsigned int defaultValue);
    inline const unsigned int value() const { return(_value); };

  private:
    unsigned int _value;
    unsigned int _defaultValue;

  protected:
    void writeHandler(String value);
    void load();

    BLE2904* _pBLE2904;
};

/*
 * String
 */
class BLEStringConfigItem : public BLEConfigItem
{
  public:
    BLEStringConfigItem(const char* name, std::string defaultValue, unsigned int max_length);
    inline const std::string value() const { return(_value); };

  private:
    std::string _value;
    std::string _defaultValue;
    unsigned int _max_length;

  protected:
    void writeHandler(String value);
    void load();

    BLE2904* _pBLE2904;
};
#endif