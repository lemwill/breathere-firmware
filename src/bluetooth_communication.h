#ifndef BLUETOOTH_COMMUNICATION_H
#define BLUETOOTH_COMMUNICATION_H

void BLE_setup();
bool BLE_isConnected();
void BLE_sendExpirationFLowRate(int flowRate);

#endif