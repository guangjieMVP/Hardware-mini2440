#include "s3c2440_soc.h"
#include "uart.h"

void uart0_init()
{
	/*��������GPH2,3���ڴ���0*/
	GPHCON &= ~(0XF << 4);
	GPHCON |= (0XA << 4);
	GPHUP &= (1 << 2) | (1 << 3);  //ʹ���ڲ�����(ʹ��ʹ�ܶ�û��ϵ)

	UCON0 |= (1 << 0) | (1 << 2);//ʹ���жϻ��ѯģʽ
	UCON0 &= ~(2 << 10); 
	
	 
	UBRDIV0 = 26;

	ULCON0 &= ~(1 << 5);
	 
	ULCON0 &= ~(1 << 2);
 
	ULCON0 |= (3 << 0);
	
}

void putchar(int c)
{
	UTXH0 = (unsigned char)c;
	while(!(UTRSTAT0&(1 << 2)));
}

int getchar()
{
	while(!(UTRSTAT0&1<<0));
	return URXH0;
}



void puts(const char *str)
{
	int i = 0;
	while(*str)
	{
		putchar(*str);
		str++;
	}
}


