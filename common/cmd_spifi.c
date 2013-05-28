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

#include <spifi_lpc.h>
#include <slld.h>

/*******************************************************************/
/* init_spifi : init spifi using library (see lpcware.com) */
/*******************************************************************/

extern SPIFIobj obj;
extern SPIFI_RTNS * pSpifi;
extern SPIFIopers opers;


int do_init_spifi (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	unsigned int nsectors;
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

	nsectors  = sizeof(	obj.protEnts) / sizeof(obj.protEnts)[1];

	return 1;
}

int do_read_reg_spifi(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	uint8_t poll=0x00;
	unsigned int ret;
	pSpifi->cancel_mem_mode(&obj);
	debug("\tCommand mode\n");
	printf("Read device's status register : ");
	ret = slld_RDSRCmd(&poll);
	printf("return : %d \tvalue: 0x%x\n",ret, poll);

	printf("Read device's configuration register : ");
	ret = slld_RCRCmd(&poll);
	printf("return : %d \tvalue: 0x%x\n",ret, poll);
	pSpifi->set_mem_mode(&obj);
	return 1;
}

int do_protect_sectors(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	uint8_t poll=0x00;
	unsigned int * commandReg = (unsigned int *) 0x40003004;
	printf("Protecting all sectors.. ");
	DEVSTATUS devStat;
	pSpifi->cancel_mem_mode(&obj);
	debug("\tCommand mode\n");
	if(!slld_BlockProtectOp(0x07, &devStat))
	printf("Command reg : 0x%x", (*commandReg));
	printf(". done.\n");
	do_read_reg_spifi(cmdtp, flag, argc, argv);
	pSpifi->set_mem_mode(&obj);
	return 1;
}

U_BOOT_CMD(init_spifi, 1, 1, do_init_spifi, "init SPI flash using LPC library", "init SPI flash using LPC library");

U_BOOT_CMD(spifi_read_reg, 1, 1, do_read_reg_spifi, "Read SPI Flash device's status register", "Read SPI Flash device's status register");

U_BOOT_CMD(spifi_prot_all, 1, 1, do_protect_sectors, "Protect all SPI Flash sectors", "Protect all SPI Flash sectors");
