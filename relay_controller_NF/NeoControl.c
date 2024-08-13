/*
 * NeoControl.c
 *
 * Created: 19-11-02 14:28:39
 *  Author: erespe
 */ 

#include "NeoControl.h"
#include "light_ws2812.h"
#include <util/delay.h>

uint8_t pos=0;
uint8_t direction=1;
uint8_t i;
uint8_t blue = 0;
uint8_t reverse = 0;

void off()
{
	colors[0].r=000; colors[0].g=000; colors[0].b=000;
		for (int i=0; i<MAXPIX; i++)
			ws2812_sendarray((uint8_t *)&colors[0],3);
}

void house_light()
{
	colors[0].r=050; colors[0].g=050; colors[0].b=050;
	for (int i=0; i<MAXPIX; i++)
		ws2812_sendarray((uint8_t *)&colors[0],3);
}

void yellow()
{
	colors[0].r=255; colors[0].g=100; colors[0].b=000;
	for(int y=0; y<8; y++)
	{
		for (i=0; i<y; i++)
			ws2812_sendarray((uint8_t *)&colors[0],3);		
	}
}

void red()
{
	colors[0].r=255; colors[0].g=000; colors[0].b=000;
	for(int y=0; y<8; y++)
	{
		for (i=0; i<y; i++)
		ws2812_sendarray((uint8_t *)&colors[0],3);
	}
}

void green()
{
	colors[0].r=000; colors[0].g=255; colors[0].b=000;
	for(int y=0; y<8; y++)
	{
		for (i=0; i<y; i++)
		ws2812_sendarray((uint8_t *)&colors[0],3);
	}
}

void rainbow()
{
	uint8_t j = 1;
	uint8_t k = 1;
	
	colors[0].r=150; colors[0].g=150; colors[0].b=150;
	colors[1].r=255; colors[1].g=000; colors[1].b=000;//red
	colors[2].r=255; colors[2].g=100; colors[2].b=000;//orange
	colors[3].r=100; colors[3].g=255; colors[3].b=000;//yellow
	colors[4].r=000; colors[4].g=255; colors[4].b=000;//green
	colors[5].r=000; colors[5].g=100; colors[5].b=255;//light blue (türkis)
	colors[6].r=000; colors[6].g=000; colors[6].b=255;//blue
	colors[7].r=100; colors[7].g=000; colors[7].b=255;//violet
	
	
	uint8_t i;
	for(i=MAXPIX; i>0; i--)
	{
		led[i-1].r=0;led[i-1].g=0;led[i-1].b=0;
	}
	
	for(int i=0; i<MAXPIX*2*MAXPIX; i++)
	{
		//shift all vallues by one led
		uint8_t i=0;
		for(i=MAXPIX; i>1; i--)
		led[i-1]=led[i-2];
		//change colour when colourlength is reached
		if(k>COLORLENGTH)
		{
			j++;
			if(j>7)
			{
				j=0;
			}

			k=0;
		}
		k++;
		//loop colouers
		
		//fade red
		if(led[0].r<(colors[j].r-FADE))
		led[0].r+=FADE;
		
		if(led[0].r>(colors[j].r+FADE))
		led[0].r-=FADE;

		if(led[0].g<(colors[j].g-FADE))
		led[0].g+=FADE;
		
		if(led[0].g>(colors[j].g+FADE))
		led[0].g-=FADE;

		if(led[0].b<(colors[j].b-FADE))
		led[0].b+=FADE;
		
		if(led[0].b>(colors[j].b+FADE))
		led[0].b-=FADE;

		_delay_ms(50);
		ws2812_sendarray((uint8_t *)led,MAXPIX*3);
	}	
}

void raindrop()
{
	led[0].r=00;led[0].g=00;led[0].b=blue;		// LED 0 is red
	led[1].r=00;led[1].g=00;led[1].b=00;		// LED 1 is White
	for (i=0; i<pos; i++)
		ws2812_sendarray((uint8_t *)&led[1],3);	
	for (i=0; i<(8-pos); i++)
		ws2812_sendarray((uint8_t *)&led[0],3);
		
	_delay_ms(1);								
	if(!reverse)
		blue+=5;
	if(reverse)
		blue-=5;
	if(blue==200)
		reverse = 1;
	if(blue==0)
		reverse = 0;
	pos+=direction*2;
	if ((pos==8)||(pos==0)) direction=-direction;
}

