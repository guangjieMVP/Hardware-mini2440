.text
.global _start

_start:
	
	ldr   r0, =0x53000000     @ WATCHDOGŒÄŽæÆ÷µØÖ·
    mov   r1, #0x0                     
    str   r1, [r0] 
	
	

	/* ���ʱ�ӣ�����FCLK:HCLK:PCLK=400M:100M:50M */
	/* LOCKTIME(�üĴ����������ϵ�ʱ�ӱ�Ϊ�ȶ���ʱ��) */
	ldr r0,= 0x4C000000
	ldr r1,= 0xffffffff
	str r1,[r0]
	
	/* ���÷�Ƶ�ȣ�CLKDIN,HCCLK = FCLK / 4,PCLK = FCLK / 8*/
	ldr r0,= 0x40000014
	ldr r1,= 0x5
	str r1,[r0]
	
	/*MMU_SetAsyncBusMode  HCLKDIVN不为零的话要设置为异步模式（手册中得来的）*/
	mrc p15,0,r0,c1,c0,0
	orr r0,r0,#0xc0000000
	mcr p15,0,r0,c1,c0,0
	
	/* 设置 MPLL(0x40000004) = (92<<12) | (1<<4)|(1<<0)*/
	ldr r0,= 0x40000004
	ldr r1,= (92<<12) | (1<<4) | (1<<0)
	str r1,[r0]
	
	mov r0,#0
	ldr r1,[r0] //����0��ַ��ֵ��������r1��
	mov r2,#123 
	str r2,[r0]  //��123д��0��ַ
	ldr r3,[r0]  //����0��ַ��ֵ
	cmp r2,r3    //�Ƚ϶�����д���ֵ�Ƿ����
	             //�����Ϊnand�������������Ϊnor����
	ldr sp, =0x40000000+4096  //������nor����
  	ldreq sp,= 4096   //�����Ϊnand������
  	streq r1,[r0]  //nand�������ָ�0��ַ��ֵ

	bl sdram_init  //���ض�λ����֮ǰҪ��ʼ��sdram,�����޷������ݶ��ض�λ��sdram��


	//�ض�λtext�Σ�rodada,data��(�ض�λ��������)  bss�β�Ҫ����
	mov r2,#0   //����ļ��ص�ַ��
	ldr r0,= _start // //��һָ������ʱ��ַ �����ӵ�ַΪ0x30000000��
	                //��0��ַ������Ϊ��λ���޹���
	ldr r1,= __bss_start  
 
 relacorte:
	ldr r3,[r2]  //��0��ַȡֵ
	str r3,[r0]  //д��start
	add r2,r2,#4
	add r0,r0,#4
	cmp r1,r0
	bne relacorte

	/* ��bss��*/
	ldr r0,= __bss_start
	ldr r1,= _end
	mov r2,#0
clean:
	str r2,[r0];
	add r0,r0,#4
	cmp r0,r1
	bne clean
	
	//bl main  /* ʹ��bl�����������ת */
	ldr pc,= main /* ʹ�þ�����ת���ܵ���sdram��ִ��*/

halt:
	b halt
