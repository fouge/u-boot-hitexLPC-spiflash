/*
 * spifi_lpc.c
 *
 *  Created on: 27 mai 2013
 *      Author: cyril
 */

#include <spifi_lpc.h>
#include <common.h>

SPIFIobj obj;
SPIFI_RTNS * pSpifi = &spifi_table;
SPIFIopers opers;

#define SPI_FLASH_SECT_SIZE		(64*1024)


/*
 * This routine erase a number of sectors (nSect) starting at address (start_addr)
 */
int32_t spifi_lpc_erase(char* start_addr, uint32_t nBytes, char* scratch){
	int32_t ret;
	opers.dest = start_addr;
	opers.length = nBytes;
  	opers.scratch = scratch;
  	opers.protect = -1; /* save & restore protection */
  	opers.options = S_CALLER_ERASE | S_VERIFY_ERASE | S_CALLER_PROT; /* Erase if not 0xFFFF and then verify memory */

  	NVIC_DisableIRQ(USB0_IRQn);
	ret = pSpifi->spifi_erase(&obj, &opers);
	NVIC_EnableIRQ(USB0_IRQn); //  reenable USB0 interrrupts
	return ret;
}

int32_t spifi_lpc_erase_all(void){
	return spifi_lpc_erase((char *)(obj.base),obj.devSize, NULL);
}

int32_t spifi_lpc_program(char* dest, char* addr_src, uint32_t length, char* scratch, uint32_t options) {
	int32_t ret;
	opers.length = length;
	opers.scratch = scratch;
	opers.protect = -1; /* save & restore protection */
	opers.options = options;
	opers.dest = (char *)dest;
  	NVIC_DisableIRQ(USB0_IRQn);
	ret = pSpifi->spifi_program(&obj, (char *)addr_src, &opers);
	NVIC_EnableIRQ(USB0_IRQn); //  reenable USB0 interrrupts
	return ret;
}

uint32_t spifi_get_device_size(void){
	return obj.devSize;
}

/*
 * Print SPI Flash info
 */
void spifi_print_info(char bank){
	printf("\nBank  # %d SPIFI: ", bank);
	printf(" Size : %d MB\n", obj.devSize>>20);
	printf("SPIFI Driver used. ");
	printf("Manufacturer ID: 0x%x, ", obj.mfger);
	printf("Device ID: 0x%x\n", obj.devID);
	printf("Base : 0x%x\n", obj.base);
}


uint32_t spifi_protect_all(int onoff){
	debug("\nSPI Flash Status register : 0x%x\n", obj.stat.hw);

	if(onoff)
		opers.protect = (7<<2) | (0x20 << 8); /* All Block Protect bits set to 1*/
	else
	{
		opers.protect = obj.stat.hw & 0xFFE3; /* All Block Protect bits set to 0*/
	}
	opers.dest = (char *)0x80000000;
	NVIC_DisableIRQ(USB0_IRQn);
	unsigned int ret = pSpifi->spifi_program(&obj, (char *)0x80000000, &opers);
	NVIC_EnableIRQ(USB0_IRQn); //  reenable USB0 interrrupts
	debug("SPI Flash Status register 0x%x\n", obj.stat.hw);
	return ret;
}

/*
 * This function protect a part of SPI flash, depending of the number of sectors.
 * !! The number of sectors must be 0 | 2 | 4 | 8 | 16 | 32 | 64 | 128
 */
uint32_t spifi_protect_fract(unsigned int nSectors){
	if(nSectors==0){
		return spifi_protect_all(0);
	}

	debug("SPI Flash Status register : 0x%x\n", obj.stat.hw);
	unsigned int prot = 1;
	unsigned int i = 2;
	for(; i<=128; i*=2){
		if(nSectors == i)
		{
			opers.protect = (prot<<2) | (0x20 << 8);
			opers.dest = (char *)0x80000000;
		  	NVIC_DisableIRQ(USB0_IRQn);
			unsigned int ret = pSpifi->spifi_program(&obj, (char *)0x80000000, &opers);
			NVIC_EnableIRQ(USB0_IRQn); //  reenable USB0 interrrupts
			debug("SPI Flash Status register 0x%x\n", obj.stat.hw);
			return ret;
		}
		prot++;
	}
	debug("Error in  %s() : Bad number of sectors\nUnable to protect sectors\n", __FUNCTION__);
	return 1;
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



