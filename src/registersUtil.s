.global pushRegisters
.type pushRegisters, @function
.pushRegisters:
    add sp, sp, -256 # allocating stack space; stack grows towards decreasing memory
    # registers 0, 1, 2 don't need to be saved
    # ABI names in (parenthesis)
    # x0 -> hardwired zero (zero)
    # x1 -> return address (ra)
    # x2 -> stack pointer  (sp)
    .irp index, 3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
    sd x\index, \index * 8(sp)
    .endr
    ret

.global popRegisters
.type popRegisters, @function
.popRegisters:
    .irp index, 3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
    ld x\index, \index * 8(sp)
    .endr
    addi sp, sp, 256
    ret