#ifndef FAT_H_
#define FAT_H_

#define MAX_FATS 4 
#define u8  unsigned char
#define u16 unsigned int
#define u32	unsigned long int		
 
// This is the struct for the file system.  We should use one more for the actual file itself. 
typedef struct 
{ 
   u8 Num_Fats;            		// Total Number of Fats 
   u32 Fat_Start[MAX_FATS];     // Start Location of each fat here 
   u32 P_Start;              	// this is the start of the partition block  
   u8 Secs_Cluster;        		// This is the number of sectors per cluster  
   int Bytes_Sector;  			// This is the number of bytes per cluster (defaut 512) 
   u16 Secs_Fat;             	// The number of sectors per fat Table 
   u32 Root_Start;            	// This is the start of the root directory 
   u16 Res_Secs;              	// The number of reserved sectors 
} FAT16_t; 
 
typedef struct 
{ 
   unsigned char File_Name[11]; // Name of the File 
   unsigned char Date_Time[4];
   u16 start_cluster;          	// Cluster in which the file begins 
   u32 start_sector;           	// Sector on SD card where the file begins  
   u32 Fat_OpenCluster;         // cluster on SD card where the write begins  
   u8  file_number;             	// This is the position in the RD of the file (starts at 0) 
   u32 Fat_OpenSec;            	// This is one greater then the last occupied sector (for fast write) 
   u16 end_sector;            	// sector in which write ends 
   u32 file_size;            	// This is one greater then the last occupied sector (for fast write) 
    
} FILE_t; 
 
void init_fat( FAT16_t *FAT, u8 *sd_buffer); 
void open_file( FILE_t *FILE, FAT16_t *FAT, u8 *sd_buffer, u8 *file_name, 
				u8 *date_time, u8 file_number, u16 cluster); 
void read_file( FILE_t *FILE, u8 *sd_buffer);
void write_file( FILE_t *FILE, u8 *sd_buffer, char *write_data, u8 *write_data_length, u16 write_pos, u8 *date_time); 
void close_file( FILE_t *FILE, FAT16_t *FAT, u8 *sd_buffer); 

#endif /*FAT_H_*/
