        .data
arrayA: .byte -6,-82,-81,-78,-91,-21,55,76
        .byte 84,85,93,32,-5,61,-14,51
        .byte 44,47,-25,65,49,15,-76,67
        .byte -94,30,-74,-65,-4,4,90,-87
        .byte 60,95,-96,-1,88,72,-58,96
        .byte -53,-44,13,-40,26,-84,14,19
        .byte 23,-7
arrayB: .byte -48,53,-23,57,-88,98,17,-29
        .byte -41,-3,-59,74,-33,-12,45,31
        .byte -45,-19,-56,-9,11,-86,-67,-95
        .byte 99,-30,62,59,64,20,-54,-97
        .byte 73,37,-93,18,-68,-64,-80,-31
        .byte -77,63,94,79,-92,66,24,-38
        .byte 1,29
arrayC: .space 50
max: .byte 0
min: .byte 0

;FIRST EXERCISE

        .text 
main:   daddi r1,r0,0 ;index to sum up all the values
        daddi r5,r0,50 ;lenght of the arrays 
        j loop_and_sum

loop_and_sum:   lb r2, arrayA(r1) ;take the value from the first array
                lb r3, arrayB(r1) ;take the value from the second array
                dadd r4,r2,r3 ;sum the two values
                sb r4, arrayC(r1)
                daddi r1,r1,1
                bne r1,r5,loop_and_sum
                j search_min

;SECOND EXERCISE

;MIN
search_min: lb r6, arrayC(r0) ;set the minimum
            daddi r1,r0,0 ;reset the index to search the minimum
            daddi r1,r1,1 ;update the counter by 1
            j loop_min

loop_min:   lb r8, arrayC(r1) ;take the r1-th value
            slt r9,r8,r6 ; r9=1 if r8<r6, else r9=0
            bnez r9,inc_index_new_min
            j inc_index_go_on_min

inc_index_new_min:  movz r6,r8,r0 ;set the new value of min - both moves are necessary 
                    movn r6,r8,r0 ;set the new value of min - MOVZ rd, rs, rt
                    daddi r1,r1,1 ;increment the index
                    beq r1,r5,search_max
                    j loop_min
                    
inc_index_go_on_min:    daddi r1,r1,1
                        beq r1,r5,search_max
                        j loop_min


;MAX
search_max: lb r7, arrayC(r0) ;set the maximum
            daddi r1,r0,0 ;reset the index to search the maximum
            daddi r1,r1,1 ;update the counter by 1
            j loop_max

loop_max:   lb r8, arrayC(r1) ;take the r1-th value
            slt r9,r7,r8 ; r9=1 if r8<r7, else r9=0
            bnez r9,inc_index_new_max
            j inc_index_go_on_max

inc_index_new_max:  movz r7,r8,r0 ;set the new value of max - both moves are necessary 
                    movn r7,r8,r0 ;set the new value of max - MOVZ rd, rs, rt
                    daddi r1,r1,1 ;increment the index
                    beq r1,r5,search_max
                    j loop_max
                    
inc_index_go_on_max:    daddi r1,r1,1
                        beq r1,r5,exit
                        j loop_max

;COMMON FUNCTIONS
exit:   sb r6,min(r0) ;sicuro in posizione r0?
        sb r7,max(r0)
        halt
        



    
        
