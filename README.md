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


# STM32 Coding


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
  * RAM used in schematic: [Alliance Memory AS4C16M16SA-6TIN](https://www.mouser.com/ProductDetail/Alliance-Memory/AS4C16M16SA-6TIN?qs=os3kWr9Y3%252BBqt2XJGYJjwg%3D%3D&utm_id=21520790617&gad_source=1)