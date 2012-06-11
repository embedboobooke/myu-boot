/*
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Marius Groeger <mgroeger@sysgo.de>
 * Gary Jennejohn <garyj@denx.de>
 * David Mueller <d.mueller@elsoft.ch>
 *
 * Configuation settings for the SAMSUNG SMDK2410 board.
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef __CONFIG_H
#define __CONFIG_H

/*
 * High Level Configuration Options
 * (easy to change)
 */
#define CONFIG_ARM920T	1	/* This is an ARM920T Core	*/
#define CONFIG_S3C24X0	1	/* in a SAMSUNG S3C24x0-type SoC	*/
//#define CONFIG_S3C2410	1	/* specifically a SAMSUNG S3C2410 SoC	*/
//#define CONFIG_SMDK2410	1	/* on a SAMSUNG SMDK2410 Board  */
#define CONFIG_S3C2440 1  /* specifically a SAMSUNG S3C2440 SoC	*/

/* input clock of PLL */
#define CONFIG_SYS_CLK_FREQ	12000000/* the SMDK2410 has 12MHz input clock */

#define USE_920T_MMU		1
//#undef CONFIG_USE_IRQ			/* we don't need IRQ/FIQ stuff */
#define CONFIG_USB_DEVICE 1
#ifdef CONFIG_USB_DEVICE
#define CONFIG_USE_IRQ 1
#endif

/*
 * Size of malloc() pool
 */
#define CONFIG_SYS_MALLOC_LEN		(CONFIG_ENV_SIZE + 128*1024)
#define CONFIG_SYS_GBL_DATA_SIZE	128	/* size in bytes reserved for initial data */

/*
 * Hardware drivers
 */
#if 0
#define CONFIG_NET_MULTI
#define CONFIG_CS8900		/* we have a CS8900 on-board */
#define CONFIG_CS8900_BASE	0x19000300
#define CONFIG_CS8900_BUS16	/* the Linux driver does accesses as shorts */
#endif

#define CONFIG_NET_MULTI		1
#define CONFIG_NET_RETRY_COUNT		20
#define CONFIG_DRIVER_DM9000		1
#define CONFIG_DM9000_BASE		0x20000300
#define DM9000_IO 			CONFIG_DM9000_BASE
#define DM9000_DATA 			(CONFIG_DM9000_BASE+4)
#define CONFIG_DM9000_USE_16BIT		1
#define CONFIG_DM9000_NO_SROM		1
#undef CONFIG_DM9000_DEBUG

/*
 * select serial console configuration
 */
#define CONFIG_S3C24X0_SERIAL
#define CONFIG_SERIAL1          1	/* we use SERIAL 1 on SMDK2410 */

/************************************************************
 * RTC
 ************************************************************/
#define	CONFIG_RTC_S3C24X0	1

/* allow to overwrite serial and ethaddr */
#define CONFIG_ENV_OVERWRITE

#define CONFIG_BAUDRATE		115200

/*
 * BOOTP options
 */
#define CONFIG_BOOTP_BOOTFILESIZE
#define CONFIG_BOOTP_BOOTPATH
#define CONFIG_BOOTP_GATEWAY
#define CONFIG_BOOTP_HOSTNAME

/*
 * Command line configuration.
 */
#include <config_cmd_default.h>

#define CONFIG_CMD_CACHE
#define CONFIG_CMD_DATE
#define CONFIG_CMD_ELF
#define CONFIG_CMD_NAND
#define CONFIG_CMD_PING
#define CONFIG_CMD_MTDPARTS
#define CONFIG_CMD_JFFS2
#define CONFIG_CMD_MENU  //add by mickeyos

#define CONFIG_BOOTDELAY	3

#if CONFIG_128MB_SDRAM
#define CONFIG_BOOTARGS	"noinitrd root=/dev/mtdblock2 init=/linuxrc console=ttySAC0 mem=128M"
#else
#define CONFIG_BOOTARGS	"noinitrd root=/dev/mtdblock2 init=/linuxrc console=ttySAC0 mem=64M"
#endif

#define CONFIG_ETHADDR	    1a:2b:3c:4d:5e:6f
#define CONFIG_NETMASK      255.255.255.0
#define CONFIG_IPADDR		192.168.1.10
#define CONFIG_SERVERIP		192.168.1.99
/*#define CONFIG_BOOTFILE	"elinos-lart" */
//#define CONFIG_BOOTCOMMAND	"nboot 0x32000000 kernel; bootm 0x32000000"
//#define CONFIG_BOOTCOMMAND    "nand read 0x30008000 120000 400000;bootm 0x30008000"
#define CONFIG_BOOTCOMMAND	"nand read.jffs2 0x30007FC0 kernel; bootm 0x30007FC0"

#if defined(CONFIG_CMD_KGDB)
#define CONFIG_KGDB_BAUDRATE	115200		/* speed to run kgdb serial port */
/* what's this ? it's not used anywhere */
#define CONFIG_KGDB_SER_INDEX	1		/* which serial port to use */
#endif

/*
 * Miscellaneous configurable options
 */
#define	CONFIG_SYS_LONGHELP				/* undef to save memory		*/
#define	CONFIG_SYS_PROMPT		"[u-boot@Mickey2440]# "	/* Monitor Command Prompt	*/
#define	CONFIG_SYS_CBSIZE		256		/* Console I/O Buffer Size	*/
#define	CONFIG_SYS_PBSIZE (CONFIG_SYS_CBSIZE+sizeof(CONFIG_SYS_PROMPT)+16) /* Print Buffer Size */
#define	CONFIG_SYS_MAXARGS		16		/* max number of command args	*/
#define CONFIG_SYS_BARGSIZE		CONFIG_SYS_CBSIZE	/* Boot Argument Buffer Size	*/

#define CONFIG_SYS_MEMTEST_START	0x30000000	/* memtest works on	*/
#define CONFIG_SYS_MEMTEST_END		0x33F00000	/* 63 MB in DRAM	*/

#define	CONFIG_SYS_LOAD_ADDR		0x30008000	/* default load address	*/

#define	CONFIG_SYS_HZ			1000

/* valid baudrates */
#define CONFIG_SYS_BAUDRATE_TABLE	{ 9600, 19200, 38400, 57600, 115200 }

/*-----------------------------------------------------------------------
 * Stack sizes
 *
 * The stack sizes are set up in start.S using the settings below
 */
#define CONFIG_STACKSIZE	(128*1024)	/* regular stack */
#ifdef CONFIG_USE_IRQ
#define CONFIG_STACKSIZE_IRQ	(4*1024)	/* IRQ stack */
#define CONFIG_STACKSIZE_FIQ	(4*1024)	/* FIQ stack */
#endif

/*-----------------------------------------------------------------------
 * Physical Memory Map
 */
#define CONFIG_NR_DRAM_BANKS	1	   /* we have 1 bank of DRAM */
#define PHYS_SDRAM_1		0x30000000 /* SDRAM Bank #1 */
#define PHYS_SDRAM_1_SIZE	0x04000000 /* 64 MB */

#define PHYS_FLASH_1		0x00000000 /* Flash Bank #1 */

#define CONFIG_SYS_FLASH_BASE		PHYS_FLASH_1
/*-----------------------------------------------------------------------
 * FLASH and environment organization
 */
#if 0
#define CONFIG_AMD_LV400	1	/* uncomment this if you have a LV400 flash */
#define CONFIG_AMD_LV800	1	/* uncomment this if you have a LV800 flash */
#endif
#define CONFIG_AMD_LV160    1

#define CONFIG_SYS_MAX_FLASH_BANKS	1	/* max number of memory banks */
#ifdef CONFIG_AMD_LV800
#define PHYS_FLASH_SIZE		0x00100000 /* 1MB */
#define CONFIG_SYS_MAX_FLASH_SECT	(19)	/* max number of sectors on one chip */
#define CONFIG_ENV_ADDR		(CONFIG_SYS_FLASH_BASE + 0x0F0000) /* addr of environment */
#endif
#ifdef CONFIG_AMD_LV400
#define PHYS_FLASH_SIZE		0x00080000 /* 512KB */
#define CONFIG_SYS_MAX_FLASH_SECT	(11)	/* max number of sectors on one chip */
#define CONFIG_ENV_ADDR		(CONFIG_SYS_FLASH_BASE + 0x070000) /* addr of environment */
#endif

#ifdef CONFIG_AMD_LV160
#define PHYS_FLASH_SIZE		0x00200000 /* 2MB */
#define CONFIG_SYS_MAX_FLASH_SECT	(35)	/* max number of sectors on one chip */
#define CONFIG_ENV_ADDR		(CONFIG_SYS_FLASH_BASE + CONFIG_ENV_OFFSET) /* addr of environment */
#endif

/* timeout values are in ticks */
#define CONFIG_SYS_FLASH_ERASE_TOUT	(5*CONFIG_SYS_HZ) /* Timeout for Flash Erase */
#define CONFIG_SYS_FLASH_WRITE_TOUT	(5*CONFIG_SYS_HZ) /* Timeout for Flash Write */

//#define	CONFIG_ENV_IS_IN_FLASH	1
#define CONFIG_ENV_IS_IN_NAND 1
#define CONFIG_ENV_OFFSET			0x100000
#define CONFIG_ENV_SIZE			0x20000	/* Total Size of Environment Sector */

/*-----------------------------------------------------------------------
 * NAND flash settings
 */
#if defined(CONFIG_CMD_NAND)
#define CONFIG_NAND_S3C2410
#define CONFIG_SYS_NAND_BASE 0x4E000000 
#define CONFIG_SYS_MAX_NAND_DEVICE	1	/* Max number of NAND devices		*/
#define SECTORSIZE 512
#define SECTORSIZE_2K 2048
#define NAND_SECTOR_SIZE SECTORSIZE
#define NAND_SECTOR_SIZE_2K SECTORSIZE_2K
#define NAND_BLOCK_MASK 511
#define NAND_BLOCK_MASK_2K 2047
#define NAND_MAX_CHIPS 1
#define CONFIG_MTD_NAND_VERIFY_WRITE 
#define CONFIG_SYS_64BIT_VSPRINTF		/* needed for nand_util.c */
#endif	/* CONFIG_CMD_NAND */

#define CONFIG_SETUP_MEMORY_TAGS
#define CONFIG_INITRD_TAG
#define CONFIG_CMDLINE_TAG

#define CONFIG_SYS_HUSH_PARSER
#define CONFIG_SYS_PROMPT_HUSH_PS2   "> "

#define CONFIG_CMDLINE_EDITING
#define CONFIG_AUTO_COMPLETE

#define CONFIG_JFFS2_NAND 1
#define CONFIG_JFFS2_CMDLINE 1
#define CONFIG_JFFS2_DEV	"nand0"
#define CONFIG_MTD_DEVICE
#define CONFIG_MTD_PARTITIONS
#define MTDIDS_DEFAULT "nand0=nandflash0"
#define MTDPARTS_DEFAULT		"mtdparts=nandflash0:1m@0(bios)," \
                           		 "128k(params)," \
                           		 "4m(kernel)," \
                           		 "-(root)"
								 
#define ENABLE_CMD_LOADB_X	1
#define ENABLE_CMD_NAND_YAFFS	1
#define ENABLE_CMD_NAND_YAFFS_SKIPFB	0
								 
#if 1
#define CONFIG_CMD_BMP 
#define CONFIG_VIDEO 
#define CONFIG_VIDEO_S3C2410 
#define CONFIG_VIDEO_LOGO 
#define VIDEO_FB_16BPP_WORD_SWAP 

#define CONFIG_VIDEO_SW_CURSOR 
#define CONFIG_VIDEO_BMP_LOGO 

#define CONFIG_CFB_CONSOLE
#define CONFIG_SYS_CONSOLE_IS_IN_ENV
#define CONFIG_SYS_CONSOLE_INFO_QUIET  //don't show console info
#define CONFIG_SPLASH_SCREEN 
#define CONFIG_SYS_VIDEO_LOGO_MAX_SIZE         (320*240+1024+100) /* 100 = slack */ 
#define CONFIG_VIDEO_BMP_GZIP 
#define CONFIG_CMD_UNZIP 
#define LCD_VIDEO_ADDR         0x33d00000

/*for PC-keyboard*/
#define VIDEO_KBD_INIT_FCT     0 
#define VIDEO_TSTC_FCT         serial_tstc 
#define VIDEO_GETC_FCT         serial_getc

#endif

#endif	/* __CONFIG_H */
