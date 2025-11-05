/*
 * keypad.c
 *
 *  Created on:  July 21, 2019
 *  Modified on: Aug 4, 2020
 *      Author: khughes
 *
 */

#include <stdint.h>
#include <stdbool.h>

// SYSCTL and GPIO definitions.
#include "ppb.h"
#include "sysctl.h"
#include "gpio.h"

// Prototypes for peripherals.
#include "osc.h"

// Initialize the specified row and column ports
void initKeypad(void) {
        SYSCTL[SYSCTL_RCGCGPIO] |= SYSCTL_RCGCGPIO_PORTN;
        SYSCTL[SYSCTL_RCGCGPIO] |= SYSCTL_RCGCGPIO_PORTN;
        SYSCTL[SYSCTL_RCGCGPIO] |= SYSCTL_RCGCGPIO_PORTM;
        SYSCTL[SYSCTL_RCGCGPIO] |= SYSCTL_RCGCGPIO_PORTM;

        GPIO_PORTN[GPIO_DIR] &= ~(GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
        GPIO_PORTN[GPIO_PUR] |= GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3;

        GPIO_PORTM[GPIO_DIR] |= GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
        GPIO_PORTM[GPIO_PUR] &= ~( GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);

        GPIO_PORTM[GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7] = 0;
        GPIO_PORTN[GPIO_DEN] |= GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3;
        GPIO_PORTM[GPIO_DEN] |= GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
}

// Check for a key in a row.  If a key is pressed, the procedure returns true and
// the row and column are set.  If a key is not pressed, the procedure returns
// false and row and columns don't matter.
bool getKey( uint8_t *col, uint8_t *row ) {
  uint32_t col_direct[4] = {GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_6, GPIO_PIN_7};
  uint32_t row_direct[4] = {GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_3};
  uint8_t test_row = 0;
  uint8_t test_col = 0;

  // Is anything high in the row?
  // if no, change to the next row and return false
  GPIO_PORTM[GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7] = 0;

  for(test_row = 0; test_row < 4; test_row = test_row + 1) {
      if(GPIO_PORTN[row_direct[test_row]] == 0) {
          // If yes, now figure out which column it is and return true
          for(test_col = 0; test_col < 4; test_col = test_col + 1) {
              GPIO_PORTM[GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7] |= GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
              GPIO_PORTM[col_direct[test_col]] &= ~col_direct[test_col];
              if(GPIO_PORTN[row_direct[test_row]] == 0) {
                  *col = test_col;
                  *row = test_row;
                  return true;
              }
          }
      }
  }
  return false;
}
