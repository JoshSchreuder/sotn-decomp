#include "common.h"
#include <libgpu.h>

const char a0123456789abcd[] = "0123456789ABCDEF";

typedef struct {
    /* 0x00 */ TILE tile;
    /* 0x10 */ DR_MODE draw_mode;
    /* 0x1C */ u32 capacity;
    /* 0x20 */ SPRT_8* sprites;
    /* 0x24 */ char* buffer;
    /* 0x28 */ s32 written;
} FntStream;

extern s32 D_8002B810;
extern s32 D_8002B814;
extern int (*GPU_printf)(const char*, ...);

void SetDumpFnt(int id) {
    if (id >= 0 && D_8002B810 >= id) {
        D_8002B814 = id;
        GPU_printf = FntPrint;
    }
}

extern u_long D_8002B818[];
extern FntStream Font[4];
extern u16 clut;
extern u16 tpage;

void FntLoad(s32 tx, s32 ty) {
    clut = LoadClut(D_8002B818, tx, ty + 0x80);
    tpage = LoadTPage(D_8002B818 + 0x80, 0, 0, tx, ty, 0x80, 0x20);
    D_8002B810 = 0;
    memset(Font, 0, sizeof(Font));
}

INCLUDE_ASM("main/nonmatchings/psxsdk/libgpu/font", FntOpen);

u_long* FntFlush(s32 id) {
    DR_MODE* dr;
    s32 x, y;
    s32 max_x, max_y;
    s32 len;
    bool line_break;
    s32 ch;
    char* buf;
    FntStream* font;
    SPRT_8* sprt;
    u8 u, v;

    if (id < 0 || id >= D_8002B810) {
        id = D_8002B814;
        if (Font[id].buffer == NULL) {
            return NULL;
        }
    }
    font = &Font[id];
    dr = &font->draw_mode;
    buf = font->buffer;
    sprt = font->sprites;
    len = font->capacity;
    x = font->tile.x0;
    y = font->tile.y0;
    max_x = x + font->tile.w;
    max_y = y + font->tile.h;
    TermPrim(dr);
    while (*buf != 0) {
        if (len == 0) {
            break;
        }
        line_break = false;
        switch (*buf) {
        case '\n':
            line_break = true;
            break;

        case '\t':
            x += 0x20;
            if (x >= max_x) {
                line_break = true;
            }
            break;

        case ' ':
            x += 8;
            if (x >= max_x) {
                line_break = true;
            }
            break;

        default:
            if (*buf >= 'a' && *buf <= 'z') {
                ch = *buf - 0x40;
            } else {
                ch = *buf - 0x20;
            }
            u = (ch % 16) * 8;
            v = (ch / 16) * 8;
            sprt->u0 = u;
            sprt->v0 = v;
            sprt->x0 = x;
            sprt->y0 = y;
            AddPrim(dr, &sprt->tag);
            x += 8;
            if (x >= max_x) {
                line_break = true;
            }
            break;
        }
        if (line_break) {
            x = font->tile.x0;
            y += 8;
            if (y >= max_y) {
                break;
            }
        }
        sprt++;
        buf++;
        len--;
    }
    if (font->tile.code != 0) {
        AddPrim(dr, &font->tile.tag);
    }
    DrawOTag((u_long*)dr);
    font->written = 0;
    *font->buffer = 0;
    return (u_long*)dr;
}

INCLUDE_ASM("main/nonmatchings/psxsdk/libgpu/font", FntPrint);
