#include <BLEConfig.h>

BLEConfig config;
BLEUIntConfigItem configItem1("UInt", 1);
BLEStringConfigItem configItem2("String", "Default Value", 20);

void setup() { 
  Serial.begin(115200);
  delay(1000);
  Serial.printf("Starting...\n");

  config.begin("ConfigTest");
  config.addItem(configItem1);
  config.addItem(configItem2);
  config.advertise();
}

void loop() {
  Serial.printf("uptime %u, unit value %u (%08x), string value \"%s\", BLE active %d, connected: %d (%d times)\n", millis()/1000, configItem1.value(), configItem1.value(), configItem2.value().c_str(), config.active(), config.connected(), config.getConnectionCount());
  delay(1000);
}