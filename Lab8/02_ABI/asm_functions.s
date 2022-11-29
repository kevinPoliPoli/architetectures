				AREA asm_functions, CODE, READONLY				
                EXPORT  check_square
				EXPORT my_division	
				IMPORT __aeabi_fdiv
					
					
check_square	PROC
	
				; save volatile registers
				STMFD sp!,{r1,r2,LR}
				
				mul r2, r2, r2 ;contains r
				
				mul r0, r0, r0 ;contains x
				mul r1, r1, r1 ;contains y
				add r0, r0, r1
				
				cmp r0, r2
				;<=
				movle r0, #1
				;>
				movgt r0, #0
				
				; restore volatile registers
				LDMFD sp!,{r1,r2,PC}
				
                ENDP


my_division		PROC	

				; save volatile registers
				STMFD sp!,{R4,R5,R6,R7,LR}
				;take a and b
				ldr r0, [r0]
				ldr r1, [r1]
				;call __aeabi_fdiv
				BL __aeabi_fdiv
				;set return value
				; restore volatile registers
				LDMFD sp!,{R4,R5,R6,R7,PC}
				
				ENDP
					
				END