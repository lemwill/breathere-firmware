#include "flow_sensor_sensiron.h"
#include <Wire.h>

void SENSIRIONFLOWSENSOR_setup(){

      Wire.begin();

}


// Read the flow from the Sensirion SFM3300-D sensor
int SENSIRION_readFlow() {

  // Begin the transmission with the device address (64)
  Wire.beginTransmission(64);

  // Request a flow measurement
  Wire.write(byte(0x10));
  Wire.write(byte(0x00));

  // Read the flow measurement
  Wire.requestFrom(64, 3);
  int msb = Wire.read();
  int lsb = Wire.read();

  // Read the CRC
  Wire.read();

  // End the transmission
  Wire.endTransmission();

  // Convert the flow measurement from the MSB and LSB
  unsigned int flow = (msb << 8) | lsb;

  // Correct the value according to the calibrated sensor offset
  int corrected_flow = flow - 32768;

  return corrected_flow;
}
