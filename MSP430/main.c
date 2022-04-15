//CCS version 10.2.0.00009
#include <msp430.h>
#include <string.h> //to use strlen() function
#include <assert.h> // to use asset function
#include <stdio.h>

char message = 0;
char Data_In = 0;


int x = 19;
int y=0;
//size 20, goes from Data_In_Array[0] until Data_In_Array[19], being [0] reg addred + [0x00 to 0x12] data
char Data_In_Array[20];

int ACLK_clock;

void I2C_B1_slave_config(void);
void SPI_A0_config(void);
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

    I2C_B1_slave_config(); // Config MSP as slave

    P1OUT |= BIT0; // LED1

    while (1){

        __delay_cycles(78000);
        P1OUT ^= BIT0; // LED0
    }
}
//-------------ISR-----------------
#pragma vector = EUSCI_B1_VECTOR
__interrupt void EUSCI_B1_I2C_ISR(void){
    switch(UCB1IV){
    case 0x16:  // ID 16: RXIFG0 according to MSP430FR4xx and MSP430FR2xx family user's guide
            //while (!(UCB1IFG & UCRXIFG0));
            Data_In = UCB1RXBUF; // Read data from RX buffer
        break;
    case 0x18:  // ID 18: TXIFG0 according to MSP430FR4xx and MSP430FR2xx family user's guide
            UCB1TXBUF = message; // Send data from TX buffer
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

void I2C_B1_slave_config(void){

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

    // always put the eUSCI module into its reset state
    UCB1CTLW0 = UCSWRST;                      //Put B1 in SW reset

    // slave config
    UCB1CTLW0 |= UCMODE_3; //put into I2C mode (others options configure the eUSCI as SPI communication)
    UCB1CTLW0 |= UCSYNC; // Synchronous mode
    UCB1CTLW0 &= ~UCMST; // set as slave

    UCB1I2COA0 = 0x66; // Set slave address
    UCB1I2COA0 |=UCGCEN; //The slave address defined in I2COA0 is enabled

    UCB1CTLW1 |= UCASTP_2; //Auto STOP mode -> UCASTP_2 = 0x8 = 0b1000 = 10 (bits 2 and 3 of UCBxCTLW1 register [0to15])
    UCB1TBCNT = 1; // transfer buffer count = size of Packet array

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

    //Obs.: To set an interrupt for I2C (in case MSP is slave), set an ordinary GPIO (P3.5);
    P3SEL0 &= ~BIT5;
    P3SEL1 &= ~BIT5;
    P3DIR |= BIT5; // Set as Output
    P3OUT &= ~BIT5; // Stay low

    PM5CTL0 &= ~LOCKLPM5;  // to apply the GPIO configurations disable the LOCKLPM5 bit in PM5CTL0
    UCB1CTLW0 &= ~UCSWRST;  //Take B1 out of SW RST

    //Enable B1 TX0 and RX0 IRQ
    UCB1IE |= UCTXIE0;  // local enable for TX0
    UCB1IE |= UCRXIE0;  // local enable for RX0
    __enable_interrupt(); // enable maskables
}


void SPI_A0_config(void){
    //To be continued;
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
