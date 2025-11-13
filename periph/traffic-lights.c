#include <stdint.h>
#include <stdbool.h>

#include "sysctl.h"
#include "gptm.h"
#include "ppb.h"
#include "gpio.h"


void initLights( void ) {
    SYSCTL[SYSCTL_RCGCGPIO] |= SYSCTL_RCGCGPIO_PORTD;
    GPIO_PORTD[GPIO_DIR] |= GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5;
    GPIO_PORTD[GPIO_DEN] |= GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5;
}

void initButtons( void ) {
    SYSCTL[SYSCTL_RCGCGPIO] |= SYSCTL_RCGCGPIO_PORTH;
    GPIO_PORTH[GPIO_DIR] &= ~(GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
    GPIO_PORTH[GPIO_DEN] |= GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3;
}

bool checkButtons13( void ) {
    return GPIO_PORTH[GPIO_PIN_2] == 0 | GPIO_PORTH[GPIO_PIN_4] == 0;
}

bool checkButtons24( void ) {
    return GPIO_PORTH[GPIO_PIN_1] == 0 | GPIO_PORTH[GPIO_PIN_3] == 0;
}

void turnLights13( uint8_t color ){
    GPIO_PORTD[GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2] &= 0;
    switch (color){
        case 0: //red
            GPIO_PORTD[GPIO_PIN_0] |= GPIO_PIN_0;
            break;
        case 1: //yellow
            GPIO_PORTD[GPIO_PIN_1] |= GPIO_PIN_1;
            break;
        case 2: //green
            GPIO_PORTD[GPIO_PIN_2] |= GPIO_PIN_2;
            break;
        case 3:
            break;
    }
}

void turnLights24( uint8_t color ){
    GPIO_PORTD[GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5] &= 0;
        switch (color){
            case 0: //red
                GPIO_PORTD[GPIO_PIN_3] |= GPIO_PIN_3;
                break;
            case 1: //yellow
                GPIO_PORTD[GPIO_PIN_4] |= GPIO_PIN_4;
                break;
            case 2: //green
                GPIO_PORTD[GPIO_PIN_5] |= GPIO_PIN_5;
                break;
            case 3:
                break;
        }
}
