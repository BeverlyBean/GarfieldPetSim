#include <ultra64.h>
#include "newtext.h"
#include "sounds.h"
#include "game/game_init.h"
#include "audio/external.h"

static char NT_TextBuffer[2048];
static s32 NewText_TextCursor = -1;
static s32 NewText_TextSubCursor = -1;
static s32 NewText_TextLen = -1;

static u8 *NewText_Cursor = 0;
static s32 NewText_FrameWait = -1;

static int NewText_X = 20;
static int NewText_Y = 20;

static u32 NewText_TextSound = 0;

static u32 read_u32(u8 *d) {
    return *(u32*)d;
}

u16 NT_ReadController() {
    return gPlayer1Controller->buttonPressed;
}
s8 NT_ReadStick() {
    return gPlayer1Controller->stickY;
}

void NewText_CopyRest(u8 *text) {
    // strcpy(NT_TextBuffer + , text, )
}

int NewText_RenderText(u8 *text) {
    if (NewText_TextCursor == -1) NewText_TextCursor = 0;
    if (NewText_TextSubCursor == -1) NewText_TextSubCursor = NewText_TextCursor;
    if (NewText_TextLen == -1) NewText_TextLen = strlen(text);

    NT_TextBuffer[NewText_TextSubCursor] = text[NewText_TextSubCursor - NewText_TextCursor];
    NT_TextBuffer[NewText_TextSubCursor + 1] = 0;

    play_sound(NewText_TextSound, gGlobalSoundSource);

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

void NT_RenderMenu(u8 *cursor) {
    static u32 sticklatch = 0;
    static s32 curpos = 0;

    char *title = read_u32(cursor + 4);
    char *ch1 = read_u32(cursor + 8);
    u8 *labels[4];
    labels[0] = read_u32(cursor + 12);
    char *ch2 = read_u32(cursor + 16);
    labels[1] = read_u32(cursor + 20);
    char *ch3 = read_u32(cursor + 24);
    labels[2] = read_u32(cursor + 28);
    char *ch4 = read_u32(cursor + 32);
    labels[3] = read_u32(cursor + 36);



    // cursor
    print_text(NewText_X - 16, NewText_Y + (16 * (curpos + 1)), "*");

    // text print
    print_text(NewText_X, NewText_Y, title);
    print_text(NewText_X + 8, NewText_Y + 16, ch1);
    print_text(NewText_X + 8, NewText_Y + 32, ch2);
    print_text(NewText_X + 8, NewText_Y + 48, ch3);
    print_text(NewText_X + 8, NewText_Y + 64, ch4);

    // cursor movement
    s8 stick = NT_ReadStick();
    if ((stick >= -14) && (stick <= 14)) {
        sticklatch = 0;
    }
    if (sticklatch == 0) {
        if (stick < -14) {
            curpos--;
            sticklatch = 1;
        }
        if (stick > 14) {
            curpos++;
            sticklatch = 1;
        }
    }

    // print_text_fmt_int(100, 100, "%d", curpos);
    // print_text_fmt_int(116, 100, "%d", sticklatch);
    // print_text_fmt_int(132, 100, "%d", stick);

    // // cursor clamp
    if (curpos < 0) curpos = 0;
    if (curpos > 3) curpos = 3;

    // // if cursor goto label
    if (NT_ReadController() & A_BUTTON) {
        NewText_Cursor = labels[curpos];
    }
}

int NewText_Parse(u8 *scene) {
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
        case NT_MENU:
            NewText_TextCursor = -1;
            NewText_TextLen = -1;
            NT_TextBuffer[0] = 0;
            // does not have a proceed condition because its a branch
            NT_RenderMenu(NewText_Cursor);
            break;
        case NT_GO:
            NewText_Cursor = *(u32*)(NewText_Cursor + 4);
            break;
        case NT_TURING:
            u32 fp = *(u32*)(NewText_Cursor + 4);
            if (((int (*)())fp)());
            proceed = 1;
            break;
        case NT_RECALL:
            if (NewText_RenderText(*(u32 *)(NewText_Cursor + 4))) proceed = 1;
            break;
        case NT_SOUND:
            play_sound(*(u32 *)(NewText_Cursor + 4), gGlobalSoundSource);
            proceed = 1;
            break;
        case NT_TXTSOUND:
            NewText_TextSound = *(u32 *)(NewText_Cursor + 4);
            proceed = 1;
            break;

    }

    if (proceed) {
        NewText_Cursor += nt_cmdlen;
    }
    append_puppyprint_log("%x", SOUND_MARIO_YAHOO);

    NT_KeepText();

    return 1;
}

