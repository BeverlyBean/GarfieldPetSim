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
wait 12
say "!!!"
endsay

play SOUND_MARIO_YAHOO
color "255 255 0 255"
say "im stupid!"
endsay

keyboard myName

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
say "fully skipped with the B button!"
endsay
unskippable
say "this message is required!"
endsay
play SOUND_ACTION_TERRAIN_STEP







endscene


