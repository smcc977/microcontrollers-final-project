/*
 * lab01.c
 *
 *  Created on: July 15, 2019
 *  Modified on: Aug 16, 2020
 *      Author: khughes
 *
 *  Register -> Pin -> Display Pin
 *  PD0 -> 50 -> 12
 *  PD1 -> 48 -> 10
 *  PD2 -> 52 -> 9
 *  PD3 -> 46 -> 7
 *  PD4 -> 40 -> 1
 *  PD5 -> 38 -> 18
 *  PD6 -> 44 -> 6
 *  PD7 -> 42 -> 13
 *  PE0 -> 14 -> 16 //May need to be swapped
 *  PE1 -> 16 -> 11 //Connected through MOSFET with 3.3V
 */

#include <stdint.h>
#include <stdbool.h>

// SYSCTL and GPIO definitions.
#include "sysctl.h"
#include "gpio.h"

// Prototypes for peripherals.
#include "osc.h"
#include "led7seg.h"

// Procedure which increments index into display data.  When the index
// reaches the end of the table, it wraps back to the start.
void incrementIndex( uint8_t *index ) {
  *index += 1;
  if( *index == getDisplayDataSize( ) ) *index = 0;
}

// Procedure which decrements index into display data.  When the index
// reaches the beginning of the table, it wraps to the end.
void decrementIndex( uint8_t *index ) {
  if( *index == 0 ) *index = getDisplayDataSize( );
  *index -= 1;
}

// State names for finite state machine.
enum {
  NONE_PRESSED, LEFT_PRESSED, LEFT_WAITING, RIGHT_PRESSED, RIGHT_WAITING 
};

void main( void )
{
  // CONFIGURATION SECTION

  // Initialize clock and display
  initOsc();
  init7SegLED();

  // Enable PORTJ gate clocks.  The second write is only necessary
  // as a delay so that the clock is stable before writing to the GPIO
  // registers.  If we don't delay, we may get a fault.
  SYSCTL[SYSCTL_RCGCGPIO] |= SYSCTL_RCGCGPIO_PORTH;
  SYSCTL[SYSCTL_RCGCGPIO] |= SYSCTL_RCGCGPIO_PORTH;

  // Turn on pull-ups for each input pin
  GPIO_PORTH[GPIO_PUR] |= GPIO_PIN_1 | GPIO_PIN_0;

  // Enable pins.
  GPIO_PORTH[GPIO_DEN] |= GPIO_PIN_1 | GPIO_PIN_0;

  // Initialize display indexes to show first and second symbols.
  uint8_t index[2] = {0,1};
  // Initialize variable which indicates which LED digit is visible.
  uint8_t display = 0;

  // Initialize state variable.
  uint8_t state = NONE_PRESSED;

  // MAIN WHILE LOOP
  while( true ) {
    // Based on the "display" variable, turn on the left or right LED character.
    displayVal( display, index[display] );

    // Delay a short amount of time.  REPLACE THIS IN LAB 2 WITH TIMER.
    // waitOnTimer();
    uint16_t i = 0xffff;
    for( i; i; i-- );

    // Implement a simple FSM.
    switch( state ) {
    case NONE_PRESSED:      // Neither switch pressed
        if( GPIO_PORTH[GPIO_PIN_0] == 0 ) {
          state = LEFT_PRESSED;
        } else if ( GPIO_PORTH[GPIO_PIN_1] == 0 ) {
          state = RIGHT_PRESSED;
        }
      break;
    case LEFT_PRESSED:      // Left switch just pressed
      incrementIndex( &index[0] );
      incrementIndex( &index[1] );
      state = LEFT_WAITING;
      break;
    case LEFT_WAITING:      // Left switch still pressed
      if( GPIO_PORTH[GPIO_PIN_0] != 0 ) {
        state = NONE_PRESSED;
      }
      break;
    case RIGHT_PRESSED:     // Right switch just pressed
      incrementIndex( &index[0] );
      incrementIndex( &index[1] );
      state = RIGHT_WAITING;
      break;
    case RIGHT_WAITING:     // Right switch still pressed
      if( GPIO_PORTH[GPIO_PIN_1] != 0 ) {
        state = NONE_PRESSED;
      }
      break;
    }

    // Toggle "display" variable between 0 and 1.
    display ^= 1;
  }
}
