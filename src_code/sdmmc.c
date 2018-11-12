#include "msp430f5438.h"
#include "hal_hardware_board.h"
#include "mmc.h"

// ================================================================
//
// ================================================================
void sdmmc_power_enable()
{
	P2DIR = P2DIR | 0x01;
	P2OUT = P2OUT | 0x01;					// Turn on the enable pin of SD power module	
}

void sdmmc_power_disable()
{
	P2DIR = P2DIR | 0x01;
	P2OUT = P2OUT & 0xFE;					// Turn on the enable pin of SD power module
	//CS_LOW();
}


void sdmmc_initilize (unsigned long *cardSize)
{
	unsigned char status = 1;
	unsigned int timeout = 0;

	sdmmc_power_enable();	
	//Initialisation of the MMC/SD-card
  	while (status != 0)                     // if return in not NULL an error did occur and the
                                            // MMC/SD-card will be initialized again 
  	{
	    status = mmcInit();
	    timeout++;
	    if (timeout == 150)             	// Try 50 times till error
	    {
	      //printf ("No MMC/SD-card found!! %x\n", status);
	      break;
	    }
  	}

  	while ((mmcPing() != MMC_SUCCESS));      // Wait till card is inserted

  	// Read the Card Size from the CSD Register
  	*cardSize =  mmcReadCardSize();
  	
  	// SDCard - USCI_B0 Init -> Led 4
  	P1OUT = P1OUT & 0xef;
  	//sdmmc_disable();
}

// ================================================================
//
// ================================================================
void sdmmc_clear_sectors(unsigned long secNum)
{
	unsigned char buffer[512] = {0};
	// Clear Sectors on MMC
  	mmcWriteSector(secNum, buffer);           // write a 512 Byte big block beginning at the (aligned) adress
}