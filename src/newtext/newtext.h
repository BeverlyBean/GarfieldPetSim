#pragma once

#ifdef NEWTEXT_ASM
    #ifdef _LANGUAGE_C
        #undef _LANGUAGE_C
        #define ASSEMBLER
    #endif
#endif

#include <PR/os_cont.h>


#define NT_NOP 0
#define NT_SAY 1
#define NT_WAIT 2
#define NT_BTN 3
#define NT_ENDSAY 4
#define NT_DONE 0xF


#ifdef NEWTEXT_ASM

.macro newtextfile
.section .newtext
.endm

.macro say str
    .byte NT_SAY, 8,0,0
    .word NT_str_\@
    .section .rodata
    .balign 4
    .global NT_str_\@
    NT_str_\@:
        .asciiz "\str"
    .section .newtext
.endm

.macro wait frames
    .byte NT_WAIT, 4
    .half \frames
.endm

.macro button btn
    .byte NT_BTN, 4
    .half \btn
.endm

.macro endscene
    .byte NT_DONE, 4, 0,0
.endm

.macro endsay
    .byte NT_ENDSAY, 4, 0,0
.endm

#else


#endif
