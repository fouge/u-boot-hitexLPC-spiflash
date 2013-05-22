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

	LPC_CGU->BASE_SPIFI_CLK = 1<<24 | 1<<11; /* IRC 12 MHz is good enough for us */

	/* set up SPIFI I/O (undocumented bit 7 set as 1, Aug 2 2011) */
	LPC_SCU->SFSP3_3 = 0xF3; /* high drive for SCLK */
	/* IO pins */
	LPC_SCU->SFSP3_4=LPC_SCU->SFSP3_5=LPC_SCU->SFSP3_6=LPC_SCU->SFSP3_7 = 0xD3;
	LPC_SCU->SFSP3_8 = 0x13; /* CS doesn't need feedback */

	pSpifi = &spifi_table;

	/* Reset */
	unsigned int * statusRegister = (unsigned int *)0x4000301C;
	unsigned int * controlRegister = (unsigned int *)0x40003000;
	printf("avant : status register : 0x%x \n\r", *(statusRegister));
	(*statusRegister) |= (1<<4);
	printf("apres : status register : 0x%x \n\r", *(statusRegister));
	printf("control register : 0x%x \n\r", *controlRegister);

	debug("Initializing SPIFI driver..");
	if (pSpifi->spifi_init(&obj, 3, S_RCVCLK | S_FULLCLK, 12)) {
		debug(". failed.\n");
		while (1);
	}
	debug(". OK! \n");
	return 1;
}

U_BOOT_CMD(init_spifi, 1, 1, do_init_spifi, "init SPI flash using LPC library", "init SPI flash using LPC library");
