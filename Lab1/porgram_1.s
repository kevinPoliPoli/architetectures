        .data
array: .byte 1,-2,3,-4,5,6,-7,8,-7,6,5,-4,3,-2,1
flag: .byte 0

;r1 first item index
;r2 last item index
;r3 always contains 1
;r4 r1-th item
;r5 r2-th item

        .text
main:   daddi r1,r0,0 ;first item index
        daddi r2,r0,14 ;last item index 
        daddi r3,r0,1 
        j loop

loop:   lb r4,array(r1)
        lb r5,array(r2)
        beq r4,r5,increment_indexes
        j not_palindrome

increment_indexes:  daddi r1,r1,1
                    daddi r2,r2,-1
                    beq r1,r2,palindrome
                    j loop

not_palindrome: sb r0,flag(r0)
                halt

palindrome: sb r3,flag(r0)  
            halt