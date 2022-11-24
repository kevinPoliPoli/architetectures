;/**************************************************************************//**
; * @file     startup_LPC17xx.s
; * @brief    CMSIS Cortex-M3 Core Device Startup File for
; *           NXP LPC17xx Device Series
; * @version  V1.10
; * @date     06. April 2011
; *
; * @note
; * Copyright (C) 2009-2011 ARM Limited. All rights reserved.
; *
; * @par
; * ARM Limited (ARM) is supplying this software for use with Cortex-M
; * processor based microcontrollers.  This file can be freely distributed
; * within development tools that are supporting such ARM based processors.
; *
; * @par
; * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
; * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
; * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
; * ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
; * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
; *
; ******************************************************************************/

; *------- <<< Use Configuration Wizard in Context Menu >>> ------------------

; <h> Stack Configuration
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Stack_Size      EQU     0x00000200

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp


; <h> Heap Configuration
;   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Heap_Size       EQU     0x00000200

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3	; 2*3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit


                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset

                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors

__Vectors       DCD     __initial_sp              ; Top of Stack
                DCD     Reset_Handler             ; Reset Handler
                DCD     NMI_Handler               ; NMI Handler
                DCD     HardFault_Handler         ; Hard Fault Handler
                DCD     MemManage_Handler         ; MPU Fault Handler
                DCD     BusFault_Handler          ; Bus Fault Handler
                DCD     UsageFault_Handler        ; Usage Fault Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     SVC_Handler               ; SVCall Handler
                DCD     DebugMon_Handler          ; Debug Monitor Handler
                DCD     0                         ; Reserved
                DCD     PendSV_Handler            ; PendSV Handler
                DCD     SysTick_Handler           ; SysTick Handler

                ; External Interrupts
                DCD     WDT_IRQHandler            ; 16: Watchdog Timer
                DCD     TIMER0_IRQHandler         ; 17: Timer0
                DCD     TIMER1_IRQHandler         ; 18: Timer1
                DCD     TIMER2_IRQHandler         ; 19: Timer2
                DCD     TIMER3_IRQHandler         ; 20: Timer3
                DCD     UART0_IRQHandler          ; 21: UART0
                DCD     UART1_IRQHandler          ; 22: UART1
                DCD     UART2_IRQHandler          ; 23: UART2
                DCD     UART3_IRQHandler          ; 24: UART3
                DCD     PWM1_IRQHandler           ; 25: PWM1
                DCD     I2C0_IRQHandler           ; 26: I2C0
                DCD     I2C1_IRQHandler           ; 27: I2C1
                DCD     I2C2_IRQHandler           ; 28: I2C2
                DCD     SPI_IRQHandler            ; 29: SPI
                DCD     SSP0_IRQHandler           ; 30: SSP0
                DCD     SSP1_IRQHandler           ; 31: SSP1
                DCD     PLL0_IRQHandler           ; 32: PLL0 Lock (Main PLL)
                DCD     RTC_IRQHandler            ; 33: Real Time Clock
                DCD     EINT0_IRQHandler          ; 34: External Interrupt 0
                DCD     EINT1_IRQHandler          ; 35: External Interrupt 1
                DCD     EINT2_IRQHandler          ; 36: External Interrupt 2
                DCD     EINT3_IRQHandler          ; 37: External Interrupt 3
                DCD     ADC_IRQHandler            ; 38: A/D Converter
                DCD     BOD_IRQHandler            ; 39: Brown-Out Detect
                DCD     USB_IRQHandler            ; 40: USB
                DCD     CAN_IRQHandler            ; 41: CAN
                DCD     DMA_IRQHandler            ; 42: General Purpose DMA
                DCD     I2S_IRQHandler            ; 43: I2S
                DCD     ENET_IRQHandler           ; 44: Ethernet
                DCD     RIT_IRQHandler            ; 45: Repetitive Interrupt Timer
                DCD     MCPWM_IRQHandler          ; 46: Motor Control PWM
                DCD     QEI_IRQHandler            ; 47: Quadrature Encoder Interface
                DCD     PLL1_IRQHandler           ; 48: PLL1 Lock (USB PLL)
                DCD     USBActivity_IRQHandler    ; 49: USB Activity interrupt to wakeup
                DCD     CANActivity_IRQHandler    ; 50: CAN Activity interrupt to wakeup


                IF      :LNOT::DEF:NO_CRP
                AREA    |.ARM.__at_0x02FC|, CODE, READONLY
CRP_Key         DCD     0xFFFFFFFF
                ENDIF
					
                AREA    static_data, DATA, READONLY, ALIGN=3
Ingredient_calories_original 	DCD 0x01, 30, 0x02, 70, 0x03, 200, 0x04, 42, 0x05, 81
								DCD 0x06, 20				
				
Ingredient_quantity_original 	DCD 0x02, 50, 0x05, 3, 0x03, 10, 0x01, 5, 0x04
								DCD 8, 0x06, 30
							
Num_ingredients 				DCB 6

				AREA 	dynamic_data, DATA, READWRITE, ALIGN=3

Ingredient_calories		SPACE 48	

Ingredient_quantity     SPACE 48
	
Calories_ordered 		SPACE 24
				
Quantity_ordered 		SPACE 24
	
Most_caloric_array 		SPACE 48
	

; Reset Handler
				AREA	|.text|, CODE, READONLY

Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]

				;--------------------- DUPLICATE ARARYS ---------------------
				ldr r0, =Ingredient_calories_original
				ldr r1, =Ingredient_calories
				
				ldrd r2, r3, [r0]
				strd r2, r3, [r1]
				add r0, #8
				add r1, #8
				ldrd r2, r3, [r0]
				strd r2, r3, [r1]
				add r0, #8
				add r1, #8
				ldrd r2, r3, [r0]
				strd r2, r3, [r1]
				add r0, #8
				add r1, #8
				ldrd r2, r3, [r0]
				strd r2, r3, [r1]
				add r0, #8
				add r1, #8
				ldrd r2, r3, [r0]
				strd r2, r3, [r1]
				add r0, #8
				add r1, #8
				ldrd r2, r3, [r0]
				strd r2, r3, [r1]
				add r0, #8
				add r1, #8
				
				mov r0, #0x0
				mov r1, #0x0
				mov r2, #0x0
				mov r3, #0x0
				
				;--------------------- REGISTER INITIALIZZATION ---------------------
				
				;obtain the maximum size of the array
				LDR r0, =Num_ingredients
				ldrb r12, [r0]
				mov r0, #0x0
				
				LDR r1, =Ingredient_calories ;pointer to the first element of the window
				add r2, r1, #8 ;pointer to the second element of the window
				
				;r3 temp label 
				;r4 temp value
				
				;r5 reserved
				;r6 reserved
				;r7 reserved
				;r8 reserved
				
				mov r11, #0x6 ;exit condition
				mov r12, #0x0 ;swap counter
				mov r0, r11 ;maximum swap per iteration
				
				
restart_cal 	;how many swaps at this iteration?
				sub r0, r0, #1
				
				;exit contition
				cmp r0, #0 
				beq save_array_cal
				
				;point to the start of the array again
				LDR r1, =Ingredient_calories
				add r2, r1, #8
				
				;reset swap counter
				mov r12, #0x0
				
				
continue_cal	;BUBBLE SORT IMPLEMENTATION
				
				;--------------------- START SWAP ---------------------
				;take the i-th and the i+1-th elements
				ldrd r5, r6, [r1]
				ldrd r7, r8, [r2]
				
				;check if maximum
				cmp r6, r8
				
				;r6 < r8 -> N=1 -> swap
				;save first item of the window in a temporary register
				movmi r3, r5 
				movmi r4, r6
				;save second item in the first
				movmi r5, r7
				movmi r6, r8
				;restore the first item
				movmi r7, r3
				movmi r8, r4
				
				;update the array
				strd r5, r6, [r1]
				strd r7, r8, [r2]
				
				;--------------------- RESTART CONDITION ---------------------
				;update restart_counter
				add r12, #1
				;check if restart
				cmp r12, r0
				;yes -> restart
				beq restart_cal
				;no -> update window
				add r1, r1, #8
				add r2, r2, #8
				bne continue_cal
				
save_array_cal	ldr r0, =Calories_ordered
				ldr r1, =Ingredient_calories
				ldrb r2, [r1]
				strb r2, [r0]
				ldrb r2, [r1, #8]!
				strb r2, [r0, #4]!
				ldrb r2, [r1, #8]!
				strb r2, [r0, #4]!
				ldrb r2, [r1, #8]!
				strb r2, [r0, #4]!
				ldrb r2, [r1, #8]!
				strb r2, [r0, #4]!
				ldrb r2, [r1, #8]!
				strb r2, [r0, #4]!
				
				b next_step

next_step 		ldr r0, =Ingredient_quantity_original
				ldr r1, =Ingredient_quantity
				
				ldrd r2, r3, [r0]
				strd r2, r3, [r1]
				add r0, #8
				add r1, #8
				ldrd r2, r3, [r0]
				strd r2, r3, [r1]
				add r0, #8
				add r1, #8
				ldrd r2, r3, [r0]
				strd r2, r3, [r1]
				add r0, #8
				add r1, #8
				ldrd r2, r3, [r0]
				strd r2, r3, [r1]
				add r0, #8
				add r1, #8
				ldrd r2, r3, [r0]
				strd r2, r3, [r1]
				add r0, #8
				add r1, #8
				ldrd r2, r3, [r0]
				strd r2, r3, [r1]
				add r0, #8
				add r1, #8
				
				mov r0, #0x0
				mov r1, #0x0
				mov r2, #0x0
				mov r3, #0x0
				mov r4, #0x0
				mov r5, #0x0
				mov r6, #0x0
				mov r7, #0x0
				mov r8, #0x0
				mov r9, #0x0
				mov r10, #0x0
				mov r11, #0x0
				mov r12, #0x0
				
;--------------------- REGISTER INITIALIZZATION ---------------------
				
				;obtain the maximum size of the array
				LDR r0, =Num_ingredients
				ldrb r12, [r0]
				mov r0, #0x0
				
				LDR r1, =Ingredient_quantity ;pointer to the first element of the window
				add r2, r1, #8 ;pointer to the second element of the window
				
				;r3 temp label 
				;r4 temp value
				
				;r5 reserved
				;r6 reserved
				;r7 reserved
				;r8 reserved
				
				mov r11, #0x6 ;exit condition
				mov r12, #0x0 ;swap counter
				mov r0, r11 ;maximum swap per iteration
				
				
restart_qua 	;how many swaps at this iteration?
				sub r0, r0, #1
				
				;exit contition
				cmp r0, #0 
				beq save_array_qua 
				
				;point to the start of the array again
				LDR r1, =Ingredient_quantity
				add r2, r1, #8
				
				;reset swap counter
				mov r12, #0x0
				
				
continue_qua	;BUBBLE SORT IMPLEMENTATION
				
				;--------------------- START SWAP ---------------------
				;take the i-th and the i+1-th elements
				ldrd r5, r6, [r1]
				ldrd r7, r8, [r2]
				
				;check if maximum
				cmp r6, r8
				
				;r6 < r8 -> N=1 -> swap
				;save first item of the window in a temporary register
				movmi r3, r5 
				movmi r4, r6
				;save second item in the first
				movmi r5, r7
				movmi r6, r8
				;restore the first item
				movmi r7, r3
				movmi r8, r4
				
				;update the array
				strd r5, r6, [r1]
				strd r7, r8, [r2]
				
				;--------------------- RESTART CONDITION ---------------------
				;update restart_counter
				add r12, #1
				;check if restart
				cmp r12, r0
				;yes -> restart
				beq restart_qua 
				;no -> update window
				add r1, r1, #8
				add r2, r2, #8
				bne continue_qua 
				
save_array_qua 	ldr r0, =Quantity_ordered
				ldr r1, =Ingredient_quantity
				ldrb r2, [r1]
				strb r2, [r0]
				ldrb r2, [r1, #8]!
				strb r2, [r0, #4]!
				ldrb r2, [r1, #8]!
				strb r2, [r0, #4]!
				ldrb r2, [r1, #8]!
				strb r2, [r0, #4]!
				ldrb r2, [r1, #8]!
				strb r2, [r0, #4]!
				ldrb r2, [r1, #8]!
				strb r2, [r0, #4]!
				
				b most_cal
				
most_cal		mov r0, #0x0
				mov r1, #0x0
				mov r2, #0x0
				mov r3, #0x0
				mov r4, #0x0
				mov r5, #0x0
				mov r6, #0x0
				mov r7, #0x0
				mov r8, #0x0
				mov r9, #0x0
				mov r10, #0x0
				mov r11, #0x0
				mov r12, #0x0
				
				ldr r0, =Most_caloric_array 
				LDR r1, =Ingredient_calories
				ldr R2, =Ingredient_quantity
				
				mov r10, #0x0 ; exit condition
				mov r12, #6	
				
loop			CMP r10, r12 ; exit?
				beq search_max
				
				ldrd r4, r5, [r1]
				ldrd r6, r7, [r2]
				
				CMP r4, r6 ; compare the labels
				
				muleq r8, r5, r7 ;multiply 
				strdeq r4, r8, [r0] ;store into the array
				addeq r0, #8 ; increment the product array
				addeq r1, r1, #8 ; go ahead on the first array
				ldreq r2, =Ingredient_quantity ; reset the pointer to the second array
				addeq r10, r10, #1; update the iteration number
				beq loop
				
				addne r2, r2, #8 ; go ahead on the second array
				bne loop
				
				
search_max		mov r0, #0x0
				mov r1, #0x0
				mov r2, #0x0
				mov r3, #0x0
				mov r4, #0x0
				mov r5, #0x0
				mov r6, #0x0
				mov r7, #0x0
				mov r8, #0x0
				mov r9, #0x0
				mov r10, #0x0
				mov r11, #0x0
				mov r12, #0x0
				
				;--------------------- REGISTER INITIALIZZATION ---------------------
				
				;obtain the maximum size of the array
				LDR r0, =Num_ingredients
				ldrb r12, [r0]
				mov r0, #0x0
				
				LDR r1, =Most_caloric_array ;pointer to the first element of the window
				add r2, r1, #8 ;pointer to the second element of the window
				
				;r3 temp label 
				;r4 temp value
				
				;r5 reserved
				;r6 reserved
				;r7 reserved
				;r8 reserved
				
				mov r11, #0x6 ;exit condition
				mov r12, #0x0 ;swap counter
				mov r0, r11 ;maximum swap per iteration
				
				
restart_max 	;how many swaps at this iteration?
				sub r0, r0, #1
				
				;exit contition
				cmp r0, #0 
				beq save_max
				
				;point to the start of the array again
				LDR r1, =Most_caloric_array
				add r2, r1, #8
				
				;reset swap counter
				mov r12, #0x0
				
				
continue_max	;BUBBLE SORT IMPLEMENTATION
				
				;--------------------- START SWAP ---------------------
				;take the i-th and the i+1-th elements
				ldrd r5, r6, [r1]
				ldrd r7, r8, [r2]
				
				;check if maximum
				cmp r6, r8
				
				;r6 < r8 -> N=1 -> swap
				;save first item of the window in a temporary register
				movmi r3, r5 
				movmi r4, r6
				;save second item in the first
				movmi r5, r7
				movmi r6, r8
				;restore the first item
				movmi r7, r3
				movmi r8, r4
				
				;update the array
				strd r5, r6, [r1]
				strd r7, r8, [r2]
				
				;--------------------- RESTART CONDITION ---------------------
				;update restart_counter
				add r12, #1
				;check if restart
				cmp r12, r0
				;yes -> restart
				beq restart_max
				;no -> update window
				add r1, r1, #8
				add r2, r2, #8
				bne continue_max

save_max		ldr r8, =Most_caloric_array
				ldrd r9, r10, [r8]
				mov r11, r9
				
stop            LDR     R0, =stop
				BX      R0
                ENDP


; Dummy Exception Handlers (infinite loops which can be modified)

NMI_Handler     PROC
                EXPORT  NMI_Handler               [WEAK]
                B       .
                ENDP
HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler         [WEAK]
                ; your code
				orr r0,r0,#1
				BX	r0
                ENDP
MemManage_Handler\
                PROC
                EXPORT  MemManage_Handler         [WEAK]
                B       .
                ENDP
BusFault_Handler\
                PROC
                EXPORT  BusFault_Handler          [WEAK]
                B       .
                ENDP
UsageFault_Handler\
                PROC
                EXPORT  UsageFault_Handler        [WEAK]
                B       .
                ENDP
SVC_Handler     PROC
                EXPORT  SVC_Handler               [WEAK]
                B       .
                ENDP
DebugMon_Handler\
                PROC
                EXPORT  DebugMon_Handler          [WEAK]
                B       .
                ENDP
PendSV_Handler  PROC
                EXPORT  PendSV_Handler            [WEAK]
                B       .
                ENDP
SysTick_Handler PROC
                EXPORT  SysTick_Handler           [WEAK]
                B       .
                ENDP

Default_Handler PROC

                EXPORT  WDT_IRQHandler            [WEAK]
                EXPORT  TIMER0_IRQHandler         [WEAK]
                EXPORT  TIMER1_IRQHandler         [WEAK]
                EXPORT  TIMER2_IRQHandler         [WEAK]
                EXPORT  TIMER3_IRQHandler         [WEAK]
                EXPORT  UART0_IRQHandler          [WEAK]
                EXPORT  UART1_IRQHandler          [WEAK]
                EXPORT  UART2_IRQHandler          [WEAK]
                EXPORT  UART3_IRQHandler          [WEAK]
                EXPORT  PWM1_IRQHandler           [WEAK]
                EXPORT  I2C0_IRQHandler           [WEAK]
                EXPORT  I2C1_IRQHandler           [WEAK]
                EXPORT  I2C2_IRQHandler           [WEAK]
                EXPORT  SPI_IRQHandler            [WEAK]
                EXPORT  SSP0_IRQHandler           [WEAK]
                EXPORT  SSP1_IRQHandler           [WEAK]
                EXPORT  PLL0_IRQHandler           [WEAK]
                EXPORT  RTC_IRQHandler            [WEAK]
                EXPORT  EINT0_IRQHandler          [WEAK]
                EXPORT  EINT1_IRQHandler          [WEAK]
                EXPORT  EINT2_IRQHandler          [WEAK]
                EXPORT  EINT3_IRQHandler          [WEAK]
                EXPORT  ADC_IRQHandler            [WEAK]
                EXPORT  BOD_IRQHandler            [WEAK]
                EXPORT  USB_IRQHandler            [WEAK]
                EXPORT  CAN_IRQHandler            [WEAK]
                EXPORT  DMA_IRQHandler            [WEAK]
                EXPORT  I2S_IRQHandler            [WEAK]
                EXPORT  ENET_IRQHandler           [WEAK]
                EXPORT  RIT_IRQHandler            [WEAK]
                EXPORT  MCPWM_IRQHandler          [WEAK]
                EXPORT  QEI_IRQHandler            [WEAK]
                EXPORT  PLL1_IRQHandler           [WEAK]
                EXPORT  USBActivity_IRQHandler    [WEAK]
                EXPORT  CANActivity_IRQHandler    [WEAK]

WDT_IRQHandler
TIMER0_IRQHandler
TIMER1_IRQHandler
TIMER2_IRQHandler
TIMER3_IRQHandler
UART0_IRQHandler
UART1_IRQHandler
UART2_IRQHandler
UART3_IRQHandler
PWM1_IRQHandler
I2C0_IRQHandler
I2C1_IRQHandler
I2C2_IRQHandler
SPI_IRQHandler
SSP0_IRQHandler
SSP1_IRQHandler
PLL0_IRQHandler
RTC_IRQHandler
EINT0_IRQHandler
EINT1_IRQHandler
EINT2_IRQHandler
EINT3_IRQHandler
ADC_IRQHandler
BOD_IRQHandler
USB_IRQHandler
CAN_IRQHandler
DMA_IRQHandler
I2S_IRQHandler
ENET_IRQHandler
RIT_IRQHandler
MCPWM_IRQHandler
QEI_IRQHandler
PLL1_IRQHandler
USBActivity_IRQHandler
CANActivity_IRQHandler

                B       .

                ENDP


                ALIGN


; User Initial Stack & Heap

                EXPORT  __initial_sp
                EXPORT  __heap_base
                EXPORT  __heap_limit                

                END
