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

/*******************************************************************/
/* init_spifi : init spifi using library (see lpcware.com) */
/*******************************************************************/

extern SPIFIobj obj;
extern SPIFI_RTNS * pSpifi;
extern SPIFIopers opers;


uint32_t do_protect_sect(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[]){
	if (argc != 2) {
		cmd_usage(cmdtp);
		return 1;
	}
	unsigned int arg = simple_strtoul(argv[1], NULL ,10);

	/*
	 * TODO : Round up the number of sectors
	 */

	if(spifi_protect_fract(arg)){
		printf("Protecting sector%s failed\n", (arg>1)?"s":"");
		return 1;
	}
	else{
		if(arg==0 || arg==128)
			printf("All sectors are now %sprotected.\n", arg?"":"un");
		else
			printf("The %d first sectors are now protected.\n", arg);
	}
	return 0;
}

uint32_t do_rdid(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[]){


}


U_BOOT_CMD(sfprot, 2, 0, do_protect_sect, "Protect a fraction of SPI Flash beginning at bottom (low addresses)", "0|2|4|16|32|64|128\n\t- Protect N sectors");

// U_BOOT_CMD(spifi_rdid, 1, 1, do_rdid, "Read ID of SPI Flash using CFI", "Read ID of SPI Flash using CFI");



