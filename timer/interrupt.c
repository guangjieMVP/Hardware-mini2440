#include "s3c2440_soc.h"

#define LED1 1
#define LED2 2
#define LED3 3
#define LED4 4


#define GPIOG0_INTER (3 << (2*0))   //K1 ����
#define GPIOG3_INTER (3 << (2*3))   //k2 ����
void key_exint_handle(int irq_num);
extern void led_on_off(char led_num, char status);
//EINTPEND
//INTOFFSET
void interrupt_init()
{
	INTMOD &= ~(1 << 5); //����ENINT8_23ΪIRQ�ж�
	INTMSK &= ~(1 << 5);//ʹ���ж�ENIN8_23
}

void regsister_irq(int irq,irq_fuc hardware_irq)
{
	irq_arry[irq] = hardware_irq;
	INTMOD &= ~(1 << irq);  //����ΪIRQ�ж�
	INTMSK &= ~(1 << irq);   //ʹ���ж�
}

void beep_init(void)
{
	GPBCON &= ~(3 << 0);
	GPBCON |= 1 << 0;
}

void key_intterupt_init()
{
	beep_init();
	/*���ð�������Ϊ�ж�����*/
	GPGCON &= ~((3 << 0) | (3 << 6));  
	GPGCON |= (2 << 6) | (2 << 0);

	/*�����жϴ�����ʽ��������or�½��أ�or˫����*/
	EXTINT1 |= (3 << 1) | (3 << 9); //����Ϊ˫���ش���

	EINTMASK &= ~((1<<8) | (1<<11));//ʹ��EINT8��EINT11
	regsister_irq(5,key_exint_handle);
}

void key_exint_handle(int irq_num)
{
	int val = EINTPEND;
	
	int val_key = GPGDAT;
	
	if(irq_num == 5)
	{
		if(val & (1<<8))
		{
			if(val_key & (1 << 0))
			{
				led_on_off(LED3,0);
			}
			else
			{
				led_on_off(LED3,1);
			}
		}
		else if(val_key & (1 << 11))
		{
			
			if(val_key & (1 << 3))
			{
				led_on_off(LED4,0);
			}
			else
			{
				led_on_off(LED4,1);
			}
		}
	}
	EINTPEND = val;  //���ж�
}

void timer_irq()
{
	static char count = 0;
	count++;
	if(count == 1)
	{
		GPBDAT &= (~(1 << 5));
	}
	else if(count == 2)
	{
		count = 0;
		GPBDAT |= (1 << 5);
	}
}



void irq_handle()
{
	int bit = INTOFFSET;
	
	irq_arry[bit](bit);

	//���ж�
	SRCPND = (1<<bit);
	INTPND = (1<<bit);
}

