#include "msp430f5438.h"

volatile long temp_adc = 0;
// ========================================================
// Configuration Temperature:
//	* ADC12CTL0:
//		+ ADC12SHT0_8		: sampling period = 2^8 cycles
//		+ ADC12REFON 	= 1	: reference on 
//		+ ADC12ON 		= 1	: ADC12 on
//		+ ADC12ENC		= 1	: ADC enable
//	* ADC12CTL1:
//		+ ADC12SHP 		= 1	: sample-and-hold pulse-mode select
//							  SAMPCON signal is source from the sampling timer
//	* ADC12MCTL0 (Conversion Memory Control Register)
//		+ ADC12SREF_1	= 1	: VR+ = VREF+ and VR- = AVSS
//		+ ADC12INCH_10		: Temparature diode
//	* ADC12IE (Interrupt Enable Register)		
// ========================================================
void adc12_initialize()
{
	ADC12CTL0 = ADC12SHT0_8 + ADC12REFON + ADC12ON;
	                                        	// Internal ref = 1.5V
	ADC12CTL1 = ADC12SHP;                     	// enable sample timer
	ADC12MCTL0 = ADC12SREF_1 + ADC12INCH_10;  	// ADC i/p ch A10 = temp sense i/p
	ADC12IE = 0x001;                          	// ADC_IFG upon conv result-ADCMEMO
	__delay_cycles(37);                       	// 35us delay to allow Ref to settle
	                                        	// based on default DCO frequency.
	                                        	// See Datasheet for typical settle time.
	ADC12CTL0 |= ADC12ENC;
	
	// End ADC12 Init -> Led 1
	P1OUT = P1OUT & 0xfd;
}

// =========================================================
// Start ADC
// =========================================================
void adc12_start()
{
	ADC12CTL0 |= ADC12SC;                   // Sampling and conversion start
}

// ========================================================
// Export temp to other files
// ========================================================
long adc12_temperature()
{
	return temp_adc;
}

// ========================================================
//
// ========================================================
#pragma vector=ADC12_VECTOR
__interrupt void ADC12ISR (void)
{
  switch(__even_in_range(ADC12IV,34))
  {
  case  0: break;                           // Vector  0:  No interrupt
  case  2: break;                           // Vector  2:  ADC overflow
  case  4: break;                           // Vector  4:  ADC timing overflow
  case  6:                                  // Vector  6:  ADC12IFG0
    temp_adc = ADC12MEM0;                // Move results, IFG is cleared
    __bic_SR_register_on_exit(LPM0_bits);   // Exit active CPU
    break;
  case  8: break;                           // Vector  8:  ADC12IFG1
  case 10: break;                           // Vector 10:  ADC12IFG2
  case 12: break;                           // Vector 12:  ADC12IFG3
  case 14: break;                           // Vector 14:  ADC12IFG4
  case 16: break;                           // Vector 16:  ADC12IFG5
  case 18: break;                           // Vector 18:  ADC12IFG6
  case 20: break;                           // Vector 20:  ADC12IFG7
  case 22: break;                           // Vector 22:  ADC12IFG8
  case 24: break;                           // Vector 24:  ADC12IFG9
  case 26: break;                           // Vector 26:  ADC12IFG10
  case 28: break;                           // Vector 28:  ADC12IFG11
  case 30: break;                           // Vector 30:  ADC12IFG12
  case 32: break;                           // Vector 32:  ADC12IFG13
  case 34: break;                           // Vector 34:  ADC12IFG14
  default: break;
  }
}
