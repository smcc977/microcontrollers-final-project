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

#define greenTimeLeft 20
#define changeTimeLeft 20
#define initTimeLeft 1

// Values for keypad state.
enum States {
  INIT, LANE_ODD, LANE_EVEN, LANE_ODD_CHANGE, LANE_EVEN_CHANGE
};



void main ( void ) {
  // CONFIGURATION SECTION

  // Initialize oscillator and all peripherals.
  initOsc();
  initDisplayTimer( 32 );
  init7SegLED();
  initKeypad();

  // Initialize variable which indicates which LED digit is visible.
  uint8_t display = 0;
  uint8_t timeLeft = 1;
  uint8_t displayTime = 0;
  bool button13 = false;
  bool button24 = false;
  bool nextGreen13 = true;

  // Assume initially that no keys are pressed.
  enum States state = INIT;

  // MAIN WHILE LOOP
  while( true ) {
    // Delay a short amount of time.
    waitOnDisplayTimer();

    // Check the keypad.
    switch( state ) {
        case INIT:
            //set all lights to red
            //should last one second
            if ( timeLeft < 1 ) {
                timeLeft = greenTimeLeft;
                //switch depending on nextGreen1.3
                if ( nextGreen13 ) {
                    state = LANE_ODD;
                }
                else {
                    state = LANE_EVEN;
                }
            }
            break;
        case LANE_ODD:
            //1+3 green, 2+4 red
            //time left = 20 seconds if button not pressed
            //if button pressed should be set to 7 seconds, reset flag
            //switch to lane_odd_change when time out
            if ( timeLeft < 1 ) {
                timeLeft = changeTimeLeft;
                state = LANE_ODD_CHANGE;
            }
            break;
        case LANE_EVEN:
            //2+4 green, 1+3 red
            //time left = 20 seconds if button not pressed
            //if button pressed should be set to 7 seconds, reset flag
            //switch to lane_even_change when time out
            if ( timeLeft < 1 ) {
                 timeLeft = changeTimeLeft;
                 state = LANE_EVEN_CHANGE;
            }
            break;
        case LANE_ODD_CHANGE:
            //1+3 green, 2+4 red
            //time left = 5 seconds
            //blink the display value or just hold at 0
            //when timeleft == 3: 1+3 yellow
            //switch to init when time out
            if ( timeLeft < 1 ) {
                 timeLeft = initTimeLeft;
                 state = INIT;
            }
            break;
        case LANE_EVEN_CHANGE:
            //1+3 green, 2+4 red
            //time left = 5 seconds
            //blink the display value or just hold at 0
            //when timeleft == 3: 1+3 yellow
            //switch to init when time out
            if ( timeLeft < 1 ) {
                  timeLeft = initTimeLeft;
                  state = INIT;
            }
            break;
    }

    if( display == 0 ){
            displayVal( 0, timeLeft % 10 );
    }
    else {
            displayVal( 1, (timeLeft % 100) / 10 );
    }

    // Toggle "display" variable between 0 and 1.
    display ^= 1;
  }
}
