#include "msp430f5438.h"

// ==============================================================
// Configuration
//	* TA1CTL:
//		+ TASSEL_1		: select ACLK
//		+ ID_3			: input divider /8
//		+ MC_2			: mode control - continuous
//		+ TAIE			: interrupt enable
// ==============================================================
void timerA_initilize()
{	
	// Clear TimerA
	TA1CTL = TACLR + MC_0;						// clear TAR
	// Config TimerA
	TA1CTL = TASSEL_1 + ID_3 + MC_2 + TAIE;	
	TA1EX0 = TAIDEX_7;	
	
	// End TIMER_A -> Led 3
	P1OUT = P1OUT & 0x0fb;
}	

// ==============================================================
// Timer_A3 Interrupt Vector (TAIV) handler
// ==============================================================
#pragma vector=TIMER1_A1_VECTOR
__interrupt void TIMER1_A1_ISR(void)
{
  switch(__even_in_range(TA1IV,14))
  {
    case  0: break;                          // No interrupt
    case  2: break;                          // CCR1 not used
    case  4: break;                          // CCR2 not used
    case  6: break;                          // reserved
    case  8: break;                          // reserved
    case 10: break;                          // reserved
    case 12: break;                          // reserved
    case 14: P1OUT ^= 0x80;				 	 // overflow - blinking led P1.8
    		 __bic_SR_register_on_exit(LPM0_bits);			 
             break;
    default: break; 
  }
}
