#include <ArduinoBLE.h>
#include <Wire.h>

BLEService ventilatorService("180d");
BLEUnsignedIntCharacteristic expirationFlowRate("2a37", BLERead | BLENotify);

int flowRateSampleCount = 0;
int flowRateSum = 0;

void setup() {
  
  // Initialize I2C peripheral
  Wire.begin();

  // Initialize serial port
  Serial.begin(9600);

  // Setup the Bluetooth connexion LED
  pinMode(LED_BUILTIN, OUTPUT);

  // Initialize bluetooth low energy (BLE)
  if (!BLE.begin())
  {
    Serial.println("starting BLE failed!");
    while (1);
  }

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


// Read the flow from the Sensirion SFM3300-D sensor
int readFlow() {

  // Begin the transmission with the device address (64)
  Wire.beginTransmission(64);

  // Request a flow measurement
  Wire.write(byte(0x10));
  Wire.write(byte(0x00));

  // Read the flow measurement
  Wire.requestFrom(64, 3);
  int msb = Wire.read();
  int lsb = Wire.read();
  int crc = Wire.read();
  Wire.endTransmission();

  // Convert the flow measurement from the MSB and LSB
  unsigned int flow = (msb << 8) | lsb;

  // Correct the value according to the calibrated sensor offset
  int corrected_flow = flow - 32768;

  return corrected_flow;
}

void loop()
{
  // Verify connexion
  BLEDevice central = BLE.central();

  // if there is a connexion
  if (central)
  {
    // Indicate the connexion success over the serial port
    Serial.print("Connected to central: ");
    Serial.println(central.address());
    
    // Light up the connexion LED
    digitalWrite(LED_BUILTIN, HIGH);

    // As long as the central is connected
    while (central.connected()) {

      // Read the flow from the SFM3300-D sensor
      int flow = readFlow();

      // Average the flow over 6 samples
      flowRateSampleCount = flowRateSampleCount + 1;
      flowRateSum = flowRateSum + flow;

      // If the 6 samples have been acquired
      if (flowRateSampleCount >= 6) {

        // Send the value over BLE
        expirationFlowRate.writeValue(flowRateSum / 6);

        // Reset the sample count and the flow sum
        flowRateSampleCount = 0;
        flowRateSum = 0;
      }

      // Wait 5 ms (200 Hz refresh rate)
      delay(5);
    }
  }

  // If there is a disconnexion, turn off the LED
  digitalWrite(LED_BUILTIN, LOW);
  
  delay(200);
}
