#ifndef BLEConfig_h
#define BLEConfig_h

#include <Preferences.h>
#include <string>

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#include "BLEConfigItem.h"

class BLEConfig
{
  class ServerCallbacks : public BLEServerCallbacks {
    public:
      ServerCallbacks(BLEConfig *pBLEConfig);
      void onConnect(BLEServer *pServer);
      void onDisconnect(BLEServer *pServer);
    protected:
      BLEConfig* _pBLEConfig;
  };

  public:
    BLEConfig();

    void addItem(BLEConfigItem& item);

    bool begin(const char* appName, bool uniqueName=true);
    void loop();
    void advertise();
    void end();

    inline const bool connected() const { return(_connected); };
    inline const bool active() const { return(_active); };
    inline const unsigned long getConnectionCount() const { return(_connectionCount); };
    inline const std::string getName() const { return(_uniqueName ? _appNameWithMac : _appName); };

    static Preferences preferences;

    static constexpr const char* SERVICE_UUID =  "84fdef37-0000-41e6-adbd-f87ced6f2e5b";
    static constexpr const char* UUID_TEMPLATE = "84fdef37-%04x-41e6-adbd-f87ced6f2e5b";

  private:
    BLEServer* _pServer;
    BLEService* _pService;
    BLEAdvertising* _pAdvertising;

    const char* _appName;
    bool _uniqueName;
    char* _appNameWithMac;

    bool _active = false;
    bool _connected = false;
    unsigned long _connectionCount = 0;
};

#endif