#include <string.h>
#include "power.h"
#include "msp430f5438.h"

unsigned char temp_uart;
unsigned char new_command = 0;
extern unsigned char state;
// ================================================================
// Configuration:
//	* UCAxCTL0
//		+ UCPEN 	= 0	: parity disabled
//		+ UCPAR 	= 0	: odd parity
//		+ UCMSB 	= 0	: MSB first
//		+ UC7BIT 	= 0	: 8-bit data
//		+ UCSBP 	= 0	: 1 stop bit
//		+ UCMODEx	= 0	: UART mode
//		+ UCSYNC 	= 0	: Asynchronous mode
//	* UCAxCTL1
//		+ Select ACLK, baud rate 9600
//	* Interrupt Receiver
// ================================================================
void uartA_initialize()
{
	P2DIR = P2DIR | 0x02;
	P2OUT = P2OUT | 0x02;							  // Turn on the enable pin of UART power module
	
	P3SEL = 0x30;                             // P3.4,5 = USCI_A0 TXD/RXD
	// UCSWRST = SW + RST = Software Reset Enable
	UCA0CTL1 |= UCSWRST;                      // **Put state machine in reset**
	UCA0CTL1 |= UCSSEL_1;                     // CLK = ACLK
	// USCI_Ax Baud Rate Control 0
	UCA0BR0 = 0x03;                           // 32kHz/9600=3.41 (see User's Guide)
	// USCI_Ax Baud Rate Control 1
	UCA0BR1 = 0x00;                           
	UCA0MCTL = UCBRS_3 + UCBRF_0;             // Modulation UCBRSx=3, UCBRFx=0
	UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
	UCA0IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt
	
	// End UART_A Init -> Led 3
	P1OUT = P1OUT & 0xf7;
}

// ================================================================
//
// ================================================================
void uartA_transmit (char *string)
{
	unsigned char i = 0;
	unsigned char length = strlen(string);
	// Xuat ki tu ra UART
	do
	{
		while (!(UCA0IFG & UCTXIFG));
		UCA0TXBUF = string[i++];
	}
	while (i != length);
}

void uartA_receive (char *string)
{
	unsigned char i = 0;

	while (1)
	{
		sleep(0);
		if (new_command)
		{
			if (temp_uart != '.')
				string[i++] = temp_uart;
			else				
			{
				new_command = 0;
				string[i] = 0x00;		// NULL: ket thuc chuoi
				break;
			}
		}
	}		
}

// ================================================================
// Echo back RXed character, confirm TX buffer is ready first
// ================================================================
#pragma vector = USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
{
	switch (__even_in_range(UCA0IV, 4))
	{
	  	case 0:									// Vector 0 - no interrupt
	  		break;                             	
	  	case 2:                                	// Vector 2 - RXIFG
	    	//while (!(UCA0IFG & UCTXIFG));       // USCI_A0 TX buffer ready?
	    		//UCA0TXBUF = UCA0RXBUF;          // TX -> RXed character
	    	temp_uart = UCA0RXBUF;
	    	new_command = 1;
	    	state = 0x01;
	    	__bic_SR_register_on_exit(LPM0_bits);   // Exit active CPU
	    	break;
	  	case 4:									// Vector 4 - TXIFG
	  		break;                             	
	  	default: 
	  		break;
	}
}
