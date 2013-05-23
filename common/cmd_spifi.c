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
