# Ksoloti Bare Metal

* Author: [Douglas P. Fields, Jr.](mailto:symbolics@lisp.engineer)
* My portions Copyright 2024 Douglas P. Fields, Jr.
* My portions license: [Apache 2.0](LICENSE-2.0.txt)
  * [License details](https://www.apache.org/licenses/LICENSE-2.0)
* Started 2024-11-02
* Last updated 2024-11-02
* [Repo Self-Link](https://github.com/LispEngineer/ksoloti_raw_1)

Raw STM32 software for running the Ksoloti Core 0.6 board.


# Functionality Implemented

## IOC File

* 2 User LEDs
* 2 User Buttons (S1-2)

## Demonstration App

* Blink Red or Green LED
  * Vary color depending on S1
  * Vary speed according to S2


# Programming the KSoloti via USB

* DFU Mode: Hold S1 and power up the Ksoloti
  * Plug USB-C into the "PWR & PROG" port
* Run Zadig to upgrade the STM32 BOOTLOADER USB driver
* STM32CubeProgrammer
  * Select "USB" in the top right
  * Pick the Port under "USB configuration"
    * It is innocuous like "USB 1"
  * Click "Connect"
  * Click the "Erasing & Programming" button on the left side
    * Looks like an arrow pointing down into a rectangle
  * Choose the `.elf` file you want to program
  * Check `Verify` and `Run after programming`
  * Click `Start Programming`
  * It will lose connection after you click it and your code will be running


# STM32 IOC

## Clocks

* External high speed clock is 8MHz per Schematic
* System clock speed is 168MHz per Ksoloti firmware
  * [Reference](https://github.com/ksoloti/ksoloti/blob/master/firmware/stm32f4xx_hal_conf.h#L131)

## SDRAM

SDRAM Pin: MCU Pin (per 0.6 Schematic)
* A0-A12: PF0-5, PF12-15, PG0-2 (FMC_Axx)
* DQ0-16: PD14-15, PD0-1, PE7-15, PD8-10 (FMC_Dxx)
* BA0-1: PG4-5 (FMC_BAx)
* CS_bar: PC2 (FMC_SDNE0)
* CKE: PC3 (FMC_SDCKE0)
* CLK: PG8 (FMC_SDCLK)
* DQML/H: PE0/1 (FMC_MBL0/1)
* WE_bar: PC0 (FMC_SDNWE)
* CAS_bar: PG15 (FMC_SDNCAS)
* RAS_bar: PF11 (FMC_SDNRAS)

Configuration in IOC:
* Connectivity -> FMC
* SDRAM 1
* Clock & chip enable: SDCKE0 + SDNE0
* Internal bank number: 4 banks
* Address: 13 bits
* Data: 16 bits
* 16-bit byte enable: Checked

Settings:
* SDRAM timing in memory clock cycles: All 16
* SDRAM control:
  * Write protection: enable ([Reference](https://github.com/ksoloti/ksoloti/blob/c6305dd5c92fde1ad0d5dcc49afcf5de4b7e3a58/firmware/stm32f4xx_fmc.c#L932))

### Status

Currently, SDRAM is not working. The initial memory access causes a
Hard Fault. I programmed the handler to blink the red LED quickly
to show this.

# Debugging

It's easier to debug this with ST-LINK.

I'm using the ST-LINK from a NUCLEO-F429ZI board disconnected
from it's local STM32. I soldered on the J6 header for SWD
and ran the 5 wires across from the NUCLEO to the Ksoloti.

Then you can do all the usual fancy stuff for debugging.


# References

* [STM32CubeIDE](TODO)
  * I'm using version 1.16.0 on Windows 11
* [STM32CubeProrammer](TODO)
  * I'm using version 2.17.0 on Windows 11
* [Ksoloti Resources](https://ksoloti.github.io/5-resources.html)
  * [Ksoloti Forum](https://ksoloti.discourse.group/)
* [STM32F429ZGT6](TODO)
  * The main chip of the KSoloti
* [Micron 48LC16M16A2P-6A-ITG](https://www.mouser.com/ProductDetail/Micron/MT48LC16M16A2P-6A-ITG?qs=rrS6PyfT74cSkrp4%2FxS6cg%3D%3D)
  * RAM used on my Ksoloti board
  * [Micron datasheet](https://www.micron.com/products/memory/dram-components/sdram/part-catalog/part-detail/mt48lc16m16a2p-6a-it-g)
    * 4 Meg x 16 x 4 banks
  * RAM used in schematic: [Alliance Memory AS4C16M16SA-6TIN](https://www.mouser.com/ProductDetail/Alliance-Memory/AS4C16M16SA-6TIN?qs=os3kWr9Y3%252BBqt2XJGYJjwg%3D%3D&utm_id=21520790617&gad_source=1)