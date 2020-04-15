#include <Arduino.h>
#include <ArduinoBLE.h>

#include "watchdog_timer.h"
#include "ventilation_parameters.h"
#include "flow_sensor_sensiron.h"
#include "bluetooth_communication.h"
#include "demo_mode.h"

int flowRateSampleCount = 0;
int flowRateSum = 0;

void setup() {
  
  // Initialize Flow Sensor
  SENSIRIONFLOWSENSOR_setup();

  // Initialize serial port
  Serial.begin(9600);

  // Setup the Bluetooth connexion LED
  pinMode(LED_BUILTIN, OUTPUT);

  // Initialize bluetooth low energy (BLE)
  BLE_setup();

  // Initialize watchdogtimer
  WATCHDOG_setup();
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
      //int flow = int SENSIRION_readFlow();
      int flow = DEMO_simulateFlowSensor();

      //VENTILATION_newFlowValue(flow);
      
      // Average the flow over 6 samples
      flowRateSampleCount = flowRateSampleCount + 1;
      flowRateSum = flowRateSum + flow;

      // If the 6 samples have been acquired
      if (flowRateSampleCount >= 6) {

        // Verify if BLE is disconnected
        if (central.rssi() != 0) {
            // As long as we have connection, reset the watchdog timer.
              WATCHDOG_reset_1s(); 
        }

        // Divide the flow rate by the number of samples
        int dividedFlowRateSum = flowRateSum / 6;
        
        // Send the value over BLE
        BLE_sendExpirationFLowRate(dividedFlowRateSum);

        // Reset the sample count and the flow sum
        flowRateSampleCount = 0;
        flowRateSum = 0;
      }

      // Wait 5 ms (200 Hz refresh rate)
      delay(5);
    }
  }

  // Reset the watchdog timer
  WATCHDOG_reset_1s(); 

  // If there is a disconnexion, turn off the LED
  digitalWrite(LED_BUILTIN, LOW);
  
  delay(200);
}
