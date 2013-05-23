/*
 * cmd_spifi.c
 *
 *  Created on: 22 mai 2013
 *      Author: cyril
 */

#define CORE_M4

#include <common.h>
#include <command.h>
#include <asm/arch/lpc43xx_cgu.h>
#include <asm/arch/spifi_rom_api.h>
#include <asm/arch/lpc43xx_scu.h>

/*******************************************************************/
/* init_spifi : init spifi using library (see lpcware.com) */
/*******************************************************************/

extern SPIFIobj obj;
extern SPIFI_RTNS * pSpifi;
extern SPIFIopers opers;


int do_init_spifi (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{

    pSpifi = &spifi_table;

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

U_BOOT_CMD(init_spifi, 1, 1, do_init_spifi, "init SPI flash using LPC library", "init SPI flash using LPC library");
