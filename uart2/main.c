#include "s3c2440_soc.h"
#include "uart.h"

#define GPIOB5_OUT (1 << (2*5))
#define GPIOB6_OUT (1 << (2*6))
#define GPIOB7_OUT (1 << (2*7))
#define GPIOB8_OUT (1 << (2*8))
 
#define GPIOG0_INT (3 << (2*0))   //K1 ����
#define GPIOG3_INT (3 << (2*3))   //k2 ����
 
#define LED1 1
#define LED2 2
#define LED3 3
#define LED4 4
/*
   ����led�������� 
*/
void led_on_off(char led_num,char status)
{
	if(status)
	{
		switch(led_num)
		{
			case LED1:GPBDAT &= (~(1 << 5));break;
			case LED2:GPBDAT &= (~(1 << 6));break;
			case LED3:GPBDAT &= (~(1 << 7));break;
			case LED4:GPBDAT &= (~(1 << 8));break;
		}
	}
	else
	{
		switch(led_num)
		{
			case LED1:GPBDAT |= (1 << 5);break;
			case LED2:GPBDAT |= (1 << 6);break;
			case LED3:GPBDAT |= (1 << 7);break;
			case LED4:GPBDAT |= (1 << 8);break;
		}
	}
	
}
void key_init(void)
{
	GPGCON &= ~(GPIOG0_INT | GPIOG3_INT);
}

void led_gpio_init(void)
{
	GPBCON |= GPIOB5_OUT | GPIOB6_OUT | GPIOB7_OUT | GPIOB8_OUT;
    GPBDAT |= (1 << 5); 
	GPBDAT |= (1 << 6); 
	GPBDAT |= (1 << 7); 
 	GPBDAT |= (1 << 8); 
}

void delay(unsigned int time)
{
	while(time--);
}

int main(void)
{
	unsigned char c;
	unsigned int dat;
	led_gpio_init();
	key_init();
	uart0_init();
	puts("hello world\n\r");

	while(1)
	{
		c = getchar();
		putchar(c);
	}
	return 0;
}