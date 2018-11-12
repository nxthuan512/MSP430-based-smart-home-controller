#include "msp430f5438.h"

void wdt_initialize()
{
	WDTCTL = WDTPW + WDTHOLD;
	
}
