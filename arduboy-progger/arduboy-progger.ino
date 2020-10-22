#include <Arduboy2.h>
#include <Sprites.h>
#include "boot.h"
#include "isp.h"
#include "fxStateBitmap.h"
#include "slotBitmap.h"
#include "mod-chip-attiny.ino.tiny8.menu.h"

#define FIRMWARE mod_chip_attiny_ino_tiny8_menu

#define MODCHIP1 0
#define MODCHIP2 1
#define MODCHIP3 2

#define MC_NOT_FOUND    0
#define MC_FX_FOUND     1
#define MC_AVR_FOUND    2
#define MC_PROGRAMMING  3
#define MC_VERIFYING    4
#define MC_FAIL         5
#define MC_PASS         6

Arduboy2 arduboy;
Sprites sprites;

struct ChipState
{
  uint8_t avr;
  uint8_t fx;
};

ChipState chipState[3];

void delayMillis(uint8_t millis) // wrapper for ISP delay
{
  arduboy.delayShort(millis);
}

void updateDisplay()
{
  for (uint8_t i = 0; i < 3; ++i)
  {
    sprites.drawSelfMasked(0, i * 22 + 6, slotBitmap, i);
    sprites.drawSelfMasked(38, i * 22, fxStateBitmap, chipState[i].fx);
    sprites.drawSelfMasked(84, i * 22, fxStateBitmap, chipState[i].avr);
  }
  arduboy.display(CLEAR_BUFFER); 
}

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
    if (TGT1_checkJedecID()) chipState[MODCHIP1].fx = MC_FX_FOUND; else chipState[MODCHIP1].fx = MC_NOT_FOUND;
    if (TGT2_checkJedecID()) chipState[MODCHIP2].fx = MC_FX_FOUND; else chipState[MODCHIP2].fx = MC_NOT_FOUND;
    if (TGT3_checkJedecID()) chipState[MODCHIP3].fx = MC_FX_FOUND; else chipState[MODCHIP3].fx = MC_NOT_FOUND;
    if (ISP1_enable()) { ISP1_disable(); chipState[MODCHIP1].avr = MC_AVR_FOUND;} else chipState[MODCHIP1].avr = MC_NOT_FOUND;
    if (ISP2_enable()) { ISP2_disable(); chipState[MODCHIP2].avr = MC_AVR_FOUND;} else chipState[MODCHIP2].avr = MC_NOT_FOUND;
    if (ISP3_enable()) { ISP3_disable(); chipState[MODCHIP3].avr = MC_AVR_FOUND;} else chipState[MODCHIP3].avr = MC_NOT_FOUND;
  }
  //update LED states  
  else if (detectCount < 11)
  {
    if (chipState[0].fx && chipState[MODCHIP1].avr) LED1_GRN; else LED1_RED;
    if (chipState[1].fx && chipState[MODCHIP2].avr) LED2_GRN; else LED2_RED;
    if (chipState[2].fx && chipState[MODCHIP3].avr) LED3_GRN; else LED3_RED;
  }
  else //turn off LED for flashing effect (one or both chips not detected)
  { 
    LED1_OFF;
    LED2_OFF;
    LED3_OFF;
  }
  if (++detectCount >=  30) detectCount = 0;
  updateDisplay();
}

bool flashModChip(uint8_t modchip)
{
  bool result;
  if      (modchip == MODCHIP1) result = ISP1_enable();
  else if (modchip == MODCHIP2) result = ISP2_enable();
  else if (modchip == MODCHIP3) result = ISP3_enable();
  if (result) {
    chipState[modchip].avr = MC_PROGRAMMING;
    if      (modchip == MODCHIP1) LED1_RED;
    else if (modchip == MODCHIP2) LED2_RED;
    else if (modchip == MODCHIP3) LED3_RED;
    updateDisplay();
    ISP_eraseChip();
    ISP_writeProgramFlash(FIRMWARE);
    if      (modchip == MODCHIP1) LED1_OFF;
    else if (modchip == MODCHIP2) LED2_OFF;
    else if (modchip == MODCHIP3) LED3_OFF;
    chipState[modchip].avr = MC_VERIFYING;
    updateDisplay();
    if (ISP_verifyProgramFlash(FIRMWARE))
    {
      if      (modchip == MODCHIP1) LED1_GRN;
      else if (modchip == MODCHIP2) LED2_GRN;
      else if (modchip == MODCHIP3) LED3_GRN;
      chipState[modchip].avr = MC_PASS;
    }
    else
    {
      if      (modchip == MODCHIP1) LED1_RED;
      else if (modchip == MODCHIP2) LED2_RED;
      else if (modchip == MODCHIP3) LED3_RED;
      chipState[modchip].avr = MC_FAIL;
      result = false;
    }
    if      (modchip == MODCHIP1) ISP1_disable();
    else if (modchip == MODCHIP2) ISP2_disable();
    else if (modchip == MODCHIP3) ISP3_disable();
    updateDisplay();
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
    if (chipState[MODCHIP1].avr) flashModChip(MODCHIP1);
    if (chipState[MODCHIP2].avr) flashModChip(MODCHIP2);
    if (chipState[MODCHIP3].avr) flashModChip(MODCHIP3);
    
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
}
