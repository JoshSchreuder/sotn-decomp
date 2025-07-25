// SPDX-License-Identifier: AGPL-3.0-or-later
#include "dra.h"
#include "dra_bss.h"

u16 D_800A37D8[] = {0x8430, 0x843F};
u16 g_JosephsCloakColors[] = {0x8000, 0x8000, 0x8000, 0x8000};

u16 D_800A37E4[] = {0x8430, 0x843F, 0xD5CE, 0xE739};
u16 D_800A37EC[] = {0xBEBB, 0xFFFF, 0x8594, 0x873F};
u16 D_800A37F4[] = {0x994A, 0xA5CE, 0x8421, 0x9CE7};
u16 D_800A37FC[] = {0x8420, 0x8427, 0x8421, 0x9CE7};
u16 D_800A3804[] = {0xA4A8, 0xAD0A, 0xCC21, 0xFC42};
u16 D_800A380C[] = {0x910C, 0x996E, 0x8048, 0x80B2};
u16 D_800A3814[] = {0x90A1, 0x9102, 0xA821, 0xCC42};
u16 D_800A381C[] = {0x90C9, 0x94EB, 0x85E4, 0xA2CD};
u16 D_800A3824[] = {0x842C, 0x8C6F, 0x8018, 0x98DD};
u16 D_800A382C[] = {0x994A, 0xA5CE, 0x0000, 0xBC0F};
u16 D_800A3834[] = {0x8433, 0x843A, 0x0000, 0x0000, 0x0000,
                    0x0000, 0xCC21, 0xFC42, 0xA4A8, 0xAD0A};

u_long* D_800A3848[] = {GFX_TERMINATE()};

extern u8 text_item_up[];
extern u8 load_room_obj[];
extern u8 save_room_obj[];
extern u8 load_room_tile[];
extern u8 save_room_tile[];
extern u16 D_800D68D4[0x10];
extern u16 D_800D68F4[0x10];
extern u16 D_800D6914[0x10];
extern u16 D_800D6934[0x10];
extern u16 D_800D6954[0x10];
extern u16 D_800D6974[0x10];
extern u16 D_800D6994[0x10];
extern u16 D_800D69B4[0x10];
extern u16 D_800D69D4[0x10];
extern u16 D_800D69F4[0x10];
extern u16 D_800D6A14[0x10];
extern u16 D_800D6A34[0x10];
extern u16 D_800D6A54[0x40]; // UNUSED
extern u16 D_800D6AD4[0x100];
extern u16 D_800D6CD4[0x100];
extern u16 D_800D6ED4[0x100];
extern u16 D_800D70D4[0x100];
extern u16 D_800D72D4[0x100];
extern u16 D_800DB0D4[0x10];
extern u16 D_800DB0F4[0x10];
extern u16 D_800DB114[0x70];
extern u16 D_800DB1F4[0xE0];

#ifdef VERSION_PSP

u8 D_psp_09189E60[] = {
#include "../dra/gen/D_psp_09189E60.h"
};

u8 D_psp_0918A690[] = {
#include "../dra/gen/D_psp_0918A690.h"
};

u8 D_psp_0918ABA8[] = {
#include "../dra/gen/D_psp_0918ABA8.h"
};

u8 D_psp_0918B1A0[] = {
#include "../dra/gen/D_psp_0918B1A0.h"
};

#endif

u_long* D_800A384C[] = {
    (u_long*)GFX_BANK_COMPRESSED,
    GFX_ENTRY(256, 800, 128, 128, text_item_up),
    GFX_TERMINATE(),
};

#ifdef VERSION_PSP

u_long* D_psp_0918B848[] = {
    (u_long*)GFX_BANK_COMPRESSED,
    GFX_ENTRY(256, 800, 128, 128, D_psp_0918B1A0),
    GFX_TERMINATE(),
};

u_long* D_psp_0918B860[] = {
    (u_long*)GFX_BANK_COMPRESSED,
    GFX_ENTRY(256, 800, 128, 128, D_psp_0918A690),
    GFX_TERMINATE(),
};

u_long* D_psp_0918B878[] = {
    (u_long*)GFX_BANK_COMPRESSED,
    GFX_ENTRY(256, 800, 128, 128, D_psp_0918ABA8),
    GFX_TERMINATE(),
};

u_long* D_psp_0918B890[] = {
    (u_long*)GFX_BANK_COMPRESSED,
    GFX_ENTRY(256, 800, 128, 128, D_psp_09189E60),
    GFX_TERMINATE(),
};

#endif

u_long* g_SaveRoomGfx[] = {
    (u_long*)GFX_BANK_COMPRESSED,
    GFX_ENTRY(256, 800, 128, 128, save_room_obj),
    GFX_ENTRY(368, 992, 80, 16, save_room_tile),
    GFX_TERMINATE(),
};

u_long* g_LoadRoomGfx[] = {
    (u_long*)GFX_BANK_COMPRESSED,
    GFX_ENTRY(256, 800, 128, 128, load_room_obj),
    GFX_ENTRY(368, 992, 80, 16, load_room_tile),
    GFX_TERMINATE(),
};

u16 D_800A38A0[] = {
    0x0000, 0xA108, 0xC210, 0xCA92, 0xD294, 0xDF97, 0xE739, 0xF39C,
    0xFBDE, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
};

u_long* D_800A38C0[] = {
    PAL_COPY_INFO(),
    PAL_COPY_DATA(0x1A10, D_800A38A0),
    PAL_TERMINATE(),
};

u_long* D_800A38D4[] = {
    MAKE_PAL_OP(PAL_GLOW_ANIM, 6),
    PAL_GLOW_INFO(0x1721, 1),
    PAL_GLOW_DATA(&D_800A37E4[0]),
    PAL_GLOW_DATA(&D_800A37E4[1]),
    PAL_TERMINATE(),
};

u_long* D_800A38EC[] = {
    PAL_COPY_INFO(),
    PAL_COPY_DATA_(0x171E, &D_800A37E4[2], 2),
};

u_long* D_800A38FC[] = {
    PAL_COPY_INFO(),
    PAL_COPY_DATA_(0x171E, &D_800A37EC[0], 2),
};

u_long* D_800A390C[] = {
    PAL_COPY_INFO(),
    PAL_COPY_DATA_(0x171E, &D_800A37EC[2], 2),
};

u_long* D_800A391C[] = {
    MAKE_PAL_OP(PAL_UNK_OP3, 4),
    PAL_UNK_OP3_INFO(0x1700, LEN(D_800D68D4)),
    PAL_UNK_OP3_DATA(D_800D68D4),
    PAL_UNK_OP3_DATA(D_800D68F4),
    PAL_TERMINATE(),
};

u_long* D_800A3934[] = {
    MAKE_PAL_OP(PAL_UNK_OP3, 4),  PAL_UNK_OP3_INFO(0x1600, LEN(D_800D6AD4)),
    PAL_UNK_OP3_DATA(D_800D6AD4), PAL_UNK_OP3_DATA(D_800D6CD4),
    PAL_UNK_OP3_DATA(D_800D6ED4), PAL_UNK_OP3_DATA(D_800D70D4),
    PAL_UNK_OP3_DATA(D_800D72D4), PAL_TERMINATE(),
};

u_long* D_800A3958[] = {
    MAKE_PAL_OP(PAL_UNK_OP3, 4),  PAL_UNK_OP3_INFO(0x17E0, LEN(D_800D6914)),
    PAL_UNK_OP3_DATA(D_800D6914), PAL_UNK_OP3_DATA(D_800D6934),
    PAL_UNK_OP3_DATA(D_800D6954), PAL_UNK_OP3_DATA(D_800D6974),
    PAL_UNK_OP3_DATA(D_800D6994), PAL_TERMINATE(),
};

u_long* D_800A397C[] = {
    MAKE_PAL_OP(PAL_UNK_OP3, 4),  PAL_UNK_OP3_INFO(0x17F0, LEN(D_800D69B4)),
    PAL_UNK_OP3_DATA(D_800D69B4), PAL_UNK_OP3_DATA(D_800D69D4),
    PAL_UNK_OP3_DATA(D_800D69F4), PAL_UNK_OP3_DATA(D_800D6A14),
    PAL_UNK_OP3_DATA(D_800D6A34), PAL_TERMINATE(),
};

u_long* D_800A39A0[] = {
    PAL_COPY_INFO(),
    PAL_COPY_DATA_(0x1006, D_800A37F4, 4),
};

u_long* D_800A39B0[] = {
    PAL_COPY_INFO(),
    PAL_COPY_DATA_(0x1008, &D_800A37FC[2], 2),
};

u_long* D_800A39C0[] = {
    PAL_COPY_INFO(),
    PAL_COPY_DATA_(0x1006, D_800A3804, 4),
};

u_long* D_800A39D0[] = {
    PAL_COPY_INFO(),
    PAL_COPY_DATA_(0x1006, &D_800A3834[6], 4),
};

u_long* D_800A39E0[] = {
    PAL_COPY_INFO(),
    PAL_COPY_DATA_(0x1006, D_800A380C, 4),
};

u_long* D_800A39F0[] = {
    PAL_COPY_INFO(),
    PAL_COPY_DATA_(0x1006, D_800A3814, 4),
};

u_long* D_800A3A00[] = {
    PAL_COPY_INFO(),
    PAL_COPY_DATA_(0x1006, D_800A381C, 4),
};

u_long* D_800A3A10[] = {
    PAL_COPY_INFO(),
    PAL_COPY_DATA_(0x1006, D_800A3824, 4),
};

u_long* D_800A3A20[] = {
    MAKE_PAL_OP(PAL_UNK_OP3, 1),
    PAL_UNK_OP3_INFO(0x1006, 4),
    PAL_UNK_OP3_DATA(D_800A382C),
    PAL_UNK_OP3_DATA(&D_800A3834[2]),
    PAL_TERMINATE(),
};

u_long* D_800A3A38[] = {
    PAL_COPY_INFO(),
    PAL_COPY_DATA(0x1006, g_JosephsCloakColors),
};

u_long* D_800A3A48[] = {
    MAKE_PAL_OP(PAL_GLOW_ANIM, 2),
    PAL_GLOW_INFO(0x1006, 2),
    PAL_GLOW_DATA(D_800A37FC),
    PAL_GLOW_DATA(&D_800A3834[0]),
    PAL_TERMINATE(),
};

u_long* D_800A3A60[] = {
    PAL_COPY_INFO(),
    PAL_COPY_DATA(0x0000, D_800DB0D4),
    PAL_TERMINATE(),
};

u_long* D_800A3A74[] = {
    PAL_COPY_INFO(),
    PAL_COPY_DATA(0x1F00, D_800DB0F4),
    PAL_TERMINATE(),
};

u_long* D_800A3A88[] = {
    PAL_COPY_INFO(),
    PAL_COPY_DATA_(0x1F20, D_800D6AD4 + 64, 16),
    PAL_TERMINATE(),
};

u_long* D_800A3A9C[] = {
    MAKE_PAL_OP(PAL_GLOW_ANIM, 1),
    PAL_GLOW_INFO(0x1F20, 16),
    PAL_GLOW_DATA(D_800D6AD4 + 160),
    PAL_GLOW_DATA(D_800D6AD4 + 64),
    PAL_TERMINATE(),
};

u16 D_800A3AB4[] = {0x8076, 0x80FB, 0x8048, 0x80B1};

u_long* D_800A3ABC[] = {
    MAKE_PAL_OP(PAL_UNK_OP2, 1),
    PAL_UNK_OP2_INFO(0x1F0E, 2),
    PAL_UNK_OP2_DATA(&D_800A3AB4[0]),
    PAL_UNK_OP2_DATA(&D_800A3AB4[2]),
    PAL_TERMINATE(),
};

u_long* D_800A3AD4[] = {
    PAL_COPY_INFO(),
    PAL_COPY_DATA(0x1D00, D_800DB114),
    PAL_TERMINATE(),
};

u_long* D_800A3AE8[] = {
    MAKE_PAL_OP(PAL_GLOW_ANIM, 2),
    PAL_GLOW_INFO(0x100A, 1),
    PAL_GLOW_DATA(&D_800A37D8[0]),
    PAL_GLOW_DATA(&D_800A37D8[1]),
    PAL_TERMINATE(),
};

u_long* D_800A3B00[] = {
    PAL_COPY_INFO(),
    PAL_COPY_DATA_(0x10F0, &g_Clut[1][0xA0], 16),
};

u_long* D_800A3B10[] = {
    MAKE_PAL_OP(PAL_UNK_OP2, 2),
    PAL_UNK_OP2_INFO(0x10F0, 16),
    PAL_UNK_OP2_DATA(&g_Clut[1][0xC0]),
    PAL_TERMINATE(),
};

u_long* D_800A3B24[] = {
    MAKE_PAL_OP(PAL_UNK_OP2, 2),
    PAL_UNK_OP2_INFO(0x10F0, 16),
    PAL_UNK_OP2_DATA(&g_Clut[1][0]),
    PAL_TERMINATE(),
};

u_long* D_800A3B38[] = {
    MAKE_PAL_OP(PAL_UNK_OP2, 2),
    PAL_UNK_OP2_INFO(0x10F0, 16),
    PAL_UNK_OP2_DATA(&g_Clut[1][0xB0]),
    PAL_TERMINATE(),
};

u_long* D_800A3B4C[] = {
    PAL_COPY_INFO(),
    PAL_COPY_DATA(0x1000, D_800DB1F4),
};

#ifdef VERSION_PSP

u_long** D_psp_0918BBD0[] = {
    D_800A3848, D_800A384C, D_800A3848, g_SaveRoomGfx, g_LoadRoomGfx};
u_long** D_psp_0918BBE8[] = {
    D_800A3848, D_psp_0918B890, D_800A3848, g_SaveRoomGfx, g_LoadRoomGfx};
u_long** D_psp_0918BC00[] = {
    D_800A3848, D_psp_0918B860, D_800A3848, g_SaveRoomGfx, g_LoadRoomGfx};
u_long** D_psp_0918BC18[] = {
    D_800A3848, D_psp_0918B848, D_800A3848, g_SaveRoomGfx, g_LoadRoomGfx};
u_long** D_psp_0918BC30[] = {
    D_800A3848, D_psp_0918B878, D_800A3848, g_SaveRoomGfx, g_LoadRoomGfx};

#else
GfxBank** g_GfxSharedBank[] = {
    D_800A3848, D_800A384C, D_800A3848, g_SaveRoomGfx, g_LoadRoomGfx,
};
#endif

extern s16* D_800CF324[]; // AluFrame
extern s16* D_800CFF10[]; // SpriteParts
extern s16* D_800D0F68[]; // SpriteParts
extern s16* D_800D2BF4[]; // SpriteParts
extern s16* D_800D2CF8[]; // SpriteParts
extern s16* D_800D2FFC[]; // SpriteParts
extern s16* D_800D3428[]; // SpriteParts
extern s16* D_800D34B8[]; // SpriteParts
extern s16* D_800D372C[]; // SpriteParts
extern s16* D_800D3B88[]; // SpriteParts
extern s16* D_800D5628[]; // SpriteParts
extern s16* D_800D327C[]; // SpriteParts
extern s16* D_800CFE48[]; // AluFrame
extern s16* D_800D56AC[]; // SpriteParts
extern s16* D_800D5868[]; // SpriteParts
extern s16* D_800D684C[]; // SpriteParts

s16** D_800A3B70[] = {
    NULL,       D_800CF324, D_800CFF10, D_800D0F68, D_800D2BF4, D_800D2CF8,
    D_800D2FFC, D_800D3428, D_800D34B8, D_800D372C, D_800D3B88, D_800D5628,
    D_800D327C, D_800CFE48, D_800D56AC, D_800D5868, D_800D5868, D_800D684C,
};

u_long* D_800A3BB8[] = {
    (u_long*)D_800A38C0, (u_long*)D_800A38EC, (u_long*)D_800A38FC,
    (u_long*)D_800A390C, (u_long*)D_800A391C, (u_long*)D_800A3934,
    (u_long*)D_800A3958, (u_long*)D_800A397C, (u_long*)D_800A38D4,
    (u_long*)D_800A39A0, (u_long*)D_800A39B0, (u_long*)D_800A39C0,
    (u_long*)D_800A39D0, (u_long*)D_800A39F0, (u_long*)D_800A3A00,
    (u_long*)D_800A3A10, (u_long*)D_800A3A20, (u_long*)D_800A3A38,
    (u_long*)D_800A39E0, (u_long*)D_800A39A0, (u_long*)D_800A39A0,
    (u_long*)D_800A3A48, (u_long*)D_800A3B4C, (u_long*)D_800A3AE8,
    (u_long*)D_800A3A74, (u_long*)D_800A3A88, (u_long*)D_800A3A60,
    (u_long*)D_800A3B00, (u_long*)D_800A3B10, (u_long*)D_800A3B24,
    (u_long*)D_800A3A9C, (u_long*)D_800A3B38, (u_long*)D_800A3ABC,
    (u_long*)D_800A3AD4,
};
