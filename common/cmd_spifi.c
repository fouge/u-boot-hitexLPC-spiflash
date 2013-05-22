/*
 * cmd_spifi.c
 *
 *  Created on: 22 mai 2013
 *      Author: cyril
 */

#define CORE_M4

#include <common.h>
#include <command.h>
// #include <asm/arch/lpc43xx_libcfg.h>
// #include <asm/arch/lpc43xx_cgu.h>
#include <asm/arch/spifi_rom_api.h>
// #include <asm/arch/lpc43xx_scu.h>

/*******************************************************************/
/* init_spifi : init spifi using library (see lpcware.com) */
/*******************************************************************/

SPIFIobj obj;
SPIFI_RTNS * pSpifi;
SPIFIopers opers;

void __aeabi_memcpy4(void *dest, const void *src, unsigned int n)
{
unsigned char * s = (unsigned char *)src;
unsigned char * d = (unsigned char *)dest;

while (n--) *d++ = *s++;
}


int do_init_spifi (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	pSpifi = &spifi_table;
	debug("Initialisation SPIFI :");
	if (pSpifi->spifi_init(&obj, 3, S_RCVCLK | S_FULLCLK, 12)) while (1);
	return 1;
}

U_BOOT_CMD(init_spifi, 1, 1, do_init_spifi, "init SPI flash using LPC library", "init SPI flash using LPC library");
