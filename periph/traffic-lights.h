/*
 * traffic-lights.h
 *
 *  Created on: Nov 5, 2025
 *      Author: sammc
 */

#ifndef TRAFFIC_LIGHTS_H_
#define TRAFFIC_LIGHTS_H_

void initLights( void );
void initButtons( void );
bool checkButtons13( void );
bool checkButtons24( void );
void turnLights13( uint8_t color );
void turnLights24( uint8_t color );

#endif /* TRAFFIC_LIGHTS_H_ */
