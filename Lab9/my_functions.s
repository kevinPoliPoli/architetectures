			AREA convert_table, DATA, READONLY
				
;****************************************
; CHAR 			  HEX MORSE CODE BITS 	; -> 0x00000b10 .0 -1
;**************************************** 
;48 4F 4C 41 20 4D 55 4E 44 4F 20 31 32 : HOLA MUNDO 12
morse_to_ascii__ 	DCB 65,  0x31, 0x30, 0x0, 0x0, 0x0, 2 		;A
					DCB 66,  0x30, 0x30, 0x30, 0x31, 0x0, 4		;B
					DCB 67, 0x30, 0x31, 0x30, 0x31, 0x0,4		;C
					DCB 68,  0x30, 0x30, 0x31, 0x0, 0x0	,3		;D
					DCB 69, 0x30, 0x0, 0x0, 0x0, 0x0,	1		;E
					DCB 70, 0x30, 0x31, 0x30, 0x30, 0x0,4		;F
					DCB 71,  0x31, 0x31, 0x30, 0x0, 0x0	,3		;G
					DCB 72, 0x30, 0x30, 0x30, 0x30, 0x0,4		;H
					DCB 73,  0x30, 0x30, 0x0, 0x0, 0x0,	2		;I
					;
					DCB 74,  0x31, 0x31, 0x31, 0x30, 0x0,3		;J
					DCB 75,  0x31, 0x30, 0x31, 0x0, 0x0,3		;K
					DCB 76,  0x30, 0x30, 0x31, 0x30, 0x0,4		;L
					DCB 77,  0x31, 0x31, 0x0, 0x0, 0x0,2		;M
					DCB 78, 0x30, 0x31, 0x0, 0x0, 0x0,2			;N
					DCB 79, 0x31, 0x31, 0x31, 0x0, 0x0,3		;O
					DCB 80, 0x30, 0x31, 0x31, 0x30, 0x0,3		;P
					DCB 81,  0x31, 0x30, 0x31, 0x31, 0x0,4		;Q
					DCB 82, 0x30, 0x31, 0x30, 0x0, 0x0,	3		;R
					;
					DCB 83, 0x30, 0x30, 0x30, 0x0, 0x0,	3		;S
					DCB 84, 0x31, 0x0, 0x0, 0x0, 0x0,	1		;T
					DCB 85,  0x31, 0x30, 0x30, 0x0, 0x0	,3		;U
					DCB 86, 0x31, 0x30, 0x30, 0x30, 0x0	,4		;V
					DCB 87, 0x31, 0x31, 0x30, 0x0, 0x0	,3		;W
					DCB 88,  0x31, 0x30, 0x30, 0x31, 0x0,4		;X
					DCB 89,  0x31, 0x31, 0x30, 0x31, 0x0,4		;Y
					DCB 90,  0x30, 0x30, 0x31, 0x31, 0x0,4		;X
					DCB 48,  0x31, 0x31, 0x31, 0x31, 0x30,5		;1
					
					DCB 49, 0x31, 0x31, 0x31, 0x30, 0x30,5		;2
					DCB 50,  0x31, 0x31, 0x30, 0x30, 0x30,5		;3
					DCB 51,  0x31, 0x30, 0x30, 0x30, 0x30,5		;4
					DCB 52, 0x30, 0x30, 0x30, 0x30, 0x30,5		;5
					DCB 53, 0x30, 0x30, 0x30, 0x30, 0x31,5		;6
					DCB 54,  0x30, 0x30, 0x30, 0x31, 0x31,5		;7
					DCB 55, 0x30, 0x30, 0x31, 0x31, 0x31,5		;8
					DCB 56, 0x30, 0x31, 0x31, 0x31, 0x31,5		;9
					DCB 57,  0x31, 0x31, 0x31, 0x31, 0x31,5		;0
					
			AREA temporary_area, DATA, READWRITE
temp SPACE 8
				
			AREA my_functions, CODE, READONLY
			EXPORT translate_morse
						
;*****************************************************************************
;	  PARAMETER 			LOCATION	  FOUND	  USED		     MOVED		 
;*****************************************************************************
;char* vett_input 			-> r0			Y		Y		
;int vet_input_lenght 		-> r1 			Y		N				
;char* vett_output 			-> r2			N 		Y		changed -> r1
;int vet_output_lenght 		-> r3			Y		N
;char change_symbol 		-> sp + 52		Y		Y
;char space 				-> sp + 56		Y		Y
;char sentence_end 			-> sp + 60		Y		Y
;*****************************************************************************/

translate_morse		PROC
					
					; save volatile registers
					STMFD sp!,{r0-r11,LR}		
					
					;resort/reset registers 
					mov r1, r2 
					mov r8, #0
					mov r9, #0
					mov r10, #0
					mov r11, #0
					mov r12, #0
					
					ldr r2, =temp ;temporary variable location
					mov r10, #12 
					
					;define length of output array ?? 
					
					;r0  input vector mempry location
					;r1  output vector memory location 
					;r2  contains labels (addresses) when required
					;r3  d
					;r4  d
					;r5  d
					;r6  to count how many 4 bytes have been wrote
					;r7  keep trace of the space
					;r8  contains the end_message, space and end_symbol characters
					;r9  contains the unconverted and converted value
					;r10 
					;r11 contains the total numeber of read symbols
					;r12 contains the number of read bytes
										
					
read				;0x100001D0
					ldrb r9, [r0] ;check the value of the the i-th element of the message
					
					ldrb r8, [sp, #52] ;check if symbol end
					cmp r9, r8 
					beq found2
					
					ldrb r8, [sp, #56] ;check if space
					cmp r9, r8  
					beq found3
					
					ldrb r8, [sp, #60] ;check if end of the message
					cmp r9, r8  
					beq found4
					
					cmp r12, #4
					;<=4
					ldrle r10, [r2] ;0x2007C000
					lslle r10, r10, #8
					strle r10, [r2]
					strble r9, [r2] ;store into temporary variable
					
					;>4
					addgt r2, #4 ;write in the next word of the temp variable
					lslgt r9, #24
					strgt r9, [r2]
					
					;common
					add r0, r0, #1 ;read next byte of the input array
					add r12, r12, #1 ;increment number of read bytes
					
					b read
					
found2				;push
					bl search_and_convert
					;pop
				
					;increment number of read symbols
					add r11, r11, #1
					
					;reset temp value
					ldr r2, =temp
					mov r3, #0
					str r3, [r2]
					add r2, r2, #4
					str r3, [r2]
					ldr r2, =temp
					
					add r0, r0, #1 ;read next byte of the input array
					add r1, r1, #1 ;next output position
					
					mov r12, #0 ;reset byte counter
					add r11, r11, #1 ;count two symbols (symbol + space)
					
					;add r6, r6, #1
					;cmp r6, #4 ;check if read 4 symbols
					;moveq r6, #0 ;reset counter
					;addeq r1, r1, #4
					
					cmp r7, #1
					moveq r7, #0
			
					b read

found3				;save into oputput vector value + space r10
					mov r7, #1 ;save the information about the space
					b found2
					
					
found4				;push
					BL search_and_convert ;search and convert the morse code to ascii
					;pop
					add r11, r11, #1 ;count last symbol
					mov r0, r11 ;return the total number of converted words into r0
					LDMFD sp!,{r1-r10,PC} ; restore volatile registers

					ENDP
											

search_and_convert	PROC
					; save volatile registers
					STMFD sp!,{r0-r11,LR}
					
					ldr r4, =temp
					ldr r2, =morse_to_ascii__ ;dictionary: value: ascii value -  key: byte rapresentation ;0x00000AE4	
					add r2, r2, #1 ; byte 2 (word)
					
					
loop				mov r11, #0 ;count the number of correct compared bytes

;iterate over entries of the table
pre_loop 			ldrb r6, [r2, #5] 	;length of the entry

;iterate over temporary variable's bytes
inner_loop			ldrb r0, [r4]		;temporary variable
					ldrb r3, [r2]		;first byte of the translation
					 
					cmp r6, r12; length of the entry is equal to length of read symbols?
					bne next_entry
					
					cmp r11, r6 ;finish to read?	
					beq store
		
					cmp r0, r3 ;byte to byte comparison
					bne next_entry
					addeq r2, r2, #1 ;read next byte entry
					addeq r4, r4, #1 ;read next byte temporary variable
					addeq r11, r11, #1 ;increment the number of equal bytes
					beq inner_loop
					
				
store				subeq r2, r2, r11 ;point to the head of the entry (adjust the pointer)
					subeq r2, r2, #3 ;point to the head of the entry (adjust the pointer)
					ldrbeq r4, [r2] ;retireve the value of the entry (converted value)
					ldreq r5, [r1]	;take the last byte of the output array
					lsreq r5, r5, #8 ; make space for the next sybol value
					streq r5, [r1]	; save prev values
					strbeq r4, [r1] ;save into output array
					beq exit		;finish
									
					
next_entry			;read next entry of the dictionary
					sub r2, r2, r11	;point to the head of the entry
					add r2, r2, #7 ;next entry of the dictionary
					ldr r4, =temp ;read again the temporary register
					bne loop		
					
	
exit				;found space?
					cmp r7, #1
					moveq r7, #0x20
					ldreq r5, [r1]	;take the last byte of the output array
					lsreq r5, r5, #8 ; make space for the next sybol value
					streq r5, [r1]	; save
					strbeq r7, [r1] ;save into output array
					mov r7, #0

					;restore volatile registers
					LDMFD sp!,{r0-r11,PC}
					ENDP
					
					END