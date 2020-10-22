#pragma once
constexpr uint8_t fxStateBitmapWidth = 44;
constexpr uint8_t fxStateBitmapHeight = 24;

const uint8_t PROGMEM fxStateBitmap[] =
{
  fxStateBitmapWidth, fxStateBitmapHeight,
  //Frame 0
  0x00, 0x54, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x20, 0x22, 0x60, 0x42, 0xC0, 0x82, 0x80, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x80, 0x82, 0xC0, 0x42, 0x60, 0x22, 0x20, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x54, 
  0x00, 0x55, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x40, 0x60, 0x20, 0x30, 0x10, 0x19, 0x09, 0x0F, 0x06, 0x06, 0x06, 0x0F, 0x09, 0x19, 0x10, 0x30, 0x20, 0x60, 0x40, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x55, 
  0x00, 0x01, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x01, 

  //Frame 1
  0xF8, 0x04, 0x04, 0x04, 0x07, 0x05, 0x05, 0x05, 0x05, 0x07, 0x84, 0xC4, 0xC4, 0xC4, 0xE7, 0xE5, 0xE5, 0x75, 0x75, 0x77, 0x34, 0xB4, 0xB4, 0x34, 0x07, 0x05, 0x05, 0x05, 0x85, 0xC7, 0xE4, 0xE4, 0xF4, 0x74, 0x27, 0x05, 0x05, 0x05, 0x05, 0x07, 0x04, 0x04, 0x04, 0xF8, 
  0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xF1, 0x7D, 0x1F, 0x1F, 0x1F, 0x0D, 0x0C, 0x0C, 0x44, 0x66, 0x66, 0x77, 0x3B, 0x3F, 0x1E, 0x1E, 0x3E, 0x3F, 0x37, 0x63, 0x63, 0x21, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 
  0x01, 0x02, 0x02, 0x02, 0x0E, 0x0A, 0x0A, 0x0A, 0x0A, 0x0E, 0x02, 0x02, 0x02, 0x02, 0x0E, 0x0A, 0x0A, 0x0A, 0x0A, 0x0E, 0x02, 0x02, 0x02, 0x02, 0x0E, 0x0A, 0x0A, 0x0A, 0x0A, 0x0E, 0x02, 0x02, 0x02, 0x02, 0x0E, 0x0A, 0x0A, 0x0A, 0x0A, 0x0E, 0x02, 0x02, 0x02, 0x01, 

  //Frame 2
  0xF8, 0x04, 0x04, 0x04, 0x87, 0x85, 0x85, 0x85, 0x05, 0x07, 0x84, 0x84, 0x84, 0x84, 0x87, 0x85, 0x05, 0x85, 0x85, 0x87, 0x84, 0x84, 0x84, 0x04, 0x07, 0x85, 0x85, 0x05, 0x05, 0x87, 0x84, 0x04, 0x04, 0x84, 0x87, 0x05, 0x85, 0x85, 0x05, 0x07, 0x84, 0x84, 0x04, 0xF8, 
  0xFF, 0x00, 0x00, 0x1F, 0x1F, 0x04, 0x04, 0x1F, 0x1F, 0x00, 0x00, 0x00, 0x1F, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x1F, 0x00, 0x00, 0x1F, 0x1F, 0x07, 0x0E, 0x1F, 0x1F, 0x00, 0x01, 0x03, 0x1E, 0x1E, 0x03, 0x01, 0x00, 0xFF, 
  0x01, 0x02, 0x02, 0x02, 0x0E, 0x0A, 0x0A, 0x0A, 0x0A, 0x0E, 0x02, 0x02, 0x02, 0x02, 0x0E, 0x0A, 0x0A, 0x0A, 0x0A, 0x0E, 0x02, 0x02, 0x02, 0x02, 0x0E, 0x0A, 0x0A, 0x0A, 0x0A, 0x0E, 0x02, 0x02, 0x02, 0x02, 0x0E, 0x0A, 0x0A, 0x0A, 0x0A, 0x0E, 0x02, 0x02, 0x02, 0x01, 

  //Frame 3
  0xF8, 0x04, 0x04, 0x04, 0x07, 0x05, 0x05, 0xE5, 0xE5, 0x67, 0x64, 0xE4, 0xC4, 0x04, 0x07, 0xE5, 0xE5, 0x65, 0x65, 0xE7, 0xC4, 0x04, 0x04, 0xC4, 0xE7, 0x65, 0x65, 0xE5, 0xC5, 0x07, 0x04, 0xC4, 0xE4, 0x64, 0x67, 0xE5, 0xC5, 0x05, 0x05, 0x07, 0x04, 0x04, 0x04, 0xF8, 
  0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x7F, 0x0C, 0x0C, 0x0F, 0x07, 0x00, 0x00, 0x7F, 0x7F, 0x0C, 0x0C, 0x7F, 0x77, 0x00, 0x00, 0x3F, 0x7F, 0x60, 0x60, 0x7F, 0x3F, 0x00, 0x00, 0x3F, 0x7F, 0x60, 0x64, 0x7C, 0x3C, 0x00, 0x60, 0x60, 0x00, 0x00, 0x00, 0xFF, 
  0x01, 0x02, 0x02, 0x02, 0x0E, 0x0A, 0x0A, 0x0A, 0x0A, 0x0E, 0x02, 0x02, 0x02, 0x02, 0x0E, 0x0A, 0x0A, 0x0A, 0x0A, 0x0E, 0x02, 0x02, 0x02, 0x02, 0x0E, 0x0A, 0x0A, 0x0A, 0x0A, 0x0E, 0x02, 0x02, 0x02, 0x02, 0x0E, 0x0A, 0x0A, 0x0A, 0x0A, 0x0E, 0x02, 0x02, 0x02, 0x01, 

  //Frame 4
  0xF8, 0x04, 0x04, 0x04, 0x07, 0x05, 0x05, 0x65, 0x65, 0xE7, 0xE4, 0x64, 0x64, 0x04, 0x07, 0xE5, 0xE5, 0x65, 0x65, 0x67, 0x64, 0x04, 0x04, 0xC4, 0xE7, 0x65, 0x65, 0xE5, 0xC5, 0x07, 0x64, 0x64, 0xE4, 0xE4, 0x67, 0x65, 0x05, 0x05, 0x05, 0x07, 0x04, 0x04, 0x04, 0xF8, 
  0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x7F, 0x66, 0x66, 0x66, 0x60, 0x00, 0x00, 0x33, 0x77, 0x66, 0x66, 0x7E, 0x3C, 0x00, 0x00, 0x00, 0x7F, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 
  0x01, 0x02, 0x02, 0x02, 0x0E, 0x0A, 0x0A, 0x0A, 0x0A, 0x0E, 0x02, 0x02, 0x02, 0x02, 0x0E, 0x0A, 0x0A, 0x0A, 0x0A, 0x0E, 0x02, 0x02, 0x02, 0x02, 0x0E, 0x0A, 0x0A, 0x0A, 0x0A, 0x0E, 0x02, 0x02, 0x02, 0x02, 0x0E, 0x0A, 0x0A, 0x0A, 0x0A, 0x0E, 0x02, 0x02, 0x02, 0x01, 

  //Frame 5
  0xF8, 0x04, 0x04, 0x04, 0x07, 0x05, 0x05, 0x05, 0xE5, 0xE7, 0x64, 0x64, 0x64, 0x64, 0x07, 0x05, 0xC5, 0xE5, 0x65, 0x67, 0xE4, 0xC4, 0x04, 0x04, 0x67, 0xE5, 0xE5, 0x65, 0x05, 0x07, 0xE4, 0xE4, 0x04, 0x04, 0x07, 0x05, 0x05, 0x05, 0x05, 0x07, 0x04, 0x04, 0x04, 0xF8, 
  0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x7F, 0x0C, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x7F, 0x0C, 0x0C, 0x7F, 0x7F, 0x00, 0x00, 0x60, 0x7F, 0x7F, 0x60, 0x00, 0x00, 0x7F, 0x7F, 0x60, 0x60, 0x60, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 
  0x01, 0x02, 0x02, 0x02, 0x0E, 0x0A, 0x0A, 0x0A, 0x0A, 0x0E, 0x02, 0x02, 0x02, 0x02, 0x0E, 0x0A, 0x0A, 0x0A, 0x0A, 0x0E, 0x02, 0x02, 0x02, 0x02, 0x0E, 0x0A, 0x0A, 0x0A, 0x0A, 0x0E, 0x02, 0x02, 0x02, 0x02, 0x0E, 0x0A, 0x0A, 0x0A, 0x0A, 0x0E, 0x02, 0x02, 0x02, 0x01, 

  //Frame 6
  0xF8, 0x04, 0x04, 0x04, 0x07, 0x05, 0x05, 0xE5, 0xE5, 0x67, 0x64, 0xE4, 0xC4, 0x04, 0x07, 0xC5, 0xE5, 0x65, 0x65, 0xE7, 0xC4, 0x04, 0x04, 0xC4, 0xE7, 0x65, 0x65, 0x65, 0x65, 0x07, 0x04, 0xC4, 0xE4, 0x64, 0x67, 0x65, 0x65, 0x05, 0x05, 0x07, 0x04, 0x04, 0x04, 0xF8, 
  0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x7F, 0x0C, 0x0C, 0x0F, 0x07, 0x00, 0x00, 0x7F, 0x7F, 0x0C, 0x0C, 0x7F, 0x7F, 0x00, 0x00, 0x63, 0x67, 0x66, 0x66, 0x7E, 0x3C, 0x00, 0x00, 0x63, 0x67, 0x66, 0x66, 0x7E, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 
  0x01, 0x02, 0x02, 0x02, 0x0E, 0x0A, 0x0A, 0x0A, 0x0A, 0x0E, 0x02, 0x02, 0x02, 0x02, 0x0E, 0x0A, 0x0A, 0x0A, 0x0A, 0x0E, 0x02, 0x02, 0x02, 0x02, 0x0E, 0x0A, 0x0A, 0x0A, 0x0A, 0x0E, 0x02, 0x02, 0x02, 0x02, 0x0E, 0x0A, 0x0A, 0x0A, 0x0A, 0x0E, 0x02, 0x02, 0x02, 0x01
};
