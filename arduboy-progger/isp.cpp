#include "isp.h"
#include "boot.h"

uint8_t ISP_command(uint16_t command, uint8_t data)
{
  writeByte(command >> 8);
  writeByte(command & 0xFF);
  uint8_t tmp = 0;
  if ((uint8_t)(command >> 8) == (uint8_t)(ISP_CMD_READ_ID >> 8))
  {
    tmp = data;
    data = 0;
  }
  uint8_t result = writeByte(tmp);
  uint8_t result2 = writeByte(data);
  if ((uint8_t)(command & 0xFF) != (uint8_t)(ISP_CMD_PROGRAM_ENABLE & 0xFF)) return result2;
  return result;
}  

uint8_t ISP_command(uint16_t command)
{
  return ISP_command(command, 0);
}

void ISP_wait()
{
  while (ISP_command(ISP_CMD_POLL) & 0x01);
}

uint8_t ISP_commandWait(uint16_t command, uint8_t data)
{
  uint8_t result = ISP_command(command, data);
  while (ISP_command(ISP_CMD_POLL) & 0x01);
  return result;
}

uint8_t ISP_commandWait(uint16_t command)
{
  return ISP_commandWait(command, 0);
}

void ISP_eraseChip()
{
  ISP_commandWait(ISP_CMD_CHIP_ERASE);
}

void ISP_flashPage(uint16_t addr)
{
  writeByte(ISP_CMD_FLASH_PAGE >> 8);
  writeByte(addr >> 8);
  writeByte(addr & 0xFF);
  writeByte(0);
  ISP_wait();
}

void ISP_writeData(const uint8_t* data) /* not used */
{
  uint8_t cmd  = pgm_read_byte(data++); //fetch FLASH/EEPROM write command, or End of Data
  while(cmd)
  {
    uint16_t addr = pgm_read_byte(data++) | (pgm_read_byte(data++) << 8);
    uint16_t len  = pgm_read_byte(data++) | (pgm_read_byte(data++) << 8);
    while(len)
    {
      writeByte(cmd);
      writeByte(addr >> 8);
      writeByte(addr & 0xFF);
      writeByte(pgm_read_byte(data++));
      if (!(cmd & ISP_EEPROM_MASK)) //FLASH: handle high/low byte and page write
      {
        if ((cmd & ISP_HIGH_BYTE_MASK) && ((addr & ISP_PAGE_MASK) == ISP_PAGE_MASK))
        {
          ISP_flashPage(addr);
        }
        cmd ^= ISP_HIGH_BYTE_MASK; //toggle HI byte /LO byte command
      }
      else //EEPROM: just wait
      {
        ISP_wait();
      }
      if (!(cmd & ISP_HIGH_BYTE_MASK)) //don't adjust after a lo byte flash write
      {
        addr++; 
        len--;
      }
    }
    if (!(cmd & ISP_EEPROM_MASK) && (addr & ISP_PAGE_MASK)) // flash partial last page 
    {
      ISP_flashPage(addr);
    }
    cmd  = pgm_read_byte(data++); //fetch FLASH/EEPROM write command, or End of Data
  }
}

void ISP_writeProgramFlash(const uint8_t* data)
{
  uint8_t cmd = ISP_CMD_FLASH_LO_BYTE >> 8;
  uint16_t len  = pgm_read_byte(data++) | (pgm_read_byte(data++) << 8);
  while(len)
  {
    uint16_t addr = pgm_read_byte(data++) | (pgm_read_byte(data++) << 8);
    while(len)
    {
      writeByte(cmd);
      writeByte(addr >> 8);
      writeByte(addr & 0xFF);
      writeByte(pgm_read_byte(data++));
      if ((cmd & ISP_HIGH_BYTE_MASK) && ((addr & ISP_PAGE_MASK) == ISP_PAGE_MASK))
      {
        ISP_flashPage(addr);
      }
      cmd ^= ISP_HIGH_BYTE_MASK; //toggle HI byte /LO byte command
      if (!(cmd & ISP_HIGH_BYTE_MASK)) //don't adjust after a lo byte flash write
      {
        addr++; 
        len--;
      }
    }
    if (addr & ISP_PAGE_MASK) // flash partial last page 
    {
      ISP_flashPage(addr);
    }
    len  = pgm_read_byte(data++) | (pgm_read_byte(data++) << 8);// next block length or end marker
  }
}

bool ISP_verifyProgramFlash(const uint8_t* data)
{
  uint8_t cmd = ISP_CMD_READ_LO_BYTE >> 8;
  uint16_t len  = pgm_read_byte(data++) | (pgm_read_byte(data++) << 8);
  while(len)
  {
    uint16_t addr = pgm_read_byte(data++) | (pgm_read_byte(data++) << 8);
    while(len)
    {
      writeByte(cmd);
      writeByte(addr >> 8);
      writeByte(addr & 0xFF);
      uint8_t prog = writeByte(0);
      //Serial.println(prog);
      if (pgm_read_byte(data++) != prog) return false;
      cmd ^= ISP_HIGH_BYTE_MASK; //toggle HI byte /LO byte command
      if (!(cmd & ISP_HIGH_BYTE_MASK)) //don't adjust after a lo byte flash write
      {
        addr++; 
        len--;
      }
    }
    len  = pgm_read_byte(data++) | (pgm_read_byte(data++) << 8);// next block length or end marker
  }
  return true;
}

uint16_t ISP_getFlashCrc(uint16_t addr, uint16_t len)
{
  uint16_t crc = 0xFFFF;
  uint8_t cmd = ISP_CMD_READ_LO_BYTE >> 8;
  while(len)
  {
    writeByte(cmd);
    writeByte(addr >> 8);
    writeByte(addr & 0xFF);
    uint8_t data =writeByte(0);
    data ^= crc & 0xFF; 
    data ^= (data << 4) & 0xFF;
    crc  = ((data << 8) | (crc >> 8)) ^ (data >> 4) ^ (data << 3);
    cmd ^= ISP_HIGH_BYTE_MASK; //toggle HI byte /LO byte command
    if (!(cmd & ISP_HIGH_BYTE_MASK)) //don't adjust after a lo byte flash write
    {
      addr++; 
      len--;
    }
  }    
  return crc;
}

uint8_t ISP_readLowFuse()
{
  return ISP_command(ISP_CMD_READ_LOW_FUSE);
}

void ISP_writeLowFuse(uint8_t fuse)
{
  ISP_commandWait(ISP_CMD_WRITE_LOW_FUSE,fuse);
}

uint8_t ISP_readHighFuse()
{
  return ISP_command(ISP_CMD_READ_HIGH_FUSE);
}

void ISP_writeHighFuse(uint8_t fuse)
{
  ISP_commandWait(ISP_CMD_WRITE_HIGH_FUSE,fuse);
}

uint8_t ISP_readExtFuse()
{
  return ISP_command(ISP_CMD_READ_EXT_FUSE);
}

void ISP_writeExtFuse(uint8_t fuse)
{
  ISP_commandWait(ISP_CMD_WRITE_EXT_FUSE,fuse);
}

uint8_t ISP_readLockBits()
{
  return ISP_command(ISP_CMD_READ_LOCK_BITS);
}

void ISP_writeLockbits(uint8_t fuse)
{
  ISP_commandWait(ISP_CMD_WRITE_LOCK_BITS,fuse);
}

void ISP_setFuse_EESAVE()
{ 
  uint8_t fuse = ISP_command(ISP_CMD_READ_HIGH_FUSE);
  if (fuse & ISP_EESAVE_FUSE_MASK)
  {
    fuse &= ~ISP_EESAVE_FUSE_MASK;
    ISP_commandWait(ISP_CMD_WRITE_HIGH_FUSE,fuse);
  }
}

void ISP_writeEEPROM_00(uint8_t data)
{
  ISP_commandWait(ISP_CMD_WRITE_EEPROM, data);
}

uint8_t ISP_readEEPROM_00()
{
  return ISP_command(ISP_CMD_READ_EEPROM);
}

/******************************************************************************/

void ISP1_disable()
{
  bitSet(PORTF, TGT1_SCL_BIT); // inactive high
  UBRR1 = 0;//max SPI speed
}

bool ISP1_enable()
{
  uint8_t result;
  UBRR1 =31;//250KHz
  for (uint8_t retries = 0; retries < 3; retries++)
  {
    bitSet(PORTF, TGT1_SCL_BIT); // inactive high
    Arduboy2::delayShort(1);
    bitClear(PORTF, TGT1_SCL_BIT); //active low
    Arduboy2::delayShort(20);
    result = ISP_command(ISP_CMD_PROGRAM_ENABLE);
    if (result == (uint8_t)(ISP_CMD_PROGRAM_ENABLE & 0xFF)) break; //entered programming mode successfully
  }
  if (result == (uint8_t)(ISP_CMD_PROGRAM_ENABLE & 0xFF)) return true;
  ISP1_disable();
  return false;
}

/******************************************************************************/

void ISP2_disable()
{
  bitSet(PORTF, TGT2_SCL_BIT); // inactive high
  UBRR1 = 0;//max SPI speed
}

bool ISP2_enable()
{
  uint8_t result;
  UBRR1 = 31; //250KHz SPI speed
  for (uint8_t retries = 0; retries < 3; retries++)
  {
    bitSet(PORTF, TGT2_SCL_BIT); // inactive high
    Arduboy2::delayShort(1);
    bitClear(PORTF, TGT2_SCL_BIT); //active low
    Arduboy2::delayShort(20);
    result = ISP_command(ISP_CMD_PROGRAM_ENABLE);
    if (result == (uint8_t)(ISP_CMD_PROGRAM_ENABLE & 0xFF)) break; //entered programming mode successfully
  }
  if (result == (uint8_t)(ISP_CMD_PROGRAM_ENABLE & 0xFF)) return true;
  ISP2_disable();
  return false;
}

void ISP3_disable()
{
  bitSet(PORTF, TGT3_SCL_BIT); // inactive high
  UBRR1 = 0;//max SPI speed
}

bool ISP3_enable()
{
  uint8_t result;
  UBRR1 = 31; //250KHz SPI speed
  for (uint8_t retries = 0; retries < 3; retries++)
  {
    bitSet(PORTF, TGT3_SCL_BIT); // inactive high
    Arduboy2::delayShort(1);
    bitClear(PORTF, TGT3_SCL_BIT); //active low
    Arduboy2::delayShort(20);
    result = ISP_command(ISP_CMD_PROGRAM_ENABLE);
    if (result == (uint8_t)(ISP_CMD_PROGRAM_ENABLE & 0xFF)) break; //entered programming mode successfully
  }
  if (result == (uint8_t)(ISP_CMD_PROGRAM_ENABLE & 0xFF)) return true;
  ISP3_disable();
  return false;
}
