    .data

v1: .double 1.1,2.2,3.3,4.4,5.5,6.6,7.7,8.8,9.9,10.1
    .double 1.1,2.2,3.3,4.4,5.5,6.6,7.7,8.8,9.9,10.1
    .double 1.1,2.2,3.3,4.4,5.5,6.6,7.7,8.8,9.9,10.1
    .double 1.1,2.2,3.3,4.4,5.5,6.6,7.7,8.8,9.9,10.1
    .double 1.1,2.2,3.3,4.4,5.5,6.6,7.7,8.8,9.9,10.1
    .double 1.1,2.2,3.3,4.4,5.5,6.6,7.7,8.8,9.9,10.1
    .double 1.1,2.2,3.3,4.4,5.5,6.6,7.7,8.8,9.9,10.1

v2: .double 1.1,2.2,3.3,4.4,5.5,6.6,7.7,8.8,9.9,10.1
    .double 1.1,2.2,3.3,4.4,5.5,6.6,7.7,8.8,9.9,10.1
    .double 1.1,2.2,3.3,4.4,5.5,6.6,7.7,8.8,9.9,10.1
    .double 1.1,2.2,3.3,4.4,5.5,6.6,7.7,8.8,9.9,10.1
    .double 1.1,2.2,3.3,4.4,5.5,6.6,7.7,8.8,9.9,10.1
    .double 1.1,2.2,3.3,4.4,5.5,6.6,7.7,8.8,9.9,10.1
    .double 1.1,2.2,3.3,4.4,5.5,6.6,7.7,8.8,9.9,10.1

v3: .double 1.1,2.2,3.3,4.4,5.5,6.6,7.7,8.8,9.9,10.1
    .double 1.1,2.2,3.3,4.4,5.5,6.6,7.7,8.8,9.9,10.1
    .double 1.1,2.2,3.3,4.4,5.5,6.6,7.7,8.8,9.9,10.1
    .double 1.1,2.2,3.3,4.4,5.5,6.6,7.7,8.8,9.9,10.1
    .double 1.1,2.2,3.3,4.4,5.5,6.6,7.7,8.8,9.9,10.1
    .double 1.1,2.2,3.3,4.4,5.5,6.6,7.7,8.8,9.9,10.1
    .double 1.1,2.2,3.3,4.4,5.5,6.6,7.7,8.8,9.9,10.1

v4: .double 1.1,2.2,3.3,4.4,5.5,6.6,7.7,8.8,9.9,10.1
    .double 1.1,2.2,3.3,4.4,5.5,6.6,7.7,8.8,9.9,10.1
    .double 1.1,2.2,3.3,4.4,5.5,6.6,7.7,8.8,9.9,10.1
    .double 1.1,2.2,3.3,4.4,5.5,6.6,7.7,8.8,9.9,10.1
    .double 1.1,2.2,3.3,4.4,5.5,6.6,7.7,8.8,9.9,10.1
    .double 1.1,2.2,3.3,4.4,5.5,6.6,7.7,8.8,9.9,10.1
    .double 1.1,2.2,3.3,4.4,5.5,6.6,7.7,8.8,9.9,10.1
    
v5: .space 480
v6: .space 480
v7: .space 480

    .text
main:
    daddui r1, r0, 0        ;counter
    daddui r2, r0, 480      ;limit for exit condition

loop:   ;first group of loads
        l.d f1, v1(r1)
        l.d f2, v2(r1)
        l.d f3, v3(r1)
        add.d f5, f1, f2
        l.d f4, v4(r1)
        mul.d f8, f4, f1
        add.d f9, f2, f3
        daddui r4, r1, 8

        ;second group of loads
        l.d f10, v1(r4)
        l.d f11, v2(r4)
        l.d f12, v3(r4)
        l.d f13, v4(r4)
        mul.d f5, f5, f3
        add.d f14, f10, f11
        add.d f18, f11, f12
        daddui r4, r1, 16
        add.d f5, f5, f4

        ;third group of loads
        l.d f19, v1(r4)
        l.d f20, v2(r4)
        mul.d f14, f14, f12
        s.d f5, v5(r1)
        l.d f21, v3(r4)
        l.d f22, v4(r4)
        div.d f6, f5, f8
        add.d f23, f19, f20
        add.d f14, f14, f13
        add.d f27, f20, f21
        mul.d f17, f13, f10
        mul.d f26, f22, f19
        mul.d f23, f23, f21

        ;store values

        div.d f15, f14, f17
        s.d f6, v6(r1)

        s.d f14, v5(r4)

        mul.d f7, f6, f9
        s.d f7, v7(r1)

        add.d f23, f23, f22
        s.d f23, v5(r4)

        div.d f15, f14, f17
        s.d f6, v6(r1)

        daddui r1, r1, 24
        div.d f24, f23, f26

        mul.d f16, f15, f18
        s.d f16, v7(r4)

        s.d f15, v6(r4)

        mul.d f25, f24, f27
        bne r1, r2, loop
        s.d f25, v7(r4) ;for branch delay
        halt