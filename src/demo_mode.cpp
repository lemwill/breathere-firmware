#include "demo_mode.h"
#include <Arduino.h>

int DEMO_simulateFlowSensor()
{
    // Initialize variables
    int simulatedFlowLPM;

    // Get current time in millisecond
    int elapsedTime = millis();

    // Configure the breath to last 3200 ms
    elapsedTime = elapsedTime % 3200;
    

    // Expiration
    if (elapsedTime < 1300)
    {
        simulatedFlowLPM = 120 * 70 * pow(2.718281828459, -elapsedTime / 280.0) + rand() % 8;
    }
    // Inspiration
    else
    {
        simulatedFlowLPM = 0;
    }

    return simulatedFlowLPM;
}
