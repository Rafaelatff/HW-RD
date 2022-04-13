# HW-RD
TRACTIAN challenge - Hardware R&amp;D

*Goal: Develop a full wireless communication system. It needs to send a 500 kB file from a 100 m open-air gap, using at least one battery powered device.*

## Step 1: Scketch it up! 

**1.1 - Let's find the main wireless component**

The preference for manufacturer goes to Texas Instruments due its good variety of components, good documentation, tools for simulation and development. 

According to: https://training.ti.com/range-test-cc1120-sub-1-ghz-performance-line, 100 m air-gap is reachable with a Sub-1GHz system.

Texas instruments shows 36 parts classified as Sub-1GHz products. All 36 were checked, but only active part numbers and with at least one place with available stock entered the comparing table below. Preference for available SPI to add external memory. There were no operating temperature requirements for the wireless communication system, nor minimum of peripheral required.  So none of those were considered componente elimination factors. It is a battery powered device, so, it must be low power.
The table below compares TI inventory and also Mouser and Digikey (two biggest global electronic components purveyor).

| Part Number | TI Inventory | Price | Mouser | Price | Digikey | Price |
|------|------|------|------|------|------|------|
| CC1311P3 | 2418 | $3.125 | Non-Stocked | $4.8 (x2500) | 0 | $8.92 |
| CC1312R7 | 2200 | $3.795 | Non-Stocked | $5.83 (x2500) | - | - |
| CC1352P | 0 | $3.917 | CC1352P1F3RGZT -107 CC1352P1F3RGZR – 5061 (On order) | $11.86 $9.89 | 0 | $9.4 |
| CC1312R | 0 | $3.341 | CC1312R1F3RGZR (7806 Expected 10/3/2022) | $8.43 | 0 | $8.02 |
| CC430F5143 | 376 | $2.34 | Non-Stocked | $5.05 (x250) | CC430F5143IRGZT - 250 | $6.74 |
| **CC1350** | **5167** | **$3.446** | **CC1350F128RSMR – 5148 CC1350F128RSMT - 257** | **$8.96 $10.7** | **CC1350F128RSMR – 841 CC1350F128RHBR - 644** | **$9.37 $8.27** |
| **CC1310** | **19084** | **$2.052** | **CC1310F32RSMR – 11472 CC1310F32RGZR - 1318** | **$5.76 $6.05** | **CC1310F128RGZR – 2212 CC1310F32RGZT - 214** | **$6.89 $6.15** |
| CC1200 | 7990 | $1.928 | 0 | $5.42 | 0 | $6.48 |
| CC1100E | 0 | $1.139 | CC1100ERGPT - 57 | $3.84 | CC1100ERGPT - 149 | $3.83 |

Note : research done in April, 11th. 
https://www.ti.com/wireless-connectivity/products.html 
https://br.mouser.com/
https://www.digikey.com

Now, let's compare Technologies. Note: values from TI site - production details (datasheet may show accurate data and were not considered, since it consumes a lot of time).

| Part Number | Available devkit? | Microcontroller | Peripherals | Memory | Power (active mode/standby mode/transmission mode*) |
|------|------|------|------|------|------|
| CC1311P3 | Not found | 48-MHz Arm Cortex-M4 processor | 1 SPI, 1 UART, 12-bit ADC 8-channel, 2 comparators, 4 timers, I2C, I2S, 26 GPIO | 32 KB RAM + 352 KB flash | 2.63 mA/ 0.7 uA / 65 mA |
| CC1312R7 | LP-CC1312R7 (10 - $69.94) | 48-MHz Arm® Cortex®-M4F processor | 12-bit ADC 8-channel, 2 SPI, 2 UART, 2 comparators, 4 timers, 8-bit DAC, I2C, I2S, Sensor controller, 30 GPIO | 256 KB ROM+ 704 KB flash | 2.63 mA/ 0.8 uA / 24.9 mA |
| CC1352P | LAUNCHXL-CC1352P1 (183 - $66.49)  | 48-MHz Arm Cortex-M4F processor | 2 SPI, 2 UART, 2 comparators, 4 timers, 8-bit DAC, I2S, Sensor controller, 26 GPIO | 256 KB ROM + 352 KB flash | 5.8 mA/ 0.85 uA / 24.9 mA |
| CC1312R | LAUNCHXL-CC1312R1 (85 - $53.19) | 48-MHz Arm Cortex-M4F processor | 2 SPI, 2 UART, 2 comparators, 4 timers, 12-bit ADC, I2S, Sensor controller, 30 GPIO | 80KB SRAM + 256 KB ROM + 352 KB flash | 5.8 mA/ 0.85 uA / 24.9 mA |
| CC430F5143 | Not found | True System-on-Chip (SoC) for Low-Power Wireless Communication Applications / 16-Bit RISC Architecture | I2C, UART,  2x SPI, 2 timers, 10-bit ADC | 4 KB RAM +  32 KB flash | 160 uA / 2 uA/ 15 mA |
| CC1350 | CC1350STKUS (82 - $78.47) LAUNCHXL-CC1350US (22 - $38.57) | 48-MHz Arm® Cortex®-M3 Processor | 1 UART, 12-bit ADC 8-channel, 2 SPI, 2 comparators, 4 timers, I2C, I2S, Sensor controller, GPIO according to package (10, 15 or 30) | 28KB SRAM + 128 KB flash | 5.4 mA/ 0.7 uA/ 13.4 mA |
| CC1310 | LAUNCHXL-CC1310 (122 - $38.57) | 48-MHz Arm® Cortex®-M3 | 1 UART, 12-bit ADC 8-channel, 2 SPI, 2 comparators, 4 timers, I2C, I2S, Sensor controller, GPIO according to package (10, 15 or 30) | 28 KB SRAM + 128 KB flash | 5.4 mA/ 0.7 uA/ 13.4 mA |

Note : To have an available devkit helps development tasks (once production firmware may start together with hardware development and even help to find issues).  Let's not forget of the  time to market..

Links: 
https://training.ti.com/ti-simplelink-sub-1-ghz-wireless-solutions-iot-technical-introduction-level-1
https://www.ti.com/design-resources/embedded-development/wireless-connectivity.html

**1.2 - Let's find the hw peripherals**

**1.2.1 - Power supply**

Function and characteristics: filter and regulate battery voltage; low LDO; low quiescent current; 

**1.2.2 - Secondary low power microcontroller**

Function and characteristics: control and manage secondary tasks; comunicate with wireless microcontroller; low power;

PN: MSP430FR2476 Mnf: Texas Instruments Datasheet: www.ti.com/lit/ds/symlink/msp430fr2476.pdf

**1.2.3 - External memory or microSD card**

Function and characteristics: accomodate data; save logs; SPI communication;

**1.2.4 - External RTC**

Function and characteristics: keeps track of the current time; low power; I2C communication;

PN: DS3231S# Mnf: Maxim Integrated Datasheet: https://datasheets.maximintegrated.com/en/ds/DS3231.pdf

**1.2.5 - Accelerometer**

Function and characteristics: main parameter to be monitored;

**1.2.6 - Battery**

Function and characteristics: 3.6 V; 

## Step 2: It’s layout time

Antenna guidelines: https://www.ti.com/lit/an/swra726/swra726.pdf

## Step 3: Write some code!

