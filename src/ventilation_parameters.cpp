#include "ventilation_parameters.h"
#include <Arduino.h>


unsigned long volume = 0;

const int inspiration = 0;
const int expiration = 1;
const int inspirationThreshold = 120;
const int expirationThreshold = 840;
unsigned long timestampPreviousCycle_ms;

int currentState = inspiration;

void detectInspiration(int flowRate)
{  
  if (currentState == inspiration) {
      if (flowRate > expirationThreshold) {
          currentState = expiration;
          Serial.print("volume");
          Serial.println(volume);
          volume = 0;
      }
  } else {
      if (flowRate < inspirationThreshold) {
          currentState = inspiration;
      }
  }
}

void VENTILATION_newFlowValue(int flowRate_LPM){

    const int second_per_minute = 60;

    detectInspiration(flowRate_LPM);

    // Integrate the volume
    unsigned long difference = millis() - timestampPreviousCycle_ms;
    volume = volume + flowRate_LPM * difference/second_per_minute; 
    timestampPreviousCycle_ms = millis();   
}

int VENTILATION_getVolume(){
    
    return volume/120;
;
}