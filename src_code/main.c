#include "main.h"

unsigned char state = 0x00;
unsigned int rtc_loop_per_day;
unsigned char adc_enable = 0x1;
// ========================================================
void main()
{
	long IntDegF;
	long IntDegC;
	unsigned long cardSize = 0;
		
	unsigned char buffer[512];
	unsigned char *buffer_ptr = buffer;
	
	unsigned char date_time[4];
	unsigned char *date_time_ptr = date_time;
	
	char buffer_data[128];
	char *write_data_ptr = buffer_data;	
	unsigned char write_data_ptr_length = 0;
	unsigned int write_pos = 0;
	
	char StrDegC[4];
	char *StrDegC_ptr = StrDegC;

	char config_data[32];
	char *config_data_ptr = config_data;
	
	u16 year;
	u8 month, day, hour, minute;
	
	FAT16_t FAT;
	FILE_t FILE_0;
	TIME_t TIME;	
	CONFIG_t CONFIG;
// --------- ----------------- //

	while (1)
	{		
		switch (state)
		{
			case 0x00: 			// khoi tao cac components
				wdt_initialize();	
				port1_initialize();
				adc12_initialize();
				//timerA_initilize();
				uartA_initialize();
				// MMC/SD Card
				sdmmc_initilize(&cardSize);
				init_fat(&FAT, buffer_ptr);
				
				++state;
				break;
			
			case 0x01: 			// doi cau hinh tu GSM
				uartA_receive(config_data_ptr);
				
				// Bit 1 = 0:	SET
				//		 = 1: 	GET
				// Bit 0 = 0:  	COMMAND
				//		 = 1: 	DATA
				// => 	SET COMMAND: SC: 00
				//		SET DATA:	 SD: 01
				//		GET COMMAND: GC: 10
				//		GET DATA:	 GD: 11 
				CONFIG.Config_Request = 0x00;
				if (config_data_ptr[0] == 'G')	CONFIG.Config_Request |= 0x02;
				if (config_data_ptr[1] == 'D')	CONFIG.Config_Request |= 0x01;
				
				// SC
				if (CONFIG.Config_Request == 0x00)
				{
					config_initialize(config_data_ptr, &CONFIG);
					year 	= CONFIG.Current_Year;				
					month 	= CONFIG.Current_Month;
					day 	= CONFIG.Current_Day;
					hour 	= CONFIG.Current_Hour;
					minute 	= CONFIG.Current_Minute;
					rtc_initialize(year, month, day, hour, minute);
					port1_clear();
					
					state = 0x02;	
				}
				// GD
				else if (CONFIG.Config_Request == 0x03)
					state = 0x05;
				break;

			case 0x02:
				//rtc_initialize(2011, 12, 01, 18, 30);
				// tao file trong sdcard
				rtc_get_time(&TIME);
				datetime_converter_sdconfig(&TIME, date_time_ptr);				
				open_file(&FILE_0, &FAT, buffer_ptr, "FETEL.txt", date_time_ptr, 0, 2);
				write_pos = 0x0;
				
				state = 0x03;
				break;
				
			case 0x03:			// thu nhan nhiet do
				adc12_start();
				sleep(0);
				// doc nhiet do
				temperature_calculation(&IntDegC, &IntDegF);
				// chuyen tu so sang chuoi
				numToString(IntDegC, StrDegC_ptr);
				
				adc_enable = 0x0;
				
				state = 0x04;
				//state = 0x10;
				break;
				
			case 0x04:			// thay doi file
				rtc_get_time(&TIME);
				datetime_converter_sdconfig(&TIME, date_time_ptr);
				writedata_init(StrDegC_ptr, &TIME, &CONFIG, write_data_ptr, &write_data_ptr_length);
				write_file( &FILE_0, buffer_ptr, write_data_ptr, &write_data_ptr_length, write_pos, date_time_ptr);
				close_file(&FILE_0, &FAT, buffer_ptr);
				
				write_pos+= write_data_ptr_length;
				
				state = 0x06;		
				break;
				
			case 0x05:			// gui ket qua len server
				uartA_transmit (write_data_ptr); 
				
				state = 0x06;
				break;
			
			case 0x06:
				//sleep(0);
				if (adc_enable)
					state = 0x03;
				break;
					
			default:;						 	
		}
	}	
}

