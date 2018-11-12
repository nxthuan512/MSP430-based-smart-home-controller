#ifndef SDMMC_H_
#define SDMMC_H_

#include "MMC.h"

void sdmmc_power_enable();
void sdmmc_power_disable();
void sdmmc_initilize (unsigned long *);
void sdmmc_clear_sectors (unsigned long ); 
 
#endif /*SDMMC_H_*/
