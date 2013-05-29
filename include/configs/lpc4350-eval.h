/*
 * (C) Copyright 2012
 *
 * Alexander Potashev, Emcraft Systems, aspotashev@emcraft.com
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */


/*
 * Further mtest
 */
// #define CONFIG_SYS_ALT_MEMTEST



/*
 * Configuration settings for the Hitex LPC4350 Eval board.
 */

#ifndef __CONFIG_H
#define __CONFIG_H

/*
 * Disable debug messages
 */
// #undef DEBUG



/*
 * Boot in SPIFI
 * Library SPIFI (see external_libs/)
 * Warning : U-Boot configured to have just 1 SPI Flash bank
 * Warning : Do not define if there is no SPI Flash device used !
 */
#define CONFIG_LPC_SPIFI

/*
 * This is an ARM Cortex-M4 CPU core. Also use the common Cortex-M3 code.
 */
#define CONFIG_SYS_ARMCORTEXM3
#define CONFIG_SYS_ARMCORTEXM4

/*
 * This is the NXP LPC4350 device which is backward-compatible with LPC1850
 */
#define CONFIG_SYS_LPC18XX

/*
 * Add header to the U-Boot image to pass necessary information
 * to the Boot ROM bootloader.
 */
#define CONFIG_LPC18XX_BOOTHEADER

/*
 * Enable GPIO driver
 */
#define CONFIG_LPC18XX_GPIO

/*
 * Display CPU and Board information
 */
#define CONFIG_DISPLAY_CPUINFO		1
#define CONFIG_DISPLAY_BOARDINFO	1

#define CONFIG_SYS_BOARD_REV_STR	"2"

/*
 * Monitor prompt
 */
#define CONFIG_SYS_PROMPT		"LPC4350-EVAL> "

/*
 * We want to call the CPU specific initialization
 */
#define CONFIG_ARCH_CPU_INIT

/*
 * We do not use cortex_m3_soc_init() yet.
 */
#undef CONFIG_ARMCORTEXM3_SOC_INIT

/*
 * Clock configuration (see cpu/arm_cortexm3/lpc18xx/clock.c for details)
 */
/*
 * This should be setup to the board specific rate for the external oscillator
 */
#define CONFIG_LPC18XX_EXTOSC_RATE		12000000

/*
 * PLL1 multiplier value (1..256)
 */
#define CONFIG_LPC18XX_PLL1_M		17	/* 12 MHz * 17 = 204 MHz */

/*
 * Number of clock ticks in 1 sec
 */
#define CONFIG_SYS_HZ			1000

/*
 * Use internal clock (CPU clock) for the Cortex-M4 SysTick timer
 */
#define CONFIG_ARMCORTEXM3_SYSTICK_CPU

/*
 * Enable/disable h/w watchdog
 */
#undef CONFIG_HW_WATCHDOG

/*
 * No interrupts
 */
#undef CONFIG_USE_IRQ



/*
 * Memory layout configuration
 */
/*
 * Used in u-boot.lds generation.
 * Useless in this case (boot from SPIFI only) because addresses are fixed
 */
#define CONFIG_MEM_NVM_BASE		0x14000000
#define CONFIG_MEM_NVM_LEN		(128 * 1024)

#define CONFIG_MEM_RAM_BASE		0x20000000
#define CONFIG_MEM_RAM_LEN		(32 * 1024)
#define CONFIG_MEM_RAM_BUF_LEN		(1 * 1024)
#define CONFIG_MEM_MALLOC_LEN		(27 * 1024)
#define CONFIG_MEM_STACK_LEN		(4 * 1024)

/*
 * malloc() pool size
 */
#define CONFIG_SYS_MALLOC_LEN		CONFIG_MEM_MALLOC_LEN

/*
 * Configuration of the external DRAM memory
 */
#define CONFIG_NR_DRAM_BANKS		1
#define CONFIG_SYS_RAM_CS		0	/* 0 .. 3 */
#define CONFIG_SYS_RAM_BASE		0x28000000
#define CONFIG_SYS_RAM_SIZE		(8 * 1024 * 1024)
/*
 * Buffers for Ethernet DMA (cannot be in the internal System RAM)
 */
#define CONFIG_MEM_ETH_DMA_BUF_BASE	0x10080000	/* Region of SRAM */
/*
 * Use the CPU_CLOCK/2 for EMC
 */
#define CONFIG_LPC18XX_EMC_HALFCPU



/*
 * Configuration of the external parallel Flash memory
 * NOT Serial FLASH (Using LPC SPIFI driver)
 */
/* Define this to enable NOR Flash support */
#define CONFIG_SYS_FLASH_CS		0


#if defined(CONFIG_SYS_FLASH_CS)
	#define CONFIG_SYS_FLASH_CFG		0x81 /* 16 bit, Byte Lane enabled */
	#define CONFIG_SYS_FLASH_WE		(1 - 1)		/* Minimum is enough */
	#define CONFIG_SYS_FLASH_OE		0		/* Minimum is enough */
	#define CONFIG_SYS_FLASH_RD		(15 - 1)	/* 70ns at 204MHz */
	#define CONFIG_SYS_FLASH_PAGE		(15 - 1)	/* 70ns at 204MHz */
	#define CONFIG_SYS_FLASH_WR		0x1f		/* Maximum */
	#define CONFIG_SYS_FLASH_TA		0x0f		/* Maximum */

	/*
	 * Define just parallel flash banks
	 */
	#define CONFIG_SYS_FLASH_BANK1_BASE		0x1C000000 /* CS0 */

	#define CONFIG_SYS_FLASH_CFI				1
	#define CONFIG_FLASH_CFI_DRIVER				1
	#define CONFIG_FLASH_CFI_LEGACY				1
	#define CONFIG_SYS_FLASH_LEGACY_2Mx16		1

	#define CONFIG_SYS_FLASH_CFI_WIDTH			FLASH_CFI_16BIT

	/* CONFIG_SYS_FLASH_BANKS_LIST	: list of all parallel Flash
	 *
	 * CONFIG_SYS_MAX_FLASH_BANKS : number of Flash banks
	 * SPI Flash not included : this ROM allow one SPIFI device which bank number
	 * is CONFIG_SYS_MAX_FLASH_BANKS + 1
	 */
	#define CONFIG_SYS_FLASH_BANKS_LIST	{ CONFIG_SYS_FLASH_BANK1_BASE }
	#define CONFIG_SYS_MAX_FLASH_BANKS			1
	#define CONFIG_SYS_MAX_FLASH_SECT			1024

	#ifdef CONFIG_LPC_SPIFI
	#define CONFIG_SYS_FLASH_BANK1_SPIFI_BASE		0x80000000 /* SPIFI bank 1 */
	#define CONFIG_SYS_FLASH_SPIFI_BANKS_LIST	{ CONFIG_SYS_FLASH_BANK1_SPIFI_BASE }
	#define SPI_FLASH_SECT_SIZE 	(64*1024) /* TODO : auto detect */
	//#define CONFIG_SPI_FLASH
	//#define CONFIG_SPI_FLASH_SPANSION
	#endif


/*
 * *** IF environment variables in SPI flash (using LPC lib)
 * *******************************************************************
 *
 * Environment variables are copied in two "banks" of configuration
 * Prevent for losing environment variables data
 *
 * Sections are divided this way :
 * 	****************
 * 	|				|
 * 	|	U-Boot		|
 * 	|	firmware	|
 * 	|				|
 * 	****************	-> 0x14020000
 * 	| env bank 1	|	old env
 * 	****************	-> 0x14020000 + n sector(s)
 * 	| env bank 2	|	newest
 * 	****************
 *
 *
 * *******************************************************************
 */



	/*
	 * Store env in flash.
	 */
	#define CONFIG_ENV_IS_IN_SPI_FLASH
#else
	/*
	 * Store env in memory only, if no flash.
	 */
	#define CONFIG_ENV_IS_NOWHERE
	#define CONFIG_SYS_NO_FLASH
#endif


#define CONFIG_ENV_SIZE			(4 * 1024)

#ifdef CONFIG_LPC_SPIFI
	#define CONFIG_ENV1_OFFSET	(SPI_FLASH_SECT_SIZE*2) /* !!! Must be start address of a sector, see SPI_FLASH_SECT_SIZE*/
	#define CONFIG_ENV1_ADDR (CONFIG_SYS_FLASH_BANK1_SPIFI_BASE + CONFIG_ENV1_OFFSET)

	#define CONFIG_ENV2_OFFSET (CONFIG_ENV1_OFFSET+SPI_FLASH_SECT_SIZE)
	#define CONFIG_ENV2_ADDR (CONFIG_SYS_FLASH_BANK1_SPIFI_BASE + CONFIG_ENV2_OFFSET)
#else
	#define CONFIG_ENV1_ADDR \
		(CONFIG_SYS_FLASH_BANK1_BASE + 128 * 1024)
#endif

#define CONFIG_INFERNO			1
#define CONFIG_ENV_OVERWRITE		1

/*
 * Support booting U-Boot from NOR flash
 */
/* U-Boot will reload itself from flash to be sure the whole image is in SRAM */
#define CONFIG_LPC18XX_NORFLASH_BOOTSTRAP_WORKAROUND
/* The image contents go immediately after the 16-byte header */
#ifdef CONFIG_LPC18XX_BOOTHEADER
#define CONFIG_LPC18XX_NORFLASH_IMAGE_OFFSET	0x10
#else
#define CONFIG_LPC18XX_NORFLASH_IMAGE_OFFSET	0x0
#endif
/*
 * Serial console configuration
 */
#define CONFIG_SYS_NS16550		1
#undef CONFIG_NS16550_MIN_FUNCTIONS
#define CONFIG_SYS_NS16550_SERIAL	1
/*
 * Registers are 32-bit. The negative value tells the ns16550 driver that
 * registers should be post-padded with zeroes (because the CPU is in
 * little-endian mode.)
 */
#define CONFIG_SYS_NS16550_REG_SIZE	(-4)
/*
 * USART0 uses the BASE_UART0_CLK clock
 */
#define CONFIG_SYS_NS16550_CLK		clock_get(CLOCK_UART0)
#define CONFIG_CONS_INDEX		1
/*
 * USART0 registers base: 0x40081000
 * UART1 registers base:  0x40082000
 * USART2 registers base: 0x400C1000
 * USART3 registers base: 0x400C2000
 */
#define CONFIG_SYS_NS16550_COM1		0x40081000
/*
 * Pin configuration for UART
 */
#define CONFIG_LPC18XX_UART_TX_IO_GROUP		15	/* PF */
#define CONFIG_LPC18XX_UART_TX_IO_PIN		10	/* PF.10 = USART0 TXD */
#define CONFIG_LPC18XX_UART_TX_IO_FUNC		1
#define CONFIG_LPC18XX_UART_RX_IO_GROUP		15	/* PF */
#define CONFIG_LPC18XX_UART_RX_IO_PIN		11	/* PF.11 = USART0 RXD */
#define CONFIG_LPC18XX_UART_RX_IO_FUNC		1

#define CONFIG_BAUDRATE			115200
#define CONFIG_SYS_BAUDRATE_TABLE	{ 9600, 19200, 38400, 57600, 115200 }

/*
 * Ethernet configuration
 */
#define CONFIG_NET_MULTI
#define CONFIG_LPC18XX_ETH
#define CONFIG_LPC18XX_ETH_DIV_SEL	4	/* 150-250 MHz */

/*
 * Ethernet RX buffers are malloced from the internal SRAM (more precisely,
 * from CONFIG_SYS_MALLOC_LEN part of it). Each RX buffer has size of 1536B.
 * So, keep this in mind when changing the value of the following config,
 * which determines the number of ethernet RX buffers (number of frames which
 * may be received without processing until overflow happens).
 */
#define CONFIG_SYS_RX_ETH_BUFFER	3

#define CONFIG_SYS_TX_ETH_BUFFER	3

/*
 * Console I/O buffer size
 */
#define CONFIG_SYS_CBSIZE		256

/*
 * Print buffer size
 */
#define CONFIG_SYS_PBSIZE		(CONFIG_SYS_CBSIZE + \
					sizeof(CONFIG_SYS_PROMPT) + 16)

#define CONFIG_SYS_MEMTEST_START	CONFIG_SYS_RAM_BASE
#define CONFIG_SYS_MEMTEST_END		(CONFIG_SYS_RAM_BASE + \
					CONFIG_SYS_RAM_SIZE)

/*
 * Needed by "loadb"
 */
#define CONFIG_SYS_LOAD_ADDR		CONFIG_SYS_RAM_BASE

/*
 * Monitor is actually in NVM. In terms of U-Boot, it is neither "flash",
 * not RAM, but CONFIG_SYS_MONITOR_BASE must be defined.
 */
#define CONFIG_SYS_MONITOR_BASE		0x0

/*
 * Monitor is not in flash. Needs to define this to prevent
 * U-Boot from running flash_protect() on the monitor code.
 */
#define CONFIG_MONITOR_IS_IN_RAM	1

/*
 * Enable all those monitor commands that are needed
 */
#include <config_cmd_default.h>
#undef CONFIG_CMD_BOOTD
#undef CONFIG_CMD_CONSOLE
#undef CONFIG_CMD_ECHO
#undef CONFIG_CMD_EDITENV
#undef CONFIG_CMD_FPGA
#undef CONFIG_CMD_IMI
#undef CONFIG_CMD_ITEST
#undef CONFIG_CMD_IMLS
#undef CONFIG_CMD_LOADS
#undef CONFIG_CMD_MISC
#define CONFIG_CMD_NET	/* Obligatory for the Ethernet driver to build */
#undef CONFIG_CMD_NFS
#undef CONFIG_CMD_SOURCE
#undef CONFIG_CMD_XIMG
// #define CONFIG_CMD_SF /* Serial Flash commands */

/*
 * >help command
 * Explain how to use commands (syntax..)
 * *** To save memory disable long help ***
 */
#define CONFIG_SYS_LONGHELP

/*
 * Max number of command args
 */
#define CONFIG_SYS_MAXARGS		16

/*
 * Auto-boot sequence configuration
 */
#define CONFIG_BOOTDELAY		3
#define CONFIG_ZERO_BOOTDELAY_CHECK
#define CONFIG_HOSTNAME			lpc4350-eval
#define CONFIG_BOOTARGS			"lpc18xx_platform=hitex-lpc4350 "\
					"console=ttyS0,115200 panic=10"
#define CONFIG_BOOTCOMMAND		"run flashboot"

/*
 * This ensures that the board-specific misc_init_r() gets invoked.
 */
#define CONFIG_MISC_INIT_R

/*
 * Short-cuts to some useful commands (macros)
 */
#define CONFIG_EXTRA_ENV_SETTINGS				\
	"loadaddr=0x28000000\0"					\
	"addip=setenv bootargs ${bootargs} ip=${ipaddr}:${serverip}:${gatewayip}:${netmask}:${hostname}:eth0:off\0"				\
	"flashaddr=1C040000\0"					\
	"flashboot=run addip;bootm ${flashaddr}\0"		\
	"ethaddr=C0:B1:3C:88:88:90\0"				\
	"ipaddr=172.17.4.215\0"					\
	"serverip=172.17.0.1\0"					\
	"image=lpc18xx/uImage\0"				\
	"netboot=tftp ${image};run addip;bootm\0"		\
	"update=tftp ${image};"					\
	"prot off ${flashaddr} +${filesize};"			\
	"era ${flashaddr} +${filesize};"			\
	"cp.b ${loadaddr} ${flashaddr} ${filesize}\0"

/*
 * Linux kernel boot parameters configuration
 */
#define CONFIG_SETUP_MEMORY_TAGS
#define CONFIG_CMDLINE_TAG

#endif /* __CONFIG_H */
