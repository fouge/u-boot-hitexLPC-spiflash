/*
 * spifi_lpc.c
 *
 *  Created on: 27 mai 2013
 *      Author: cyril
 */

#include <spifi_lpc.h>

SPIFIobj obj;
SPIFI_RTNS * pSpifi = &spifi_table;
SPIFIopers opers;

#define SPI_FLASH_SECT_SIZE		(64*1024)
/*
 * This routine erase a number of sectors (nSect) starting at address (start_addr)
 */
int32_t spifi_lpc_erase(char* start_addr, uint32_t nBytes, char* scratch, uint32_t options){
	opers.dest = start_addr;
	opers.length = nBytes;
  	opers.scratch = scratch;
  	opers.options = options;

	return pSpifi->spifi_erase(&obj, &opers);
}

int32_t spifi_lpc_erase_all(void){
	return spifi_lpc_erase((char *)(obj.base),obj.devSize, NULL , S_VERIFY_ERASE);
}

int32_t spifi_lpc_program(char* dest, char* addr_src, uint32_t length, int32_t protect, uint32_t options) {
	opers.length = length;
	opers.scratch = NULL;
	opers.protect = protect;
	opers.options = options;
	opers.dest = (char *)dest;
	return pSpifi->spifi_program(&obj, (char *)addr_src, &opers);
}

uint32_t spifi_get_device_size(void){
	return obj.devSize;
}


/*
 * Print SPI Flash info
 */
void spifi_print_info(char bank){
	printf("\nBank  # %ld SPIFI: ", bank);
	printf(" Size : %ld MB\n", obj.devSize>>20);
	printf("SPIFI Driver used. ");
	printf("Manufacturer ID: 0x%x, ", obj.mfger);
	printf("Device ID: 0x%x\n", obj.devID);
	printf("Base : 0x%x\n", obj.base);

}




/*
 * needed for SPIFI library from LPCware
 */
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



