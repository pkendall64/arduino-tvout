/*
Copyright (c) 2010 Myles Metzer

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
*/
/*
This library provides a simple method for outputting data to a tv
from a frame buffer stored in sram.  This implementation is done
completly by interupt and will return give as much cpu time to the
application as possible.
*/
#ifndef TVOUT_H
#define TVOUT_H

#include <stdint.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <Print.h>

#include "video_gen.h"
#include "spec/hardware_setup.h"
#include "spec/video_properties.h"

// macros for readability when selecting mode.
#define WHITE					1
#define BLACK					0
#define INVERT					2

#define UP						0
#define DOWN					1
#define LEFT					2
#define RIGHT					3

#define DEC 10
#define HEX 16
#define OCT 8
#define BIN 2
#define BYTE 0

// Macros for clearer usage
#define clear_screen()				fill(0)
#define invert(color)				fill(2)

/*
TVout.cpp contains a brief expenation of each function.
*/
class TVout : public Print {
public:
	uint8_t * screen;

	char begin(uint8_t mode);
	char begin(uint8_t mode, uint8_t x, uint8_t y);
	char begin(uint8_t mode, uint8_t x, uint8_t y, uint8_t *framebuffer);
	void end();

	//accessor functions
	unsigned char hres();
	unsigned char vres();
	char char_line();

	//flow control functions
	void delay(unsigned int x);
	void delay_frame(unsigned int x);
	unsigned long millis();

	//override setup functions
	void force_vscale(char sfactor);
	void force_outstart(uint8_t time);
	void force_linestart(uint8_t line);

	//basic rendering functions
	void set_pixel(uint8_t x, uint8_t y, char c);
	unsigned char get_pixel(uint8_t x, uint8_t y);
	void fill(uint8_t color);
	void shift(uint8_t distance, uint8_t direction);
	void draw_line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, char c);
	void draw_row(uint8_t line, uint16_t x0, uint16_t x1, uint8_t c);
	void draw_column(uint8_t row, uint16_t y0, uint16_t y1, uint8_t c);
	void draw_rect(uint8_t x0, uint8_t y0, uint8_t w, uint8_t h, char c, char fc = -1);
	void draw_circle(uint8_t x0, uint8_t y0, uint8_t radius, char c, char fc = -1);
	void bitmap(uint8_t x, uint8_t y, const uint8_t *bmp, uint16_t i = 0, uint8_t width = 0, uint8_t lines = 0);

	//hook setup functions
	void set_vbi_hook(void (*func)());
	void set_hbi_hook(void (*func)());

	//tone functions
	void tone(unsigned int frequency, unsigned long duration_ms);
	void tone(unsigned int frequency);
	void noTone();

	//printing functions
	void setCursor(uint8_t, uint8_t);
	void selectFont(const uint8_t *f);
	virtual size_t write(uint8_t);
	using Print::write;

	size_t printPGM(PGM_P);

private:
	uint8_t cursor_x, cursor_y;
	const uint8_t *font;

	void write(uint8_t x, uint8_t y, unsigned char c);
};

#endif
