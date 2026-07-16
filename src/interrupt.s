.global interruptRoutine
.extern handleInterrupt

.align 4

interruptRoutine:
addi sp,sp,-256
sd ra, (sp)
sd s0, 8(sp)
sd s1, 8*2(sp)
sd s2, 8*3(sp)
sd s3, 8*4(sp)
sd s4, 8*5(sp)
sd s5, 8*6(sp)
sd s6, 8*7(sp)
sd s7, 8*8(sp)
sd s8, 8*9(sp)
sd s9, 8*10(sp)
sd s10, 8*11(sp)
sd s11,  8*12(sp)
sd a0, 8*13(sp)
sd a1, 8*14(sp)
sd a2, 8*15(sp)
sd a3, 8*16(sp)
sd a4, 8*17(sp)
sd a5, 8*18(sp)
sd a6, 8*19(sp)
sd a7, 8*20(sp)
sd t0, 8*21(sp)
sd t1, 8*22(sp)
sd t2, 8*23(sp)
sd t3, 8*24(sp)
sd t4, 8*25(sp)
sd t5, 8*26(sp)
sd t6, 8*27(sp)
sd gp, 8*28(sp)
sd tp, 8*29(sp)

csrr t0, sepc
sd t0,  30*8(sp)
csrr t0, sstatus
sd t0,  31*8(sp)

la t0, running
ld t0, 0(t0)
sd sp, 0(t0)

call handleInterrupt

la t0, running
ld t0, 0(t0)
ld sp, 0(t0)

ld t0,  30*8(sp)
csrw sepc, t0
ld t0,  31*8(sp)
csrw sstatus, t0

ld ra, (sp)
ld s0, 8(sp)
ld s1, 8*2(sp)
ld s2, 8*3(sp)
ld s3, 8*4(sp)
ld s4, 8*5(sp)
ld s5, 8*6(sp)
ld s6, 8*7(sp)
ld s7, 8*8(sp)
ld s8, 8*9(sp)
ld s9, 8*10(sp)
ld s10, 8*11(sp)
ld s11,  8*12(sp)
ld a0, 8*13(sp)
ld a1, 8*14(sp)
ld a2, 8*15(sp)
ld a3, 8*16(sp)
ld a4, 8*17(sp)
ld a5, 8*18(sp)
ld a6, 8*19(sp)
ld a7, 8*20(sp)
ld t0, 8*21(sp)
ld t1, 8*22(sp)
ld t2, 8*23(sp)
ld t3, 8*24(sp)
ld t4, 8*25(sp)
ld t5, 8*26(sp)
ld t6, 8*27(sp)
ld gp, 8*28(sp)
ld tp, 8*29(sp)

addi sp,sp,256

sret
# rezim u koji se prelazi je defenisan bitom spp
# bit sie dobija vrednost od spie
# regist pc dobija vrednost sepc