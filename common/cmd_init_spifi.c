/* 
 * Init SDRAM command for Diolan LPC 4350 DB1
 */


#include <common.h>
#include <command.h>

#include <asm/processor.h>
#include <asm/io.h>
#include <asm/byteorder.h>
#include <environment.h>
#include <mtd/cfi_flash.h>
#include <flash.h>

#define FLASH_SIZE_BYTES		(1024UL * 1024UL * 2UL)
#define FLASH_BASE_ADDR			0x1C000000

/*******************************************************************/
/* detect_spifi
/*******************************************************************/



int do_detect_spifi (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	unsigned long size = 0;
	
		/* Init: no FLASHes known */
	flash_info_t flash_info;	/* FLASH chips info */

			flash_info.flash_id = FLASH_UNKNOWN;

			flash_get_size (0x14000000, 1);
			size += flash_info.size;
	return 1;

}



U_BOOT_CMD(detect_spifi, 1, 1, do_detect_spifi, "Detect SPIFI on board using CFI", "Detect SPIFI on board using CFI. Base address is 0x14000000.");


