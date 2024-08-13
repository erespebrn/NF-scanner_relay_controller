/*
 * NeoControl.h
 *
 * Created: 19-11-02 14:28:23
 *  Author: erespe
 */ 
#define F_CPU 16000000UL

#ifndef NEOCONTROL_H_
#define NEOCONTROL_H_
#include "light_ws2812.h"

#define MAXPIX 9
#define COLORLENGTH (MAXPIX/2)
#define FADE (256/COLORLENGTH)

struct cRGB colors[8];

struct cRGB led[MAXPIX];

	
void rainbow();
void red();
void house_light();
void green();
void yellow();
void raindrop();
void off();
void set_pixel(uint8_t i, char * color);


#endif /* NEOCONTROL_H_ */