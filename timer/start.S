.text
.global _start

_start:
	b reset  //复位异常
	ldr pc,= undef_addr  //未定义指令异常 用绝对跳转是因为重定位完之后异常处理函数可能在片内4k地址之外
	ldr pc,= swi_addr
	b halt 
	b halt
	b halt
	ldr pc,= irq_addr
	b halt
		
undef_addr:
	.word do_undef

swi_addr:
	.word do_swi
irq_addr:
	.word do_irq
	
do_undef:  
	/*执行到这里之前
	 *  lr_undef(undef异常的sp寄存器)，保存有被中断前的下一条指令的地址
	 * cpsr_undef保存被中断前模式的cpsr寄存器的值
	*/
	/* sp_undef未设置*/
	ldr sp,= 0x34000000  //设置未定义指令异常的堆栈
	/* 保护现场 */
	stmdb sp!,{r0-r12,lr}  //在未定义指令异常函数可能会修改r0-r12
	
	/* 处理函数*/
	bl led_test2

/* 恢复现场 */
	ldmia sp!,{r0-r12,pc}^  // ^符号表示会吧spsr的值恢复到cpsr中

.align 4  //四字节对齐
do_swi:
	ldr sp,= 0x33c00000 //设置软中断下的堆栈
	stmdb sp!,{r0-r12,lr} //保护现场
	//mov r4,lr  //保存lr到r4中
	bl led_test3
	
	ldmia sp!,{r0-r12,pc}^  //恢复现场  
	
.align 4
do_irq:
	ldr sp,= 0x32c00000 //设置软中断下的堆栈
	sub lr,lr ,#4
	stmdb sp!,{r0-r12,lr} //保护现场
	//mov r4,lr  //保存lr到r4中
	bl irq_handle
	
	ldmia sp!,{r0-r12,pc}^  //恢复现场  
		
.align 4  //四字节对齐  //ARM指令集是以四字节对齐的
reset:	
	ldr   r0, =0x53000000     @ WA澹篢CHDOG艗脛沤忙脝梅碌脴脰路
    mov   r1, #0x0                     
    str   r1, [r0] 
	
	/* 提高时钟，设置FCLK:HCLK:PCLK=400M:100M:50M */
	/* LOCKTIME(该寄存器是用来上电时钟变为稳定的时间) */
	ldr r0,= 0x4C000000
	ldr r1,= 0xffffffff
	str r1,[r0]
	
	/* 设置分频比，CLKDIN,HCCLK = FCLK / 4,PCLK = FCLK / 8*/
	ldr r0,= 0x40000014
	ldr r1,= 0x5
	str r1,[r0]
	
	/*MMU_SetAsyncBusMode  HCLKDIVN涓嶄负闆剁殑璇濊璁剧疆涓哄紓姝ユā寮忥紙鎵嬪唽涓緱鏉ョ殑锛�*/
	mrc p15,0,r0,c1,c0,0
	orr r0,r0,#0xc0000000
	mcr p15,0,r0,c1,c0,0
	
	/* 璁剧疆 MPLL(0x40000004) = (92<<12) | (1<<4)|(1<<0)*/
	ldr r0,= 0x40000004
	ldr r1,= (92<<12) | (1<<4) | (1<<0)
	str r1,[r0]
	
	mov r0,#0
	ldr r1,[r0] //读出0地址的值，保存在r1中
	mov r2,#123 
	str r2,[r0]  //将123写入0地址
	ldr r3,[r0]  //读出0地址的值
	cmp r2,r3    //比较读出和写入的值是否相等
	             //相等则为nand启动，不相等则为nor启动
	ldr sp, =0x40000000+4096  //假设是nor启动
  	ldreq sp,= 4096   //相等则为nand启动，
  	streq r1,[r0]  //nand启动，恢复0地址的值

	bl sdram_init  //在重定位代码之前要初始化sdram,否则无法将数据段重定位到sdram中

	bl copy_to_sdram  //重定位代码
	bl clear_bss    //清除bss段

	/* 复位之后cpu处于svc模式 
	* 现在切换到usr 模式
	*/
	mrs r0,cpsr  /*读出cpsr*/
	bic r0,r0,#0xf   //把低四位清零
	bic r0,r0,#(1<<7)   //清楚I为，使能irq总中断
	msr cpsr,r0
	ldr sp,= 0x33f00000  //设置用户模式(usr)下的堆栈
	ldr pc,= sdram
sdram:
	bl led_gpio_init

undef_code:  //认为定义未定义指令
	.word 0xdeadc0de

	swi 0x123

	//bl main  /* 使用bl命令是相对跳转 */
	ldr pc,= main /* 使用绝对跳转才能调到sdram中执行*/

halt:
	b halt
