/**********************************************************************
* $Id$		Spifi_ReadWrite.c	2011-06-02
*//**
* @file		Spifi_ReadWrite.c
* @brief	This example describes how to use GPIO to drive LEDs
* @version	1.0
* @date		02. June. 2011
* @author	NXP MCU SW Application Team
*
* Copyright(C) 2011, NXP Semiconductor
* All rights reserved.
*
***********************************************************************
* Software that is described herein is for illustrative purposes only
* which provides customers with programming information regarding the
* products. This software is supplied "AS IS" without any warranties.
* NXP Semiconductors assumes no responsibility or liability for the
* use of the software, conveys no license or title under any patent,
* copyright, or mask work right to the product. NXP Semiconductors
* reserves the right to make changes in the software without
* notification. NXP Semiconductors also make no representation or
* warranty that such application will be suitable for the specified
* use without further testing or modification.
**********************************************************************/

#include "asm/arch/lpc43xx_scu.h"
#include "asm/arch/lpc43xx_cgu.h"
#include "asm/arch/spifi_rom_api.h"
#include <string.h>

typedef float float32_t;

/* Example group ----------------------------------------------------------- */
/** @defgroup Spifi_ReadWrite	Spifi_ReadWrite
 * @ingroup SPIFI_Examples
 * @{
 */


/************************** PRIVATE DEFINITIONS *************************/
#define LED1_BIT			1 //LEDUSB
#define LED1_PORT			4

#define HIGH_SPEED			   // Set up CPU to run at highest speed

#define MAX_BLOCKSIZE	128



/************************** PRIVATE VARIABLES *************************/
extern SPIFIobj obj;
extern SPIFI_RTNS * pSpifi;
extern SPIFIopers opers;

unsigned char ProgramData[PROG_SIZE];
unsigned char WorkspaceData[PROG_SIZE];



/*-------------------------PRIVATE FUNCTIONS------------------------------*/
/*********************************************************************//**
 * @brief		Print menu
 * @param[in]	None
 * @return 		None
 **********************************************************************/



/*----------------- INTERRUPT SERVICE ROUTINES --------------------------*/

/*-------------------------MAIN FUNCTION------------------------------*/
/*********************************************************************//**
 * @brief		Main program body
 * @param[in]	None
 * @return 		int
 **********************************************************************/

int init_spifi(void) {                       /* Main Program                       */

    uint32_t i, start_time, end_time, interval_clk;
	unsigned long * spifi_registers = 0x40003000;

	SystemInit();
/* spifi_init(&sobj, 9, S_RCVCLK | S_FULLCLK, EMCClk) is used which is intended for operation with known serial FLASH */
 	// CGU_Init();		   /* generic Clock Generation and Control */
	/* followed by manual clock and I/O setting */
	/* set SPIFI clock */
// 	LPC_CGU->BASE_SPIFI_CLK = 1<<24 | 1<<11; /* IRC 12 MHz is good enough for us */

	/* set up SPIFI I/O (undocumented bit 7 set as 1, Aug 2 2011) */
	LPC_SCU->SFSP3_3 = 0xF3; /* high drive for SCLK */
	/* IO pins */
	LPC_SCU->SFSP3_4=LPC_SCU->SFSP3_5=LPC_SCU->SFSP3_6=LPC_SCU->SFSP3_7 = 0xD3;
	LPC_SCU->SFSP3_8 = 0x13; /* CS doesn't need feedback */


#ifdef USE_SPIFI_LIB
    pSpifi = &spifi_table;
#else
    pSpifi = (SPIFI_RTNS *)(SPIFI_ROM_TABLE);
#endif
	/* Initialize SPIFI driver */
	printf("Init SPIFI :\n");
	if (pSpifi->spifi_init(&obj, 3, S_RCVCLK | S_FULLCLK, 12)) {
		printf("fail");
		while (1);
	}
	printf("FLASH manufacturer ID = 0x%x \n\r", obj.mfger);
	printf("FLASH Device Type = 0x%x \n\r", obj.devType);
	printf("FLASH Device ID = 0x%x \n\r", obj.devID);
 

	return 1;
}

