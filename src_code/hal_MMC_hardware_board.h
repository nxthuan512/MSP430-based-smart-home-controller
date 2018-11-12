//----------------------------------------------------------------------------
//  This include file contains definitions specific to the hardware board.
//----------------------------------------------------------------------------
// ********************************************************
//
//   
//
//            MSP430F5438                  MMC Card 
//         -----------------          -----------------
//     /|\|              XIN|-   /|\ |                 |
//      | |                 |     |  |                 |
//      --|RST          XOUT|-    |--|Pin4/Vcc         |
//        |                 |        |                 |
//        |                 |        |                 |
//        |            P2.5 |<-------|Pin6/CD          |
//        |            P3.0 |------->|Pin1/CS          |
//        |                 |        |                 |
//        |      P3.2/SOMI1 |------->|Pin7/DOUT        |
//        |      P3.1/SIMO1 |<-------|Pin2/DIN         |
//        |      P3.3/UCLK1 |------->|Pin5/CLK         |
//        |                 |        |                 |
//        |                 |     |--|Pin3/GND         |
//                                |
//                                =
//
//  Pin configuration at MSP430F169:
//  --------------------------------
//  MSP430F5438      MSP Pin        MMC             MMC Pin
//  -------------------------------------------------------------
//  P3.0              33           ChipSelect       1
//  P3.2 / SOMI       35           DataOut          7
//                                 GND              3 (0 V)
//                                 VDD              4 (3.3 V)
//  P3.3 / UCLK1      36           Clock            5
//  P2.5              30           CardDetect       6
//  P3.1 / SIMO       34           DataIn           2
//  -------------------------------------------------------------

/*
#include "msp430x16x.h"              // Adjust this according to the
                                     // MSP430 device being used.
// SPI port definitions              // Adjust the values for the chosen
#define SPI_PxSEL         P5SEL      // interfaces, according to the pin
#define SPI_PxDIR         P5DIR      // assignments indicated in the
#define SPI_PxIN          P5IN       // chosen MSP430 device datasheet.
#define SPI_PxOUT         P5OUT
#define SPI_SIMO          0x02
#define SPI_SOMI          0x04
#define SPI_UCLK          0x08

//----------------------------------------------------------------------------
// SPI/UART port selections.  Select which port will be used for the interface 
//----------------------------------------------------------------------------
//#define SPI_SER_INTF      SER_INTF_USART1  // Interface to MMC
#define SPI_SER_INTF      SER_INTF_BITBANG  // Interface to MMC
*/

#include "msp430f5438.h"
									 // Adjust this according to the
                                     // MSP430 device being used.
// SPI port definitions              // Adjust the values for the chosen
#define SPI_PxSEL         P3SEL      // interfaces, according to the pin
#define SPI_PxDIR         P3DIR      // assignments indicated in the
#define SPI_PxIN          P3IN       // chosen MSP430 device datasheet.
#define SPI_PxOUT         P3OUT
#define SPI_SIMO          0x02		// mask bit
#define SPI_SOMI          0x04		// mask bit
#define SPI_UCLK          0x08		// mask bit

//----------------------------------------------------------------------------
// SPI/UART port selections.  Select which port will be used for the interface 
//----------------------------------------------------------------------------
#define SPI_SER_INTF      SER_INTF_USCIB0  // Interface to MMC
//#define SPI_SER_INTF      SER_INTF_BITBANG  // Interface to MMC

// SPI port definitions              	 	// Adjust the values for the chosen
#define MMC_PxSEL         SPI_PxSEL      	// interfaces, according to the pin
#define MMC_PxDIR         SPI_PxDIR     	// assignments indicated in the
#define MMC_PxIN          SPI_PxIN       	// chosen MSP430 device datasheet.
#define MMC_PxOUT         SPI_PxOUT      
#define MMC_SIMO          SPI_SIMO
#define MMC_SOMI          SPI_SOMI
#define MMC_UCLK          SPI_UCLK

/*
// Chip Select
#define MMC_CS_PxOUT      P5OUT
#define MMC_CS_PxDIR      P5DIR
#define MMC_CS            0x01

// Card Detect
#define MMC_CD_PxIN       P5IN
#define MMC_CD_PxDIR      P5DIR
#define MMC_CD            0x40
*/

// Chip Select
#define MMC_CS_PxOUT      P3OUT
#define MMC_CS_PxDIR      P3DIR
#define MMC_CS            0x01				// mask bit

// Card Detect
#define MMC_CD_PxIN       P2IN
#define MMC_CD_PxDIR      P2DIR
#define MMC_CD            0x20				// mask bit

#define CS_LOW()    MMC_CS_PxOUT &= ~MMC_CS               // Card Select
#define CS_HIGH()   while(!halSPITXDONE); MMC_CS_PxOUT |= MMC_CS  // Card Deselect

#define DUMMY_CHAR 0xFF
