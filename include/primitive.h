/**
 * Standard Primitive and its variants
 */

#ifndef PRIMITIVE_H
#define PRIMITIVE_H
#include "common.h"

typedef enum {
    PRIORITY_DIALOGUE = 0x1FE,
} PrimitivePriority;

// Macros for simplifying long stretches setting prim colors.
// "prim red". Do PRED(prim) = 0x80 to set all reds to 0x80.
#define PRED(p) p->r0 = p->r1 = p->r2 = p->r3
// "prim green". Do PGRN(prim) = 0x80 to set all greens to 0x80.
#define PGRN(p) p->g0 = p->g1 = p->g2 = p->g3
// "prim blue". Do PBLU(prim) = 0x80 to set all blues to 0x80.
#define PBLU(p) p->b0 = p->b1 = p->b2 = p->b3
// "prim color". Do PCOL(prim) =  0x80 to set all RGBs to 0x80.
#define PCOL(p) PRED(p) = PGRN(p) = PBLU(p)

// Alternatively, set a point in a prim to a given shade of grey.
// prim->r0 = prim->g0 = prim->b0 = 255 becomes
// PGREY(prim, 0) = 255
#define PGREY(p, n) p->r##n = p->g##n = p->b##n
// TODO: Merge these two macros
// This one was created to work for both PSP and PSX, but touches
// enough files to justify a separate PR to change the original.
#define PGREY_ALT(p, n, v)                                                     \
    p->r##n = v;                                                               \
    p->g##n = v;                                                               \
    p->b##n = v;

typedef enum {
    PRIM_NONE,
    PRIM_TILE,
    PRIM_LINE_G2,
    PRIM_G4,
    PRIM_GT4,
    PRIM_GT3,
    PRIM_SPRT,
    PRIM_ENV,
    PRIM_TILE_ALT = PRIM_TILE | 0x10,
    PRIM_LINE_G2_ALT = PRIM_LINE_G2 | 0x10,
    PRIM_G4_ALT = PRIM_G4 | 0x10
} PrimitiveType;

typedef struct Primitive {
    /* 0x00 */ struct Primitive* next;
#if defined(VERSION_PC) || defined(VERSION_PSP)
    u_long dummy;
#endif
    /* 0x04 */ u8 r0;
    /* 0x05 */ u8 g0;
    /* 0x06 */ u8 b0;
    /* 0x07 */ u8 type; // PrimitiveType
    /* 0x08 */ s16 x0;
    /* 0x0A */ s16 y0;
    /* 0x0C */ u8 u0;    // w for PrimitiveType::TILE
    /* 0x0D */ u8 v0;    // h for PrimitiveType::TILE
    /* 0x0E */ u16 clut; // TODO not verified
    /* 0x10 */ u8 r1;
    /* 0x11 */ u8 g1;
    /* 0x12 */ u8 b1;
    /* 0x13 */ u8 p1; // TODO not verified
    /* 0x14 */ s16 x1;
    /* 0x16 */ s16 y1;
    /* 0x18 */ u8 u1;     // TODO not verified
    /* 0x19 */ u8 v1;     // TODO not verified
    /* 0x1A */ u16 tpage; // TODO not verified
    /* 0x1C */ u8 r2;
    /* 0x1D */ u8 g2;
    /* 0x1E */ u8 b2;
    /* 0x1F */ u8 p2; // TODO not verified
    /* 0x20 */ s16 x2;
    /* 0x22 */ s16 y2;
    /* 0x24 */ u8 u2; // TODO not verified
    /* 0x25 */ u8 v2; // TODO not verified
    /* 0x26 */ u16 priority;
    /* 0x28 */ u8 r3;
    /* 0x29 */ u8 g3;
    /* 0x2A */ u8 b3;
    /* 0x2B */ u8 p3; // TODO not verified (CHI uses this like a "step" value)
    /* 0x2C */ s16 x3;
    /* 0x2E */ s16 y3;
    /* 0x30 */ u8 u3; // TODO not verified
    /* 0x31 */ u8 v3; // TODO not verified
    /* 0x32 */ u16 drawMode;
} Primitive; /* size=0x34 */

// FakePrim is really the wrong name for this.
// But it's an alternate use of the Primitive structure.
typedef struct FakePrim {
    struct FakePrim* next;
#if defined(VERSION_PC) || defined(VERSION_PSP)
    u_long dummy;
#endif
    /* 0x04 */ u8 r0;
    /* 0x05 */ u8 g0;
    /* 0x06 */ u8 b0;
    /* 0x07 */ u8 type; // PrimitiveType
    /* 0x08 */ s16 x0;
    /* 0x0A */ s16 y0;
    /* 0x0C */ u8 w;
    /* 0x0D */ u8 h;
    /* 0x0E */ u16 clut;
    /* 0x10 */ f32 posX;
    /* 0x14 */ f32 posY;
    /* 0x18 */ f32 velocityX;
    /* 0x1C */ f32 velocityY;
    /* 0x20 */ s16 x2;
    /* 0x22 */ s16 y2;
    /* 0x24 */ s16 delay;
    /* 0x26 */ u16 priority;
    /* 0x28 */ f32 accelerationX;
    /* 0x2C */ f32 accelerationY;
    /* 0x30 */ s16 timer;
    /* 0x32 */ u16 drawMode;
} FakePrim;

// Not actually sure if this is for LineG2, but the only time LineG2
// is used is also the only time this struct is used, so we will call
// it this for now. That one use is in RicEntityCrashHydroStorm.
typedef struct PrimLineG2 {
    struct PrimLineG2* next;
#if defined(VERSION_PC) || defined(VERSION_PSP)
    u_long dummy;
#endif
    /* 0x04 */ u8 r0;
    /* 0x05 */ u8 g0;
    /* 0x06 */ u8 b0;
    /* 0x07 */ u8 type;
    /* 0x08 */ s16 x0;
    /* 0x0A */ s16 y0;
    /* 0x0C */ s16 xLength;
    /* 0x0E */ s16 yLength;
    /* 0x10 */ u8 r1;
    /* 0x11 */ u8 g1;
    /* 0x12 */ u8 b1;
    /* 0x13 */ u8 p1;
    /* 0x14 */ s16 x1;
    /* 0x16 */ s16 y1;
    /* 0x18 */ f32 velocityX;
    /* 0x1C */ f32 velocityY;
    /* 0x20 */ s32 angle;
    /* 0x24 */ s16 delay;
    /* 0x26 */ u16 priority;
    /* 0x28 */ f32 preciseX;
    /* 0x2C */ f32 preciseY;
    /* 0x30 */ s16 timer;
    /* 0x32 */ u16 drawMode;
} PrimLineG2;

typedef struct AxePrim {
    struct AxePrim* next;
    /* 0x04 */ u8 r0;
    /* 0x05 */ u8 g0;
    /* 0x06 */ u8 b0;
    /* 0x07 */ u8 type; // PrimitiveType
    /* 0x08 */ s16 x0;
    /* 0x0A */ s16 y0;
    /* 0x0C */ s32 unk0C;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s16 x1;
    /* 0x16 */ s16 y1;
    /* 0x18 */ u8 u1;
    /* 0x19 */ u8 v1;
    /* 0x1A */ u16 tpage;
    /* 0x1C */ s16 unk1C;
    /* 0x1E */ s16 unk1E;
    /* 0x20 */ s16 x2;
    /* 0x22 */ s16 y2;
    /* 0x24 */ u8 step;
    /* 0x26 */ u16 priority;
    /* 0x28 */ s32 pad;
    /* 0x2C */ s16 timer;
    /* 0x2E */ s16 unk2E;
    /* 0x30 */ u8 u3;
    /* 0x31 */ u8 v3;
    /* 0x32 */ u16 drawMode;
} AxePrim;

// If we find more uses, can get a better name.
typedef struct EntranceCascadePrim {
    /* 0x00 */ struct EntranceCascadePrim* next;
#if defined(VERSION_PC) || defined(VERSION_PSP)
    u_long dummy;
#endif
    /* 0x04 */ s32 : 32;
    /* 0x08 */ s16 x0;
    /* 0x0A */ s16 y0;
    /* 0x0C */ s32 : 32;
    /* 0x10 */ s32 : 32;
    /* 0x14 */ s16 x1;
    /* 0x16 */ s16 y1;
    /* 0x18 */ s32 : 32;
    /* 0x1C */ s32 : 32;
    /* 0x20 */ f32 velocityY;
    /* 0x24 */ s32 : 32;
    /* 0x28 */ s32 : 24;
    /* 0x2B */ u8 p3; // TODO not verified
    /* 0x2C */ f32 velocityX;
    /* 0x30 */ s16 : 16;
    /* 0x32 */ u16 drawMode;
} EntranceCascadePrim; /* size=0x34 */
#endif

typedef struct FrozenShadePrim {
    /* 0x00 */ struct FrozenShadePrim* next;
#if defined(VERSION_PC) || defined(VERSION_PSP)
    u_long dummy;
#endif
    /* 0x04 */ u8 r0;
    /* 0x05 */ u8 g0;
    /* 0x06 */ u8 b0;
    /* 0x07 */ u8 type; // PrimitiveType
    /* 0x08 */ s16 x0;
    /* 0x0A */ s16 y0;
    /* 0x0C */ f32 posX; // TODO not verified
    /* 0x10 */ u8 r1;
    /* 0x11 */ u8 g1;
    /* 0x12 */ u8 b1;
    /* 0x13 */ u8 p1; // TODO not verified
    /* 0x14 */ s16 x1;
    /* 0x16 */ s16 y1;
    /* 0x18 */ f32 posY; // TODO not verified
    /* 0x1C */ u8 r2;
    /* 0x1D */ u8 g2;
    /* 0x1E */ u8 b2;
    /* 0x1F */ u8 p2; // TODO not verified
    /* 0x20 */ s16 x2;
    /* 0x22 */ s16 y2;
    /* 0x24 */ u8 u2; // TODO not verified
    /* 0x25 */ u8 v2; // TODO not verified
    /* 0x26 */ u16 priority;
    /* 0x28 */ u8 r3;
    /* 0x29 */ u8 g3;
    /* 0x2A */ u8 b3;
    /* 0x2B */ u8 p3; // TODO not verified
    /* 0x2C */ s16 x3;
    /* 0x2E */ s16 y3;
    /* 0x30 */ u8 u3; // TODO not verified
    /* 0x31 */ u8 v3; // TODO not verified
    /* 0x32 */ u16 drawMode;
} FrozenShadePrim; /* size=0x34 */
