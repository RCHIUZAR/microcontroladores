#include <msp430.h> 
#include <stdio.h>

/**
 * main.c
 */
char msg[] = "\33[2J \033[H";
char data[100];
void sendData(char *str);
unsigned int adcVal;
void main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	P6DIR &= ~BIT0;
	P6SEL |= BIT0;
	// serial communication
	P4SEL |= BIT5 + BIT4;
	UCA1CTL1 |= UCSWRST;
	UCA1CTL1 |= UCSSEL_2;
	UCA1BR0 = 9;
	UCA1BR1 = 0;
	UCA1MCTL = UCBRS_1 + UCBRF_0;
	UCA1CTL1 &= ~UCSWRST;

	//adc configuration
	REFCTL0 &= ~REFMSTR;
	ADC12CTL0 |= ADC12ON + ADC12SHT02 + ADC12REFON;
	ADC12CTL0 &= ~ADC12REF2_5V; //Vref = 1.5 v
	ADC12CTL1 |= ADC12SHP;
	ADC12MCTL0 |= ADC12SREF_1;
	__delay_cycles(30000);
	ADC12CTL0 |=ADC12ENC;
	while(1){
	   ADC12CTL0 |= ADC12SC;
	      while(!(ADC12IFG & BIT0));
	      adcVal = ADC12MEM0;
	      sendData(msg);
	      sprintf(data,"V = %d",adcVal);
	      sendData(data);
	      __delay_cycles(500000);
	      __no_operation();
	}
	
}
void sendData(char *str){
    while(*str != 0){
        while(!(UCA1IFG & UCTXIFG));
        UCA1TXBUF = *str++;
    }
}
