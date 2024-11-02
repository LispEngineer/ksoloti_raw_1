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
* External clock (8MHz)
* SDRAM
  * Additional SDRAM initialization sequence manually written

## Demonstration App

Capabilities:
* Blink Red or Green LED
  * Vary color depending on S1
  * Vary speed according to S2
* Write data and read data to all SDRAM
  * Flash green if OK, red if not

Useful stuff:
* Sets up SDRAM for Use

## TODO

* Set up linker file to allow allocating ordinary variables
  in SDRAM, in both initialized and BSS (zero-initialized)
  sections.
  * See [my firmware here](https://github.com/LispEngineer/nucleo-uart/blob/main/STM32F767ZITX_FLASH.ld#L121)
    for how to do this. It requires changing ARM assembly 
    initialization code as well.

* STM32 Pins/Peripherals
  * ADAU1961 Audio Codec
    * I2S, I2C
  * GPIO
  * SD Card
  * MIDI In & Out
  * USB: Programmer, Host


# Programming the KSoloti

## Via USB

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

## Via SWD

* See `Debugging` below
* Just use an ST-LINK and connect the 5 wires for SWD
* Note that the SWD power does *not* power the Ksoloti

To use a Nucleo board for this purpose, see UM1974 Rev 10
page 19 section 6.3.4.

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
  * See the [Ksoloti settings](https://github.com/ksoloti/ksoloti/blob/master/firmware/sdram.c#L62)
    * Note that my STM32CubeIDE IOC editor does not like `Write Recovery Time` of `2`
      and required me to set it to `3` to get rid of the error. (It explains why.)

The SDRAM clock runs at base HCLK speed, divided by
2 or 3 per `SDRAM common clock` setting. The RAM can run
at 84MHz from the 168MHz main clock with a divide-by-2 setting.
(84MHz = 11.9ns)

### SDRAM In Code

The generated code by STM32CubeIDE does *not* do everything necessary.
[More details are here](https://community.st.com/t5/stm32-mcus/how-to-set-up-the-fmc-peripheral-to-interface-with-the-sdram/ta-p/49457)
or watch [Phil's video](https://www.youtube.com/watch?v=h28D4AaPSjg).

In short, you have to send the SDRAM startup sequence manually yourself.
You also have to set the SDRAM refresh interval.

The initialization sequence is described in the
[Micron Datasheet](https://www.micron.com/products/memory/dram-components/sdram/part-catalog/part-detail/mt48lc16m16a2p-6a-it-g) (Revision W dated 5/15)
on page 44. I did not code in the exact wait times,
but it seems to work fine.


# Debugging

It's easier to debug this with ST-LINK.

I'm using the ST-LINK from a NUCLEO-F429ZI board disconnected
from it's local STM32. I soldered on the J6 header for SWD
and ran the 5 wires across from the NUCLEO to the Ksoloti.

Then you can do all the usual fancy stuff for debugging.

## Debug Registers

These aren't really registers, but are built-in peripherals in the
ARM Cortex-M4 CPU.

Reference: PM0214 Rev 10

See: *System Control Block*

See also this
[ARM site here](https://developer.arm.com/documentation/ddi0439/b/Debug/About-debug/Debug-register-summary)
as a `Debug Fault Status Register`. That is documented
on the [ARM site here](https://developer.arm.com/documentation/ddi0403/d/Debug-Architecture/ARMv7-M-Debug/Debug-register-support-in-the-SCS/Debug-Fault-Status-Register--DFSR?lang=en).



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
* [ST FMC SDRAM Setup Guide](https://community.st.com/t5/stm32-mcus/how-to-set-up-the-fmc-peripheral-to-interface-with-the-sdram/ta-p/49457)