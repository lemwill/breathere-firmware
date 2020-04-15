#include "watchdog_timer.h"
#include <Arduino.h>

void WATCHDOG_setup(){

    #if defined(ARDUINO_ARDUINO_NANO33BLE)
    NRF_WDT->CONFIG = 0x01;   // Configure WDT to run when CPU is asleep
    NRF_WDT->CRV = 2 * 32768; // Set timeout for 2 seconds
    NRF_WDT->RREN = 0x01;     // Enable the RR[0] reload register
    NRF_WDT->TASKS_START = 1; // Start WDT
    #endif
}

void WATCHDOG_reset_1s(){

    #if defined(ARDUINO_ARDUINO_NANO33BLE)
    NRF_WDT->RR[0] = WDT_RR_RR_Reload;
    #endif
}