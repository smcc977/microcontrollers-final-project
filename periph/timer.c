/*
 * timer.c
 *
 *  Created on:  Aug 3, 2020
 *  Modified on: Nov 6, 2020
 *      Author: khughes
 *
 */

#include <stdint.h>
#include <stdbool.h>

// The code relies on a symbol "USING_TIMER3A" that controls whether to 
// compile the SYSTICK code or your Timer3A code.  You will need to define 
// this symbol within the peripheral project configuration.  DO NOT define
// within this file!

#ifdef USING_TIMER3A

#include "sysctl.h"
#include "gptm.h"

#else
#include "ppb.h"
#endif

// Move these definitions into a "ppb.h" header file.  Create it using the
// same format as sysctl.h and gpio.h.


// osc.h defines MAINOSCFREQ
#include "osc.h"


// Frequency of switching between left and right LED display
#define   SWITCHFREQ    200

// Add the calculation for your reload value here.
#define RELOAD_VALUE    (MAINOSCFREQ / SWITCHFREQ) - 1

// Configure Timer3A as a periodic timer with a period of 1/200th second.
// The names of each register you will need are defined in gptm.h; you do not 
// need to use any registers not named there.  You will need to define the 
// offsets for each bit or field in the registers.
//
// The "size" parameter should be either 16 or 32, depending on whether
// Timer3A should be configured as a 16-bit or 32-bit timer.
// 
// ALSO: remember to enable the Run Mode Clock Gating for the timer before
// attempting to access any of its registers!
void initTimer( uint8_t size ) {
#ifdef USING_TIMER3A
  SYSCTL[SYSCTL_RCGCTIMER] |= SYSCTL_RCGCTIMER_TIMER3;
  SYSCTL[SYSCTL_RCGCTIMER] |= SYSCTL_RCGCTIMER_TIMER3;

  GPTM_TIMER3[GPTM_CTL] &= ~GPTM_CTL_TAEN;

  GPTM_TIMER3[GPTM_CFG] &= ~GPTM_CFG_M;

  if (size == 32){
    GPTM_TIMER3[GPTM_CFG] |= GPTM_CFG_32BITTIMER;
    GPTM_TIMER3[GPTM_TAILR] = RELOAD_VALUE;
  } else if (size == 16) {
    GPTM_TIMER3[GPTM_CFG] |= GPTM_CFG_16BITTIMERS;
    GPTM_TIMER3[GPTM_TAPR] = 1 << 5;
    GPTM_TIMER3[GPTM_TAILR] = RELOAD_VALUE >> 2;
  }

  GPTM_TIMER3[GPTM_TAMR] &= ~GPTM_TAMR_TAMR_M;
  GPTM_TIMER3[GPTM_TAMR] |= GPTM_TAMR_TAMR_PERIODIC;
  GPTM_TIMER3[GPTM_ICR] = GPTM_ICR_TATOCINT;
  GPTM_TIMER3[GPTM_CTL] |= GPTM_CTL_TAEN | GPTM_CTL_TASTALL;

#else

  PPB[PPB_STCTRL] = 0;
  PPB[PPB_STRELOAD] = RELOAD_VALUE-1;
  PPB[PPB_STCTRL] = PPB_STCTRL_CLK_SRC | PPB_STCTRL_ENABLE;

#endif
}

// Wait for Timer3A to reload, then return.  Perform any necessary actions to
// clear the reload condition.  DO NOT turn off the timer!
void waitOnTimer( void ) {
#ifdef USING_TIMER3A
    while ((GPTM_TIMER3[GPTM_RIS] & GPTM_RIS_TATORIS) == 0);
    GPTM_TIMER3[GPTM_ICR] = GPTM_ICR_TATOCINT;
#else

  while( ( PPB[PPB_STCTRL] & PPB_STCTRL_COUNT ) == 0 );

#endif
}
