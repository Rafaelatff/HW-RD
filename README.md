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

CC1310 User guide: https://www.ti.com/lit/ug/swcu117i/swcu117i.pdf

CC1310 Datasheet: https://www.ti.com/lit/ds/symlink/cc1310.pdf

**Pin mapping - RSM Package (VQFN-32)**

| Pin name | Pin number | Type | Description | 
|------|------|------|------|
| DCDC_SW | 18 | Power | Output from internal DC/DC |
| DCOUPL | 12 | Power | 1.27-V regulated digital-supply decoupling capacitor |
| DIO_0 | 8 | Digital I/O | GPIO, Sensor Controller, high-drive capability |
| DIO_1 | 9 | Digital I/O | GPIO, Sensor Controller, high-drive capability |
| DIO_2 | 10 | Digital I/O | GPIO, Sensor Controller, high-drive capability |
| DIO_3 | 15 | Digital I/O | GPIO, high-drive capability, JTAG_TDO |
| DIO_4 | 16 | Digital I/O | GPIO, high-drive capability, JTAG_TDI |
| DIO_5 | 22 | Digital or analog I/O | GPIO, Sensor Controller, analog |
| DIO_6 | 23 | Digital or analog I/O | GPIO, Sensor Controller, analog |
| DIO_7 | 24 | Digital or analog I/O | GPIO, Sensor Controller, analog |
| DIO_8 | 25 | Digital or analog I/O | GPIO, Sensor Controller, analog |
| DIO_9 | 26 | Digital or analog I/O | GPIO, Sensor Controller, analog |
| EGP | - | Power | Ground; exposed ground pad |
| JTAG_TMSC | 13 | Digital I/O | JTAG_TMSC |
| JTAG_TCKC | 14 | Digital I/O | JTAG_TCKC |
| RESET_N  | 21 | Digital input | Reset, active low. No internal pullup |
| RF_N | 2 | RF I/O  | Negative RF input signal to LNA during RX Negative RF output signal from PA during TX |
| RF_P | 1 | RF I/O | Positive RF input signal to LNA during RX Positive RF output signal from PA during TX |
| RX_TX | 4 | RF I/O  | Optional bias pin for the RF LNA |
| VDDS | 27 | Power | 1.8-V to 3.8-V main chip supply |
| VDDS2 | 11 | Power | 1.8-V to 3.8-V GPIO supply |
| VDDS_DCDC | 19 | Power | 1.8-V to 3.8-V DC/DC supply |
| VDDR | 28 | Power | 1.7-V to 1.95-V supply, connect to output of internal DC/DC |
| VDDR_RF | 31 | Power | 1.7-V to 1.95-V supply, connect to output of internal DC/DC |
| VSS | 3, 7, 17, 20, 29  | Power | Ground |
| X32K_Q1 | 5 | Analog I/O | 32-kHz crystal oscillator pin 1 |
| X32K_Q2 | 6 | Analog I/O | 32-kHz crystal oscillator pin 2 |
| X24M_N | 30 | Analog I/O | 24-MHz crystal oscillator pin 1 |
| X24M_P | 31 | Analog I/O | 24-MHz crystal oscillator pin 2 |


**1.2 - Let's find the hw peripherals**

**1.2.1 - Power supply**

Function and characteristics: filter and regulate battery voltage; low LDO; low quiescent current; 

**1.2.2 - Secondary low power microcontroller**

Function and characteristics: control and manage secondary tasks; comunicate with wireless microcontroller; low power;

PN: MSP430FR2476 Mnf: Texas Instruments Datasheet: www.ti.com/lit/ds/symlink/msp430fr2476.pdf

Used pins:
| Pin number | Port.Pin | Type/Peripheral | Description | 
|------|------|------|------|
| 1 | DVCC | Power | VDD |
| 2 | SBW | JTAG/SBW | JTAG/SBW |
| 3 | SBW | JTAG/SBW | JTAG/SBW |
| 4 | P1.4 | UCA0SIMO | eUSCI_A0 SPI slave in/master out |
| 5 | P1.5 | UCA0SOMI | eUSCI_A0 SPI slave out/master in |
| 6 | P1.6 | UCA0CLK | eUSCI_A0 SPI clock input/output |
| 7 | P1.7 | UCA0STE |  eUSCI_A0 SPI slave transmit enable |
| 8 | P4.3 | GPIO | SPI_INT |
| 37 | P3.5 | GPIO | I2C_INT |
| 38 | P3.2 | UCB1SDA | eUSCI_B1 I2C data |
| 39 | P3.6 | UCB1SCL | eUSCI_B1 I2C clock |
| 46 | XOUT | Xtal | Crystal out |
| 47 | XIN | Xtal | Crystal in |
| 48 | DVSS | Power | GND |

**1.2.3 - External memory or microSD card**

Function and characteristics: accomodate data; save logs; SPI communication;

**1.2.4 - Accelerometer**

Function and characteristics: main parameter to be monitored;

**1.2.5 - Battery**

Function and characteristics: 3.6 V; 

## Step 2: It’s layout time

For Altium project -> HW-RD/tree/main/Altium%20Project

For 3D project -> HW-RD/tree/main/3D%20files

Antenna guidelines: https://www.ti.com/lit/an/swra726/swra726.pdf

## Step 3: Write some code!

For CC1310 code -> HW-RD/tree/main/CC1310

For MSP430 code -> HW-RD/tree/main/MSP430 
