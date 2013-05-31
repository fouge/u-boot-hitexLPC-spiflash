/*
 * spifi_lpc.h
 *
 *  Created on: 27 mai 2013
 *      Author: Cyril Fougeray
 */

#ifndef SPIFI_LPC_H_
#define SPIFI_LPC_H_

#include <asm/arch/lpc43xx_cgu.h>
#include <asm/arch/spifi_rom_api.h>
#include <asm/arch/lpc43xx_scu.h>

int32_t spifi_lpc_erase(char* start_addr, uint32_t nBytes, char* scratch);

int32_t spifi_lpc_erase_all(void);

int32_t spifi_lpc_program(char* dest, char* addr_src, uint32_t length, char* scratch, uint32_t options);

uint32_t spifi_get_device_size(void);

void spifi_print_info(char bank);

uint32_t spifi_protect_all(int onoff);

uint32_t spifi_protect_fract(unsigned int nSectors);

#endif /* SPIFI_LPC_H_ */
