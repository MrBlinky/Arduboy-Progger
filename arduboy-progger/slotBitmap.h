#pragma once
constexpr uint8_t slotBitmapWidth = 36;
constexpr uint8_t slotBitmapHeight = 8;

const uint8_t PROGMEM slotBitmap[] =
{
  slotBitmapWidth, slotBitmapHeight,
  //Frame 0
  0x46, 0xCF, 0x89, 0x89, 0xFB, 0x72, 0x00, 0xFF, 0xFF, 0x80, 0x80, 0x80, 0x80, 0x00, 0x7E, 0xFF, 0x81, 0x81, 0xFF, 0x7E, 0x00, 0x01, 0x01, 0xFF, 0xFF, 0x01, 0x01, 0x00, 0x00, 0x00, 0x82, 0x82, 0xFF, 0xFF, 0x80, 0x80, 

  //Frame 1
  0x46, 0xCF, 0x89, 0x89, 0xFB, 0x72, 0x00, 0xFF, 0xFF, 0x80, 0x80, 0x80, 0x80, 0x00, 0x7E, 0xFF, 0x81, 0x81, 0xFF, 0x7E, 0x00, 0x01, 0x01, 0xFF, 0xFF, 0x01, 0x01, 0x00, 0x00, 0x00, 0xC2, 0xE3, 0xB1, 0x99, 0x8F, 0x86, 

  //Frame 2
  0x46, 0xCF, 0x89, 0x89, 0xFB, 0x72, 0x00, 0xFF, 0xFF, 0x80, 0x80, 0x80, 0x80, 0x00, 0x7E, 0xFF, 0x81, 0x81, 0xFF, 0x7E, 0x00, 0x01, 0x01, 0xFF, 0xFF, 0x01, 0x01, 0x00, 0x00, 0x00, 0x42, 0xC3, 0x89, 0x89, 0xFF, 0x76
};
