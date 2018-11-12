#ifndef MAIN_H_
#define MAIN_H_

#include <stdio.h>
#include <string.h>
#include "io.h"
#include "wdt.h"
#include "uart.h"
#include "adc.h"
#include "timer.h"
#include "power.h"
#include "sdmmc.h"
#include "fat.h"
#include "rtc.h"

#define STATION_NAME_DEFAULT 	0x04
#define BOARD_NAME_DEFAULT		0x01

extern unsigned int rtc_loop_per_day;

typedef struct 
{ 
   unsigned char	Config_Request;  
   unsigned char 	Station_Name;
   unsigned char	Board_Name;
   unsigned char	Mode;
   unsigned int		Current_Year;
   unsigned char	Current_Month;
   unsigned char	Current_Day;
   unsigned char	Current_Hour;
   unsigned char	Current_Minute;
   unsigned char	Send_Hour;
   unsigned char	Send_Minute;
   unsigned char	Loop_at_Exact_Time_Enable;		// 4 bit dau quy dinh loop, 4 bit sau enable
   unsigned int		Loop_per_Day_Enable;			// 4 bit dau quy dinh loop, 4 bit sau enable
   unsigned char	Temperature_Max;				
   unsigned char	Temperature_Min;	    		
   unsigned char	Humanity_Max;
   unsigned char	Humanity_Min;
   unsigned char    Start;
} CONFIG_t;

void temperature_calculation(long *IntDegC, long *IntDegF);
void numToString(long number, char *string);
void datetime_converter_sdconfig(TIME_t *TIME, unsigned char *date_time_ptr);
void writedata_init(char *DegC, TIME_t *TIME, CONFIG_t *CONFIG, char *write_data_ptr, unsigned char *write_data_ptr_length);
void pre_writedata_init(char *str1, char *str2, unsigned char *str2_pos, unsigned char value);
void config_initialize(char *config_data_ptr, CONFIG_t *CONFIG);

unsigned int loop_per_day();

//void strslipt (unsigned char from, unsigned char to, char *str1, char *str2);

// =========================================================
// Convert Temperature to C/F degree
// =========================================================
void temperature_calculation(long *IntDegC, long *IntDegF)
{
	unsigned long temperature = 0;
	
	temperature = adc12_temperature();
	// Temperature in Celsius
	// ((A10/4096*1500mV) - 894mV)*(1/3.66mV) = (A10/4096*410) - 244
	// = (A10 - 2438) * (410 / 4096)
	*IntDegC = ((temperature - 2438) * 410) / 4096;

	// Temperature in Fahrenheit
	// ((A10/4096*1500mV) - 829mV)*(1/2.033mV) = (A10/4096*738) - 408
	// = (A10 - 2264) * (738 / 4096)
	*IntDegF = ((temperature - 2264) * 738) / 4096;
}

// ==========================================================
//
// ==========================================================
void numToString(long number, char *string)
{
	unsigned char n = 0;
	
	// Chuyen gia tri tu so sang ki tu
	do
	{
		string[n++] = (number % 10) + 0x30;
		number = number / 10; 
	}
	while (number != 0);
	string[n] = 0x00;		// NULL
}

// ========================================================
//
// ========================================================
void datetime_converter_sdconfig(TIME_t *TIME, unsigned char *date_time_ptr)
{
	unsigned int temp;
	
	temp = (TIME->Hour << 11) + (TIME->Minute << 5) + (TIME->Second/2);
	date_time_ptr[0] = (u8)(temp);
	date_time_ptr[1] = (u8)(temp >> 8);
	
	temp = ((TIME->Year - 1980) << 9) + (TIME->Month << 5) + (TIME->Day);
	date_time_ptr[2] = (u8)(temp);
	date_time_ptr[3] = (u8)(temp >> 8);
}

// =======================================================
//
// =======================================================
void pre_writedata_init(char *str1, char *str2, unsigned char *str2_pos, unsigned char value)
{
	unsigned char j;
	unsigned char n = strlen(str1);
	
	for (j = 0; j < n; ++(*str2_pos), ++j)
		str2[*str2_pos] = str1[j];
	str2[(*str2_pos)++] = 0x30;						// '0'
	str2[(*str2_pos)++] = 0x30;						// '0'	
	str2[(*str2_pos)++] = value + 0x30;
	str2[(*str2_pos)++] = 0x20;						// 0x20 = Space
	str2[(*str2_pos)++] = 0x20;						// 0x20 = Space	
} 

// =======================================================
void writedata_init(char *DegC, TIME_t *TIME, CONFIG_t *CONFIG, char *write_data_ptr, unsigned char *write_data_ptr_length)
{
	char string[20];
	char *string_ptr = string;
	
	char str1[] = "Temperature=";
	char str2[] = "Humidity=";
	unsigned char i = 0;
	unsigned char j = 0;
	
	write_data_ptr[i++] = 'G';
	write_data_ptr[i++] = 'D';
	write_data_ptr[i++] = 0x20;						// 0x20 = Space
	write_data_ptr[i++] = 0x20;						// 0x20 = Space
	
	////
	pre_writedata_init("Station ID=", write_data_ptr, &i, CONFIG->Station_Name);
	pre_writedata_init("Board ID=", write_data_ptr, &i, CONFIG->Board_Name);
	//pre_writedata_init("Mode ID=", write_data_ptr, &i, CONFIG->Mode);
	
	////
	numToString(TIME->Year, string_ptr);
	write_data_ptr[i++]  = string[3];
	write_data_ptr[i++]  = string[2];
	write_data_ptr[i++]  = string[1];
	write_data_ptr[i++]  = string[0];
	write_data_ptr[i++]  = 0x3A;						// 0x3A = :
	
	write_data_ptr[i++]  = (TIME->Month / 10) + 0x30;
	write_data_ptr[i++]  = (TIME->Month % 10) + 0x30;
	write_data_ptr[i++]  = 0x3A;						// 0x3A = :	

	write_data_ptr[i++]  = (TIME->Day / 10) + 0x30;
	write_data_ptr[i++]  = (TIME->Day % 10) + 0x30;
	write_data_ptr[i++] = 0x20;						// 0x20 = Space
	write_data_ptr[i++] = 0x20;						// 0x20 = Space
			
	write_data_ptr[i++] = (TIME->Hour / 10) + 0x30;
	write_data_ptr[i++] = (TIME->Hour % 10) + 0x30;
	write_data_ptr[i++] = 0x3A;						// 0x3A = :
	
	write_data_ptr[i++] = (TIME->Minute / 10) + 0x30;
	write_data_ptr[i++] = (TIME->Minute % 10) + 0x30;
	write_data_ptr[i++] = 0x3A;						// 0x3A = :
	
	write_data_ptr[i++] = (TIME->Second / 10) + 0x30;
	write_data_ptr[i++] = (TIME->Second % 10) + 0x30;
	write_data_ptr[i++] = 0x20;						// 0x20 = Space
	write_data_ptr[i++] = 0x20;						// 0x20 = Space

	for (j = 0; j < strlen(str1); ++j, ++i)
		write_data_ptr[i] = str1[j];
	write_data_ptr[i++] = DegC[1]; write_data_ptr[i++] = DegC[0];
	write_data_ptr[i++] = 'o';     write_data_ptr[i++] = 'C';
	write_data_ptr[i++] = 0x20;    write_data_ptr[i++] = 0x20;
	
	for (j = 0; j < strlen(str2); ++j, ++i)
		write_data_ptr[i] = str2[j];
	write_data_ptr[i++] = 0x30; write_data_ptr[i++] = 0x30;
	
	write_data_ptr[i++] = '.';						// ket thuc lenh gui len
		
	write_data_ptr[i++] = 0x0D;						// xuong dong
	write_data_ptr[i++] = 0x0A;
	
	write_data_ptr[i] = 0x00;
	
	*write_data_ptr_length = i;
}

// ==================================================================
void config_initialize(char *config_data_ptr, CONFIG_t *CONFIG)
{
	//SC-512-yymdhm-hmA-BB-CCDD1.
	// 0  -  1: SC: 		set config
	//
	// 3	  : 5:			station name, kieu HEX
	// 4	  : 1:			board name, kieu HEX
	// 5	  : 2:			mode
	//
	// 7 - 8  : yy:			Current year: 1 bytes dau luu yy, 1 bytes cuoi luu yy, yyyy kieu hex
	// 9	  : m:			Current Month
	// 10	  : d:			Current Day
	// 11	  : h:			Current Hour
	// 12	  : m:			Current Minute
	//
	// 14 - 15: h:m			h:m, 2 bytes, kieu hex
	// 16	  : A:			Bit thu 4 = 0: ko lap, = 1: lap
	//						Bit thu 0 = 0: ko hd, = 1: hd
	//
	// 18 - 19: BB:		    Bit 15 - 1: quy dinh lap bao nhieu lan 1 ngay, kieu hexa
	//						Bit thu 0 = 0: ko hd, = 1: hd
	//								
	// 21 - 22: CC:			Nhiet do Bit 15 - 8: Gioi han tren, kieu hexa
	//								     7 - 0:  Gioi han duoi, kieu hexa
	// 23 - 24: DD:			Do am	 Bit 15 - 8: Gioi han tren, kieu hexa
	//								     7 - 0:  Gioi han duoi, kieu hexa				
	// 25	  : Start		Bat dau
	
	// Doc ten_tram va ten_board
	CONFIG->Station_Name 	= (unsigned char)(config_data_ptr[3]);
	CONFIG->Board_Name 		= (unsigned char)(config_data_ptr[4]);
	CONFIG->Mode 			= (unsigned char)(config_data_ptr[5]);
	
	//
	CONFIG->Current_Year  	= (unsigned int)((config_data_ptr[7] << 8) + config_data_ptr[8]);  
	CONFIG->Current_Month 	= (unsigned char)(config_data_ptr[9]);
	CONFIG->Current_Day   	= (unsigned char)(config_data_ptr[10]);
	CONFIG->Current_Hour 	= (unsigned char)(config_data_ptr[11]);
	CONFIG->Current_Minute 	= (unsigned char)(config_data_ptr[12]);
	
	//	
	CONFIG->Send_Hour		= (unsigned char)(config_data_ptr[14]);
	CONFIG->Send_Minute		= (unsigned char)(config_data_ptr[15]);
	CONFIG->Loop_at_Exact_Time_Enable	= (unsigned char)(config_data_ptr[16]);
	
	//
	CONFIG->Loop_per_Day_Enable			= (unsigned int)((config_data_ptr[18] << 8) + config_data_ptr[19]);
	
	// 
	CONFIG->Temperature_Max	= (unsigned char)(config_data_ptr[21]);
	CONFIG->Temperature_Min	= (unsigned char)(config_data_ptr[22]);
	CONFIG->Humanity_Max	= (unsigned char)(config_data_ptr[23]);
	CONFIG->Humanity_Min	= (unsigned char)(config_data_ptr[24]);
	
	// 
	CONFIG->Start			= (unsigned char)(config_data_ptr[25]);
	
	// Xac nhan thanh cong
	if ((CONFIG->Station_Name == STATION_NAME_DEFAULT) && (CONFIG->Board_Name == BOARD_NAME_DEFAULT))
		uartA_transmit("OK");
	
	rtc_loop_per_day = (CONFIG->Loop_per_Day_Enable >> 8) * 60;
}
/*
unsigned int loop_per_day()
{
	return (CONFIG.Loop_per_Day_Enable >> 1);	
}
*/
/*
// ==================================================================
void strslipt (unsigned char from, unsigned char to, char *str1, char *str2)
{
	unsigned char i;
	unsigned char j = 0;
	
	strcpy(str2, "");
	
	for (i = from; i <= to; ++i, ++j)
		str2[j] = str1[i];
}
*/

// =======================================================
//
// =======================================================

#endif /*MAIN_H_*/
