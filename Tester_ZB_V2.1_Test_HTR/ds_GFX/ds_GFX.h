/*
 * ds_GFX.h
 *
 *  Created on: 15 lip 2020
 *      Author: Dawid Sobków
 *      base on adafruit gfx
 */

#ifndef DS_GFX_H_
#define DS_GFX_H_

#include <avr/io.h>

#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 64

#define BUF_SIZE (SCREEN_WIDTH*SCREEN_HEIGHT/8)
#define swap(a, b) { int16_t t = a; a = b; b = t; }

extern const uint8_t font[];
extern int cursor_x, cursor_y;

// Rysowanko
void writeLine(int x0, int y0, int x1, int y1, int color);
void drawFastVLine(int x, int y, int h, int color);
void drawFastHLine(int x, int y, int w, int color);

void drawCircle(int x0, int y0, int r, int color);
void fillCircle( int x, int y, uint8_t r, uint8_t color );
void drawCircleHelper(  int x0, int y0, uint8_t r, uint8_t cornername, uint8_t color );
void fillCircleHelper( int x0, int y0, int r, uint8_t cornername, int delta, uint8_t color );

void drawRect(int x, int y, int w, int h, int color);
void fillRect( int x, int y, int w, int h, uint8_t color );

void drawRoundRect( int x, int y, int w, int h, uint8_t r, uint8_t color );
void fillRoundRect( int x, int y, int w, int h, uint8_t r, uint8_t color );

//TEXT
void mk_ssd1306_drawChar( int x, int y, char c, uint8_t color, uint8_t bg, uint8_t size );
void mk_ssd1306_puts( int x, int y, char * str, uint8_t txt_size, uint8_t color, uint8_t bg);

// Prywatne
//void _swap_int16_t(uint16_t a, uint16_t b);



#endif /* DS_GFX_H_ */
