
#include <stdio.h>
#include <stdlib.h>

#include <xc.h>

#include <xc.h>

// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection Bits (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select (MCLR/VPP pin function is MCLR)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable (Brown-out Reset enabled)
#pragma config CLKOUTEN = ON   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = ON        // Internal/External Switchover Mode (Internal/External Switchover Mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is enabled)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LPBOR = OFF      // Low-Power Brown Out Reset (Low-Power BOR is disabled)
#pragma config LVP = OFF        // Low-Voltage Programming Enable (High-voltage on MCLR/VPP must be used for programming)



/** LED ************************************************************/
#define mLED0			LATC0
#define mGetLED0()		mLED0
#define mLED0_On()		mLED0 = 1;
#define mLED0_Toggle()          mLED0 = !mLED0;

#define mLED1			LATC1
#define mGetLED1()		mLED1
#define mLED1_On()		mLED1 = 1;
#define mLED1_Off()		mLED1 = 0;
#define mLED1_Toggle()          mLED1 = !mLED1;


void init_timer(void);
void init_uart(void);
void sendUartCh(char ch);

int SendFlg=0;

void main(void)
{
    TRISC0=0;
    ANSC0 = 0;

    TRISB7 = 0;

    // Setup Clock
    OSCCONbits.IRCF = 0xf; // HFINT 16MHz
    
    mLED0_On();

    init_timer();
    init_uart();

    ei();

    while(1){
        if(SendFlg){
            di();
            SendFlg=0;
            ei();
            sendUartCh('a');
        }
    }

    
}

void sendUartCh(char ch)
{
    while (!TXIF);
    TXREG = ch;
}



void init_timer(void)
{
    // timer
    OPTION_REGbits.TMR0CS = 0; // Internal clock
    OPTION_REGbits.PSA = 0; // Use Prescaler
    OPTION_REGbits.PS = 7; // 1:256

    TMR0 = 0;	// clear 8bit counter
    INTCONbits.TMR0IF = 0; // timer0 interrupt flag clear
    INTCONbits.TMR0IE = 1; // enable timer0  interrupt
}

void init_uart(void)
{
        // 115200bps
	BAUDCONbits.BRG16 = 1;
	TXSTAbits.SYNC = 0;
	TXSTAbits.BRGH = 1;
	SPBRGL = 34;

	RCSTAbits.SPEN = 1;

	TXSTAbits.TXEN = 1;

	PIE1bits.RCIE = 1; // Allow Receive Interrupt
	INTCONbits.PEIE = 1; // allow perepheral interrupt

	RCSTAbits.CREN = 1;
}


void init_clc(void)
{
    CLC3CON = 0;


    CLC3GLS0 = 0x02;
    CLC3GLS1 = 0X40;
    CLC3GLS2 = 0X00;
    CLC3GLS3 = 0X00;
    CLC3SEL0 = 0X00;
    CLC3SEL1 = 0X04;
    CLC3POL  = 0X00;
    CLC3CON  = 0XC0; //LC3OE=1,LC3EN=1;




}



void interrupt int_main(void)
{
    static unsigned int count=0;

    if(TMR0IE && TMR0IF){
        TMR0IF = 0;
        count++;

        if((count%3)==0){
            SendFlg=1;
        }

        if(count>=31){
            count = 0;
            mLED0_Toggle();
        }

    }


}






