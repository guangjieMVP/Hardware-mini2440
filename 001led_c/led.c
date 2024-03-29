#define GPBCON  (*((volatile unsigned int *) 0x56000010))
#define GPBDAT  (*((volatile unsigned int *) 0x56000014))
#define GPBUP   (*((volatile unsigned int *) 0x56000018))

#define LED1 1
#define LED2 2
#define LED3 3
#define LED4 4

#define GPIOB5_OUT (1 << (2*5))
#define GPIOB6_OUT (1 << (2*6))
#define GPIOB7_OUT (1 << (2*7))
#define GPIOB8_OUT (1 << (2*8))

void led_gpio_init(void);
void led_on_off(char led_num,char status);
void delay(unsigned int time);

int main(void)
{
 
	led_gpio_init();
	
	while(1)
	{
   		 led_on_off(LED1,1);
	}
}
// 
void led_gpio_init(void)
{
	GPBCON |= GPIOB5_OUT | GPIOB6_OUT | GPIOB7_OUT | GPIOB8_OUT;
}

void delay(unsigned int time)
{
	while(time--);
}

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
