.global contextSwitch

contextSwitch:
    # a0 -> &old->context
    # a1 -> &running->context
    sd ra, 0 * 8(a0)
    sd sp, 1 * 8(a0)

    ld ra, 0 * 8(a1)
    ld sp, 1 * 8(a1)
    ret