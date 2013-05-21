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

/* SPIFIobj obj;
SPIFI_RTNS * pSpifi;
SPIFIopers opers; */


int do_detect_spifi (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{

	return 1;

}



U_BOOT_CMD(detect_spifi, 1, 1, do_detect_spifi, "Detect SPIFI on board using CFI", "Detect SPIFI on board using CFI. Base address is 0x14000000.");


