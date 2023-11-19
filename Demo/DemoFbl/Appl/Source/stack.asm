;-------------------------------------------------------------------------------
; sys_core.asm
;
; (c) Texas Instruments 2009-2010, All rights reserved.
;

    .text
    .arm

;-------------------------------------------------------------------------------
; Initialize CPU Registers

    .def     _coreInitRegisters_
    .asmfunc
    
_coreInitRegisters_

        mov   r0,         lr
        mov   r1,         #0x03D0
        mov   r2,         #0x0000
        mov   r3,         #0x0000
        mov   r4,         #0x0000
        mov   r5,         #0x0000
        mov   r6,         #0x0000
        mov   r7,         #0x0000
        mov   r8,         #0x0000
        mov   r9,         #0x0000
        mov   r10,        #0x0000
        mov   r11,        #0x0000
        mov   r12,        #0x0000
        orr   r13,        r1,     #0x0001
        msr   cpsr_cxsf,  r13
        msr   spsr_cxsf,  r13
        mov   lr,         r0
        mov   r8,         #0x0000
        mov   r9,         #0x0000
        mov   r10,        #0x0000
        mov   r11,        #0x0000
        mov   r12,        #0x0000
        orr   r13,        r1,     #0x0002
        msr   cpsr_c,     r13
        msr   spsr_cxsf,  r13
        mov   lr,         r0
        orr   r13,        r1,     #0x0007
        msr   cpsr_c,     r13
        msr   spsr_cxsf,  r13
        mov   lr,         r0
        orr   r13,        r1,     #0x000B
        msr   cpsr_c,     r13
        msr   spsr_cxsf,  r13
        mov   lr,         r0
        orr   r13,        r1,     #0x0003
        msr   cpsr_c,     r13
        msr   spsr_cxsf,  r13
        bl    next1
next1
        bl    next2
next2
        bl    next3
next3
        bl    next4
next4
        bx    r0

    .endasmfunc


;-------------------------------------------------------------------------------
; Initialize Stack Pointers

    .def     _coreInitStackPointer_
    .asmfunc

_coreInitStackPointer_

        msr   cpsr_c,   #0xD1
        ldr   sp,       fiqSp
        msr   cpsr_c,   #0xD2
        ldr   sp,       irqSp
        msr   cpsr_c,   #0xD7
        ldr   sp,       abortSp
        msr   cpsr_c,   #0xDB
        ldr   sp,       undefSp
        msr   cpsr_c,   #0xDF
        ldr   sp,       userSp
        msr   cpsr_c,   #0xD3
        ldr   sp,       svcSp
        bx    lr

userSp  .word 0x08000000+0x00000B00
svcSp   .word 0x08000000+0x00000B00+0x00000100
fiqSp   .word 0x08000000+0x00000B00+0x00000100+0x00000100
irqSp   .word 0x08000000+0x00000B00+0x00000100+0x00000100+0x00000100
abortSp .word 0x08000000+0x00000B00+0x00000100+0x00000100+0x00000100+0x00000100
undefSp .word 0x08000000+0x00000B00+0x00000100+0x00000100+0x00000100+0x00000100+0x00000100

    .endasmfunc


;-------------------------------------------------------------------------------
; Enable Event Bus Export

    .def     _coreEnableEventBusExport_
    .asmfunc

_coreEnableEventBusExport_

        mrc   p15, #0x00, r0,         c9, c12, #0x00
        orr   r0,  r0,    #0x10
        mcr   p15, #0x00, r0,         c9, c12, #0x00
        bx    lr

    .endasmfunc


;-------------------------------------------------------------------------------
; Disable Event Bus Export

    .def     _coreDisableEventBusExport_
    .asmfunc

_coreDisableEventBusExport_

        mrc   p15, #0x00, r0,         c9, c12, #0x00
        bic   r0,  r0,    #0x10
        mcr   p15, #0x00, r0,         c9, c12, #0x00
        bx    lr

    .endasmfunc


;-------------------------------------------------------------------------------
; Enable RAM ECC Support

    .def     _coreEnableRamEcc_
    .asmfunc

_coreEnableRamEcc_

        mrc   p15, #0x00, r0,         c1, c0,  #0x01
        orr   r0,  r0,    #0x0C000000
        mcr   p15, #0x00, r0,         c1, c0,  #0x01
        bx    lr

    .endasmfunc


;-------------------------------------------------------------------------------
; Disable RAM ECC Support

    .def     _coreDisableRamEcc_
    .asmfunc

_coreDisableRamEcc_

        mrc   p15, #0x00, r0,         c1, c0,  #0x01
        bic   r0,  r0,    #0x0C000000
        mcr   p15, #0x00, r0,         c1, c0,  #0x01
        bx    lr

    .endasmfunc


;-------------------------------------------------------------------------------
; Enable Flash ECC Support

    .def     _coreEnableFlashEcc_
    .asmfunc

_coreEnableFlashEcc_

        mrc   p15, #0x00, r0,         c1, c0,  #0x01
        orr   r0,  r0,    #0x02000000
        mcr   p15, #0x00, r0,         c1, c0,  #0x01
        bx    lr

    .endasmfunc


;-------------------------------------------------------------------------------
; Disable Flash ECC Support

    .def     _coreDisableFlashEcc_
    .asmfunc

_coreDisableFlashEcc_

        mrc   p15, #0x00, r0,         c1, c0,  #0x01
        bic   r0,  r0,    #0x02000000
        mcr   p15, #0x00, r0,         c1, c0,  #0x01
        bx    lr

    .endasmfunc


;-------------------------------------------------------------------------------
; Enable Offset via Vic controller

    .def     _coreEnableIrqVicOffset_
    .asmfunc

_coreEnableIrqVicOffset_

        mrc   p15, #0, r0,         c1, c0,  #0
        orr   r0,  r0,    #0x01000000
        mcr   p15, #0, r0,         c1, c0,  #0
        bx    lr

    .endasmfunc


;-------------------------------------------------------------------------------
; Get data fault status register

    .def     _coreGetDataFault_
    .asmfunc

_coreGetDataFault_

        mrc   p15, #0, r0, c5, c0,  #0
        bx    lr

    .endasmfunc


;-------------------------------------------------------------------------------
; Clear data fault status register

    .def     _coreClearDataFault_
    .asmfunc

_coreClearDataFault_

        mov   r0,  #0
        mcr   p15, #0, r0, c5, c0,  #0
        bx    lr

    .endasmfunc


;-------------------------------------------------------------------------------
; Get instruction fault status register

    .def     _coreGetInstructionFault_
    .asmfunc

_coreGetInstructionFault_

        mrc   p15, #0, r0, c5, c0, #1
        bx    lr

    .endasmfunc


;-------------------------------------------------------------------------------
; Clear instruction fault status register

    .def     _coreClearInstructionFault_
    .asmfunc

_coreClearInstructionFault_

        mov   r0,  #0
        mcr   p15, #0, r0, c5, c0, #1
        bx    lr

    .endasmfunc


;-------------------------------------------------------------------------------
; Get data fault address register

    .def     _coreGetDataFaultAddress_
    .asmfunc

_coreGetDataFaultAddress_

        mrc   p15, #0, r0, c6, c0,  #0
        bx    lr

    .endasmfunc


;-------------------------------------------------------------------------------
; Clear data fault address register

    .def     _coreClearDataFaultAddress_
    .asmfunc

_coreClearDataFaultAddress_

        mov   r0,  #0
        mcr   p15, #0, r0, c6, c0,  #0
        bx    lr

    .endasmfunc


;-------------------------------------------------------------------------------
; Get instruction fault address register

    .def     _coreGetInstructionFaultAddress_
    .asmfunc

_coreGetInstructionFaultAddress_

        mrc   p15, #0, r0, c6, c0, #2
        bx    lr

    .endasmfunc


;-------------------------------------------------------------------------------
; Clear instruction fault address register

    .def     _coreClearInstructionFaultAddress_
    .asmfunc

_coreClearInstructionFaultAddress_

        mov   r0,  #0
        mcr   p15, #0, r0, c6, c0, #2
        bx    lr

    .endasmfunc


;-------------------------------------------------------------------------------
; Get auxiliary data fault status register

    .def     _coreGetAuxiliaryDataFault_
    .asmfunc

_coreGetAuxiliaryDataFault_

        mrc   p15, #0, r0, c5, c1, #0
        bx    lr

    .endasmfunc


;-------------------------------------------------------------------------------
; Clear auxiliary data fault status register

    .def     _coreClearAuxiliaryDataFault_
    .asmfunc

_coreClearAuxiliaryDataFault_

        mov   r0,  #0
        mcr   p15, #0, r0, c5, c1, #0
        bx    lr

    .endasmfunc


;-------------------------------------------------------------------------------
; Get auxiliary instruction fault status register

    .def     _coreGetAuxiliaryInstructionFault_
    .asmfunc

_coreGetAuxiliaryInstructionFault_

        mrc   p15, #0, r0, c5, c1, #1
        bx    lr

    .endasmfunc


;-------------------------------------------------------------------------------
; Clear auxiliary instruction fault status register

    .def     _coreClearAuxiliaryInstructionFault_
    .asmfunc

_coreClearAuxiliaryInstructionFault_

        mov   r0,  #0
        mrc   p15, #0, r0, c5, c1, #1
        bx    lr

    .endasmfunc
    
    

;-------------------------------------------------------------------------------

; C++ construct table pointers

    .global __TI_PINIT_Base, __TI_PINIT_Limit
    .weak   SHT$$INIT_ARRAY$$Base, SHT$$INIT_ARRAY$$Limit

__TI_PINIT_Base:  .long SHT$$INIT_ARRAY$$Base
__TI_PINIT_Limit: .long SHT$$INIT_ARRAY$$Limit


;-------------------------------------------------------------------------------
; Enable VFP Unit

    .def     _coreEnableVfp_
    .asmfunc

_coreEnableVfp_
 .if __TI_VFP_SUPPORT__
        mrc   p15,     #0x00,      r0,       c1, c0, #0x02
        orr   r0,      r0,         #0xF00000
        mcr   p15,     #0x00,      r0,       c1, c0, #0x02
        mov   r0,      #0x40000000
        fmxr  fpexc,   r0
 .endif
        bx    lr

    .endasmfunc

;-------------------------------------------------------------------------------
; Initialize memory

    .def     _memoryInit_
    .asmfunc

_memoryInit_
        
        ldr   r12, regMinitGcr    ; MINITGCR register pointer 
        mov   r4, #0xA
        str   r4, [r12]
        ldr   r4, ramInitMask     ; load RAM initialization mask
        str   r4, [r12, #4]
mloop
        ldr   r5, [r12, #12]
        tst   r5, #0x100
        beq   mloop
        mov   r4, #5
        str   r4, [r12]
        bx    lr
    
;ramInitMask   .word 0x217F ; MIBSPI1,2 and 5 RAM is initialized as soon as multi-buffered mode is enabled 
;ramInitMask   .word 0x215F
ramInitMask   .word 0x0007; RAM+DMA+VIM only due to problems with peripherial RAM initialisation
regMinitGcr   .word 0xFFFFFF5C

    .endasmfunc

    

;-------------------------------------------------------------------------------

