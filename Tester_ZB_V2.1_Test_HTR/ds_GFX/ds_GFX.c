/*
 * ds_GFX.c
 *
 *  Created on: 15 lip 2020
 *      Author: Dawid Sobków
 *      base on adafruit gfx
 */

#include <stdlib.h>
#include <avr/pgmspace.h>
//#include <string.h>

#include "ds_GFX.h"
#include "../T6963C.h"

int cursor_x, cursor_y;

uint8_t ssd1306_buf[1024] = {

};

void _swap_int16_t(uint16_t a, uint16_t b)
{
int16_t t = a;
a = b;
b = t;
}

void writeLine(int x0, int y0, int x1, int y1, int color)
{

  int steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep) {
	  swap(x0, y0);
	  swap(x1, y1);
  }

  if (x0 > x1) {
	  swap(x0, x1);
	  swap(y0, y1);
  }

  int dx, dy;
  dx = x1 - x0;
  dy = abs(y1 - y0);

  int err = dx / 2;
  int ystep;

  if (y0 < y1) {
	  ystep = 1;
  } else {
	  ystep = -1;
  }

  for (; x0 <= x1; x0++) {
    if (steep) {
//      writePixel(y0, x0, color);
      LCD_SetPixel(y0, x0, color);
    } else {
//      writePixel(x0, y0, color);
      LCD_SetPixel(x0, y0, color);
    }
    err -= dy;
    if (err < 0) {
      y0 += ystep;
      err += dx;
    }
  }
}

void drawFastVLine(int x, int y, int h, int color)
{
	writeLine(x, y, x, y + h - 1, color);
}

void drawFastHLine(int x, int y, int w, int color)
{
	writeLine(x, y, x + w - 1, y, color);
}

void drawCircle(int x0, int y0, int r, int color)
{
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;

  LCD_SetPixel(x0, y0 + r, color);
  LCD_SetPixel(x0, y0 - r, color);
  LCD_SetPixel(x0 + r, y0, color);
  LCD_SetPixel(x0 - r, y0, color);

  while (x < y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;

    LCD_SetPixel(x0 + x, y0 + y, color);
    LCD_SetPixel(x0 - x, y0 + y, color);
    LCD_SetPixel(x0 + x, y0 - y, color);
    LCD_SetPixel(x0 - x, y0 - y, color);
    LCD_SetPixel(x0 + y, y0 + x, color);
    LCD_SetPixel(x0 - y, y0 + x, color);
    LCD_SetPixel(x0 + y, y0 - x, color);
    LCD_SetPixel(x0 - y, y0 - x, color);
  }
}

void fillCircle( int x, int y, uint8_t r, uint8_t color ) {
	drawFastVLine( x, y-r, 2*r+1, color );
	fillCircleHelper( x, y, r, 3, 0, color );
}

// Used to do circles and roundrects
void fillCircleHelper( int x0, int y0, int r, uint8_t cornername, int delta, uint8_t color ) {

	int f		= 1 - r;
	int ddF_x   = 1;
	int ddF_y	= -2 * r;
	int x		= 0;
	int y		= r;

	while (x<y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f	  += ddF_y;
		}
		x++;
		ddF_x += 2;
		f	  += ddF_x;

		if(cornername & 0x1) {
			drawFastVLine(x0+x, y0-y, 2*y+1+delta, color);
			drawFastVLine(x0+y, y0-x, 2*x+1+delta, color);
		}
		if(cornername & 0x2) {
			drawFastVLine(x0-x, y0-y, 2*y+1+delta, color);
			drawFastVLine(x0-y, y0-x, 2*x+1+delta, color);
		}
	}
}

void drawRect(int x, int y, int w, int h, int color)
{
  drawFastHLine(x, y, w, color);
  drawFastHLine(x, y + h - 1, w, color);
  drawFastVLine(x, y, h, color);
  drawFastVLine(x + w - 1, y, h, color);
}

void fillRect( int x, int y, int w, int h, uint8_t color ) {

	for ( uint8_t i = x; i < x+w; i++) {
		drawFastVLine(i, y, h, color);
	}
}

void fillRoundRect( int x, int y, int w, int h, uint8_t r, uint8_t color ) {
	fillRect(x+r, y, w-2*r, h, color);

	fillCircleHelper(x+w-r-1, y+r, r, 1, h-2*r-1, color);
	fillCircleHelper(x+r	   , y+r, r, 2, h-2*r-1, color);
}

void drawRoundRect( int x, int y, int w, int h, uint8_t r, uint8_t color ) {

	drawFastHLine(x+r	, y		, w-2*r, color);
	drawFastHLine(x+r	, y+h-1	, w-2*r, color);
	drawFastVLine(x		, y+r	, h-2*r, color);
	drawFastVLine(x+w-1	, y+r	, h-2*r, color);

	drawCircleHelper(x+r		, y+r	 , r, 1, color);
	drawCircleHelper(x+w-r-1	, y+r    , r, 2, color);
	drawCircleHelper(x+w-r-1	, y+h-r-1, r, 4, color);
	drawCircleHelper(x+r		, y+h-r-1, r, 8, color);
}

void drawCircleHelper(  int x0, int y0, uint8_t r, uint8_t cornername, uint8_t color ) {
	int f		= 1 - r;
	int ddF_x   = 1;
	int ddF_y	= -2 * r;
	int x		= 0;
	int y		= r;

	while (x<y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f	  += ddF_y;
		}
		x++;
		ddF_x += 2;
		f	  += ddF_x;
		if(cornername & 0x4) {
			LCD_SetPixel(x0+x, y0+y, color);
			LCD_SetPixel(x0+y, y0+x, color);
		}
		if(cornername & 0x2) {
			LCD_SetPixel(x0+x, y0-y, color);
			LCD_SetPixel(x0+y, y0-x, color);
		}
		if(cornername & 0x8) {
			LCD_SetPixel(x0-y, y0+x, color);
			LCD_SetPixel(x0-x, y0+y, color);
		}
		if(cornername & 0x1) {
			LCD_SetPixel(x0-y, y0-x, color);
			LCD_SetPixel(x0-x, y0-y, color);
		}
	}
}

// TEXT ************************************************************************************************************ TEXT
void mk_ssd1306_drawChar( int x, int y, char c, uint8_t color, uint8_t bg, uint8_t size ) {

	if((x >= SCREEN_WIDTH) || (y >= SCREEN_HEIGHT) || ((x + 6 * size -1) < 0) || ((y + 8 * size -1) < 0)) return;

	uint8_t line;

	for (uint8_t i = 0; i < 6; i++) {

		if(i == 5)
			line = 0x0;
		else
			line = pgm_read_byte(font+(c*5)+i);

		for (uint8_t j = 0; j < 8; j++) {
			if(line & 0x1){
				if(size == 1)
					LCD_SetPixel(x+i, y+j, color);
				else {
					fillRect(x+(i*size), y+(j*size), size, size, color);
				}
			} else if (bg != color) {
				if(size == 1)
					LCD_SetPixel(x+i, y+j, bg);
				else {
					fillRect(x+i*size, y+j*size, size, size, bg);
				}
			}
			line >>= 1;
		}
	}
}

void mk_ssd1306_puts( int x, int y, char * str, uint8_t txt_size, uint8_t color, uint8_t bg) {

	cursor_x = x; cursor_y = y;

	while( *str ) {
		mk_ssd1306_drawChar( cursor_x, cursor_y, *str++, color, bg, txt_size );
		cursor_x += txt_size * 6;
	}
}
