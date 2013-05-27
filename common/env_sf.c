/*
 * (C) Copyright 2000-2002
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * (C) Copyright 2001 Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Andreas Heppel <aheppel@sysgo.de>
 *
 * (C) Copyright 2008 Atmel Corporation
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */
#include <common.h>
#include <environment.h>
#include <malloc.h>

#ifdef CONFIG_LPC_SPIFI
#include "asm/arch/spifi_rom_api.h"

extern SPIFIobj obj;
extern SPIFI_RTNS * pSpifi;
extern SPIFIopers opers;
#else
#include <spi_flash.h>
#endif

#ifndef CONFIG_ENV_SPI_BUS
# define CONFIG_ENV_SPI_BUS	0
#endif
#ifndef CONFIG_ENV_SPI_CS
# define CONFIG_ENV_SPI_CS		0
#endif
#ifndef CONFIG_ENV_SPI_MAX_HZ
# define CONFIG_ENV_SPI_MAX_HZ	1000000
#endif
#ifndef CONFIG_ENV_SPI_MODE
# define CONFIG_ENV_SPI_MODE	SPI_MODE_3
#endif

#ifndef CONFIG_ENV_SECT_SIZE
#ifdef CONFIG_LPC_SPIFI
	#define CONFIG_ENV_SECT_SIZE SPI_FLASH_SECT_SIZE
#else
	#define CONFIG_ENV_SECT_SIZE 64
#endif
#endif



DECLARE_GLOBAL_DATA_PTR;

/* references to names in env_common.c */
extern uchar default_environment[];

char * env_name_spec = "SPI Flash";
#ifdef CONFIG_LPC_SPIFI
env_t *env1_ptr = (env_t *)CONFIG_ENV_ADDR;
static env_t *flash_addr = (env_t *)CONFIG_ENV_ADDR;
#else
env_t *env1_ptr ;
static struct spi_flash *env_flash;
#endif


#ifdef CONFIG_LPC_SPIFI

/* two environment variables banks */
int update_env_1(env_t env1){

}

int saveenv(void){
	/* Unprotect
	 * TODO : Protection
	 */

	/* Erase */

	opers.dest = (char*)CONFIG_ENV_ADDR;
	opers.length = (unsigned int)(CONFIG_ENV_SIZE / CONFIG_ENV_SECT_SIZE +1);
  	opers.scratch = NULL;
  	opers.options = S_VERIFY_ERASE;
	printf("Erasing SPI Flash ..");
	pSpifi->spifi_erase(&obj, &opers);
	printf(". done. \n");
	printf("\tErased %d sector(s).\n", opers.length);
	/* Program */

	/* Protect */
}


#else /* ifndef CONFIG_LPC_SPIFI */

	int saveenv(void)
	{
		u32 saved_size, saved_offset;
		char *saved_buffer = NULL;
		u32 sector = 1;
		int ret;

		if (!env_flash) {
			puts("Environment SPI flash not initialized\n");
			return 1;
		}

		/* Is the sector larger than the env (i.e. embedded) */
		if (CONFIG_ENV_SECT_SIZE > CONFIG_ENV_SIZE) {
			saved_size = CONFIG_ENV_SECT_SIZE - CONFIG_ENV_SIZE;
			saved_offset = CONFIG_ENV1_OFFSET + CONFIG_ENV_SIZE;
			saved_buffer = malloc(saved_size);
			if (!saved_buffer) {
				ret = 1;
				goto done;
			}
			ret = spi_flash_read(env_flash, saved_offset, saved_size, saved_buffer);
			if (ret)
				goto done;
		}

		if (CONFIG_ENV_SIZE > CONFIG_ENV_SECT_SIZE) {
			sector = CONFIG_ENV_SIZE / CONFIG_ENV_SECT_SIZE;
			if (CONFIG_ENV_SIZE % CONFIG_ENV_SECT_SIZE)
				sector++;
		}

		puts("Erasing SPI flash...");
		ret = spi_flash_erase(env_flash, CONFIG_ENV1_OFFSET, sector * CONFIG_ENV_SECT_SIZE);
		if (ret)
			goto done;

		puts("Writing to SPI flash...");
		ret = spi_flash_write(env_flash, CONFIG_ENV1_OFFSET, CONFIG_ENV_SIZE, env1_ptr);
		if (ret)
			goto done;

		if (CONFIG_ENV_SECT_SIZE > CONFIG_ENV_SIZE) {
			ret = spi_flash_write(env_flash, saved_offset, saved_size, saved_buffer);
			if (ret)
				goto done;
		}

		ret = 0;
		puts("done\n");

	 done:
		if (saved_buffer)
			free(saved_buffer);
		return ret;
	}

#endif /* ifndef CONFIG_LPC_SPIFI */

	void env_relocate_spec(void)
		{
#ifndef CONFIG_LPC_SPIFI
			int ret;
			env_flash = spi_flash_probe(CONFIG_ENV_SPI_BUS, CONFIG_ENV_SPI_CS,
					CONFIG_ENV_SPI_MAX_HZ, CONFIG_ENV_SPI_MODE);
			if (!env_flash)
				goto err_probe;

			ret = spi_flash_read(env_flash, CONFIG_ENV1_OFFSET, CONFIG_ENV_SIZE, env1_ptr);
			if (ret)
				goto err_read;

			if (crc32(0, env1_ptr->data, ENV_SIZE) != env1_ptr->crc)
				goto err_crc;

			gd->env_valid = 1;

			return;

			err_read:
				spi_flash_free(env_flash);
				env_flash = NULL;
			err_probe:
			err_crc:
				puts("*** Warning - bad CRC, using default environment\n\n");

				set_default_env();
#else
/*			unsigned int ret;

			opers.length = CONFIG_ENV_SIZE;
			opers.scratch = NULL;
			opers.protect = 0;
			opers.options = S_CALLER_ERASE;
			opers.dest = (char *)env1_ptr;
			ret = pSpifi->spifi_program(&obj, (char *)flash_addr, &opers);
			if(ret==0x2000B){
					printf("Relocating env failed : dest memory has to be erased \n Error code : 0x%x \n", ret);
					return;
			}
			else if(ret == 0x20004){
				printf("Relocating env failed : dest address not in SPI Flash \n error code : 0x%x \n", ret);
				return;
			}
			else if(ret){
					printf("Relocating env failed : error unknown \n error code : 0x%x \n", ret);
					return;
			}
			else
					printf("Relocating env in SPI Flash ok\n"); */
			return;
#endif
		}

int env_init(void)
{
	if (crc32(0, env1_ptr->data, ENV_SIZE) == env1_ptr->crc) {
		gd->env_addr  = (ulong)&(env1_ptr->data);
		gd->env_valid = 1;
		return(0);
	}
	/* SPI flash isn't usable before relocation */
	gd->env_addr = (ulong)&default_environment[0];
	gd->env_valid = 1;

	return 0;
}

uchar env_get_char_spec(int index)
{
	return *((uchar *)(gd->env_addr + index));
}


