#include <BLEConfig.h>

BLEConfig config("ConfigTest");
BLEUIntConfigItem configItem("item1", 1);

void setup() { 
  Serial.begin(115200);

  config.begin();
  config.addItem(configItem);
  config.advertise();
}

void loop() {
  Serial.printf("uptime %d, item1 value %d (%08x), BLE active %d, connected: %d\n", millis()/1000, configItem.value(), configItem.value(), config.active(), BLE.connected());
  delay(1000);
}