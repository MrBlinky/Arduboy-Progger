#ifndef BOOT_H
#define BOOT_H

#include <Arduboy2.h>
#include "ArduboyFX.h"

#define MAX_FX_SIZE 16384 // flash chip size in kBytes

// LEDs
#define USB_LED_BIT     PORTB0 // RX LED

#define LED1_RED_BIT    PORTB4
#define LED1_GRN_BIT    PORTB5
#define LED2_RED_BIT    PORTB6
#define LED2_GRN_BIT    PORTB7
#define LED3_RED_BIT    PORTC6
#define LED3_GRN_BIT    PORTC7

// OLED display
#define OLED_DC_BIT     PORTD4
#define OLED_CS_BIT     PORTD6
#define OLED_RST_BIT    PORTD7

// source flash
#define SRC_CS_BIT      PORTD1
#define SPI_SCK_BIT     PORTB1
#define SPI_MOSI_BIT    PORTB2
#define SPI_MISO_BIT    PORTB3

// target flash
#define TGT1_SDA_BIT    PORTF0
#define TGT2_SDA_BIT    PORTF4
#define TGT3_SDA_BIT    PORTF6
#define TGT_SCK_BIT     PORTD5
#define TGT_MOSI_BIT    PORTD3
#define TGT_MISO_BIT    PORTD2

// target modchip
#define TGT1_SCL_BIT    PORTF1
#define TGT2_SCL_BIT    PORTF5
#define TGT3_SCL_BIT    PORTF7

// LED macro's
#define LED1_OFF        PORTB |=  (1 << LED1_RED_BIT), PORTB |=  (1 << LED1_GRN_BIT)
#define LED1_RED        PORTB &= ~(1 << LED1_RED_BIT), PORTB |=  (1 << LED1_GRN_BIT)
#define LED1_GRN        PORTB |=  (1 << LED1_RED_BIT), PORTB &= ~(1 << LED1_GRN_BIT)
#define LED1_YEL        PORTB &= ~(1 << LED1_RED_BIT), PORTB &= ~(1 << LED1_GRN_BIT)

#define LED2_OFF        PORTB |=  (1 << LED2_RED_BIT), PORTB |=  (1 << LED2_GRN_BIT)
#define LED2_RED        PORTB &= ~(1 << LED2_RED_BIT), PORTB |=  (1 << LED2_GRN_BIT)
#define LED2_GRN        PORTB |=  (1 << LED2_RED_BIT), PORTB &= ~(1 << LED2_GRN_BIT)
#define LED2_YEL        PORTB &= ~(1 << LED2_RED_BIT), PORTB &= ~(1 << LED2_GRN_BIT)

#define LED3_OFF        PORTC |=  (1 << LED3_RED_BIT), PORTC |=  (1 << LED3_GRN_BIT)
#define LED3_RED        PORTC &= ~(1 << LED3_RED_BIT), PORTC |=  (1 << LED3_GRN_BIT)
#define LED3_GRN        PORTC |=  (1 << LED3_RED_BIT), PORTC &= ~(1 << LED3_GRN_BIT)
#define LED3_YEL        PORTC &= ~(1 << LED3_RED_BIT), PORTC &= ~(1 << LED3_GRN_BIT)

#define USB_LED_OFF      PORTB |=  (1 << USB_LED_BIT)
#define USB_LED_ON       PORTB &= ~(1 << USB_LED_BIT)
#define USB_LED_TOGGLE   PINB = (1 << USB_LED_BIT)

// button macro
#define BUTTON_IDLE (PINE & (1 << A_BUTTON_BIT))

//source macros
#define SRC_ENABLE     PORTD &= ~(1 << SRC_CS_BIT)
#define SRC_DISABLE    PORTD |=  (1 << SRC_CS_BIT)

//target macros
#define TGT1_ENABLE     PORTF &= ~(1 << TGT1_SDA_BIT)
#define TGT1_DISABLE    PORTF |=  (1 << TGT1_SDA_BIT)
#define TGT2_ENABLE     PORTF &= ~(1 << TGT2_SDA_BIT)
#define TGT2_DISABLE    PORTF |=  (1 << TGT2_SDA_BIT)
#define TGT3_ENABLE     PORTF &= ~(1 << TGT3_SDA_BIT)
#define TGT3_DISABLE    PORTF |=  (1 << TGT3_SDA_BIT)

#define ISP1_ENABLE     PORTF &= ~(1 << TGT1_SCL_BIT)
#define ISP1_DISABLE    PORTF |=  (1 << TGT1_SCL_BIT)
#define ISP2_ENABLE     PORTF &= ~(1 << TGT2_SCL_BIT)
#define ISP2_DISABLE    PORTF |=  (1 << TGT2_SCL_BIT)
#define ISP3_ENABLE     PORTF &= ~(1 << TGT3_SCL_BIT)
#define ISP3_DISABLE    PORTF |=  (1 << TGT3_SCL_BIT)

void bootPins();
void bootSPI();
void bootOLED();
void boot();

inline void wait() __attribute__((always_inline)); // wait for a pending flash transfer to complete

uint8_t writeByte(uint8_t data);

inline void writeByteBeforeWait(uint8_t data) __attribute__((always_inline));

inline void writeByteAfterWait(uint8_t data) __attribute__((always_inline));
uint8_t readByte(); //read a single byte from flash memory

void detectLastPageUsed();

void TGT1_readJedecID(JedecID* id);
bool TGT1_checkJedecID();

void TGT2_readJedecID(JedecID* id);
bool TGT2_checkJedecID();

void TGT3_readJedecID(JedecID* id);
bool TGT3_checkJedecID();

/******************************************************************************/

#endif