# Arduboy-Progger
A programmer to program multiple Arduboy FX mod chips.

At the core the programmer is stripped down Arduboy with only the OLED, A-button and FX flash chip remaining. All the other GPIO is used to program 3 mod chip modules and control the status LEDs. For the hardware a Teensy 2.0 is used as it has the same atmega32u4 as Arduboy and has all the extra GPIO pins broken out. This setup was chosen to speed up the development of a mod chip programmer.

The programmer will both program the attiny MCU of Arduboy FX the mod chip. the attiny firmware is included in the programmers code and the flash chip image is stored in an onboard flashchip.

### Power on / Exit from bootloader
* The 3 RED and GREEN LEDs will light up and a start up screen is displayed on the OLED display.
* The size of flash image stored in the onbaord flash chip is determined and is displayed on the OLED display.
* !TO DO! A checksum  of the flash image is generated and displayed on the OLED display.
* The LEDs turn off and the button can be pressed to enter **detection mode**

### Detection mode
In detection mode each of the 3 mod chip is scanned for it's presence and each mod chip LED will flash. A LED will flash green if both the flash chip and attiny of a mod chip are detected and red otherwise. The state of each of the chips is displayed on the OLED display.
To start **programming mode** the button must be pressed.

### Programming mode
During programming the mod chip red LED will light up and turn of during verification. After verification is complete the green LED will light up to indicate a success or the red LED will light up again to indicate a fail. The programming states and verification resultsare displayed on the OLED display.

At the end of the programming session the USB LED will flash and the button can be pressed to return to **detection mode**.

### Erasing stored flash image
holding down the button for 2 seconds after powering on will erases the flash image stored in the onboard flash chip.
