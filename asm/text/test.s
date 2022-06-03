#define NEWTEXT_ASM
#include "newtext.h"
#include "sounds_asm.h"

scene NT_Testscene1

maketextbox 255 255 255 255

textsound SOUND_ACTION_TERRAIN_STEP

align left

textpos top
color "0 0 0 255"
say "hello world"
// wait 12
say "!!!"
endsay

textpos middle
color "0 0 0 255"
play SOUND_MARIO_YAHOO
say "im stupid!"
endsay

keyboard myName

align left
textpos bottom
maketextbox 255 0 0 255
color "0 0 0 255"
menu "Select a choice!", \
     "games", pickA, \
     "tv", pickB, \
     "anime", pickC, \
     "being lazy", pickD

pickA:
    say "wrong!"
    go myTextEnd

pickB:
    say "wrong!"
    go myTextEnd

pickC:
    say "Correct!"
    go myTextEnd

pickD:
    say "please dont say you are lazy!"
    go myTextEnd


myTextEnd:
say " "
recall myName
endsay // optimization built right in!

say "This message can be fully shown with the A button, or..."
endsay
say "fully skipped with\n"
color "255 0 0 255"
say "the B button!"
endsay
unskippable
say "this message is required!"
endsay
play SOUND_ACTION_TERRAIN_STEP







endscene


