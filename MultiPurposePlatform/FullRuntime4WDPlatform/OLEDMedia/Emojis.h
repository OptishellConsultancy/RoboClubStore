
#include <Wire.h>

static const unsigned char PROGMEM logo_bmp[] =
{ 0b00000000, 0b11000000,
  0b00000001, 0b11000000,
  0b00000001, 0b11000000,
  0b00000011, 0b11100000,
  0b11110011, 0b11100000,
  0b11111110, 0b11111000,
  0b01111110, 0b11111111,
  0b00110011, 0b10011111,
  0b00011111, 0b11111100,
  0b00001101, 0b01110000,
  0b00011011, 0b10100000,
  0b00111111, 0b11100000,
  0b00111111, 0b11110000,
  0b01111100, 0b11110000,
  0b01110000, 0b01110000,
  0b00000000, 0b00110000 };


static const unsigned char PROGMEM drunkEmoji[] =
{ 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xef, 0xf1, 0xff, 0xff, 0xcf, 0xf8, 0xff, 0xff, 0x9f, 0xfe, 0x7f, 0xfc, 0x3f, 0xe3, 0x3f, 
0xff, 0xc7, 0xc3, 0xff, 0xff, 0x87, 0xc1, 0xff, 0xff, 0x07, 0xc1, 0xff, 0xff, 0x8f, 0xe3, 0xff, 
0xff, 0xdf, 0xf7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xfc, 0x7f, 0xff, 0xff, 0xf8, 0x7f, 0xfe, 0x3f, 0xfc, 0xf1, 0xfe, 0x1f, 0xfe, 0x40, 0x7e, 0x7f, 
0xff, 0x0f, 0x00, 0xff, 0xff, 0xff, 0xc3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,  };