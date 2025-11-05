/*
 * main.c
 *
 *  Created on:  Oct 29, 2025
 *      Author: smccollo
 */

#include <stdint.h>
#include <stdbool.h>

// SYSCTL and GPIO definitions.
#include "ppb.h"
#include "sysctl.h"
#include "gpio.h"

// Prototypes for peripherals.
#include "osc.h"
#include "led7seg.h"
#include "timer.h"
#include "lookup.h"
#include "keypad.h"

// Values for keypad state.
enum States {
  LANE_ODD, LANE_EVEN, LANE_ODD_CHANGE, LANE_EVEN_CHANGE
};



void main ( void ) {
  // CONFIGURATION SECTION

  // Initialize oscillator and all peripherals.
  initOsc();
  initTimer( 32 );
  init7SegLED();
  initKeypad();

  // Initialize variable which indicates which LED digit is visible.
  uint8_t display = 0;
  unit8_t timeLeft = 10;

  // Assume initially that no keys are pressed.
  enum States state = LANE_ODD;

  // MAIN WHILE LOOP
  while( true ) {
    // Delay a short amount of time.
    waitOnTimer();

    // Check the keypad.
    switch( state ) {
        case LANE_ODD:
        case LANE_EVEN:
        case LANE_ODD_CHANGE:
        case LANE_EVEN_CHANGE:
    }

    if( display == 0 )
            displayVal( 0, timeLeft % 10 );
    else
            displayVal( 1, (timeLeft % 100) / 10 );

    // Toggle "display" variable between 0 and 1.
    display ^= 1;
  }
}
