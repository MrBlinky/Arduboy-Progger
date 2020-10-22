#include <Arduboy2.h>
#include <Sprites.h>
#include "boot.h"
#include "isp.h"
#include "atBitmap.h"
#include "fxBitmap.h"
#include "ncBitmap.h"
#include "slotBitmap.h"
#include "mod-chip-attiny.ino.tiny8.menu.h"

#define FIRMWARE mod_chip_attiny_ino_tiny8_menu

Arduboy2 arduboy;
Sprites sprites;

void delayMillis(uint8_t millis) // wrapper for ISP delay
{
  arduboy.delayShort(millis);
}

uint8_t chipState;

void setup() 
{
   boot();
   arduboy.display(CLEAR_BUFFER); 
   while(!BUTTON_IDLE); //wait if button still depressed from bootloader exit
}

uint8_t detectCount;

void chipDetect()
{
  if (detectCount == 0)
  {
    //update chipstates
    if (TGT1_checkJedecID()) chipState |= 0x04; else chipState &= ~0x04;
    if (TGT2_checkJedecID()) chipState |= 0x10; else chipState &= ~0x10;
    if (TGT3_checkJedecID()) chipState |= 0x40; else chipState &= ~0x40;
    if (ISP1_enable()) { ISP1_disable(); chipState |= 0x08; } else chipState &= ~0x08;
    if (ISP2_enable()) { ISP2_disable(); chipState |= 0x20; } else chipState &= ~0x20;
    if (ISP3_enable()) { ISP3_disable(); chipState |= 0x80; } else chipState &= ~0x80;
  }
  //update LED states  
  else if (detectCount < 11)
  {
    if ((chipState & 0x0c) == 0x0C) LED1_GRN; else LED1_RED;
    if ((chipState & 0x30) == 0x10) LED2_GRN; else LED2_RED;
    if ((chipState & 0xc0) == 0x40) LED3_GRN; else LED3_RED;
  }
  else //turn off LED for flashing effect (one or both chips not detected)
  { 
    if ((chipState & 0x0C) != 0x0C) LED1_OFF;
    if ((chipState & 0x30) != 0x30) LED2_OFF;
    if ((chipState & 0xC0) != 0xC0) LED3_OFF;
  }
  //update display state  
  uint8_t mask = 0x04; 
  for (uint8_t i = 0; i < 3; ++i)
  {
    sprites.drawSelfMasked(0, i * 22 + 6, slotBitmap, i);
    if (chipState & mask) sprites.drawSelfMasked(38, i * 22, fxBitmap, 0);
    else sprites.drawSelfMasked(38, i * 22, ncBitmap, 0);
    mask = mask << 1;
    if (chipState & mask) sprites.drawSelfMasked(84, i * 22, atBitmap, 0);
    else sprites.drawSelfMasked(84, i * 22, ncBitmap, 0);
    mask = mask << 1;
  }
  if (++detectCount >=  30) detectCount = 0;
}

bool flashModChip1()
{
  bool result = false;
  if (ISP1_enable())  
  {
    LED1_RED;
    ISP_eraseChip();
    ISP_writeProgramFlash(FIRMWARE);
    LED1_YEL;
    if (ISP_verifyProgramFlash(FIRMWARE))
    {
      LED1_GRN;
      result = true;
    }
    else
      LED1_RED;
    ISP1_disable();
  }
  return result;
}

bool flashModChip2()
{
  bool result = false;
  if (ISP2_enable())  
  {
    LED1_RED;
    ISP_eraseChip();
    ISP_writeProgramFlash(FIRMWARE);
    LED1_YEL;
    if (ISP_verifyProgramFlash(FIRMWARE))
    {
      LED1_GRN;
      result = true;
    }
    else
      LED1_RED;
    ISP2_disable();
  }
  return result;
}

bool flashModChip3()
{
  bool result = false;
  if (ISP3_enable())  
  {
    LED1_RED;
    ISP_eraseChip();
    ISP_writeProgramFlash(FIRMWARE);
    LED1_YEL;
    if (ISP_verifyProgramFlash(FIRMWARE))
    {
      LED1_GRN;
      result = true;
    }
    else
      LED1_RED;
    ISP3_disable();
  }
  return result;
}
    
void loop() {
  if (!arduboy.nextFrame()) return;
  // no button detect mode
  if (BUTTON_IDLE) chipDetect();
  else 
  {
    //start with flashing attinies (saves time in case of failure)
    LED1_OFF;  
    LED2_OFF;  
    LED3_OFF;  
    if (chipState & 0x08) if (flashModChip1());
    if (chipState & 0x20) if (flashModChip2());
    if (chipState & 0x80) if (flashModChip3());
    
    //wait for button press to return to detect mode
    detectCount = 0;
    while(BUTTON_IDLE)
    {
      if ((detectCount) < 11) USB_LED_ON;       
      else USB_LED_OFF;
      if (++detectCount >= 30) detectCount = 0;
      delayMillis(16);
    }
    while(!BUTTON_IDLE);
    USB_LED_OFF;
    detectCount = 0;
    delayMillis(16);
  } 
  arduboy.display(CLEAR_BUFFER); 
}
