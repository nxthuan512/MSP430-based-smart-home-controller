#include "msp430f5438.h"
#include "rtc.h"

volatile unsigned long rtc_minute = 0;
extern unsigned int rtc_loop_per_day;
extern unsigned char adc_enable;

// ==========================================================
void rtc_initialize(u16 year, u8 month, u8 day, u8 hour, u8 minute)
{
   	//RTCCTL01 |= RTCTEVIE + RTCRDYIE + RTCBCD + RTCHOLD + RTCMODE;
   	//RTCCTL01 |= RTCBCD + RTCHOLD + RTCMODE;
													// RTC enable, BCD mode, RTC hold
                                            		// enable RTC read ready interrupt
                                            		// enable RTC time event interrupt                                            
   	RTCCTL01 |= RTCTEVIE + RTCRDYIE + RTCHOLD + RTCMODE;
   	RTCYEAR 		= year;	        // Year = 0x2010
  	RTCMON 			= month;        // Month = 0x04 = April
  	RTCDAY 			= day;          // Day = 0x05 = 5th
  	RTCDOW	 		= 0x01;	        // Day of week = 0x01 = Monday
  	RTCHOUR			= hour;
  	RTCMIN 			= minute;       // Minute = 0x32
  	RTCSEC 			= 0x00;			// Seconds = 0x45
  	/*
  	RTCADOWDAY = 0x2;                         // RTC Day of week alarm = 0x2
  	RTCADAY = 0x20;                           // RTC Day Alarm = 0x20
  	RTCAHOUR = 0x10;                          // RTC Hour Alarm
  	RTCAMIN = 0x23;                           // RTC Minute Alarm
  	*/
  	RTCCTL01 &= ~(RTCHOLD);                   		// Start RTC calendar mode
 	
 	// End RTC -> Led 5
	P1OUT = P1OUT & 0x0ef;		
 	 	
  	//__bis_SR_register(LPM3_bits + GIE);       		// Enter LPM3 mode with interrupts enabled
  	__no_operation();
}

void rtc_get_time(TIME_t *TIME)
{
	//unsigned int temp_year_0 = 0;
	//unsigned int temp_year_1 = 0;
	
	// Su dung he thap luc phan
	TIME->Second	= RTCSEC;
    TIME->Minute	= RTCMIN;
    TIME->Hour		= RTCHOUR;
    TIME->Day		= RTCDAY;
    TIME->Month		= RTCMON;
    TIME->Year		= RTCYEAR;
    /*
    // Su dung he thap phan
    TIME->Second	= (TIME->Second >> 4) * 10 + (TIME->Second % 16);
    TIME->Minute	= (TIME->Minute >> 4) * 10 + (TIME->Minute % 16);
    TIME->Hour		= (TIME->Hour >> 4)   * 10 + (TIME->Hour % 16);
    TIME->Day		= (TIME->Day >> 4)    * 10 + (TIME->Day % 16);
    TIME->Month		= (TIME->Month >> 4)  * 10 + (TIME->Month % 16);
    
    temp_year_1		= TIME->Year >> 8;
    temp_year_0		= (u8)(TIME->Year);
    TIME->Year		= (temp_year_1 >> 4) * 1000 + (temp_year_1 % 16) * 100 + (temp_year_0 >> 4) * 10 + (temp_year_0 % 16);
    */
}


// ==================================================================================
#pragma vector=RTC_VECTOR
__interrupt void RTC_ISR(void)
{
  	switch(__even_in_range(RTCIV,16))
	{
    	case RTC_NONE:                          // No interrupts
      		break;
    	case RTC_RTCRDYIFG:                     // RTCRDYIFG
      		//P1OUT = ~RTCSEC;                      // Toggles P1.0 every second
      		break;
    	case RTC_RTCTEVIFG:                     // RTCEVIFG
    		P1OUT = ~P1OUT;
    		if (rtc_minute < (minute_in_day / rtc_loop_per_day))
    			++rtc_minute;
    		else
    		{
    			rtc_minute = 0;
    			adc_enable = 1;
    		}
      		__bic_SR_register_on_exit(LPM0_bits);   // Exit active CPU
      		//__no_operation();                   // Interrupts every minute
      		break;
    	case RTC_RTCAIFG:                       // RTCAIFG
      		break;
    	case RTC_RT0PSIFG:                      // RT0PSIFG
      		break;
    	case RTC_RT1PSIFG:                      // RT1PSIFG
      		break;
    	case 12: break;                         // Reserved
    	case 14: break;                         // Reserved
  		case 16: break;                         // Reserved
    	default: break;
	}
}
