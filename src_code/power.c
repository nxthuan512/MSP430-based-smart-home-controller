#include "msp430f5438.h"

// ===================================================
void sleep (unsigned char mode)
{
	if (mode == 0) 
		__bis_SR_register(LPM0_bits + GIE);
	else if (mode == 1)
		__bis_SR_register(LPM1_bits + GIE);
	else if (mode == 2)
		__bis_SR_register(LPM2_bits + GIE);
	else if (mode == 3)
		__bis_SR_register(LPM3_bits + GIE);
	else
		__bis_SR_register(LPM4_bits + GIE);		
	__no_operation();
}
