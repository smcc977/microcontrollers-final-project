#include <stdint.h>
#include <stdbool.h>

#include "sysctl.h"
#include "gptm.h"
#include "ppb.h"


void initTimerInterupt( void ) {
    GPTM_TIMER2[GPTM_IMR] |= GPTM_IMR_TATOIM;
    PPB[PPB_EN0] |= PPB_EN0_TIMER2A;
    GPTM_TIMER2[GPTM_CTL] |= GPTM_CTL_TAEN | GPTM_CTL_TASTALL;
}
