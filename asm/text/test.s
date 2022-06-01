#define NEWTEXT_ASM
#include "newtext.h"
#include "sounds_asm.h"

newtextfile

.balign 4
.global NT_Testscene1
NT_Testscene1:


// now that the boilerplate is done

textsound SOUND_ACTION_TERRAIN_STEP

say "hello world"
wait 30
say "!!!"
endsay

button A_BUTTON
// play SOUND_MARIO_YAHOO
say "im stupid!"
endsay
button A_BUTTON

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
endsay // optimization built right in!
button A_BUTTON
endscene


