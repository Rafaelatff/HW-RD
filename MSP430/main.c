//CCS version 10.2.0.00009
#include <msp430.h>
#include <string.h> //to use strlen() function
#include <assert.h> // to use asset function
#include <stdio.h>


int valordec;

char message = 0;
int escreve_RTC = 0;

//RTC
#define SLAVE_ADDR_RTC  0x68 //0110 1000 0x68
char Data_In = 0;

int Data_Cnt = 0;
//size 20, goes from Packet[0] until Packet[19], being [0] reg addred + [0x00 to 0x12] data
char Packet[20] = {0x00, //  Register address [0x00]
                   0x00, // Seconds   [0x00]
                   0x15, // Minutes   [0x01]
                   0x03, // Hour      [0x02]
                   0x02, // DOW       [0x03]
                   0x16, // Day       [0x04]
                   0x09, // Month     [0x05]
                   0x92, // Year      [0x06]
                   0x00, // A1S       [0x07]
                   0x00, // A1M       [0x08]
                   0x00, // A1H       [0x09]
                   0x00, // A1D       [0x0A]
                   0x00, // A2M       [0x0B]
                   0x00, // A2H       [0x0C]
                   0x00, // A2D       [0x0D]
                   0b00010100, //Ctl  [0x0E]
                   0b00010100, //C/S  [0x0F]
                   0x00, // Agin Off  [0x10]
                   0x00, // MSB Temp  [0x11]
                   0x00};// LSB Temp  [0x12]
int x = 19;
int y=0;
char Data_In_Array[20];

// *******************************************DS3231*****************************************
/*-------------------------------------Timekeeper Registers---------------------------------
 *|  Address | Bit 7 | Bit 6 | Bit 5 | Bit 4 | Bit 3 | Bit 2 | Bit 1 | Bit 0 | Function | Range |
 *|    0x00  |   0   |      10 Seconds       |             Seconds           | Seconds  | 00-59 |
 *|    0x01  |   0   |      10 Minutes       |             Minutes           | Minutes  | 00-59 |
 *|    0x02  |   0   | 12/24 | H/AmPm|  10H  |              Hours            |  Hours   | 1-12 + AMn/PM or 00 -23
 *|    0x03  |   0   |   0   |   0   |   0   |   0   |     Day (DOW)         |   Day    | 1-7   |
 *|    0x04  |   0   |   0   |     10 Date   |               Date            |  Date    | 01-31 |
 *|    0x05  | Cent  |   0   |   0   |  10M  |              Month            | Mont/Cent| 01-12 + Cent
 *|    0x06  |            10 Year            |               Year            |  Year    | 00-99 |
 *|    0x07  | A1M1  |      10 Seconds       |             Seconds           | alarm1 S | 00-59 |
 *|    0x08  | A1M2  |      10 Minutes       |             Minutes           | alarm1 M | 00-59 |
 *|    0x09  | A1M3  | 12/24 | H/AmPm|  10H  |              Hours            | alarm1 H |
 *|    0x0A  |
 *|    0x0B  |
 *|    0x0C  |
 *|    0x0D  |
 *|    0x0E  |EOSCn | BBSQW | CONV  |  RS2   |  RS1  | INTCN |  A2IE |  A1IE |  Control |
 *|    0x0F  | OSF  |   0   |   0   |   0    | EN32k |  BSY  |  A2F  |  A1F  | Cont/Stat|
 *|    0x10  | SIGN | DATA  | DATA  |  DATA  | DATA  | DATA  | DATA  | DATA  | Agin Off |
 *|    0x11  | SIGN | DATA  | DATA  |  DATA  | DATA  | DATA  | DATA  | DATA  | MSB Temp |
 *|    0x12  | DATA | DATA  |   0   |   0    |   0   |   0   |   0   |   0   | LSB Temp |                                                       | RAM 56x8 |
 *------------------------------------------------------------------------------------------------
 *    //On the first application of power (VCC) or when a valid I2C
      //address is written to the part (VBAT), the time and date
      //registers are reset to 01/01/00 01 00:00:00 (DD/MM/YY DOW HH:MM:SS).
 *------------------------------------------------------------------------------------------------
     //On an I2C START or address pointer incrementing to location 00h, the current time
     //is transferred to a second set of registers. The
     //time information is read from these secondary registers
 */


int ACLK_clock;

void start_I2C_B1(void);
void SetDateHour(void);
int BCDtoDecimal(int data);
int DecimalToBCD(int data); //not needed!
void delay_ms(int time);
void start_clock_sys(void);

int main(void)
{
     WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    // Set LED1 for test routines
    // Set P1.0 as output direction [J7 needs to be connected]
    // LED White: Port P1 / Pin 0
    P1SEL0 &= ~BIT0;
    P1SEL1 &= ~BIT0;
    P1DIR |= BIT0; // Set LED1 as Output
    P1OUT &= ~BIT0; // turn off LED1

    PM5CTL0 &= ~LOCKLPM5;       //to apply the GPIO configurations disable the LOCKLPM5 bit in PM5CTL0
    //unlock the GPIO high-impedance mode by resetting the LOCKLPM5 bit

    start_I2C_B1();

    P1OUT |= BIT0; // LED1

    SetDateHour(); // send Packet[] to RTC
    //P1OUT &= ~BIT0; // LED1


    while (1){
        // RECEBER OS 20 BYTES DO RTC E ALOCAR EM UM ARRAY
        //char array_char[20];  //tamanho 20, vai de array[0] até array [19]


        //-- Send Data from Master with initial register address 0x00
        // it may not be needed

        /*escreve_RTC =1;

        UCB1I2CSA = SLAVE_ADDR_RTC;
        UCB1CTLW0 |= UCTR;      // Put into TX mode
        UCB1CTLW0 |= UCTXSTT;     // Generate START
        //UCB1CTLW0 |= UCTR | UCTXSTT; // To put in RX mode and also generate START
            while ((UCB1IFG & UCSTPIFG)==0){}  //UCSTPIFG - STOP condition interrupt flag
        UCB1IFG &= ~UCSTPIFG;       // clear the stop flag
        */

        //-- Receive Data from slave with a READ message

        escreve_RTC = 0;
        UCB1TBCNT = 1;
        UCB1CTLW0 &= ~UCTR;      // Put into RX mode
        UCB1I2CSA = SLAVE_ADDR_RTC;
        UCB1CTLW0 |= UCTXSTT;     // Generate START
        while ((UCB1IFG & UCSTPIFG)==0){}
        //while(!(UCB1IFG & UCTXIFG0));//Internet example
        UCB1IFG &= ~UCSTPIFG;       // clear the stop flag
        //while(UCB1CTLW0 & UCTXSTP);  //Internet example: wait for stop
        // pelo que eu entendi, UCTXSTP eu tbm posso setar
        // ou posso esperar receber (0 = stop)

        y=0;

        for (x=0;x==19;x++){
               Data_In = Data_In_Array[x];
               valordec = BCDtoDecimal(Data_In);

        }

        __delay_cycles(78000);
        P1OUT ^= BIT0; // LED0
    }
}
//-------------ISR-----------------
#pragma vector = EUSCI_B1_VECTOR
__interrupt void EUSCI_B1_I2C_ISR(void){
    switch(UCB1IV){
    case 0x16:  // ID 16: RXIFG0 according to MSP430FR4xx and MSP430FR2xx family user's guide
        //Data_In = UCB1RXBUF;    // Read data from RX buffer
        P1OUT &= ~BIT0; // LED1
        for (x=19;x==0;x--){
            while (!(UCB1IFG & UCRXIFG0)); //CHECAR SE EU COLOQUEI REG CERTO E PROGRAMA NÃO VAI TRAVAR AQUI
            //UCRXIFG0 = 0x1 = 0b0001  /* UCBxIFG Control Bits - UCRXIFG0: eUSCI_B receive interrupt flag 0 */
            Data_In_Array[x] = UCB1RXBUF;
        }
        //while(UCB1CTLW0 & UCTXSTP); PELO QUE EU ENTENDI CHECA PELO STOP
        break;
    case 0x18:  // ID 18: TXIFG0 according to MSP430FR4xx and MSP430FR2xx family user's guide
        if(escreve_RTC==1){
                    UCB1I2CSA == SLAVE_ADDR_RTC;
                    if (Data_Cnt == (sizeof(Packet)-1)){
                        UCB1TXBUF = Packet[Data_Cnt];
                        Data_Cnt = 0;
                    } else {
                        UCB1TXBUF = Packet[Data_Cnt];
                        Data_Cnt++;
                    }
                    //UCB1TXBUF = RTCmessage; //create a new for RTCregister
                }
                else if(escreve_RTC==0){
                    UCB1TXBUF = message; //other slave in i2c
                }
                escreve_RTC=0;
        break;
    default:
        break;
    }
}

// Timer A0 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_A0_VECTOR))) Timer_A (void)
#else
#error Compiler not supported!
#endif
{
    ACLK_clock++;

    //P1OUT ^= BIT0;
    TA0CCR0 +=1; // Add Offset to TACCR0
}

void SetDateHour() //
{
    escreve_RTC =1;

    UCB1CTLW0 |= UCTR;      // Put into TX mode
    UCB1I2CSA = SLAVE_ADDR_RTC;
    UCB1CTLW0 |= UCTXSTT;     // Generate START (this flag is cleared as soon as the complete address is sent)

    //TODO1: Check, with oscilloscope, is Packet is really sent! yes[ ] / no [ ]

    //TODO2: Select mask and go to 'Open declaration' (F3) to check mask for control bits on 'msp430fr2476.h' file
        //UCSTPIFG = 0x0008 = 0b1000  /* UCBxIFG Control Bits - UCSTPIFG: STOP condition interrupt flag */
        //TODO3 = This may be happening because Packet has no size declared 'char Packet[];'. Investigate.

    //while ((UCB1IFG & UCSTPIFG)==0){}  //Stays in loop until R/W a stop condction (end of transmition);
    //UCB1IFG &= ~UCSTPIFG;       // clear the stop flag
    //P1OUT &= ~BIT0; // --> Stop condction isn't happening
}

int BCDtoDecimal (int data){
 // Convert from BCD to Decimal
 // Step 1: Break the BCD into nibbles: 1001 0001 0001 · Step 2: Convert each nibble into decimal digits.
    assert(((data & 0xF0) >> 4) < 10);  // Test if more significant nybble is valid
    assert((data & 0x0F) < 10);         // Test if less significant nybble is valid
    //not expected to receive hexadecimals greater than 9
    int dec = ((data & 0xF0) >> 4) * 10 + (data & 0x0F);
    return dec;
 // x % y   module operator: x mod y: produces the remainder when x is divided by y.
 // 59 mod 16 = 11 -> 59 = (16×3) + 11
    //mod garantes  that value wont be bigger
    // (((data/1000)<<8) | ((data/10)<<4) | (Decimal % 10));
}

int DecimalToBCD (int data){
 // Convert from Decimal to BCD
 // x % y   module operator: x mod y: produces the remainder when x is divided by y.
 // 59 mod 10 = 9 -> 59 = (10×5) + 9
    int BCD = (((data/10)<<4) | (data % 10));
    return BCD;
    //Example decimal 52 -> 52/10 = 5 then 5 << 4 = 80 (00101 0000)
    //then 80 | 2 (0101 0010) = 0x52
}


void start_I2C_B1(void){

    /*   eUSCI_B Registers
     *      UCBxCTLW0 (eUSCI_Bx Control Register 0)
     *      UCBxBRW R (eUSCI_Bx Bit Rate Control Register 1)
     *      UCBxSTATW (eUSCI_Bx Status Register)
     *      UCBxRXBUF (eUSCI_Bx Receive Buffer Register)
     *      UCBxTXBUF (eUSCI_Bx Transmit Buffer Register)
     *      UCBxIE    (eUSCI_Bx Interrupt Enable Register)
     *      UCBxIFG   (eUSCI_Bx Interrupt Flag Register) --> (0 = No interrupt pending / 1 = Interrupt pending)
     *          UCRXIFG0  0x01   - eUSCI_B receive interrupt flag 0 - is set when UCBxRXBUF has received a complete character (M/S mode)
     *          UCTXIFG0  0x02   - eUSCI_B transmit interrupt flag 0  - UCTXIFG0 is set when UCBxTXBUF is empty (M/S mode)
     *          UCSTTIFG  0x04   - START condition interrupt flag
     *          UCSTPIFG  0x08   - STOP condition interrupt flag
     *          UCALIFG   0x10   - Arbitration lost interrupt flag
     *          UCNACKIFG 0x20   - Not-acknowledge received interrupt flag
     *          UCBCNTIFG 0x40   - Byte counter interrupt flag - Is set when byte counter threshold value UCBxTBCNT is reached (M/S mode)
     *          UCCLTOIFG 0x80   - Clock low time-out interrupt flag
     *          UCRXIFG1  0x100  - Receive interrupt flag 1
     *          UCTXIFG1  0x200  - eUSCI_B transmit interrupt flag 1
     *          UCRXIFG2  0x400  - Receive interrupt flag 2
     *          UCTXIFG2  0x800  - eUSCI_B transmit interrupt flag 2
     *          UCRXIFG3  0x1000 - Receive interrupt flag 3
     *          UCTXIFG3  0x2000 - eUSCI_B transmit interrupt flag 3
     *          UCBIT9IFG 0x4000 - Bit position 9 interrupt flag
     */
        // Reading UCxRXBUF resets the receive error bits and UCRXIFG.
        // Writing to the transmit data buffer clears UCTXIFG
        // (Bit 1) UCTXIFG -> Transmit interrupt flag. UCTXIFG is set when UCBxTXBUF empty.
            // 0 = no interrupt pending / 1 = interrupt pending
        // (Bit 0) UCRXIFG -> Receive interrupt flag. UCRXIFG is set when UCBxRXBUF has received a
            // complete character.0 = no interrupt pending / 1 = interrupt pending

    // always put the eUSCI module into its reset state
    UCB1CTLW0 = UCSWRST;                      //Put B1 in SW reset

    //configure the eUSCI module as a synchronous I2C peripheral, master mode, using the SMCLK source
    UCB1CTLW0 |= UCSSEL__SMCLK; // or UCSSEL_3 = SMCLK,
    // The SMCLK is configured for 10MHz, so we must divide it down to meet the 100kHz I2C clock requirement
    UCB1BRW = 10; // fSCL = SMCLK/10 = ~100kHz // set prescalar to 10

    UCB1CTLW0 |= UCMODE_3; //put into I2C mode (others options configure the eUSCI as SPI communication)
    UCB1CTLW0 |= UCMST; //set as master
    //UCB1CTLW0 |= UCTR; //put into Tx mode (to write) --> Removed, since sometimes code also READ

    UCB1CTLW1 |= UCASTP_2; //Auto STOP mode -> UCASTP_2 = 0x8 = 0b1000 = 10 (bits 2 and 3 of UCBxCTLW1 register [0to15])
    UCB1TBCNT = sizeof(Packet); // transfer buffer count = size of Packet array

    //Config PIN SEL  - for UCB0SDA e UCB0SCL PxSELx = 01
    //SDA pin 14 (P1.2) - UCB0SDA
    //SCL pin 15 (P1.3) - UCB0SCL
    //P1SEL0 |= BIT2 | BIT3;
    //P1SEL1 &= ~(BIT2 | BIT3);

    //To use UCB1SDA e UCB1SCL
    //SDA pin 38 (P3.2) - UCB1SDA
    //SCL pin 39 (P3.6) - UCB1SCL
    P3SEL0 |= BIT2 | BIT6;
    P3SEL1 &= ~(BIT2 | BIT6);

    //Obs.: To set an interrupt for I2C (in case MSP is slave), set an ordinary GPIO;

    PM5CTL0 &= ~LOCKLPM5;  // to apply the GPIO configurations disable the LOCKLPM5 bit in PM5CTL0
    UCB1CTLW0 &= ~UCSWRST;  //Take B1 out of SW RST

    //Enable B1 TX0 and RX0 IRQ
    UCB1IE |= UCTXIE0;  // local enable for TX0
    UCB1IE |= UCRXIE0;  // local enable for RX0
    __enable_interrupt(); // enable maskables
}

void start_clock_sys(void){
    //Clock configured as counter mode
    //Timer configuration: 16 bits = 65.536 - 1 (0 até 65535/ 0x0 até 0xFFFF)
    //Timer A Instance 0 (TA0) -> Compare/Capture Register 0 (CCR0)
    //1 - To set Enable bit:
       TA0CCTL0 |= CCIE;
    //Now the MSP430 will interrupt once the timer reaches TA0CCR0.
    //2 - Choose value for TA0CCR0:
       TA0CCR0 = 1;
    //3 - Choose clock source for Timer_A0 and also choose Continuous Mode:
    //ACLK = 32678 Hz
       TA0CTL |= TASSEL__ACLK | MC__CONTINUOUS;
    //It will enter ISR of timer A each 1/32678 seconds -> 30.6 us
    //1ms = 32678 clocks
    //4 - Enable global interrupts in the MSP430 so that we can perform an action once TA0R = TA0CCR0.
       __bis_SR_register(GIE);
}

void delay_ms(int time){
    //Time isn't very accurate, but enought for applications
    int count;
    for (count=0;count<=time;count++){
        ACLK_clock=0;
        while(ACLK_clock<=30);
    }
    //place following lines in main to test
    /* while(1){
        P5OUT ^= BIT3; // LED0
        delay_ms(10);
    }*/
}
