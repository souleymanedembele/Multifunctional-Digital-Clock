/*
 * SSD1306.h
 *
 *  Created on: Jan 27, 2023
 *      Author: soul
 */

#ifndef SSD1306_H_
#define SSD1306_H_

#include "i2c.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define SSD1306_WIDTH 128
#define SSD1306_HEIGHT 64

#define SSD1306_I2C_ADDR 0x3C // 0B011110+SA0+RW - 0x3C or 0x3D

typedef enum {
  SSD1306_COLOR_BLACK = 0x00, /* Black color, no pixel */
  SSD1306_COLOR_WHITE = 0x01  /* Pixel is set. Color depends on LCD */
} SSD1306_COLOR_t;

void ssd1306_fill(SSD1306_COLOR_t color);
void ssd1306_update_screen(void);
void ssd1306_scroll_right(uint8_t start_row, uint8_t end_row);
void ssd1306_scroll_left(uint8_t start_row, uint8_t end_row);
void ssd1306_scroll_diag_right(uint8_t start_row, uint8_t end_row);
void ssd1306_scroll_diag_left(uint8_t start_row, uint8_t end_row);
void ssd1306_stop_scroll(void);
void ssd1306_set_contrast(uint8_t contrast);

void ssd1306_set_display_mode(uint8_t mode);

uint8_t ssd1306_init(void);
void ssd1306_draw_pixel(uint8_t x, uint8_t y, SSD1306_COLOR_t color);
void ssd1306_update_screen(void);
void ssd1306_gotoxy(uint8_t x, uint8_t y);
char ssd1306_put_ch(char ch, SSD1306_COLOR_t color);
char ssd1306_put_str(char *str, SSD1306_COLOR_t color);
void ssd1306_draw_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,
                       SSD1306_COLOR_t color);
void ssd1306_clear(void);
void ssd1306_draw_bmp(int16_t x, int16_t y, const unsigned char *bitmap,
                      int16_t w, int16_t h, uint16_t color);
#endif /* SSD1306_H_ */