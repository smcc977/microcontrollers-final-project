/*
 * main.c
 *
 *  Created on:  Oct 29, 2025
 *      Author: smccollo
 */


//TODO
//Configure timer timing to be one second or half second
#include <stdint.h>
#include <stdbool.h>

// SYSCTL and GPIO definitions.
#include "ppb.h"
#include "sysctl.h"
#include "gpio.h"
#include "gptm.h"

// Prototypes for peripherals.
#include "osc.h"
#include "led7seg.h"
#include "timer.h"
#include "lookup.h"
#include "keypad.h"
#include "traffic-lights.h"
#include "interupt.h"

#define greenTimeLeft 10
#define changeTimeLeft 3
#define pressedTimeLeft 5
#define initTimeLeft 1

bool button13 = false;
bool button24 = false;
uint8_t timeLeft = 1;
bool used = false;

// Values for light state.
enum States {
  INIT, STATE_1, STATE_2, STATE_25, STATE_3, STATE_4, STATE_45
};
/*
void Button13Handler( void ) {
    button13 = true;
}

void Button24Handler( void ) {
    button24 = true;
}
*/
void Timer2AHandler( void ) {
    GPTM_TIMER2[GPTM_ICR] = GPTM_ICR_TATOCINT;
    if (used == false) {
        timeLeft -= 1;
    }
}


void main ( void ) {
  // CONFIGURATION SECTION

  // Initialize oscillator and all peripherals.
  initOsc();
  initDisplayTimer( 32 );
  init7SegLED();
  initTimerInterupt();
  initLights();
  initButtons();

  // Initialize variable which indicates which LED digit is visible.
  uint8_t display = 0;
  //uint8_t displayTime = 0;
  //bool nextGreen13 = true;

  // Assume initially that no keys are pressed.
  enum States state = INIT;

  // MAIN WHILE LOOP
  while( true ) {
    // Delay a short amount of time.
    waitOnDisplayTimer();
    used = true;
    uint32_t time = timeLeft;
    used = false;

    // Check the buttons
    button13 = checkButtons13();
    button24 = checkButtons24();

    switch( state ) {
        case INIT:
            //set all lights to flash red
            //should last one second
            if (time % 2 == 1) {
                turnLights13(0);
                turnLights24(0);
            }
            else {
                turnLights13(3);
                turnLights24(3);
            }

            if ( time < 1 ) {
                used = true;
                timeLeft = greenTimeLeft;
                used = false;
                time = timeLeft;
                state = STATE_1;
            }
            break;
        case STATE_1:
            turnLights13(2);
            turnLights24(0);
            if ( time < 1 ) {
                used = true;
                timeLeft = changeTimeLeft;
                used = false;
                time = timeLeft;
                state = STATE_2;
            }
            break;
        case STATE_2:
            turnLights13(1);
            turnLights24(0);
            if ( time < 1 ) {
                if (button24) {
                    used = true;
                    timeLeft = pressedTimeLeft;
                    used = false;
                    state = STATE_25;
                }
                else {
                    used = true;
                    timeLeft = greenTimeLeft;
                    used = false;
                    state = STATE_3;
                }
                time = timeLeft;
            }
            break;
        case STATE_25:
            turnLights13(0);
            turnLights24(2);
            if ( time < 1 ) {
                used = true;
                timeLeft = greenTimeLeft;
                used = false;
                time = timeLeft;
                state = STATE_3;
           }
           break;
        case STATE_3:
            turnLights13(0);
            turnLights24(2);
            if ( time < 1 ) {
                used = true;
                timeLeft = greenTimeLeft;
                used = false;
                time = timeLeft;
                state = STATE_4;
            }
            break;
        case STATE_4:
            turnLights13(0);
            turnLights24(1);
            if ( time < 1 ) {
                if (button13) {
                    used = true;
                    timeLeft = pressedTimeLeft;
                    used = false;
                    state = STATE_45;
                }
                else {
                    used = true;
                    timeLeft = greenTimeLeft;
                    used = false;
                    state = STATE_1;
                }
                time = timeLeft;
            }
            break;
        case STATE_45:
            turnLights13(2);
            turnLights24(0);
            if ( time < 1 ) {
                used = true;
                timeLeft = greenTimeLeft;
                used = false;
                time = timeLeft;
                state = STATE_1;
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
