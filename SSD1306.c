/*
 * SSD1306.c
 *
 *  Created on: Jan 27, 2023
 *      Author: soul
 */

#include "SSD1306.h"
#include "string.h"
#include <stdint.h>
///////////////////////////////////////////
// SSD1306 i2c GPIO Configuration
// PB8 - SCL
// PB9 - SDA
// GND - GND
// VCC - 3.3V
////////////////////////////////////

#define FONT_HEIGHT 10
#define FONT_WIDTH 7
const uint16_t font_data[] = {
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, // sp
    0x1000, 0x1000, 0x1000, 0x1000, 0x1000,
    0x1000, 0x0000, 0x1000, 0x0000, 0x0000, // !
    0x2800, 0x2800, 0x2800, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, // "
    0x2400, 0x2400, 0x7C00, 0x2400, 0x4800,
    0x7C00, 0x4800, 0x4800, 0x0000, 0x0000, // #
    0x3800, 0x5400, 0x5000, 0x3800, 0x1400,
    0x5400, 0x5400, 0x3800, 0x1000, 0x0000, // $
    0x2000, 0x5400, 0x5800, 0x3000, 0x2800,
    0x5400, 0x1400, 0x0800, 0x0000, 0x0000, // %
    0x1000, 0x2800, 0x2800, 0x1000, 0x3400,
    0x4800, 0x4800, 0x3400, 0x0000, 0x0000, // &
    0x1000, 0x1000, 0x1000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, // '
    0x0800, 0x1000, 0x2000, 0x2000, 0x2000,
    0x2000, 0x2000, 0x2000, 0x1000, 0x0800, // (
    0x2000, 0x1000, 0x0800, 0x0800, 0x0800,
    0x0800, 0x0800, 0x0800, 0x1000, 0x2000, // )
    0x1000, 0x3800, 0x1000, 0x2800, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, // *
    0x0000, 0x0000, 0x1000, 0x1000, 0x7C00,
    0x1000, 0x1000, 0x0000, 0x0000, 0x0000, // +
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x1000, 0x1000, 0x1000, // ,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x3800, 0x0000, 0x0000, 0x0000, 0x0000, // -
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x1000, 0x0000, 0x0000, // .
    0x0800, 0x0800, 0x1000, 0x1000, 0x1000,
    0x1000, 0x2000, 0x2000, 0x0000, 0x0000, // /
    0x3800, 0x4400, 0x4400, 0x5400, 0x4400,
    0x4400, 0x4400, 0x3800, 0x0000, 0x0000, // 0
    0x1000, 0x3000, 0x5000, 0x1000, 0x1000,
    0x1000, 0x1000, 0x1000, 0x0000, 0x0000, // 1
    0x3800, 0x4400, 0x4400, 0x0400, 0x0800,
    0x1000, 0x2000, 0x7C00, 0x0000, 0x0000, // 2
    0x3800, 0x4400, 0x0400, 0x1800, 0x0400,
    0x0400, 0x4400, 0x3800, 0x0000, 0x0000, // 3
    0x0800, 0x1800, 0x2800, 0x2800, 0x4800,
    0x7C00, 0x0800, 0x0800, 0x0000, 0x0000, // 4
    0x7C00, 0x4000, 0x4000, 0x7800, 0x0400,
    0x0400, 0x4400, 0x3800, 0x0000, 0x0000, // 5
    0x3800, 0x4400, 0x4000, 0x7800, 0x4400,
    0x4400, 0x4400, 0x3800, 0x0000, 0x0000, // 6
    0x7C00, 0x0400, 0x0800, 0x1000, 0x1000,
    0x2000, 0x2000, 0x2000, 0x0000, 0x0000, // 7
    0x3800, 0x4400, 0x4400, 0x3800, 0x4400,
    0x4400, 0x4400, 0x3800, 0x0000, 0x0000, // 8
    0x3800, 0x4400, 0x4400, 0x4400, 0x3C00,
    0x0400, 0x4400, 0x3800, 0x0000, 0x0000, // 9
    0x0000, 0x0000, 0x1000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x1000, 0x0000, 0x0000, // :
    0x0000, 0x0000, 0x0000, 0x1000, 0x0000,
    0x0000, 0x0000, 0x1000, 0x1000, 0x1000, // ;
    0x0000, 0x0000, 0x0C00, 0x3000, 0x4000,
    0x3000, 0x0C00, 0x0000, 0x0000, 0x0000, // <
    0x0000, 0x0000, 0x0000, 0x7C00, 0x0000,
    0x7C00, 0x0000, 0x0000, 0x0000, 0x0000, // =
    0x0000, 0x0000, 0x6000, 0x1800, 0x0400,
    0x1800, 0x6000, 0x0000, 0x0000, 0x0000, // >
    0x3800, 0x4400, 0x0400, 0x0800, 0x1000,
    0x1000, 0x0000, 0x1000, 0x0000, 0x0000, // ?
    0x3800, 0x4400, 0x4C00, 0x5400, 0x5C00,
    0x4000, 0x4000, 0x3800, 0x0000, 0x0000, // @
    0x1000, 0x2800, 0x2800, 0x2800, 0x2800,
    0x7C00, 0x4400, 0x4400, 0x0000, 0x0000, // A
    0x7800, 0x4400, 0x4400, 0x7800, 0x4400,
    0x4400, 0x4400, 0x7800, 0x0000, 0x0000, // B
    0x3800, 0x4400, 0x4000, 0x4000, 0x4000,
    0x4000, 0x4400, 0x3800, 0x0000, 0x0000, // C
    0x7000, 0x4800, 0x4400, 0x4400, 0x4400,
    0x4400, 0x4800, 0x7000, 0x0000, 0x0000, // D
    0x7C00, 0x4000, 0x4000, 0x7C00, 0x4000,
    0x4000, 0x4000, 0x7C00, 0x0000, 0x0000, // E
    0x7C00, 0x4000, 0x4000, 0x7800, 0x4000,
    0x4000, 0x4000, 0x4000, 0x0000, 0x0000, // F
    0x3800, 0x4400, 0x4000, 0x4000, 0x5C00,
    0x4400, 0x4400, 0x3800, 0x0000, 0x0000, // G
    0x4400, 0x4400, 0x4400, 0x7C00, 0x4400,
    0x4400, 0x4400, 0x4400, 0x0000, 0x0000, // H
    0x3800, 0x1000, 0x1000, 0x1000, 0x1000,
    0x1000, 0x1000, 0x3800, 0x0000, 0x0000, // I
    0x0400, 0x0400, 0x0400, 0x0400, 0x0400,
    0x0400, 0x4400, 0x3800, 0x0000, 0x0000, // J
    0x4400, 0x4800, 0x5000, 0x6000, 0x5000,
    0x4800, 0x4800, 0x4400, 0x0000, 0x0000, // K
    0x4000, 0x4000, 0x4000, 0x4000, 0x4000,
    0x4000, 0x4000, 0x7C00, 0x0000, 0x0000, // L
    0x4400, 0x6C00, 0x6C00, 0x5400, 0x4400,
    0x4400, 0x4400, 0x4400, 0x0000, 0x0000, // M
    0x4400, 0x6400, 0x6400, 0x5400, 0x5400,
    0x4C00, 0x4C00, 0x4400, 0x0000, 0x0000, // N
    0x3800, 0x4400, 0x4400, 0x4400, 0x4400,
    0x4400, 0x4400, 0x3800, 0x0000, 0x0000, // O
    0x7800, 0x4400, 0x4400, 0x4400, 0x7800,
    0x4000, 0x4000, 0x4000, 0x0000, 0x0000, // P
    0x3800, 0x4400, 0x4400, 0x4400, 0x4400,
    0x4400, 0x5400, 0x3800, 0x0400, 0x0000, // Q
    0x7800, 0x4400, 0x4400, 0x4400, 0x7800,
    0x4800, 0x4800, 0x4400, 0x0000, 0x0000, // R
    0x3800, 0x4400, 0x4000, 0x3000, 0x0800,
    0x0400, 0x4400, 0x3800, 0x0000, 0x0000, // S
    0x7C00, 0x1000, 0x1000, 0x1000, 0x1000,
    0x1000, 0x1000, 0x1000, 0x0000, 0x0000, // T
    0x4400, 0x4400, 0x4400, 0x4400, 0x4400,
    0x4400, 0x4400, 0x3800, 0x0000, 0x0000, // U
    0x4400, 0x4400, 0x4400, 0x2800, 0x2800,
    0x2800, 0x1000, 0x1000, 0x0000, 0x0000, // V
    0x4400, 0x4400, 0x5400, 0x5400, 0x5400,
    0x6C00, 0x2800, 0x2800, 0x0000, 0x0000, // W
    0x4400, 0x2800, 0x2800, 0x1000, 0x1000,
    0x2800, 0x2800, 0x4400, 0x0000, 0x0000, // X
    0x4400, 0x4400, 0x2800, 0x2800, 0x1000,
    0x1000, 0x1000, 0x1000, 0x0000, 0x0000, // Y
    0x7C00, 0x0400, 0x0800, 0x1000, 0x1000,
    0x2000, 0x4000, 0x7C00, 0x0000, 0x0000, // Z
    0x1800, 0x1000, 0x1000, 0x1000, 0x1000,
    0x1000, 0x1000, 0x1000, 0x1000, 0x1800, // [
    0x2000, 0x2000, 0x1000, 0x1000, 0x1000,
    0x1000, 0x0800, 0x0800, 0x0000, 0x0000, /* \ */
    0x3000, 0x1000, 0x1000, 0x1000, 0x1000,
    0x1000, 0x1000, 0x1000, 0x1000, 0x3000, // ]
    0x1000, 0x2800, 0x2800, 0x4400, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, // ^
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0xFE00, // _
    0x2000, 0x1000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, // `
    0x0000, 0x0000, 0x3800, 0x4400, 0x3C00,
    0x4400, 0x4C00, 0x3400, 0x0000, 0x0000, // a
    0x4000, 0x4000, 0x5800, 0x6400, 0x4400,
    0x4400, 0x6400, 0x5800, 0x0000, 0x0000, // b
    0x0000, 0x0000, 0x3800, 0x4400, 0x4000,
    0x4000, 0x4400, 0x3800, 0x0000, 0x0000, // c
    0x0400, 0x0400, 0x3400, 0x4C00, 0x4400,
    0x4400, 0x4C00, 0x3400, 0x0000, 0x0000, // d
    0x0000, 0x0000, 0x3800, 0x4400, 0x7C00,
    0x4000, 0x4400, 0x3800, 0x0000, 0x0000, // e
    0x0C00, 0x1000, 0x7C00, 0x1000, 0x1000,
    0x1000, 0x1000, 0x1000, 0x0000, 0x0000, // f
    0x0000, 0x0000, 0x3400, 0x4C00, 0x4400,
    0x4400, 0x4C00, 0x3400, 0x0400, 0x7800, // g
    0x4000, 0x4000, 0x5800, 0x6400, 0x4400,
    0x4400, 0x4400, 0x4400, 0x0000, 0x0000, // h
    0x1000, 0x0000, 0x7000, 0x1000, 0x1000,
    0x1000, 0x1000, 0x1000, 0x0000, 0x0000, // i
    0x1000, 0x0000, 0x7000, 0x1000, 0x1000,
    0x1000, 0x1000, 0x1000, 0x1000, 0xE000, // j
    0x4000, 0x4000, 0x4800, 0x5000, 0x6000,
    0x5000, 0x4800, 0x4400, 0x0000, 0x0000, // k
    0x7000, 0x1000, 0x1000, 0x1000, 0x1000,
    0x1000, 0x1000, 0x1000, 0x0000, 0x0000, // l
    0x0000, 0x0000, 0x7800, 0x5400, 0x5400,
    0x5400, 0x5400, 0x5400, 0x0000, 0x0000, // m
    0x0000, 0x0000, 0x5800, 0x6400, 0x4400,
    0x4400, 0x4400, 0x4400, 0x0000, 0x0000, // n
    0x0000, 0x0000, 0x3800, 0x4400, 0x4400,
    0x4400, 0x4400, 0x3800, 0x0000, 0x0000, // o
    0x0000, 0x0000, 0x5800, 0x6400, 0x4400,
    0x4400, 0x6400, 0x5800, 0x4000, 0x4000, // p
    0x0000, 0x0000, 0x3400, 0x4C00, 0x4400,
    0x4400, 0x4C00, 0x3400, 0x0400, 0x0400, // q
    0x0000, 0x0000, 0x5800, 0x6400, 0x4000,
    0x4000, 0x4000, 0x4000, 0x0000, 0x0000, // r
    0x0000, 0x0000, 0x3800, 0x4400, 0x3000,
    0x0800, 0x4400, 0x3800, 0x0000, 0x0000, // s
    0x2000, 0x2000, 0x7800, 0x2000, 0x2000,
    0x2000, 0x2000, 0x1800, 0x0000, 0x0000, // t
    0x0000, 0x0000, 0x4400, 0x4400, 0x4400,
    0x4400, 0x4C00, 0x3400, 0x0000, 0x0000, // u
    0x0000, 0x0000, 0x4400, 0x4400, 0x2800,
    0x2800, 0x2800, 0x1000, 0x0000, 0x0000, // v
    0x0000, 0x0000, 0x5400, 0x5400, 0x5400,
    0x6C00, 0x2800, 0x2800, 0x0000, 0x0000, // w
    0x0000, 0x0000, 0x4400, 0x2800, 0x1000,
    0x1000, 0x2800, 0x4400, 0x0000, 0x0000, // x
    0x0000, 0x0000, 0x4400, 0x4400, 0x2800,
    0x2800, 0x1000, 0x1000, 0x1000, 0x6000, // y
    0x0000, 0x0000, 0x7C00, 0x0800, 0x1000,
    0x2000, 0x4000, 0x7C00, 0x0000, 0x0000, // z
    0x1800, 0x1000, 0x1000, 0x1000, 0x2000,
    0x2000, 0x1000, 0x1000, 0x1000, 0x1800, // {
    0x1000, 0x1000, 0x1000, 0x1000, 0x1000,
    0x1000, 0x1000, 0x1000, 0x1000, 0x1000, // |
    0x3000, 0x1000, 0x1000, 0x1000, 0x0800,
    0x0800, 0x1000, 0x1000, 0x1000, 0x3000, // }
    0x0000, 0x0000, 0x0000, 0x7400, 0x4C00,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, // ~
};

// commands macros
#define SSD1306_WRITE_COMMAND(command)                                         \
  I2C0_ByteWrite(SSD1306_I2C_ADDR, 0x00, (command))
#define SSD1306_WRITE_DATA(data) I2C0_ByteWrite(SSD1306_I2C_ADDR, 0x40, (data))

// commands table page 29 from datasheet
#define SSD1306_RIGHT_HORIZONTAL_SCROLL 0x26
#define SSD1306_LEFT_HORIZONTAL_SCROLL 0x27
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL 0x2A
#define SSD1306_DEACTIVATE_SCROLL 0x2E
#define SSD1306_ACTIVATE_SCROLL 0x2F
#define SSD1306_SET_VERTICAL_SCROLL_AREA 0xA3
#define SSD1306_SET_LOWER_COLUMN 0x00
#define SSD1306_SET_HIGHER_COLUMN 0x10
#define SSD1306_MEMORY_ADDR_MODE 0x20
#define SSD1306_COLUMN_ADDR 0x21
#define SSD1306_PAGE_ADDR 0x22
#define SSD1306_SET_START_LINE 0x40
#define SSD1306_SET_CONTRAST 0x81
#define SSD1306_CHARGE_PUMP 0x8D
#define SSD1306_SEG_REMAP 0xA0
#define SSD1306_DISPLAY_ALL_ON_RESUME 0xA4
#define SSD1306_DISPLAY_ALL_ON 0xA5
#define SSD1306_NORMAL_DISPLAY 0xA6
#define SSD1306_INVERT_DISPLAY 0xA7
#define SSD1306_SET_MULTIPLEX_RATIO 0xA8
#define SSD1306_DISPLAY_OFF 0xAE
#define SSD1306_DISPLAY_ON 0xAF
#define SSD1306_COM_SCAN_INC 0xC0
#define SSD1306_COM_SCAN_DEC 0xC8
#define SSD1306_SET_DISPLAY_OFFSET 0xD3
#define SSD1306_SET_DISPLAY_CLOCK_DIV 0xD5
#define SSD1306_SET_PRECHARGE 0xD9
#define SSD1306_SET_COM_PINS 0xDA
#define SSD1306_SET_VCOM_DETECT 0xDB
#define SSD1306_SET_NOP 0xE3
// end of commands table

// SSD1306 buffer
// 128x64 = 1024 bits = 128 bytes
// 8 pages * 128 bytes
char SSD1306_Buffer[SSD1306_WIDTH * SSD1306_HEIGHT / 8];

typedef struct {
  uint8_t CurrentX;
  uint8_t CurrentY;
  uint8_t Inverted;
  uint8_t Initialized;
} SSD1306_t;

static SSD1306_t SSD1306;

void ssd1306_scroll_right(uint8_t start_row, uint8_t end_row) {
  SSD1306_WRITE_COMMAND(SSD1306_RIGHT_HORIZONTAL_SCROLL);
  SSD1306_WRITE_COMMAND(0X00);
  SSD1306_WRITE_COMMAND(start_row);
  SSD1306_WRITE_COMMAND(0X00);
  SSD1306_WRITE_COMMAND(end_row);
  SSD1306_WRITE_COMMAND(0X00);
  SSD1306_WRITE_COMMAND(0XFF);
  SSD1306_WRITE_COMMAND(SSD1306_ACTIVATE_SCROLL);
}
// scroll left
void ssd1306_scroll_left(uint8_t start_row, uint8_t end_row) {
  SSD1306_WRITE_COMMAND(SSD1306_LEFT_HORIZONTAL_SCROLL);
  SSD1306_WRITE_COMMAND(0X00);
  SSD1306_WRITE_COMMAND(start_row);
  SSD1306_WRITE_COMMAND(0X00);
  SSD1306_WRITE_COMMAND(end_row);
  SSD1306_WRITE_COMMAND(0X00);
  SSD1306_WRITE_COMMAND(0XFF);
  SSD1306_WRITE_COMMAND(SSD1306_ACTIVATE_SCROLL);
}

// scroll diagonal right
void ssd1306_scroll_diag_right(uint8_t start_row, uint8_t end_row) {
  SSD1306_WRITE_COMMAND(SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL);
  SSD1306_WRITE_COMMAND(0X00);
  SSD1306_WRITE_COMMAND(start_row);
  SSD1306_WRITE_COMMAND(0X00);
  SSD1306_WRITE_COMMAND(end_row);
  SSD1306_WRITE_COMMAND(0X01);
  SSD1306_WRITE_COMMAND(SSD1306_ACTIVATE_SCROLL);
}
// scroll diagonal left
void ssd1306_scroll_diag_left(uint8_t start_row, uint8_t end_row) {
  SSD1306_WRITE_COMMAND(SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL);
  SSD1306_WRITE_COMMAND(0X00);
  SSD1306_WRITE_COMMAND(start_row);
  SSD1306_WRITE_COMMAND(0X00);
  SSD1306_WRITE_COMMAND(end_row);
  SSD1306_WRITE_COMMAND(0X01);
  SSD1306_WRITE_COMMAND(SSD1306_ACTIVATE_SCROLL);
}
// stop scroll
void ssd1306_stop_scroll(void) {
  SSD1306_WRITE_COMMAND(SSD1306_DEACTIVATE_SCROLL);
}
// set contrast
// contrast - 0 to 255
// default contrast - 0x7F
void ssd1306_set_contrast(uint8_t contrast) {
  SSD1306_WRITE_COMMAND(SSD1306_SET_CONTRAST);
  SSD1306_WRITE_COMMAND(contrast);
}
// set display mode
// mode - 0 - normal, 1 - inverse
// default mode - 0
void ssd1306_set_display_mode(uint8_t mode) {
  if (mode) {
    SSD1306_WRITE_COMMAND(SSD1306_INVERT_DISPLAY);
  } else {
    SSD1306_WRITE_COMMAND(SSD1306_NORMAL_DISPLAY);
  }
}

// ssd1306 init
// init I2C and ssd1306

uint8_t ssd1306_init(void) {
  // init I2C
  I2C0_Init();
  /*Wait a bit*/
  for (int i = 0; i < 3000; i++) {
  }

  /*Run initialization commands*/
  SSD1306_WRITE_COMMAND(0xAE); /*Set Display off*/
  SSD1306_WRITE_COMMAND(0x20); /*Set Memory Addressing Mode*/
  SSD1306_WRITE_COMMAND(0x10); /*Set Page Addressing Mode*/
  SSD1306_WRITE_COMMAND(
      0xB0); /*Set Page Start Address for Page Addressing Mode*/
  SSD1306_WRITE_COMMAND(0xC8); /*Set COM Output Scan Direction*/

  SSD1306_WRITE_COMMAND(0x00); /*Set low column address*/
  SSD1306_WRITE_COMMAND(0x10); /*Set high column address*/
  SSD1306_WRITE_COMMAND(0x40); /*Set start line address*/
  SSD1306_WRITE_COMMAND(0x81); /*Set contrast control register*/
  SSD1306_WRITE_COMMAND(0xFF); /*Set to highest contrast*/
  SSD1306_WRITE_COMMAND(0xA1); /*Set segment re-map 0 to 127*/
  SSD1306_WRITE_COMMAND(0xA6); /*Set normal display*/
  SSD1306_WRITE_COMMAND(0xA8); /*Set multiplex ratio(1 to 64)*/
  SSD1306_WRITE_COMMAND(0x3F); /*Set mux ratio value*/
  SSD1306_WRITE_COMMAND(0xA4); /*Set output to follow RAM content: 0xa4*/
  SSD1306_WRITE_COMMAND(0xD3); /*Set display offset*/
  SSD1306_WRITE_COMMAND(0x00); /*Set display offset alue*/
  SSD1306_WRITE_COMMAND(
      0xD5); /*Set display clock divide ratio/oscillator frequency*/
  SSD1306_WRITE_COMMAND(0xF0); /*Set divide ratio*/
  SSD1306_WRITE_COMMAND(0xD9); /*Set pre-charge period*/
  SSD1306_WRITE_COMMAND(0x22); /*Set pre-charge period value*/
  SSD1306_WRITE_COMMAND(0xDA); /*Set com pins hardware configuration*/
  SSD1306_WRITE_COMMAND(0x12); /*Set com pins value*/
  SSD1306_WRITE_COMMAND(0xDB); /*Adjust VCOM regulator*/
  SSD1306_WRITE_COMMAND(0x20); /*Set  VCOM regulator value*/
  SSD1306_WRITE_COMMAND(0x8D); /*Charge pump regulator setting */
  SSD1306_WRITE_COMMAND(0x14); /*Enable charge pump regulator*/
  SSD1306_WRITE_COMMAND(0xAF); /*Turn on SSD1306 panel*/

  /*Clear screen*/
  ssd1306_clear();
  // update screen
  ssd1306_update_screen();

  /*Reset XY position*/
  SSD1306.CurrentX = 0;
  SSD1306.CurrentY = 0;

  /*Set Init flag*/
  SSD1306.Initialized = 1;

  return 1;
}

// void ssd1306_clear(void) {
//   /*Clear screen*/
//   for (int i = 0; i < 8; i++) {
//     SSD1306_WRITE_COMMAND(0xB0 + i);
//     SSD1306_WRITE_COMMAND(0x00);
//     SSD1306_WRITE_COMMAND(0x10);
//     for (int j = 0; j < 128; j++) {
//       SSD1306_WRITE_DATA(0x00);
//     }
//   }
// }
//
static void SSD_I2C0_MultiWrite(uint8_t address, uint8_t reg, char *data,
                                uint16_t count) {
  I2C0_BurstWrite(address, reg, data, count);
}

void ssd1306_fill(SSD1306_COLOR_t color) {
  memset(SSD1306_Buffer, color == SSD1306_COLOR_BLACK ? 0x00 : 0xFF,
         sizeof(SSD1306_Buffer));
}

void ssd1306_update_screen(void) {
  uint8_t m;
  for (m = 0; m < 8; m++) {
    SSD1306_WRITE_COMMAND(0xB0 + m);
    SSD1306_WRITE_COMMAND(0x00);
    SSD1306_WRITE_COMMAND(0x10);
    // write multi data
    SSD_I2C0_MultiWrite(SSD1306_I2C_ADDR, 0x40,
                        &SSD1306_Buffer[SSD1306_WIDTH * m], SSD1306_WIDTH);
  }
}

// draw pixel
// x - x coordinate
// y - y coordinate
// color - color
void ssd1306_draw_pixel(uint8_t x, uint8_t y, SSD1306_COLOR_t color) {
  if (x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT) {
    // Error
    return;
  }

  // Check if pixel should be inverted
  if (SSD1306.Inverted) { // Inverted mode
    color = (SSD1306_COLOR_t)!color;
  } else {
    color = (SSD1306_COLOR_t)color;
  } // Normal mode = do nothing
    // Draw in currently selected color
  if (color == SSD1306_COLOR_WHITE) {
    // Set color to white
    SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] |= 1 << (y % 8);
  } else {
    // Set color to black
    SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] &= ~(1 << (y % 8));
  }
}

// gotoxy
// x - x coordinate
// y - y coordinate
void ssd1306_gotoxy(uint8_t x, uint8_t y) {
  // Set write pointers
  SSD1306.CurrentX = x;
  SSD1306.CurrentY = y;
}

// draw char
// ch - character to be written
// Font - font used
// color - color
// return - width of character written
char ssd1306_put_ch(char ch, SSD1306_COLOR_t color) {
  uint32_t i, b, j;
  // check remaining space on current line
  if (SSD1306_WIDTH <= (SSD1306.CurrentX + FONT_WIDTH) ||
      SSD1306_HEIGHT <= (SSD1306.CurrentY + FONT_HEIGHT)) {
    // Not enough space on current line
    return 0;
  }
  // go through font
  for (i = 0; i < FONT_HEIGHT; i++) {
    b = font_data[(ch - 32) * FONT_HEIGHT + i];
    for (j = 0; j < FONT_WIDTH; j++) {
      if ((b << j) & 0x8000) {
        ssd1306_draw_pixel(SSD1306.CurrentX + j, (SSD1306.CurrentY + i),
                           (SSD1306_COLOR_t)color);
      } else {
        ssd1306_draw_pixel(SSD1306.CurrentX + j, (SSD1306.CurrentY + i),
                           (SSD1306_COLOR_t)!color);
      }
    }
  }
  // the current space is now taken
  SSD1306.CurrentX += FONT_WIDTH;
  // return written char for validation
  return ch;
}

char ssd1306_put_str(char *str, SSD1306_COLOR_t color) {
  // Write until null-byte
  while (*str) {
    if (ssd1306_put_ch(*str, color) != *str) {
      // Char could not be written
      return *str;
    }
    // Next char
    str++;
  }
  // Everything ok
  return *str;
}
// draw line
// x0 - x coordinate of start point
// y0 - y coordinate of start point
// x1 - x coordinate of end point
// y1 - y coordinate of end point
// color - color
// return - width of character written
void ssd1306_draw_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,
                       SSD1306_COLOR_t color) {
  uint16_t dx, dy, sx, sy, err, e2, i, tmp;
  // Check for overflow
  if (x0 >= SSD1306_WIDTH) {
    x0 = SSD1306_WIDTH - 1;
  }
  if (x1 >= SSD1306_WIDTH) {
    x1 = SSD1306_WIDTH - 1;
  }
  if (y0 >= SSD1306_HEIGHT) {
    y0 = SSD1306_HEIGHT - 1;
  }

  // calculate delta
  dx = (x0 < x1) ? (x1 - x0) : (x0 - x1);
  dy = (y0 < y1) ? (y1 - y0) : (y0 - y1);
  // calculate step
  sx = (x0 < x1) ? 1 : -1;
  sy = (y0 < y1) ? 1 : -1;
  err = ((dx > dy) ? dx : -dy) / 2;
  if (dx == 0) {
    // vertical line
    if (y1 < y0) {
      tmp = y0;
      y0 = y1;
      y1 = tmp;
    }
    if (x1 < x0) {
      tmp = x0;
      x0 = x1;
      x1 = tmp;
    }
    for (i = y0; i <= y1; i++) {
      ssd1306_draw_pixel(x0, i, color);
    }
  } else if (dy == 0) {
    // horizontal line
    if (y1 < y0) {
      tmp = y0;
      y0 = y1;
      y1 = tmp;
    }
    if (x1 < x0) {
      tmp = x0;
      x0 = x1;
      x1 = tmp;
    }
    for (i = x0; i <= x1; i++) {
      ssd1306_draw_pixel(i, y0, color);
    }
  } else {
    // other line
    while (1) {
      ssd1306_draw_pixel(x0, y0, color);
      /* If the end point has been reached, break out of the loop */
      if (x0 == x1 && y0 == y1) {
        break;
      }
      /* Calculate the next error value and update the x and y coordinates
       * accordingly */

      e2 = err;
      if (e2 > -dx) {
        err -= dy;
        x0 += sx;
      }
      if (e2 < dy) {
        err += dx;
        y0 += sy;
      }
    }
  }
}

void ssd1306_clear(void) {
  ssd1306_fill(SSD1306_COLOR_BLACK);
  // ssd1306_update_screen();
}

void ssd1306_draw_bmp(int16_t x, int16_t y, const unsigned char *bitmap,
                      int16_t w, int16_t h, uint16_t color) {
  uint8_t byte = 0;

  // int16_t byteWidth = (w + 7) / 8; // Bitmap scanline pad = whole byte
  int16_t byteWidth = (w + 7) / 8;

  // Bitmap scanline pad = whole byte
  for (int16_t j = 0; j < h; j++, y++) {
    // iterate through the width of the image
    for (int16_t i = 0; i < w; i++) {
      // check if the current bit is not the first in the byte
      if (i & 7) {
        // shift the byte left by 1
        byte <<= 1;
      }

      else {
        // load the next byte of image data
        byte = (*(const unsigned char *)(&bitmap[j * byteWidth + i / 8]));
      }

      // check if the current bit is set
      if (byte & 0x80) {

        // draw the pixel at the current x and y position with the specified
        // color
        ssd1306_draw_pixel(x + i, y, color);
      }
    }
  }
}