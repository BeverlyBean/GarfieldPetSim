#include <ultra64.h>
#include "newtext.h"
#include "game/game_init.h"

static char NT_TextBuffer[2048];
static s32 NewText_TextCursor = -1;
static s32 NewText_TextSubCursor = -1;
static s32 NewText_TextLen = -1;

u16 NT_ReadController() {
    return gPlayer1Controller->buttonDown;
}

int NewText_RenderText(u8 *text) {
    if (NewText_TextCursor == -1) NewText_TextCursor = 0;
    if (NewText_TextSubCursor == -1) NewText_TextSubCursor = NewText_TextCursor;
    if (NewText_TextLen == -1) NewText_TextLen = strlen(text);

    NT_TextBuffer[NewText_TextSubCursor] = text[NewText_TextSubCursor - NewText_TextCursor];
    NT_TextBuffer[NewText_TextSubCursor + 1] = 0;


    if (NewText_TextSubCursor - NewText_TextCursor >= NewText_TextLen) {
        NewText_TextCursor = NewText_TextSubCursor;
        NewText_TextSubCursor = -1;
        return 1;
    } else {
        NewText_TextSubCursor += 1;
        return 0;
    }
}

void NT_KeepText(void) {
    print_text(40, 40, NT_TextBuffer);
}

int NewText_Parse(u8 *scene) {
    static u8 *NewText_Cursor = 0;
    static s32 NewText_FrameWait = -1;

    if (NewText_Cursor == 0) {
        NewText_Cursor = scene;
    }


    u8 nt_cmd = NewText_Cursor[0];
    u8 nt_cmdlen = NewText_Cursor[1];

    if (nt_cmd == NT_DONE) {
        NewText_Cursor = 0;
        return 0;
    }

    u32 proceed = 0;

    switch (nt_cmd) {
        case NT_SAY:
            // this is gonna be epic
            if (NewText_RenderText(*(u32 *)(NewText_Cursor + 4))) proceed = 1;
            break;
        case NT_ENDSAY:
            NewText_TextCursor = -1;
            NewText_TextLen = -1;
            proceed = 1;
            break;
        case NT_WAIT:
            if (NewText_FrameWait == -1) {
                NewText_FrameWait = *(u16*)(NewText_Cursor + 2);
            } else {
                if (--NewText_FrameWait == 0) {
                    proceed = 1;
                    NewText_FrameWait = -1;
                }
            }
            break;
        case NT_BTN:
            if (NT_ReadController() & *(u16*)(NewText_Cursor + 2)) {
                proceed = 1;
            }
            break;
    }

    if (proceed) {
        NewText_Cursor += nt_cmdlen;
    }

    NT_KeepText();

    return 1;
}

