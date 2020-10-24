#include <Arduboy2.h>
#include <Sprites.h>
#include "boot.h"
#include "isp.h"
#include "ArduboyFX.h"
#include "fxStateBitmap.h"
#include "slotBitmap.h"
#include "startBitmap.h"
#include "numbersBitmap.h"
#include "eraseBitmap.h"
#include "mod-chip-attiny.ino.tiny8.menu.h"

#define FIRMWARE mod_chip_attiny_ino_tiny8_menu

#define MODCHIP1 0
#define MODCHIP2 1
#define MODCHIP3 2

#define MC_NOT_FOUND    0
#define MC_FX_FOUND     1
#define MC_AVR_FOUND    2
#define MC_ERASE        3
#define MC_PROGRAM      4
#define MC_VERIFY       5
#define MC_FAIL         6
#define MC_PASS         7

extern uint16_t lastPage;
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

void updateStartDisplay()
{
  CS_PORT &= ~(1 << CS_BIT);
  arduboy.display(); 
  CS_PORT |= (1 << CS_BIT);
}

void updateStatusDisplay()
{
  for (uint8_t i = 0; i < 3; ++i)
  {
    sprites.drawSelfMasked(0, i * 22 + 6, slotBitmap, i);
    sprites.drawSelfMasked(38, i * 22, fxStateBitmap, chipState[i].fx);
    sprites.drawSelfMasked(84, i * 22, fxStateBitmap, chipState[i].avr);
  }
  CS_PORT &= ~(1 << CS_BIT);
  arduboy.display(CLEAR_BUFFER); 
  CS_PORT |= (1 << CS_BIT);
}

void setup() 
{
   boot();
   updateStartDisplay(); // black display
   uint8_t count = 0;
   while (!BUTTON_IDLE)
   {
     if (++count == 200) break; // button hold down for 2 seconds
     delayMillis(10);
   }
   FX::begin();
   if (count == 200) //erase mode
   {
     sprites.drawSelfMasked((WIDTH - 110) / 2, (HEIGHT - 10) / 2, eraseBitmap, 0);
     updateStartDisplay();
     LED1_RED;
     LED2_RED;
     LED3_RED;
     // erase chip  
     FX::writeEnable();
     FX::writeCommand(0xC7);
     FX::enable();
     while (FX::writeByte(SFC_READSTATUS1) & 1);
     FX::disable();
     LED1_OFF;
     LED2_OFF;
     LED3_OFF;
     lastPage = 0;
   }
   else // normal startup
   {
     sprites.drawSelfMasked(0,0,startBitmap,0);
     updateStartDisplay();
     LED1_YEL;
     LED2_YEL;
     LED3_YEL;
     detectLastPageUsed();
     uint16_t size = (lastPage >> 2) + 1;
     for (uint8_t i = 0; i < 5; ++i)
     {
       sprites.drawSelfMasked(106 - i * 7, 30, numbersBitmap, size % 10);
       size /= 10;
       if (size == 0) break;
     }
     updateStartDisplay();
     
     uint32_t csum = 0;
     for (uint8_t i = 0; i < 8; ++i)
     {
       sprites.drawSelfMasked(122 - i * 7, 49, numbersBitmap, csum % 10);
       csum >>= 4;
     }
     updateStartDisplay();
     LED1_OFF;
     LED2_OFF;
     LED3_OFF;
     while(BUTTON_IDLE);
     delayMillis(20);
     while(!BUTTON_IDLE);
     delayMillis(20);
   }
   arduboy.clear();
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
  updateStatusDisplay();
}

bool flashModChip(uint8_t modchip)
{
  bool result;
  if      (modchip == MODCHIP1) result = ISP1_enable();
  else if (modchip == MODCHIP2) result = ISP2_enable();
  else if (modchip == MODCHIP3) result = ISP3_enable();
  if (result) {
    if      (modchip == MODCHIP1) LED1_RED;
    else if (modchip == MODCHIP2) LED2_RED;
    else if (modchip == MODCHIP3) LED3_RED;
    chipState[modchip].avr = MC_ERASE;
    updateStatusDisplay();
    ISP_eraseChip();
    chipState[modchip].avr = MC_PROGRAM;
    updateStatusDisplay();
    ISP_writeProgramFlash(FIRMWARE);
    if      (modchip == MODCHIP1) LED1_OFF;
    else if (modchip == MODCHIP2) LED2_OFF;
    else if (modchip == MODCHIP3) LED3_OFF;
    chipState[modchip].avr = MC_VERIFY;
    updateStatusDisplay();
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
    updateStatusDisplay();
  }
  return result;
}

bool usedFxChip(uint8_t modchip)
{
  bool used = false;
  if (modchip == MODCHIP1) TGT1_ENABLE;
  if (modchip == MODCHIP2) TGT2_ENABLE;
  if (modchip == MODCHIP3) TGT3_ENABLE;
  // test for 1st 256 bytes are empty (0xFF)
  writeByte(SFC_READ);
  writeByte(0);
  writeByte(0);
  writeByte(0);
  uint8_t i = 0;
  do 
  {
    if (readByte() != 0xFF)
    {
      used = true;  
      break;
    }
  } 
  while (--i != 0);
  if (modchip == MODCHIP1) TGT1_DISABLE;
  if (modchip == MODCHIP2) TGT2_DISABLE;
  if (modchip == MODCHIP3) TGT3_DISABLE;
  return used;
}

void fxChipErase(uint8_t modchips)
{
  if (modchips & (1 << MODCHIP1)) TGT1_ENABLE;
  if (modchips & (1 << MODCHIP2)) TGT2_ENABLE;
  if (modchips & (1 << MODCHIP3)) TGT3_ENABLE;
  writeByte(SFC_WRITE_ENABLE);
  if (modchips & (1 << MODCHIP1)) TGT1_DISABLE;
  if (modchips & (1 << MODCHIP2)) TGT2_DISABLE;
  if (modchips & (1 << MODCHIP3)) TGT3_DISABLE;
  
  if (modchips & (1 << MODCHIP1)) TGT1_ENABLE;
  if (modchips & (1 << MODCHIP2)) TGT2_ENABLE;
  if (modchips & (1 << MODCHIP3)) TGT3_ENABLE;
  writeByte(0xC7);
  if (modchips & (1 << MODCHIP1)) TGT1_DISABLE;
  if (modchips & (1 << MODCHIP2)) TGT2_DISABLE;
  if (modchips & (1 << MODCHIP3)) TGT3_DISABLE;
  
  if (modchips & (1 << MODCHIP1)) TGT1_ENABLE;
  if (modchips & (1 << MODCHIP2)) TGT2_ENABLE;
  if (modchips & (1 << MODCHIP3)) TGT3_ENABLE;
  writeByte(SFC_READSTATUS1);
  while (readByte() & 1); 
  if (modchips & (1 << MODCHIP1)) TGT1_DISABLE;
  if (modchips & (1 << MODCHIP2)) TGT2_DISABLE;
  if (modchips & (1 << MODCHIP3)) TGT3_DISABLE;
}

void verifyFxChip(uint8_t modchip)
{
  chipState[modchip].fx = MC_VERIFY;
  updateStatusDisplay();
  
  // verify
  uint16_t page = 0;
  setSourceStartAddress();
  if (modchip == MODCHIP1) TGT1_ENABLE;
  if (modchip == MODCHIP2) TGT2_ENABLE;
  if (modchip == MODCHIP3) TGT3_ENABLE;
  writeByte(SFC_READ);
  writeByte(page >> 8);
  writeByte(page & 0xFF);
  writeByte(0);
  bool fail = false;
  do
  {
    uint8_t i = 0;
    do
    {
      uint8_t data = SPDR;
      SPSR;
      SPDR = 0;
      if (readByte() != data)
      {
        fail = true;  
        break;
      }
    } 
    while (--i != 0);
  }
  while ((page++ != lastPage) && (!fail));
  FX::disable();  

  if (modchip == MODCHIP1)
  {
    TGT1_DISABLE;
    if (fail)
    {
      chipState[MODCHIP1].fx = MC_FAIL;        
      LED1_RED;
    }
    else
    {
      chipState[MODCHIP1].fx = MC_PASS;        
      LED1_GRN;
    }
  }
  if (modchip == MODCHIP2)
  {
    TGT2_DISABLE;
    if (fail)
    {
      chipState[MODCHIP2].fx = MC_FAIL;        
      LED2_RED;
    }
    else
    {
      chipState[MODCHIP2].fx = MC_PASS;        
      LED2_GRN;
    }
  }
  if (modchip == MODCHIP3)
  {
    TGT3_DISABLE;
    if (fail)
    {
      chipState[MODCHIP3].fx = MC_FAIL;        
      LED3_RED;
    }
    else
    {
      chipState[MODCHIP3].fx = MC_PASS;        
      LED3_GRN;
    }
  }
  updateStatusDisplay();
}

  

void flashFxChip()
{
  uint8_t modchips = 0;
  if ((chipState[MODCHIP1].fx == MC_FX_FOUND) & (chipState[MODCHIP1].avr == MC_PASS))
  {
    modchips |= (1 << MODCHIP1);
    if (usedFxChip(MODCHIP1)) modchips |= 0x80;
    LED1_RED;
  }
  if ((chipState[MODCHIP2].fx == MC_FX_FOUND) & (chipState[MODCHIP2].avr == MC_PASS)) 
  {
    modchips |= (1 << MODCHIP2);
    if (usedFxChip(MODCHIP2)) modchips |= 0x80;
    LED2_RED;
  }
  if ((chipState[MODCHIP3].fx == MC_FX_FOUND) & (chipState[MODCHIP3].avr == MC_PASS)) 
  {
    modchips |= (1 << MODCHIP3);
    if (usedFxChip(MODCHIP3)) modchips |= 0x80;
    LED3_RED;
  }
  // chip erase
  if (modchips & 0x80) // required
  {
    if (modchips & (1 << MODCHIP1)) chipState[MODCHIP1].fx = MC_ERASE;
    if (modchips & (1 << MODCHIP2)) chipState[MODCHIP2].fx = MC_ERASE;
    if (modchips & (1 << MODCHIP3)) chipState[MODCHIP3].fx = MC_ERASE;
    updateStatusDisplay();
    fxChipErase(modchips);
  }
  
  // program
  if (modchips & (1 << MODCHIP1)) chipState[MODCHIP1].fx = MC_PROGRAM;
  if (modchips & (1 << MODCHIP2)) chipState[MODCHIP2].fx = MC_PROGRAM;
  if (modchips & (1 << MODCHIP3)) chipState[MODCHIP3].fx = MC_PROGRAM;
  updateStatusDisplay();
  setSourceStartAddress();
  uint16_t page = 0;
  do
  {
    if (modchips & (1 << MODCHIP1)) TGT1_ENABLE;
    if (modchips & (1 << MODCHIP2)) TGT2_ENABLE;
    if (modchips & (1 << MODCHIP3)) TGT3_ENABLE;
    writeByte(SFC_WRITE_ENABLE);
    if (modchips & (1 << MODCHIP1)) TGT1_DISABLE;
    if (modchips & (1 << MODCHIP2)) TGT2_DISABLE;
    if (modchips & (1 << MODCHIP3)) TGT3_DISABLE;
    
    if (modchips & (1 << MODCHIP1)) TGT1_ENABLE;
    if (modchips & (1 << MODCHIP2)) TGT2_ENABLE;
    if (modchips & (1 << MODCHIP3)) TGT3_ENABLE;
    writeByte(SFC_WRITE);
    writeByte(page >> 8);
    writeByte(page & 0xFF);
    writeByte(0);
    uint8_t i = 0;
    do
    {
      uint8_t data = SPDR;
      SPSR;
      SPDR = 0;
      writeByte(data);
    } 
    while (--i != 0);
    if (modchips & (1 << MODCHIP1)) TGT1_DISABLE;
    if (modchips & (1 << MODCHIP2)) TGT2_DISABLE;
    if (modchips & (1 << MODCHIP3)) TGT3_DISABLE;
    
    //wait for page write complete
    if (modchips & (1 << MODCHIP1)) TGT1_ENABLE;
    if (modchips & (1 << MODCHIP2)) TGT2_ENABLE;
    if (modchips & (1 << MODCHIP3)) TGT3_ENABLE;
    writeByte(SFC_READSTATUS1);
    while (readByte() & 1); 
    if (modchips & (1 << MODCHIP1)) TGT1_DISABLE;
    if (modchips & (1 << MODCHIP2)) TGT2_DISABLE;
    if (modchips & (1 << MODCHIP3)) TGT3_DISABLE;
  }
  while (page++ != lastPage);
  FX::disable();  

  if (modchips & (1 << MODCHIP1))
  {
    chipState[MODCHIP1].fx = MC_FX_FOUND;
    LED1_OFF;
  }
  if (modchips & (1 << MODCHIP2))
  {
    chipState[MODCHIP2].fx = MC_FX_FOUND;
    LED2_OFF;
  }
  if (modchips & (1 << MODCHIP3))
  {
    chipState[MODCHIP3].fx = MC_FX_FOUND;
    LED3_OFF;
  }
  if (modchips & (1 << MODCHIP1)) verifyFxChip(MODCHIP1);
  if (modchips & (1 << MODCHIP2)) verifyFxChip(MODCHIP2);
  if (modchips & (1 << MODCHIP3)) verifyFxChip(MODCHIP3);
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
    
    flashFxChip();
    
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
