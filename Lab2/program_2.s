.data
ifmap:  .byte 1,2,3,4,5,1,2,3,4,5,1,2,3,4,5,1,2,3,4,5,1,2,3,4,5
kernel: .byte 1,1,1,1,1,1,1,1,1
ofmap:  .space 9

.text
main:           daddi r8,r0,0 ;(if) index of the first window
                daddi r9,r0,5 ;(if) index of the second window
                daddi r10,r0,10 ;(if) index of the third window 

                daddi r11,r0,0; (kernel) first index, line i, element j
                daddi r12,r0,3; (kernel) index, line i+1, element j
                daddi r13,r0,6; (kernel) index, line i+2, element j
                
                xor r14,r14,r14; (of) index of the i-th element

                daddi r21,r0,0 ;counter for the new window condition
                daddi r22,r0,3 ;new window condition

                daddi r23,r0,0 ;counter for the new line condition
                daddi r24,r0,3 ;new line condition

                daddi r25,r0,0 ;counter for exit condition
                daddi r26,r0,27 ;exit condition (total of 27 steps)

                j loop

loop:        
                ;reset registers used in the previously step
                xor r15,r15,r15
                xor r16,r16,r16
                xor r17,r17,r17
                xor r18,r18,r18
                xor r19,r19,r19
                xor r20,r20,r20

                ;take the elements of the ifmap   
                lb r15,ifmap(r8); first row of the window
                lb r16,ifmap(r9); second row of the window
                lb r17,ifmap(r10); third row of the window

                ;take the elements of the kernel
                lb r18,kernel(r11); first row
                lb r19,kernel(r12); second row
                lb r20,kernel(r13); third row

                ;compute the products
                dmul r4,r15,r18
                dmul r5,r16,r19
                dmul r6,r17,r20
                
                ;update the partial sum
                dadd r3,r3,r4
                dadd r3,r3,r5
                dadd r3,r3,r6

                ;reset used registers
                xor r4,r4,r4
                xor r5,r5,r5
                xor r6,r6,r6

                ;increment ifmap indexes
                daddi r8,r8,1
                daddi r9,r9,1
                daddi r10,r10,1

                ;increment kernel indexes
                daddi r11,r11,1
                daddi r12,r12,1
                daddi r13,r13,1

                ;increment the counter for the exit condition
                daddi r25,r25,1
                beq r25,r26,exit

                ;increment the counter for the new window condition
                daddi r21,r21,1
                beq r21,r22,next_window
                
                j loop


next_window:    ;reset the counter for the new window condition
                daddi r21,r0,0 

                ;increment the counter for the new line condition
                daddi r23,r23,1
                beq r23,r24,new_line

                ;set new ifmap indexes
                daddi r8,r8,-2
                daddi r9,r9,-2
                daddi r10,r10,-2

                ;set new kernel indexes
                daddi r11,r0,0
                daddi r12,r0,3
                daddi r13,r0,6

                ;store the total sum
                sb r3,ofmap(r14)
                ;reset the temporary sum 
                xor r3,r3,r3
                ;increment ofmap index
                daddi r14,r14,1
        
                j loop


new_line:       ;reset the counter for new line condition
                daddi r23,r0,0

                ;set new ifmap indexes
                daddi r8,r8,0
                daddi r9,r9,0
                daddi r10,r10,0


                ;set new kernel indexes
                daddi r11,r0,0
                daddi r12,r0,3
                daddi r13,r0,6

                ;store the total sum into the ofmap
                sb r3,ofmap(r14)
                ;reset the temporary sum 
                xor r3,r3,r3   
                ;increment ofmap index
                daddi r14,r14,1
                

                j loop

exit:           ;store the total sum into the ofmap
                sb r3,ofmap(r14)
                halt      

