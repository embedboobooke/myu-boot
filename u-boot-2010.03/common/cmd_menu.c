#include <common.h>
#include <command.h>
#include <def.h>
#include <nand.h>

extern char console_buffer[];
extern int readline (const char *const prompt);
extern char awaitkey(unsigned long delay, int* error_p);
extern void download_nkbin_to_flash(void);
extern int boot_zImage(ulong from, size_t size);
extern char bLARGEBLOCK;

/**
 * Parses a string into a number.  The number stored at ptr is
 * potentially suffixed with K (for kilobytes, or 1024 bytes),
 * M (for megabytes, or 1048576 bytes), or G (for gigabytes, or
 * 1073741824).  If the number is suffixed with K, M, or G, then
 * the return value is the number multiplied by one kilobyte, one
 * megabyte, or one gigabyte, respectively.
 *
 * @param ptr where parse begins
 * @param retptr output pointer to next char after parse completes (output)
 * @return resulting unsigned int
 */
static unsigned long memsize_parse2 (const char *const ptr, const char **retptr)
{
	unsigned long ret = simple_strtoul(ptr, (char **)retptr, 0);
	int sixteen = 1;

	switch (**retptr) {
		case 'G':
		case 'g':
			ret <<= 10;
		case 'M':
		case 'm':
			ret <<= 10;
		case 'K':
		case 'k':
			ret <<= 10;
			(*retptr)++;
			sixteen = 0;
		default:
			break;
	}

	if (sixteen)
		return simple_strtoul(ptr, NULL, 16);
	
	return ret;
}


void param_menu_usage()
{
    printf("\r\n##### Parameter Menu #####\r\n");
    printf("[1] 查看参数\r\n");
    printf("[2] 设置参数 \r\n");
    printf("[3] 删除参数 \r\n");
    printf("[4] 写入参数到flash \r\n");
    printf("[5] 返回 \r\n");
	printf("Enter your selection: ");
}


void param_menu_shell(void)
{
	char c;
	char cmd_buf[256];
	char name_buf[20];
	char val_buf[256];
	char param_buf1[25];
	char param_buf2[25];
	char param_buf3[25];
	char param_buf4[64];

	while (1)
	{
		param_menu_usage();
		c = awaitkey(-1, NULL);
		printf("%c\n", c);
		switch (c)
		{
            case '1':
            {
                strcpy(cmd_buf, "printenv ");
                printf("Name(enter to view all paramters): ");
                readline(NULL);
                strcat(cmd_buf, console_buffer);
                run_command(cmd_buf, 0);
                break;
            }
            
            case '2':
            {
                sprintf(cmd_buf, "setenv ");

                printf("Name: ");
                readline(NULL);
                strcat(cmd_buf, console_buffer);

                printf("Value: ");
                readline(NULL);
                strcat(cmd_buf, " ");
                strcat(cmd_buf, console_buffer);

                run_command(cmd_buf, 0);
                break;
            }
            
            case '3':
            {
                sprintf(cmd_buf, "setenv ");

                printf("Name: ");
                readline(NULL);
                strcat(cmd_buf, console_buffer);

                run_command(cmd_buf, 0);
                break;
            }
            
            case '4':
            {
                sprintf(cmd_buf, "saveenv");
                run_command(cmd_buf, 0);
                break;
            }
            
            case '5':
            {
                return;
                break;
            }
		}
	}
}


void erase_menu_usage()
{
	printf("\r\n##### Erase Nand Menu #####\r\n");
	printf("[1] Nand scrub - really clean NAND erasing bad blocks (UNSAFE) \r\n");
	printf("[2] Nand earse - clean NAND eraseing \r\n");
	printf("[q] Return main Menu \r\n");
	printf("Enter your selection: ");
}


void erase_menu_shell(void)
{
	char c;
	char cmd_buf[256];
	char *p = NULL;
	unsigned long size;
	unsigned long offset;
	struct mtd_info *mtd = &nand_info[nand_curr_device];

	while (1)
	{
		erase_menu_usage();
		c = awaitkey(-1, NULL);
		printf("%c\n", c);
		switch (c)
		{
			case '1':
			{
				strcpy(cmd_buf, "nand scrub ");
				run_command(cmd_buf, 0);
				break;
			}

			case '2':
			{
				strcpy(cmd_buf, "nand erase ");

				printf("Start address: ");
				readline(NULL);
				strcat(cmd_buf, console_buffer);

				printf("Size(eg. 4000000, 0x4000000, 64m and so on): ");
				readline(NULL);
				p = console_buffer;
				size = memsize_parse2(p, &p);
				sprintf(console_buffer, " %x", size);
				strcat(cmd_buf, console_buffer);

				run_command(cmd_buf, 0);
				break;
			}

			case 'q':
			{
				return;
				break;
			}
		}
	}
}

void main_menu_usage(void)
{
	printf("[1] 烧写bootloader到nandflash\r\n");
    printf("[2] 烧写Linux Kernel到nandflash\r\n");
    printf("[3] 烧写yaffs2文件系统到nandflash\r\n");
    printf("[4] 下载程序在SDRAM中运行\r\n");
	printf("[5] 设置uboot参数\r\n");
    printf("[6] 格式化Nandflash\r\n");
    printf("[7] 启动Linux系统\r\n");
    printf("[8] 进入Shell模式\r\n");
    printf("[9] 重启uboot\r\n");
	if (bBootFrmNORFlash == 1)
		printf("[0] 下载bootloader到NorFlash\r\n");
	printf("Enter your selection: ");
}


void menu_shell(void)
{
	char c;
	char cmd_buf[200];
	char *p = NULL;
	unsigned long size;
	unsigned long offset;
	struct mtd_info *mtd = &nand_info[nand_curr_device];

	while (1)
	{
		main_menu_usage();
		c = awaitkey(-1, NULL);
		printf("%c\n", c);
		switch (c)
		{
			case '1': //download bootloader
			{
				strcpy(cmd_buf, "usbslave 1 0x30000000; nand erase bios; nand write.jffs2 0x30000000 bios 0x100000");
				run_command(cmd_buf, 0);
				break;
			}

			case '2': //kernel
			{
				strcpy(cmd_buf, "usbslave 1 0x30000000; nand erase kernel; nand write.jffs2 0x30000000 kernel 0x400000");
				run_command(cmd_buf, 0);				
				break;
			}

			case '3': //yaffs
			{
				strcpy(cmd_buf, "usbslave 1 0x30000000; nand erase root; nand write.yaffs 0x30000000 root $(filesize)");
				run_command(cmd_buf, 0);
				break;
			}

			case '4':  //sdram
			{
				extern volatile U32 downloadAddress;
				extern int download_run;
				
				download_run = 1;
				strcpy(cmd_buf, "usbslave 1 0x30000000");
				run_command(cmd_buf, 0);
				download_run = 0;
				sprintf(cmd_buf, "go %x", downloadAddress);
				run_command(cmd_buf, 0);
				break;
			}

			case '5':  //set param
			{
				param_menu_shell();
				break;
			}
			
			case '6':  //format
			{
				erase_menu_shell();
				break;
			}

			case '7':  //boot
			{
				printf("Start Linux ...\n");
				strcpy(cmd_buf, "boot_zImage");
				run_command(cmd_buf, 0);
				break;
			}

			case '8':  //quit
			{
				return;	
				break;
			}

			case '9':  //reset
			{
				strcpy(cmd_buf, "reset");
				run_command(cmd_buf, 0);
				break;
			}
			
			case '0':  //download uboot to NOR
			{
				if (bBootFrmNORFlash == 1)
				{
					strcpy(cmd_buf, "usbslave 1 0x30000000; protect off all; erase 0 +$(filesize); cp.b 0x30000000 0 $(filesize)");
					run_command(cmd_buf, 0);
				}
				break;
			}

		}
				
	}
}

int do_menu (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	menu_shell();
	return 0;
}

U_BOOT_CMD(
	menu,	3,	0,	do_menu,
	"menu - display a menu, to select the items to do something\n",
	" - display a menu, to select the items to do something"
);

