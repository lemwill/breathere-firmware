#include "bluetooth_communication.h"
#include <ArduinoBLE.h>

BLEService ventilatorService("180d");
BLEUnsignedIntCharacteristic expirationFlowRate("2a37", BLERead | BLENotify);

void BLE_setup(){
  
  // Start BLE
  BLE.begin();
  
  // Set BLE local name
  BLE.setLocalName("Ventilator");

  // Advertise the BLE ventilator service
  BLE.setAdvertisedService(ventilatorService);

  // Add the BLE expirationFlowRate characteristic
  ventilatorService.addCharacteristic(expirationFlowRate);
  
  // Advertise the BLE ventilator service
  BLE.addService(ventilatorService);

  // Start advertisement
  BLE.advertise();

}


bool BLE_isConnected(){
    return false;
}


void BLE_sendExpirationFLowRate(int flowRate){
    expirationFlowRate.writeValue(flowRate);
}
