# HW-RD
TRACTIAN challenge - Hardware R&amp;D

*Goal: Develop a full wireless communication system. It needs to send a 500 kB file from a 100 m open-air gap, using at least one battery powered device.*

## Step 1: Scketch up! 

**1.1 - Let's find the main component.**

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
| CC1350 | 35167 | $3.446 | CC1350F128RSMR – 5148 CC1350F128RSMT - 257 | $8.96 $10.7 | CC1350F128RSMR – 841 CC1350F128RHBR - 644 | $9.37 $8.27 |
| CC1310 | 19084 | $2.052 | CC1310F32RSMR – 11472 CC1310F32RGZR - 1318 | $5.76 $6.05 | CC1310F128RGZR – 2212 CC1310F32RGZT - 214 | $6.89 $6.15 |
| CC1200 | 7990 | $1.928 | 0 | $5.42 | 0 | $6.48 |
| CC1100E | 0 | $1.139 | CC1100ERGPT - 57 | $3.84 | CC1100ERGPT - 149 | $3.83 |

Links: 

https://training.ti.com/ti-simplelink-sub-1-ghz-wireless-solutions-iot-technical-introduction-level-1


https://www.ti.com/design-resources/embedded-development/wireless-connectivity.html

