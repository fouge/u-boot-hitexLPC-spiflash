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
#include <asm/arch/lpc43xx_cgu.h>
#include <asm/arch/spifi_rom_api.h>
#include <asm/arch/lpc43xx_scu.h>

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

/* hardware-control routine used by spifi_rom_api */
void pullMISO(int high) {
    /* undocumented bit 7 included as 1, Aug 2 2011 */
	LPC_SCU->SFSP3_6 = high == 0 ? 0xDB	 /* pull down */
					 : high == 1 ? 0xC3  /* pull up */
					             : 0xD3; /* neither */
}

int do_init_spifi (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	unsigned int * statusRegisterSpifi = (unsigned int *)0x4000301C;
	*statusRegisterSpifi |= (1<<4);
	debug("Initializing SPIFI ..");
	init_spifi();
	debug(". OK\n");
	return 1;
}

U_BOOT_CMD(init_spifi, 1, 1, do_init_spifi, "init SPI flash using LPC library", "init SPI flash using LPC library");
