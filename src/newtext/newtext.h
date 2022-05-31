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
#define NT_MENU 5
#define NT_GO 6
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
    NT_str_\@:
        .asciiz "\str"
    .section .newtext
.endm

.macro menu title, ch1, lb1, ch2, lb2, ch3, lb3, ch4, lb4
    .section .rodata
    .balign 4
    NT_menutitle_\@:
        .asciiz "\title"
    .balign 4
    NT_menuchoice1_\@:
        .asciiz "\ch1"
    .balign 4
    NT_menuchoice2_\@:
        .asciiz "\ch2"
    .balign 4
    NT_menuchoice3_\@:
        .asciiz "\ch3"
    .balign 4
    NT_menuchoice4_\@:
        .asciiz "\ch4"
    .section .newtext

    .byte NT_MENU, 40, 0, 0
    .word NT_menutitle_\@
    .word NT_menuchoice1_\@, \lb1
    .word NT_menuchoice2_\@, \lb2
    .word NT_menuchoice3_\@, \lb3
    .word NT_menuchoice4_\@, \lb4
.endm

.macro wait frames
    .byte NT_WAIT, 4
    .half \frames
.endm

.macro button btn
    .byte NT_BTN, 4
    .half \btn
.endm

.macro go lbl
    .byte NT_GO, 8, 0, 0
    .word \lbl
.endm

.macro endscene
    .byte NT_DONE, 4, 0,0
.endm

.macro endsay
    .byte NT_ENDSAY, 4, 0,0
.endm

#else


#endif
