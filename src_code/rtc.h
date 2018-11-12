#ifndef RTC_H_
#define RTC_H_

#define u8  unsigned char
#define u16 unsigned int
#define u32	unsigned long int

#define minute_in_day 	1440

typedef struct 
{ 
   u16 	Year;  
   u8 	Month;
   u8	Day;
   u8	Hour;
   u8	Minute;
   u8	Second; 
} TIME_t; 

void rtc_initialize(u16 year, u8 month, u8 day, u8 hour, u8 minute);
void rtc_get_time(TIME_t *TIME);

#endif /*RTC_H_*/
