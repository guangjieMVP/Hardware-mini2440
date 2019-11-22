#include "s3c2440_soc.h"
#include "my_printf.h"
#include "string_utils.h"
#include "uart.h"

#define NOR_FLASH_BASE 0

void nor_write_word(unsigned int base,unsigned int offset,unsigned int val)
{
	volatile unsigned short *p = (volatile unsigned short *)(base+(offset<<1));
	*p = val;
}

void nor_write_cmd(unsigned int offset,unsigned int cmd)
{
	nor_write_word(NOR_FLASH_BASE,offset,cmd);
}

unsigned int nor_read_word(unsigned int base,unsigned int offset)
{
	volatile unsigned short *p = (volatile unsigned short *)(base+(offset<<1));
	return *p;
}

unsigned int nor_read_data(unsigned int offset)
{
	return nor_read_word(NOR_FLASH_BASE,offset);
}
void do_scan_norflash(void)
{
	char str[4];
	//��������ID���豸ID
	nor_write_cmd(0x55,0x98);
	str[0] = nor_read_data(0x10);
	str[1] = nor_read_data(0x11);
	str[2] = nor_read_data(0x12);
	str[3] = '\0';
	printf("%s\n\r",str);
	//��ӡ����
	//��ӡ������������ʼ��ַ
}

void do_quit_norflash()
{

}

void do_write_norflash()
{

}
void do_erase_norflash()
{
	
}

void do_read_norflash()
{
	
}

void nor_flash_test(void)
{
	unsigned char c;
	while(1)
	{
		/*��ӡһ���˵����ṩ����ѡ����Ե�����*/
		printf("[s] Scan nor falsh\r\n");
		printf("[e] Erase nor falsh\r\n");
		printf("[w] Write nor falsh\r\n");
		printf("[r] Read nor falsh\r\n");
		printf("[q] Quit nor falsh\r\n");
		printf("please enter your selection:");
		/*
		* 1��ʶ��nor falsh
		* 2������nor flashĳ������
		* 3����дĳ����ַ
		* 4����ĳ����ַ
		*/
		c = getchar();
		printf("%c\r\n",c);
		switch(c)
		{
			case 'q':
			case 'Q':
				do_quit_norflash();break;
			case 's':
			case 'S':
				do_scan_norflash();break;
			case 'w':
			case 'W':
				do_write_norflash();break;
			case 'e':
			case 'E':
				do_erase_norflash();break;
			case 'r':
			case 'R':
				do_read_norflash();break;
		}
	}
}