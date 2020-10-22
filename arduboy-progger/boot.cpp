#include "boot.h"

void bootPins()
{
  // LEDs and source SPI
  PORTB = (1 << USB_LED_BIT)  |  // LED1 and 2 off, SPI pins low
          (0 << SPI_SCK_BIT)  |
          (0 << SPI_MOSI_BIT) |
          (0 << SPI_MISO_BIT) |
          (1 << LED1_RED_BIT) | 
          (1 << LED1_GRN_BIT) | 
          (1 << LED2_RED_BIT) | 
          (1 << LED2_GRN_BIT);  

  DDRB = (1 << USB_LED_BIT)  |  // All outputs, except MISO
         (1 << SPI_SCK_BIT)  |
         (1 << SPI_MOSI_BIT) |
         (0 << SPI_MISO_BIT) |
         (1 << LED1_RED_BIT) | 
         (1 << LED1_GRN_BIT) | 
         (1 << LED2_RED_BIT) | 
         (1 << LED2_GRN_BIT);  

  PORTC = (1 << LED3_RED_BIT) | //LED3 off
          (1 << LED3_GRN_BIT);

  DDRC = (1 << LED3_RED_BIT) | //LED3 outputs
         (1 << LED3_GRN_BIT);
  
  PORTD =  (1 << SRC_CS_BIT)   | // OLED inactive
           (1 << TGT_MISO_BIT) | // pullup
           (0 << TGT_MOSI_BIT) |
           (1 << DC_BIT)       |
           (0 << TGT_SCK_BIT)  |
           (1 << CS_BIT)       | // OLED inactive
           (1 << RST_BIT);       // OLED RESET idle
            
  DDRD  =  (1 << SRC_CS_BIT)   | // output
           (0 << TGT_MISO_BIT) | // input
           (1 << TGT_MOSI_BIT) |
           (1 << DC_BIT)       |
           (1 << TGT_SCK_BIT)  |
           (1 << CS_BIT)       |
           (1 << RST_BIT);      

  //button input
  PORTE = (1 << A_BUTTON_BIT); //enable pullup on button
  DDRE  = (0 << A_BUTTON_BIT); // input
  
  //targetr flash and modchip controls
  PORTF = (1 << TGT1_SDA_BIT) |  //all inactive high
          (1 << TGT1_SCL_BIT) |  
          (1 << TGT2_SDA_BIT) |  
          (1 << TGT2_SCL_BIT) |  
          (1 << TGT3_SDA_BIT) |  
          (1 << TGT3_SCL_BIT);
  
  DDRF = (1 << TGT1_SDA_BIT) |  //all outputs
         (1 << TGT1_SCL_BIT) |  
         (1 << TGT2_SDA_BIT) |  
         (1 << TGT2_SCL_BIT) |  
         (1 << TGT3_SDA_BIT) |  
         (1 << TGT3_SCL_BIT);
}          

void bootSPI()
{
  // normal SPI for source
  //SPI_PORT &= ~((1 << SPI_MOSI_BIT) | (1 << SPI_SCK_BIT));
  //SPI_PORT |= (1 << SPI_MISO_BIT) | (1 << SPI_SS_BIT);
  //SPI_DDR  |= (1 << SPI_MOSI_BIT) | (1 << SPI_SCK_BIT) /*| (1 << SPI_SS_BIT)*/;
  SPCR = (1 << SPE) | (1 << MSTR); // master, mode 0, MSB first, CPU clock / 2 (8MHz)
  SPSR = (1 << SPI2X);  
  
  //put USART in Master SPI Mode for targets
  UBRR1 = 0;
  //DDRD  |= (1 << TGT_SCK_BIT);
  UCSR1C = (1<<UMSEL11) | (1<<UMSEL10); // Master SPI Mode, MODE 0
  UCSR1B = (1<<RXEN1) | (1<<TXEN1);     // enable
}

const PROGMEM uint8_t OLEDBootData[] =
{
  0xD5, 0xF0,
  0x8D, 0x14,
  0xA1,
  0xC8,
  0x81, 0xCF,
  0xD9, 0xF1,
  0xAF,
  0x20, 0x00,
  //0x21, 0x00, COLUMN_ADDRESS_END,
  //0x22, 0x00, PAGE_ADDRESS_END    
};

void bootOLED()
{
  //reset the display
  bitClear(RST_PORT, RST_BIT);  // reset
  Arduboy2::delayShort(10); 
  bitSet(RST_PORT, RST_BIT);    //release reset
  Arduboy2::delayShort(10);
  bitClear(CS_PORT, CS_BIT);
  Arduboy2::LCDCommandMode();
  for (uint8_t i = 0; i < sizeof(OLEDBootData); i++) 
  {
    Arduboy2::SPItransfer(pgm_read_byte(OLEDBootData + i));
  }
  Arduboy2::LCDDataMode();
}
  
void boot()
{
  bootPins();
  bootSPI();
  bootOLED();
}

/******************************************************************************/

inline void wait()
{
  while ((UCSR1A & (1 << UDRE1)) == 0);
}

void writeByteBeforeWait(uint8_t data)
{
  UDR1 = data;
  asm volatile("nop\n");
  wait();
}

void writeByteAfterWait(uint8_t data)
{
  wait();
  UDR1 = data;
}

uint8_t writeByte(uint8_t data)
{
  writeByteAfterWait(data);
  while ((UCSR1A & (1 << RXC1)) == 0);
  return UDR1;
}

uint8_t readByte()
{
  return writeByte(0);
}

void TGT1_readJedecID(JedecID* id)
{
  TGT1_ENABLE;
  writeByte(SFC_JEDEC_ID);
  id -> manufacturer = readByte();
  id -> device = readByte();
  id -> size = readByte();
  TGT1_DISABLE;
}

bool TGT1_checkJedecID()
{
  JedecID id;
  for(uint8_t i = 0; i < 10; ++i)
  {
    TGT1_readJedecID(&id);
    if (((uint8_t)(id.manufacturer + 1) <= 1) | ((uint8_t)(id.device + 1) <= 1) | ((uint8_t)(id.size + 1) <= 1)) return false;
  }
  return true;
}

void TGT2_readJedecID(JedecID* id)
{
  TGT2_ENABLE;
  writeByte(SFC_JEDEC_ID);
  id -> manufacturer = readByte();
  id -> device = readByte();
  id -> size = readByte();
  TGT2_DISABLE;
}

bool TGT2_checkJedecID()
{
  JedecID id;
  for(uint8_t i = 0; i < 10; ++i)
  {
    TGT2_readJedecID(&id);
    if (((uint8_t)(id.manufacturer + 1) <= 1) | ((uint8_t)(id.device + 1) <= 1) | ((uint8_t)(id.size + 1) <= 1)) return false;
  }
  return true;
}

void TGT3_readJedecID(JedecID* id)
{
  TGT3_ENABLE;
  writeByte(SFC_JEDEC_ID);
  id -> manufacturer = readByte();
  id -> device = readByte();
  id -> size = readByte();
  TGT3_DISABLE;
}

bool TGT3_checkJedecID()
{
  JedecID id;
  for(uint8_t i = 0; i < 10; ++i)
  {
    TGT3_readJedecID(&id);
    if (((uint8_t)(id.manufacturer + 1) <= 1) | ((uint8_t)(id.device + 1) <= 1) | ((uint8_t)(id.size + 1) <= 1)) return false;
  }
  return true;
}

/******************************************************************************/
