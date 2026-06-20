// SPDX-License-Identifier: AGPL-3.0-or-later
#include "../cen/cen.h"

extern PlayerState g_Maria;

extern s16 D_pspeu_09266D18[];
extern s16 D_pspeu_09266D30[];
extern s16 D_pspeu_09266D48[];
extern s16 D_pspeu_09266D70[];
extern s16 D_pspeu_09266D80[];
extern s16 D_pspeu_09266DA8[];
extern u16 D_pspeu_09266DB0[];
extern u16 D_pspeu_09266DC0[];
extern u16 D_pspeu_09266DB8[];
// yOffsets
extern s16 D_pspeu_09266DD8[];
extern SpriteParts* D_pspeu_09277A58[];
extern SpriteParts* D_pspeu_09278628[];
extern SpriteParts* D_pspeu_09279678[];
extern SpriteParts* D_pspeu_0927A8B8[];
extern SpriteParts* D_pspeu_0927C168[];
extern SpriteParts* D_pspeu_0927E130[];
extern SpriteParts* D_pspeu_0927F820[];
extern AnimationFrame D_pspeu_09285618;
extern s32 D_pspeu_09294308;
extern s32 D_pspeu_09294318[];
extern s32 D_pspeu_09294398[];
extern u16 g_EInitUnk;
extern Point16 D_pspeu_09276CB8[];
extern Point16 D_pspeu_09276CC8[];
extern Point16 D_pspeu_09276CD8[];
extern s16 D_pspeu_09276D10[];
extern s16 D_pspeu_09276D18[];
extern s16 D_pspeu_09276D20[];
extern s16 D_pspeu_09276D30[];
extern s16 D_pspeu_09276D38[];
extern s16 D_pspeu_09276D40[];
extern s16 D_pspeu_09276D50[];
extern s16 D_pspeu_09295B58[];
extern s32 D_pspeu_09295E00;
extern s32 D_pspeu_09295DF0;
extern s32 D_pspeu_09295E18;
extern s32 D_psp_091CE570;
extern s8 D_pspeu_09295D88;
extern s32 D_pspeu_09295D90;
extern s32 D_pspeu_09295DA0;
extern s32 D_pspeu_09295DB0;
extern s32 D_pspeu_09295DF8;
extern s32 D_pspeu_09295E10;
extern s32 g_CutsceneFlags;
extern s32 D_pspeu_09295D98;
extern s16 D_pspeu_09294310;
extern s16 D_pspeu_09294312;
extern s16 D_pspeu_09294314;
extern s16 D_pspeu_09294316;

static s32 func_pspeu_09237700(s32 x, s32 y, s32 arg2) {
    Collider collider;
    g_api.CheckCollision(x, y, &collider, 0);

    while (collider.effects & EFFECT_UNK_0002) {
        if (arg2-- < 0) {
            return -1;
        }

        y--;
        g_api.CheckCollision(x, y, &collider, 0);
    }

    return y;
}

static s32 func_pspeu_092377B0(
    Entity* self, s32 arg1, s32 arg2, Collider* collider) {
    s32 origPosX, origPosY; // sp3C, s8
    s32 posX, posY;         // s7, s6
    s32 var_s5;             // s5
    s32 var_s4;             // s4
    s32 var_s3;             // s3
    s32 var_s2;             // s2
    s32 i, j;               // s1, s0

    var_s3 = abs(arg1) >> 0x10;
    var_s2 = abs(arg2) >> 0x10;
    if (arg1 < 0) {
        arg1 = -var_s3;
    } else {
        arg1 = var_s3;
    }

    if (arg2 < 0) {
        arg2 = -var_s2;
    } else {
        arg2 = var_s2;
    }

    posX = self->posX.i.hi;
    origPosX = posX;
    posY = self->posY.i.hi;
    origPosY = posY;

    if (var_s2 < var_s3) {
        if (var_s3 == 0) {
            return 0;
        }

        for (i = 0; i <= var_s3; i++) {
            if (arg1 < 0) {
                var_s5 = -i;
            } else {
                var_s5 = i;
            }
            self->posX.i.hi = origPosX + var_s5;
            self->posY.i.hi = origPosY + ((arg2 * i) / var_s3);
            g_api.CheckCollision(self->posX.i.hi, self->posY.i.hi, collider, 0);
            if (collider->effects & (EFFECT_UNK_0002 | EFFECT_SOLID)) {
                if (var_s2 == 0) {
                    collider->effects = EFFECT_SOLID;
                }
                self->posX.i.hi = posX;
                self->posY.i.hi = posY;
                if (i != 0 && arg2 > 0) {
                    g_api.CheckCollision(
                        self->posX.i.hi, self->posY.i.hi + 1, collider, 0);
                    if (collider->effects & (EFFECT_UNK_0002 | EFFECT_SOLID)) {
                        collider->effects = EFFECT_UNK_0002;
                    } else {
                        collider->effects = EFFECT_SOLID;
                    }
                }
                return 1;
            }
            posX = self->posX.i.hi;
            posY = self->posY.i.hi;
        }
    } else if (var_s2 == 0) {
        return 0;
    } else {
        for (j = 0; j <= var_s2; j++) {
            self->posX.i.hi = origPosX + ((arg1 * j) / var_s2);
            if (arg2 < 0) {
                var_s4 = -j;
            } else {
                var_s4 = j;
            }
            self->posY.i.hi = origPosY + var_s4;
            g_api.CheckCollision(self->posX.i.hi, self->posY.i.hi, collider, 0);
            if (collider->effects & (EFFECT_UNK_0002 | EFFECT_SOLID)) {
                if (var_s3 == 0) {
                    collider->effects = EFFECT_UNK_0002;
                }
                self->posX.i.hi = posX;
                self->posY.i.hi = posY;
                if (j != 0 && arg2 > 0) {
                    g_api.CheckCollision(
                        self->posX.i.hi, self->posY.i.hi + 1, collider, 0);
                    if (collider->effects & (EFFECT_UNK_0002 | EFFECT_SOLID)) {
                        collider->effects = EFFECT_UNK_0002;
                    } else {
                        collider->effects = EFFECT_SOLID;
                    }
                }
                return 1;
            }
            posX = self->posX.i.hi;
            posY = self->posY.i.hi;
        }
    }

    return 0;
}

void func_pspeu_09237BF8(Entity* self) {
    Collider collider;
    s32 var_s3;
    s32 velocityXOne;
    s32 velocityXTwo;
    s32 velocityXThree;

    switch (self->step) {
    case 0:
        self->flags = FLAG_POS_CAMERA_LOCKED | FLAG_KEEP_ALIVE_OFFCAMERA |
                      FLAG_UNK_100000;
        self->unk5A = 0x54;
        self->zPriority = g_Entities[64].zPriority - 8;
        self->palette = 0x8134;
        self->animSet = -0x7FFA;
        func_pspeu_09242D28(&D_pspeu_09266D18);
        self->facingLeft = g_Entities[64].facingLeft;
        self->velocityX = FIX(3.0);
        self->posX.i.hi = g_Entities[64].posX.i.hi;
        self->posY.i.hi = g_Entities[64].posY.i.hi;
        self->hitboxWidth = 0x12;
        self->hitboxHeight = 0x1E;
        self->hitboxOffX = 0;
        self->hitboxOffY = -8;
        self->ext.mariaCen.unkB0 = 0;
        func_pspeu_0924A148(self, 0x24, 0x1000, 8, 0x10, 0x10, 1, 0);
        self->ext.mariaCen.unk7C = 0;
        g_Maria.unk7A = 1;
        self->ext.mariaCen.unk7E = 3;
        self->step = 2;
        self->ext.mariaCen.unk84 = 0x80;
        break;
    case 1:
        self->velocityX += FIX(1.0);
        if (self->velocityX > FIX(3.0)) {
            self->velocityX = FIX(3.0);
        }

        if (!(self->ext.mariaCen.unk7C % 10)) {
            g_api.PlaySfx(SFX_QUIET_STEPS);
        }
        self->ext.mariaCen.unk7C++;

        if (self->facingLeft) {
            velocityXOne = -self->velocityX;
        } else {
            velocityXOne = self->velocityX;
        }

        func_pspeu_092377B0(self, velocityXOne, 0, &collider);
        if (collider.effects & EFFECT_SOLID || self->hitFlags == 0x80) {
            self->velocityX = FIX(-3.0);
            self->velocityY = FIX(-8.0);
            self->ext.mariaCen.unk7E = 0;
            if (collider.effects & EFFECT_SOLID) {
                self->ext.mariaCen.unk7E = 0;
            }

            if (self->ext.mariaCen.unk7E > 0) {
                self->ext.mariaCen.unk7E--;
                self->step = 3;
            } else {
                self->step = 4;
            }

            self->ext.mariaCen.unk7C = 0;
        } else {
            g_api.CheckCollision(
                self->posX.i.hi, self->posY.i.hi + 1, &collider, 0);
            if (!(collider.effects & EFFECT_UNK_0002)) {
                self->velocityY = FIX(-8.0);
                self->step = 2;
                self->ext.mariaCen.unk7C = 0;
            }
        }
        break;
    case 2:
    case 3:
        if (self->step == 2) {
            self->velocityX += FIX(1.0);
            if (self->velocityX > FIX(3.0)) {
                self->velocityX = FIX(3.0);
            }
        }

        self->velocityY += FIX(1.0);
        if (self->velocityY > FIX(4.0)) {
            self->velocityY = FIX(4.0);
        }

        if (self->facingLeft) {
            velocityXTwo = -self->velocityX;
        } else {
            velocityXTwo = self->velocityX;
        }

        func_pspeu_092377B0(self, velocityXTwo, self->velocityY, &collider);
        if (collider.effects & EFFECT_UNK_0002) {
            var_s3 = func_pspeu_09237700(
                self->posX.i.hi, self->posY.i.hi, self->velocityY >> 0x10);
            if (var_s3 != -1) {
                self->velocityX = FIX(3.0);
                self->posY.i.hi = var_s3;
                self->posY.i.lo = 0;
                self->step = 1;
                collider.effects = EFFECT_NONE;
                self->hitFlags = 0;
            }
        }

        if (collider.effects & EFFECT_SOLID || self->hitFlags == 0x80) {
            self->velocityX = FIX(-3.0);
            self->velocityY = FIX(-8.0);
            self->ext.mariaCen.unk7E = 0;
            if (collider.effects & EFFECT_SOLID) {
                self->ext.mariaCen.unk7E = 0;
            }

            if (self->ext.mariaCen.unk7E > 0) {
                self->ext.mariaCen.unk7E--;
                self->step = 3;
            } else {
                self->step = 4;
            }
        }
        break;
    case 4:
        self->hitboxState = 0;
        self->velocityY += FIX(1.0);
        if (self->velocityY > FIX(4.0)) {
            self->velocityY = FIX(4.0);
        }

        if (self->facingLeft) {
            velocityXThree = -self->velocityX;
        } else {
            velocityXThree = self->velocityX;
        }

        self->posX.val = self->posX.val + velocityXThree;
        self->posY.val += self->velocityY;
        self->ext.mariaCen.unk84 -= 8;
        if (self->ext.mariaCen.unk84 <= 0) {
            func_pspeu_0924FA38(self, 0);
            self->step = 5;
        } else {
            func_pspeu_0924FA38(self, self->ext.mariaCen.unk84);
        }
        break;
    case 5:
        g_Maria.unk7A = 0;
        DestroyEntity(self);
        break;
    }

    if (self->step != 5 && (self->posX.i.hi < -8 || self->posX.i.hi > 0x108 ||
                            self->posY.i.hi > 0x108)) {
        self->step = 5;
    }
    self->hitFlags = 0;
}

void func_pspeu_09238318(Entity* self) {
    s32 var_s1;
    s32 var_s0;
    u32 temp_v1;

    switch (self->step) {
    case 0:
        self->flags = FLAG_POS_CAMERA_LOCKED | FLAG_KEEP_ALIVE_OFFCAMERA |
                      FLAG_UNK_100000;
        self->unk5A = 0x54;
        self->zPriority = g_Entities[64].zPriority - 8;
        self->palette = 0x8135;
        self->animSet = -0x7FFA;
        func_pspeu_09242D28(&D_pspeu_09266D30);
        self->ext.mariaCen.unk7C = 0;
        self->ext.mariaCen.unk84 = 0;
        self->ext.mariaCen.unk82 = g_Maria.unk46;
        self->step = 1;
        func_pspeu_092508B8(2);
        func_pspeu_0924FA38(self, 0);
        break;
    case 1:
        if (g_Maria.unk5C == 1) {
            g_api.PlaySfx(0x906);
            self->step = 3;
        }
        break;
    case 3:
        self->facingLeft = g_Entities[64].facingLeft;
        self->posX.i.hi = g_Entities[64].posX.i.hi;
        self->posY.i.hi = g_Entities[64].posY.i.hi + 0x18;
        self->ext.mariaCen.unk84 += 0x10;

        if (self->ext.mariaCen.unk84 >= 0x80) {
            func_pspeu_0924FA38(self, 0x80);
            self->ext.mariaCen.unk84 = 0;
            self->step = 2;
        } else {
            func_pspeu_0924FA38(self, self->ext.mariaCen.unk84);
        }

        break;
    case 4:
        if (self->facingLeft) {
            var_s1 = -4;
        } else {
            var_s1 = 4;
        }

        self->posX.i.hi += var_s1;
        self->ext.mariaCen.unk84 -= 0x10;
        if (self->ext.mariaCen.unk84 <= 0) {
            func_pspeu_0924FA38(self, 0);
            self->ext.mariaCen.unk84 = 0;

            if (self->ext.mariaCen.unk7C > 0x12C) {
                var_s0 = 5;
            } else {
                var_s0 = 3;
            }

            self->step = var_s0;
        } else {
            func_pspeu_0924FA38(self, self->ext.mariaCen.unk84);
        }
        break;
    case 2:
        if (self->facingLeft != g_Entities[64].facingLeft) {
            self->ext.mariaCen.unk84 = 0x80;
            self->step = 4;
        } else {
            self->facingLeft = g_Entities[64].facingLeft;
            self->posX.i.hi = g_Entities[64].posX.i.hi;
            self->posY.i.hi = g_Entities[64].posY.i.hi + 0x18;

            if (self->ext.mariaCen.unk7C > 0x12C) {
                self->ext.mariaCen.unk84 = 0x80;
                self->step = 4;
            }
        }
        break;
    case 5:
        g_Maria.unk5C = 0;
        DestroyEntity(self);
        break;
    }

    self->ext.mariaCen.unk7C++;
    if (self->step != 5 && (g_Maria.padTapped & 0x8000)) {
        func_pspeu_092474A8(self, 0x28, 0);
    }

    self->ext.mariaCen.unk82 = g_Maria.unk46;
}

void func_pspeu_09238708(Entity* self) {
    s32 velocityX;

    switch (self->step) {
    case 0:
        self->flags = FLAG_POS_CAMERA_LOCKED | FLAG_KEEP_ALIVE_OFFCAMERA |
                      FLAG_UNK_100000;
        self->unk5A = 0x54;
        self->zPriority = g_Entities[64].zPriority - 8;
        self->palette = 0x8134;
        self->animSet = -0x7FFA;
        func_pspeu_09242D28(&D_pspeu_09266D48);
        self->facingLeft = g_Entities[64].facingLeft;
        self->velocityX = FIX(4.0);
        self->posX.i.hi = g_Entities[64].posX.i.hi;
        self->posY.i.hi = g_Entities[64].posY.i.hi + 0x18;
        self->hitboxWidth = 0x10;
        self->hitboxHeight = 0x20;
        self->hitboxOffX = 0;
        self->hitboxOffY = 0;
        self->ext.mariaCen.unkB0 = 0;
        func_pspeu_0924A148(self, 0x10, 0x1040, 2, 0x10, 0x10, 1, 0);
        g_api.PlaySfx(SFX_ALUCARD_SWORD_SWISH);
        self->step = 1;
        break;
    case 1:
        if (self->facingLeft) {
            velocityX = -self->velocityX;
        } else {
            velocityX = self->velocityX;
        }

        self->posX.val = self->posX.val + velocityX;
        if (self->poseTimer < 0) {
            self->step = 2;
        }

        if (self->posX.i.hi < -0x10 || self->posX.i.hi > 0x110) {
            self->step = 2;
        }

        break;
    case 2:
        DestroyEntity(self);
        break;
    }
}

void func_pspeu_09238938(Entity* self) {
    s32 var_s1;
    s32 var_s0;

    switch (self->step) {
    case 0:
        self->flags = FLAG_POS_CAMERA_LOCKED | FLAG_KEEP_ALIVE_OFFCAMERA |
                      FLAG_UNK_100000;
        self->unk5A = 0x59;
        self->zPriority = g_Entities[64].zPriority - 8;
        self->palette = 0x8139;
        self->animSet = -0x7FF7;
        func_pspeu_09242D28(&D_pspeu_09266DA8);
        self->facingLeft = g_Entities[64].facingLeft;
        self->velocityX = 0;

        if (self->facingLeft) {
            var_s1 = -0x18;
        } else {
            var_s1 = 0x18;
        }

        self->posX.i.hi = g_Entities[64].posX.i.hi + var_s1;
        self->posY.i.hi = g_Entities[64].posY.i.hi + 0x18;
        self->hitboxWidth = 0;
        self->hitboxHeight = 0;
        self->hitboxOffX = 0x20;
        self->hitboxOffY = 8;
        self->ext.mariaCen.unkB0 = 0;
        func_pspeu_0924A148(self, 0x40, 0x1000, 0, 0x10, 0x10, 1, 0);
        g_Maria.unk5C = 1;
        self->ext.mariaCen.unk7C = 0;
        self->step = 1;
        func_pspeu_092474A8(self, 0x32, 0);
        break;
    case 1:
        if (self->ext.mariaCen.unk7C > 0xA) {
            func_pspeu_09242D28(&D_pspeu_09266D70);
            self->step = 2;
            self->velocityX = FIX(3.25);
            self->ext.mariaCen.unk7C = 0;
        }

        self->ext.mariaCen.unk7C++;
        break;
    case 2:
        if (self->hitboxWidth < 0x10) {
            self->hitboxWidth += 2;
        } else {
            self->hitboxWidth = 0x10;
        }

        if (self->hitboxHeight < 0x20) {
            self->hitboxHeight += 4;
        } else {
            self->hitboxHeight = 0x20;
        }

        if (self->ext.mariaCen.unk7C < 5) {
            if (self->facingLeft) {
                var_s0 = -self->velocityX;
            } else {
                var_s0 = self->velocityX;
            }
            self->posX.val = self->posX.val + var_s0;
        }

        if (self->ext.mariaCen.unk7C == 5) {
            self->velocityX = 0;
            func_pspeu_09242D28(&D_pspeu_09266D80);
        }

        if (!(self->ext.mariaCen.unk7C % 6)) {
            g_api.PlaySfx(SFX_WEAPON_SWISH_A);
        }

        self->ext.mariaCen.unk7C++;
        if (self->ext.mariaCen.unk7C > 0x41) {
            func_pspeu_09242D28(&D_pspeu_09266DA8);
            self->ext.mariaCen.unk80 = 0x80;
            self->ext.mariaCen.unk86 = 0xF;
            self->step = 3;
            self->hitboxState = 0;
        }
        break;
    case 3:
        if (self->ext.mariaCen.unk86 > 0) {
            self->ext.mariaCen.unk86--;
        } else {
            self->ext.mariaCen.unk80 -= 8;
        }
        if (self->ext.mariaCen.unk80 <= 0) {
            func_pspeu_0924FA38(self, 0);
            self->step = 4;
            break;
        }
        func_pspeu_0924FA38(self, self->ext.mariaCen.unk80);
        break;
    case 4:
        g_Maria.unk5C = 0;
        DestroyEntity(self);
        break;
    }
}

void func_pspeu_09238DC0(Entity* self) {
    switch (self->step) {
    case 0:
        self->flags = FLAG_POS_CAMERA_LOCKED | FLAG_KEEP_ALIVE_OFFCAMERA |
                      FLAG_UNK_100000;
        self->unk5A = 0x59;
        self->zPriority = g_Entities[64].zPriority - 8;
        self->palette = 0x8139;
        self->animSet = -0x7FF7;
        func_pspeu_09242D28(&D_pspeu_09266DA8);
        self->ext.mariaCen.unk7C = 0;
        self->ext.mariaCen.unk80 = 0;
        self->step = 2;
        self->posX.val = self->ext.mariaCen.position->x.val;
        self->posY.val = self->ext.mariaCen.position->y.val;
        self->ext.mariaCen.unk80 = 0x40;
        self->ext.mariaCen.unk84 = 0x100;
        self->scaleX = self->ext.mariaCen.unk84;
        self->scaleY = self->ext.mariaCen.unk84;
        func_pspeu_0924FA38(self, 0x40);
        self->drawFlags |= ENTITY_SCALEY | ENTITY_SCALEX;
        return;
    case 2:
        self->ext.mariaCen.unk7C++;
        if (self->ext.mariaCen.unk7C > 0xA) {
            self->ext.mariaCen.unk80 = 0x80;
            self->step = 4;
            return;
        }
        self->ext.mariaCen.unk80 -= 2;
        self->ext.mariaCen.unk84 += 8;
        func_pspeu_0924FA38(self, (s16)self->ext.mariaCen.unk80);
        self->drawFlags |= ENTITY_SCALEY | ENTITY_SCALEX;
        self->scaleX = self->ext.mariaCen.unk84;
        self->scaleY = self->ext.mariaCen.unk84;
        return;
    case 4:
        DestroyEntity(self);
        return;
    }
}

void func_pspeu_09238FD8(Entity* self) {
    switch (self->step) {
    case 0:
        self->flags = FLAG_POS_CAMERA_LOCKED | FLAG_KEEP_ALIVE_OFFCAMERA |
                      FLAG_UNK_100000;
        self->unk5A = 0x50;
        self->zPriority = g_Entities[64].zPriority - 8;
        self->palette = 0x8132;
        self->animSet = -0x7FFB;
        func_pspeu_09242D28(&D_pspeu_09266DB0);
        self->facingLeft = g_Entities[64].facingLeft;
        self->velocityX = 0;
        self->ext.mariaCen.unk7E = 0;
        self->posX.i.hi = g_Entities[64].posX.i.hi + self->ext.mariaCen.unk7E;
        self->posY.i.hi = g_Entities[64].posY.i.hi + 0x18;
        self->hitboxWidth = 0x18;
        self->hitboxHeight = 0x30;
        self->hitboxOffX = 0;
        self->hitboxOffY = 0;
        self->attack = 0xFF;
        self->attackElement = ELEMENT_NONE;
        self->hitboxState = 5;
        self->flags |= FLAG_NOT_AN_ENEMY;
        self->nFramesInvincibility = 6;
        self->stunFrames = 0;
        self->hitEffect = 1;
        self->entityRoomIndex = 1;
        g_Maria.unk7A = 1;
        self->ext.mariaCen.unk7C = 0;
        self->step++;
        break;
    case 1:
        self->posX.i.hi = g_Entities[64].posX.i.hi + self->ext.mariaCen.unk7E;
        self->posY.i.hi = g_Entities[64].posY.i.hi + 0x18;
        self->ext.mariaCen.unk7C++;
        if (self->ext.mariaCen.unk7C >= 0xB4) {
            self->step++;
        }
        break;
    case 2:
        g_Maria.unk7A = 0;
        DestroyEntity(self);
        break;
    }
}

void func_pspeu_09239238(Entity* self) {
    s32 var_s0;

    switch (self->step) {
    case 0:
        self->flags = FLAG_POS_CAMERA_LOCKED | FLAG_KEEP_ALIVE_OFFCAMERA |
                      FLAG_UNK_100000;
        self->unk5A = 0x50;
        self->zPriority = g_Entities[64].zPriority - 6;
        self->palette = 0x8132;
        self->animSet = -0x7FFB;
        func_pspeu_09242D28(&D_pspeu_09266DC0);

        self->facingLeft = g_Entities[64].facingLeft;
        if (g_Entities[64].facingLeft) {
            var_s0 = -0x4A;
        } else {
            var_s0 = 0x4A;
        }

        self->posX.i.hi = g_Entities[64].posX.i.hi + var_s0;
        self->posY.i.hi = g_Entities[64].posY.i.hi - 0x10;
        self->scaleX = 0;
        self->scaleY = 0;
        self->drawFlags |= ENTITY_SCALEY | ENTITY_SCALEX;
        g_Maria.unk28 = 0;
        g_Maria.unk20 = (s32)self;
        g_Maria.unk24 = 0;
        self->ext.mariaCen.unk7C = 0;
        self->ext.mariaCen.unk80 = 0;
        g_Maria.unk2C = -1;
        self->step = 1;
        func_pspeu_092508B8(3);
        func_pspeu_0924FA38(self, 0);
        break;
    case 1:
        if (g_Maria.unk5C == 1) {
            g_api.PlaySfx(0x906);
            func_pspeu_092474A8(self, 0xE, 0);
            self->step = 3;
        }
        break;
    case 3:
        self->ext.mariaCen.unk80 += 0x10;
        if (self->ext.mariaCen.unk80 >= 0x80) {
            func_pspeu_0924FA38(self, 0x80);
            self->step = 2;
            break;
        }

        func_pspeu_0924FA38(self, self->ext.mariaCen.unk80);
        break;
    case 2:
        if (self->scaleX < 0x100) {
            self->scaleX += 0x10;
            self->scaleY += 0x10;
        } else {
            self->drawFlags &= ENTITY_BLINK | ENTITY_MASK_B | ENTITY_MASK_G |
                               ENTITY_MASK_R | ENTITY_OPACITY | ENTITY_ROTATE;
        }

        if (!(self->ext.mariaCen.unk7C % 60)) {
            g_api.PlaySfx(SFX_MAGIC_NOISE_SWEEP);
        }

        self->ext.mariaCen.unk7C += 1;
        if (self->ext.mariaCen.unk7C >= 0xFF) {
            self->step = 5;
        }
        break;
    case 5:
        if (self->scaleX > 0x80) {
            self->drawFlags |= ENTITY_ROTATE | ENTITY_SCALEY | ENTITY_SCALEX;
            self->scaleX -= 0x20;
            self->scaleY -= 0x20;
            self->rotate += 0xF78;
            break;
        }

        self->ext.mariaCen.unk80 = 0x80;
        self->step = 4;
        break;
    case 4:
        self->ext.mariaCen.unk80 -= 0x10;
        if (self->ext.mariaCen.unk80 <= 0) {
            func_pspeu_0924FA38(self, 0);
            self->drawFlags |= ENTITY_ROTATE | ENTITY_SCALEY | ENTITY_SCALEX;
            self->step = 6;
            func_pspeu_09250ED8(g_Maria.unk20);
            g_Maria.unk2C = 0xB4;
            break;
        }
        func_pspeu_0924FA38(self, self->ext.mariaCen.unk80);
        self->drawFlags |= ENTITY_ROTATE | ENTITY_SCALEY | ENTITY_SCALEX;
        break;
    case 6:
        if (g_Maria.unk28 == 0) {
            self->step = 7;
            self->ext.mariaCen.unk84 = 0;
            break;
        }

        if (g_Maria.unk2C == 0) {
            g_api.PlaySfx(SFX_HEALTH_PICKUP);
            g_Maria.unk56 = 1;
            g_Maria.unk58 = g_Maria.unk24;
            self->step = 7;
            self->ext.mariaCen.unk84 = 0;
            break;
        }

        g_Maria.unk2C--;
        break;
    case 7:
        self->ext.mariaCen.unk84 += 1;
        if (self->ext.mariaCen.unk84 > 0x3C) {
            g_Maria.unk28 = 0;
            g_Maria.unk20 = NULL;
            g_Maria.unk5C = 0;
            DestroyEntity(self);
        }
        break;
    }
}

void func_pspeu_09239778(Entity* self) {
    Entity* entity;
    s32 xOffset;

    entity = (Entity*)g_Maria.unk20;
    switch (self->step) {
    case 0:
        self->flags = FLAG_POS_CAMERA_LOCKED | FLAG_KEEP_ALIVE_OFFCAMERA |
                      FLAG_UNK_100000;
        self->unk5A = 0x50;
        self->zPriority = g_Entities[64].zPriority - 7;
        self->palette = 0x8132;
        self->animSet = -0x7FFB;
        func_pspeu_09242D28(&D_pspeu_09266DB8);
        self->facingLeft = 0;
        self->velocityX = 0;
        self->ext.mariaCen.unk84 = 0xF0;
        if (entity == NULL) {
            if (g_Entities[64].facingLeft) {
                xOffset = -0x4A;
            } else {
                xOffset = 0x4A;
            }
            self->posX.i.hi = g_Entities[64].posX.i.hi + xOffset;
            self->posY.i.hi = g_Entities[64].posY.i.hi - 0x10;
        } else {
            self->posX.i.hi = entity->posX.i.hi;
            self->posY.i.hi = entity->posY.i.hi;
        }
        self->scaleX = 0;
        self->scaleY = 0;
        self->drawFlags |= ENTITY_SCALEY | ENTITY_SCALEX;
        self->blendMode |= BLEND_TRANSP;
        self->drawFlags |= ENTITY_OPACITY | ENTITY_ROTATE;
        self->rotate = 0;
        self->opacity = 0x80;
        self->hitboxWidth = 0x30;
        self->hitboxHeight = 0x30;
        self->hitboxOffX = 0;
        self->hitboxOffY = 0;
        self->ext.mariaCen.unkB0 = 0;
        func_pspeu_0924A148(self, 5, 0x1400, 2, 0x20, 0, 2, 0);
        self->ext.mariaCen.unk82 = self->hitboxState;
        self->hitboxState = 0;
        self->ext.mariaCen.unk7C = 0;
        self->step = 1;
        self->rotate += (0x1000 - (0x1000 / self->ext.mariaCen.unk84));
        break;
    case 1:
        if (self->scaleX < 0x100) {
            self->scaleX += 0x10;
            self->scaleY += 0x10;
        } else {
            self->hitboxState = self->ext.mariaCen.unk82;
            self->drawFlags &= ENTITY_BLINK | ENTITY_MASK_B | ENTITY_MASK_G |
                               ENTITY_MASK_R | ENTITY_OPACITY | ENTITY_ROTATE;
        }
        self->ext.mariaCen.unk7C += 1;
        if (self->ext.mariaCen.unk7C >= 0xF0) {
            self->hitboxState = 0;
            self->step = 2;
        }

        if (!(self->ext.mariaCen.unk7C % 8)) {
            self->ext.mariaCen.unk84 -= 5;
        }

        self->rotate += (0x1000 - (0x1000 / self->ext.mariaCen.unk84));
        break;
    case 2:
        if (self->ext.mariaCen.unk84 >= 0x41) {
            self->ext.mariaCen.unk84 -= 5;
        } else {
            self->step = 3;
        }
        self->rotate += (0x1000 - (0x1000 / self->ext.mariaCen.unk84));
        break;
    case 3:
        if (self->scaleX > 0) {
            self->scaleX -= 8;
            self->scaleY -= 8;
            self->drawFlags |= ENTITY_SCALEY | ENTITY_SCALEX;
        } else {
            self->step = 4;
        }
        self->rotate += (0x1000 - (0x1000 / self->ext.mariaCen.unk84));
        break;
    case 4:
        DestroyEntity(self);
        break;
    }
}

static s32 func_pspeu_09239C68(void) {
    if (g_Entities[64].step != 0 || g_Entities[64].step_s != 1) {
        return 0;
    }

    if (g_StageId == STAGE_TOP) {
        if ((abs(((g_Tilemap.left << 8) + g_PlayerX) - 0x1F40) < 4) &&
            (abs(((g_Tilemap.top << 8) + g_PlayerY) - 0x84F) < 4)) {
            return 2;
        }
    }

    if (g_StageId == STAGE_RTOP) {
        if ((abs(((g_Tilemap.left << 8) + g_PlayerX) - 0x20C0) < 4) &&
            (abs(((g_Tilemap.top << 8) + g_PlayerY) - 0x3847) < 4)) {
            return 4;
        }
    }

    return 0;
}

// CheckCollisionCount
static s16 func_pspeu_09239DC0(void) {
    Collider collider;
    s32 i;
    s32 posX;
    s32 posY;
    s32 posXPlusFour;
    s32 posXMinusFour;
    s32 collisionCounter;
    s32 solidCollider = EFFECT_SOLID | EFFECT_SOLID_FROM_ABOVE;

    posX = g_Entities[64].posX.i.hi;
    posXPlusFour = g_Entities[64].posX.i.hi + 4;
    posXMinusFour = g_Entities[64].posX.i.hi - 4;

    for (i = 0; i < 3; i++) {
        posY = g_Entities[64].posY.i.hi + D_pspeu_09266DD8[i];
        collisionCounter = 0;

        g_api.CheckCollision(posX, posY, &collider, 0);
        if ((collider.effects & solidCollider) == EFFECT_SOLID) {
            collisionCounter++;
        }

        g_api.CheckCollision(posXPlusFour, posY, &collider, 0);
        if ((collider.effects & solidCollider) == EFFECT_SOLID) {
            collisionCounter++;
        }

        g_api.CheckCollision(posXMinusFour, posY, &collider, 0);
        if ((collider.effects & solidCollider) == EFFECT_SOLID) {
            collisionCounter++;
        }

        if (collisionCounter != 0) {
            return i + 1;
        }
    }

    return 0;
}

static void func_pspeu_09239F50(SpriteParts** spriteParts, s32 arg1) {
    s32 i;

    for (i = 0; i < arg1; i++) {
        // BUG? Is this supposed to set something?
        spriteParts[i];
    }
}

void func_pspeu_09251270(void);
void func_pspeu_0923C2F8(void);
void func_pspeu_09239FA0(s32 unused) {
    s32 i;                     // s0
    Entity* entity;            // s1
    SpriteParts** spriteParts; // s2
    Primitive* prim;           // s3
    s16 var_s5;                // s5
    s16 var_s4;                // s4
    s32* var_s6;               // s6
    s32 var_s7;                // s7

    func_pspeu_09251278();
    g_CurrentEntity = &g_Entities[64];
    InitializeEntity(&g_EInitUnk);
    g_CurrentEntity->unk5A = 0xC;
    g_Entities[64].posX.val = 0x200000;
    g_Entities[64].posY.val = 0x200000;
    g_Entities[64].animSet = -0x7FFD;
    g_Entities[64].zPriority = g_unkGraphicsStruct.g_zEntityCenter;
    g_Entities[64].facingLeft = 0;
    g_Entities[64].palette = 0x8130;
    g_Entities[64].scaleY = 0x100;
    g_Entities[64].scaleX = 0x100;
    g_Entities[64].hitboxOffX = 0;
    g_Entities[64].hitboxOffY = 0;
    g_Entities[64].rotPivotY = 0x18;

    var_s7 = 0xF4;
    var_s6 = (s32*)g_Maria.colFloor;
    for (i = 0; i < var_s7; i++) {
        *(var_s6++) = 0;
    }

    g_Maria.vram_flag = g_Maria.unk04 = 1;
    func_pspeu_09241558(0);
    g_Entities[64].anim = &D_pspeu_09285618;
    g_PlayerDraw[5].enableColorBlend = 0;
    func_pspeu_0923DB48();

    for (i = 0; i < 0x20; i++) {
        var_s5 = (rand() & 0x3FF) + 0x100;
        var_s4 = (rand() & 0xFF) + 0x100;
        D_pspeu_09294398[i] = (var_s4 * (rcos(var_s5) * 0x10)) >> 8;
        D_pspeu_09294318[i] = -((var_s4 * (rsin(var_s5) * 0x10)) >> 7);
    }

    func_pspeu_09239F50(D_pspeu_09278628, 0x16);
    func_pspeu_09239F50(D_pspeu_0927A8B8, 8);
    func_pspeu_09239F50(D_pspeu_0927E130, 0x12);
    func_pspeu_09239F50(D_pspeu_0927C168, 0x14);
    func_pspeu_09239F50(D_pspeu_0927F820, 0xD);
    func_pspeu_09239F50(D_pspeu_09279678, 0xB);

    spriteParts = g_api.o.spriteBanks;
    spriteParts += 3;
    *spriteParts = (SpriteParts*)D_pspeu_09277A58;
    spriteParts++;
    *spriteParts = (SpriteParts*)D_pspeu_09278628;
    spriteParts++;
    *spriteParts = (SpriteParts*)D_pspeu_0927A8B8;
    spriteParts++;
    *spriteParts = (SpriteParts*)D_pspeu_0927E130;
    spriteParts++;
    *spriteParts = (SpriteParts*)D_pspeu_0927C168;
    spriteParts++;
    *spriteParts = (SpriteParts*)D_pspeu_0927F820;
    spriteParts++;
    *spriteParts = (SpriteParts*)D_pspeu_09279678;

    for (entity = &g_Entities[65], i = 0; i < 3; i++, entity++) {
        DestroyEntity(entity);
        entity->animSet = -0x7FFD;
        entity->unk5A = i + 0xD;
        entity->palette = 0x8130;
        entity->flags = FLAG_POS_CAMERA_LOCKED | FLAG_UNK_20000;
    }

    g_Entities[65].primIndex = g_api.AllocPrimitives(PRIM_TILE, 6);
    g_Entities[65].flags |= FLAG_HAS_PRIMS;
    prim = &g_PrimBuf[g_Entities[65].primIndex];

    // nb. use of i here seems unnecessary
    for (i = 0; prim != NULL; i++, prim = prim->next) {
        prim->drawMode = DRAW_UNK_100 | DRAW_HIDE | DRAW_UNK02;
    }

    if (D_80097C98 == 6) {
        func_pspeu_092474A8(g_CurrentEntity, 0x1004D, 0);
        func_pspeu_09241400(1);
    }

    if (D_80097C98 == 4) {
        func_pspeu_092474A8(g_CurrentEntity, 0x3004D, 0);
        func_pspeu_09241400(3);
    }

    if (D_80097C98 == 5) {
        func_pspeu_092474A8(g_CurrentEntity, 0x5004D, 0);
        func_pspeu_09241400(5);
    }

    func_pspeu_09251068();
    func_pspeu_09251270();
    D_pspeu_09294308 = 0x1E;
    func_pspeu_0923C2F8();
}

static void func_pspeu_0923A480(s32 arg0) {
    Collider collider;
    s32 i;          // s0
    s16 posX;       // s2
    s16 posY;       // s1
    s32 status;     // s3
    s32* unk04_ptr; // s4
    s32 velocityX;  // s5
    s32* vram_ptr;  // s6

    vram_ptr = &g_Maria.vram_flag;
    unk04_ptr = &g_Maria.unk04;
    *unk04_ptr = *vram_ptr;
    *vram_ptr = 0;
    status = g_Maria.status;

    if (arg0 != 0) {
        for (i = 0; i < 4; i++) {
            if (status & 0x20) {
                if (status & 0x80000000) {
                    D_pspeu_09276CC8[i].y = D_pspeu_09276D38[i];
                    D_pspeu_09276CB8[i].y = D_pspeu_09276D30[i];
                } else {
                    D_pspeu_09276CC8[i].y = D_pspeu_09276D18[i];
                    D_pspeu_09276CB8[i].y = D_pspeu_09295B58[i];
                }
            } else {
                D_pspeu_09276CC8[i].y = D_pspeu_09276D18[i];
                D_pspeu_09276CB8[i].y = D_pspeu_09276D10[i];
            }
        }

        for (i = 0; i < 7; i++) {
            if (status & 0x20) {
                if (status & 0x80000000) {
                    D_pspeu_09276CD8[i].y = D_pspeu_09276D40[i];
                    D_pspeu_09276CD8[i + 7].y = D_pspeu_09276D40[i];
                } else {
                    D_pspeu_09276CD8[i].y = D_pspeu_09276D50[i];
                    D_pspeu_09276CD8[i + 7].y = D_pspeu_09276D50[i];
                }
            } else {
                D_pspeu_09276CD8[i].y = D_pspeu_09276D20[i];
                D_pspeu_09276CD8[i + 7].y = D_pspeu_09276D20[i];
            }
        }
    }

    velocityX = g_Entities[64].velocityX;
    if (g_Entities[64].velocityX < 0 && !(*unk04_ptr & 8)) {
        if ((*unk04_ptr & 0xF000) == 0xC000) {
            velocityX = (velocityX * 10) / 16;
        }

        if ((*unk04_ptr & 0xF000) == 0xD000) {
            velocityX = (velocityX * 13) / 16;
        }
        g_Entities[64].posX.val = (g_Entities[64].posX.val + velocityX);
    }
    if (g_Entities[64].velocityX > 0 && !(*unk04_ptr & 4)) {
        if ((*unk04_ptr & 0xF000) == 0x8000) {
            velocityX = (velocityX * 10) / 16;
        }
        if ((*unk04_ptr & 0xF000) == 0x9000) {
            velocityX = (velocityX * 13) / 16;
        }
        g_Entities[64].posX.val += velocityX;
    }
    if (g_Entities[64].velocityY < 0 && !(*unk04_ptr & 2)) {
        g_Entities[64].posY.val += g_Entities[64].velocityY;
    }

    if (g_Entities[64].velocityY > 0 && !(*unk04_ptr & 1)) {
        g_Entities[64].posY.val += g_Entities[64].velocityY;
    }

    for (i = 0; i < 4; i++) {
        posX = g_Entities[64].posX.i.hi + D_pspeu_09276CC8[i].x;
        posY = g_Entities[64].posY.i.hi + D_pspeu_09276CC8[i].y;
        g_api.CheckCollision(posX, posY, &g_Maria.colFloor[i], 0);
        if (g_Maria.timers[7] &&
            (g_Maria.colFloor[i].effects & EFFECT_SOLID_FROM_ABOVE)) {
            g_api.CheckCollision(posX, posY + 0xC, &collider, 0);
            if (!(collider.effects & EFFECT_SOLID)) {
                g_Maria.colFloor[i].effects = EFFECT_NONE;
            }
        }
    }

    func_pspeu_09243A78();

    for (i = 0; i < 4; i++) {
        posX = g_Entities[64].posX.i.hi + D_pspeu_09276CB8[i].x;
        posY = g_Entities[64].posY.i.hi + D_pspeu_09276CB8[i].y;
        g_api.CheckCollision(posX, posY, &g_Maria.colFloor[i] + 4, 0);
    }

    func_pspeu_09244358();
    if ((*vram_ptr & 1) && g_Entities[64].velocityY >= 0) {
        LOH(g_Entities[64].posY.val) = 0;
    }

    if ((*vram_ptr & 2) && g_Entities[64].velocityY <= 0) {
        LOH(g_Entities[64].posY.val) = 0;
    }

    for (i = 0; i < 0xE; i++) {
        posX = g_Entities[64].posX.i.hi + D_pspeu_09276CD8[i].x;
        posY = g_Entities[64].posY.i.hi + D_pspeu_09276CD8[i].y;
        g_api.CheckCollision(posX, posY, &g_Maria.colFloor[i] + 8, 0);
    }

    func_pspeu_09244BA0();
    func_pspeu_09244EE0();

    if ((*vram_ptr & 4) && g_Entities[64].velocityX > 0) {
        LOH(g_Entities[64].posX.val) = 0;
    }

    if ((*vram_ptr & 8) && g_Entities[64].velocityX < 0) {
        LOH(g_Entities[64].posX.val) = 0;
    }
}

static void func_pspeu_0923ACC8(void) {
    s32 var_s1; // s1
    s32 var_s0; // s0
    s32 var_s4; // s4
    s32 var_s3; // s3
    s32 var_s2; // s2
    s32 var_s5; // s5

    var_s1 = g_Maria.padPressed & 0xF0;
    if (g_Entities[64].facingLeft == false) {
        var_s0 = 0x60;
        var_s2 = 0x20;
    } else {
        var_s0 = 0xC0;
        var_s2 = 0x80;
    }

    var_s4 = 0x10;
    var_s3 = 0x40;

    switch (D_pspeu_09294310) {
    case 0:
        if (g_Maria.padTapped == var_s4) {
            D_pspeu_09294312 = 0x14;
            D_pspeu_09294310++;
        }
        break;
    case 1:
        if (var_s1 == var_s3) {
            D_pspeu_09294312 = 0x14;
            D_pspeu_09294310++;
            break;
        }

        if (--D_pspeu_09294312 == 0) {
            D_pspeu_09294310 = 0;
        }
        break;
    case 2:
        if (var_s1 == var_s0) {
            D_pspeu_09294312 = 0x14;
            D_pspeu_09294310++;
            break;
        }

        if (--D_pspeu_09294312 == 0) {
            D_pspeu_09294310 = 0;
        }
        break;
    case 3:
        if (--D_pspeu_09294312 == 0) {
            D_pspeu_09294310 = 0;
        }

        if (g_Entities[64].step == 0 || g_Entities[64].step == 1 ||
            g_Entities[64].step == 2 || g_Entities[64].step == 4 ||
            g_Entities[64].step == 3) {
            if (g_Maria.unk72) {
                D_pspeu_09294310 = 0;
                break;
            }

            if (!g_Maria.unk46 && (g_Maria.padTapped & 0x8000)) {
                func_pspeu_092425D0();
            }
        }

        break;
    }
}

static void func_pspeu_0923AF78(void) {
    switch (D_pspeu_09294314) {
    case 0:
        if ((g_Maria.padTapped & 0x40) && g_Maria.padHeld == 0) {
            D_pspeu_09294316 = 0x10;
            D_pspeu_09294314 += 1;
        }
        break;
    case 1:
        if (g_Maria.padTapped & 0x10) {
            D_pspeu_09294316 = 0x10;
            D_pspeu_09294314 += 1;
            break;
        }

        if (--D_pspeu_09294316 == 0) {
            D_pspeu_09294314 = 0;
            break;
        }
        break;
    case 2:
        if (D_pspeu_09294316) {
            if (--D_pspeu_09294316 == 0) {
                D_pspeu_09294314 = 0;
                break;
            }
        }

        if ((g_Maria.padTapped & 0x4000) && !g_Maria.unk46) {
            if (g_Entities[64].step == 2 || g_Entities[64].step == 0 ||
                (g_Entities[64].step == 4 &&
                 g_Entities[64].velocityY > 0x10000) ||
                g_Entities[64].step == 3) {
                if (g_Maria.unk72) {
                    D_pspeu_09294314 = 0;
                    break;
                }

                func_pspeu_092419E0();
                D_pspeu_09294314 = 0;
            }
        }

        break;
    }
}

extern u16 D_pspeu_09266DE0[];
extern AnimationFrame D_pspeu_09285730[];
extern AnimationFrame D_pspeu_09285A90;
extern FrameProperty D_pspeu_09285B00[];
extern AnimationFrame* D_pspeu_09285B20[];
extern s32 D_pspeu_09294300;

s32 func_pspeu_0924FAB0(s16 arg0);
void func_pspeu_09242D00(s16 arg0);

void func_pspeu_0923B190(void) {
    s32 sp40[4];
    // Point32 pos;  // wants sp40, getting sp48
    s32 posX;     // wants sp3C, getting sp44
    s32 posY;     // wants sp38, getting sp40
    s32 vramFlag; // wants sp34, getting sp3C

    PlayerDraw* playerDraw; // s0
    s16 angle;              // s1
    s32 i;                  // s2
    s32 newStatus;          // s3
    s16 step;               // s5
    s16 step_s;             // s4
    s32 var_s8;             // s8
    s32 damageEffects;      // s7
    s32 damageKind;         // s6

    var_s8 = 0;
    g_CurrentEntity = &g_Entities[64];
    playerDraw = &g_PlayerDraw[5];
    damageEffects = 0;
    damageKind = 0;

    if (g_Maria.unk78) {
        g_Maria.unk7A = 0;
        g_Maria.unk5C = 0;
        g_Maria.unk78 = 0;
    }

    g_Maria.unk4C = 0;
    g_Maria.unk72 = func_pspeu_09239DC0();
    if (!(g_Maria.status & PLAYER_STATUS_DEAD)) {
        if (g_Entities[64].step != 0x10 && (g_Entities[64].flags & FLAG_DEAD)) {
            D_pspeu_09294300 |= 1;
            step = g_Entities[64].step;
            step_s = g_Entities[64].step_s;
            sp40[0] = D_pspeu_09266DE0[g_CurrentEntity->nFramesInvincibility];
            func_pspeu_09242D00(0x10);
        } else {
            for (i = 0; i < LEN(g_Maria.timers); i++) {
                if (!g_Maria.timers[i]) {
                    continue;
                }

                switch (i) {
                case 0:
                case 1:
                    break;
                case 2:
                    g_Entities[64].palette = g_Maria.damagePalette;
                    break;
                case 3:
                    break;
                case 4:
                    angle = (g_GameTimer & 0xF) * 256;
                    playerDraw->r0 = playerDraw->g0 = playerDraw->b0 =
                        ((rsin(angle) + 0x1000) / 64) + 0x60;
                    angle += 0x200;
                    playerDraw->r1 = playerDraw->g1 = playerDraw->b1 =
                        ((rsin(angle) + 0x1000) / 64) + 0x60;
                    angle += 0x200;
                    playerDraw->r3 = playerDraw->g3 = playerDraw->b3 =
                        ((rsin(angle) + 0x1000) / 64) + 0x60;
                    angle += 0x200;
                    playerDraw->r2 = playerDraw->g2 = playerDraw->b2 =
                        ((rsin(angle) + 0x1000) / 64) + 0x60;
                    playerDraw->enableColorBlend = 1;
                    break;
                case 5:
                case 6:
                case 7:
                case 8:
                case 9:
                case 10:
                case 11:
                case 12:
                case 13:
                case 14:
                    break;
                case 15:
                    func_pspeu_09243020(0, 0);
                    break;
                }

                if (--g_Maria.timers[i] != 0) {
                    continue;
                }

                switch (i) {
                case 0:
                case 1:
                    break;
                case 2:
                    g_Entities[64].palette = PAL_FLAG(0x130);
                    break;
                case 3:
                    break;
                case 4:
                    playerDraw->enableColorBlend = 0;
                    break;
                case 5:
                    break;
                case 6:
                    if (g_Entities[64].step == 3 &&
                        g_Entities[64].anim != D_pspeu_09285730) {
                        func_pspeu_09242D28(&D_pspeu_09285730);
                        g_Maria.unk44 &= ~0x10;
                    }
                    break;
                case 7:
                case 8:
                case 9:
                case 10:
                case 11:
                case 12:
                case 13:
                case 14:
                    break;
                case 15:
                    func_pspeu_092430E0();
                    break;
                }
            }

            g_Maria.padHeld = g_Maria.padPressed;
            if (g_Maria.demo_timer) {
                g_Maria.demo_timer--;
                g_Maria.padPressed = g_Maria.padSim;
            } else {
                g_Maria.padPressed = g_pads[0].pressed;
                if (D_pspeu_09294308 > 0) {
                    D_pspeu_09294308--;
                    g_Maria.padPressed = 0;
                }
            }

            g_Maria.padTapped =
                g_Maria.padPressed & (g_Maria.padHeld ^ g_Maria.padPressed);
            if (g_Entities[64].step != 0x10) {
                // Reuse the i variable here even though we aren't iterating
                i = func_pspeu_09239C68();
                if (i != 0) {
                    func_pspeu_09241400(i);
                }

                if (g_Entities[64].step != 0x20) {
                    if (g_DebugPlayer && func_pspeu_0923DBF0()) {
                        return;
                    }

                    if (g_Maria.unk56) {
                        g_Status.hp += g_Maria.unk58;
                        func_9101FC8();
                        func_pspeu_0924FAB0(g_Maria.unk58);
                        if (g_Status.hpMax < g_Status.hp) {
                            g_Status.hp = g_Status.hpMax;
                        }
                        g_Maria.unk56 = 0;
                    }

                    if (!(g_Maria.timers[13] | g_Maria.timers[14]) &&
                        g_Maria.unk60 < 2) {
                        if (g_Maria.unk60 == 1) {
                            step = g_Entities[64].step;
                            step_s = g_Entities[64].step_s;
                            func_pspeu_09242D00(0xC);
                        } else if (g_CurrentEntity->hitFlags > 0 &&
                                   g_CurrentEntity->hitFlags < 4) {
                            step = g_Entities[64].step;
                            step_s = g_Entities[64].step_s;
                            sp40[0] =
                                D_pspeu_09266DE0[g_CurrentEntity
                                                     ->nFramesInvincibility];
                            if (g_Maria.unk6C - g_Maria.unk6A >= 0xA) {
                                sp40[1] = 3;
                            } else {
                                sp40[1] = 2;
                            }

                            i = 3;
                            if (sp40[0] & 0x200) {
                                i = 7;
                            }

                            if (g_Entities[64].step == 11) {
                                i = 8;
                            }

                            switch (i) {
                            case 0:
                            case 1:
                            case 2:
                                break;
                            case 3:
                                func_pspeu_09242D00(0xA);
                                break;
                            case 4:
                            case 5:
                            case 6:
                                break;
                            case 7:
                                func_pspeu_09242D00(0xB);
                                break;
                            case 8:
                                // Looks unused / copied from Doppleganger's
                                // stone state
                                var_s8 = 1;
                                break;
                            case 9:
                                break;
                            }
                        }
                    }
                    goto check_input_combo;
                }
            } else {
            check_input_combo:
                // CheckBladeDashInput();
                func_pspeu_0923ACC8();
                // CheckHighJumpInput();
                func_pspeu_0923AF78();
            }
        }
    }

    g_Maria.prev_step = g_Entities[64].step;
    g_Maria.prev_step_s = g_Entities[64].step_s;
    switch (g_Entities[64].step) {
    case 0:
        func_pspeu_0923DFF0();
        break;
    case 1:
        func_pspeu_0923E2D8();
        break;
    case 2:
        func_pspeu_0923EAB0();
        break;
    case 3:
        func_pspeu_0923EA08();
        break;
    case 4:
        func_pspeu_0923E388();
        break;
    case 8:
        func_pspeu_092411A0();
        break;
    case 10:
        func_pspeu_0923F0D0(damageEffects, damageKind, step, step_s);
        break;
    case 12:
        func_pspeu_0923FFA8();
        break;
    case 16:
        func_pspeu_092401F8(damageEffects, damageKind, step, step_s);
        break;
    case 18:
        func_pspeu_09240530();
        break;
    case 19:
        func_pspeu_09240590();
        break;
    case 20:
        func_pspeu_092405E0();
        break;
    case 21:
        func_pspeu_09240630();
        break;
    case 27:
        func_pspeu_09240680();
        break;
    case 28:
        func_pspeu_092406D0();
        break;
    case 23:
        func_pspeu_09240710();
        break;
    case 25:
        func_pspeu_092409D0();
        break;
    case 24:
        func_pspeu_09241050();
        break;
    case 32:
        func_pspeu_09240D60();
        break;
    }

    g_Maria.unk08 = g_Maria.status;
    newStatus = 0;
    switch (g_Entities[64].step) {
    case 0:
        newStatus = 0x08000000;
        break;
    case 1:
        newStatus = 0x08000000;
        break;
    case 2:
        newStatus = 0x08000000;
        if (g_Entities[64].step_s != 2) {
            newStatus = 0x08000020;
        }
        break;
    case 3:
        newStatus = 0x08002000;
        break;
    case 4:
        newStatus = 0x08002000;
        break;
    case 8:
        break;
    case 10:
        newStatus = 0x08010000;
        func_pspeu_09242F98(1, 0x10);
        break;
    case 12:
        newStatus = 0x110040;
        func_pspeu_09242F98(1, 0x10);
        break;
    case 16:
        newStatus = 0x08050000;
        if (g_Entities[64].step_s == 0x80) {
            newStatus |= 0x80000;
        }
        func_pspeu_09242F98(1, 0x10);
        break;
    case 19:
    case 20:
    case 21:
    case 27:
    case 18:
        newStatus = 0x08000000;
        func_pspeu_09242F98(1, 0x10);
        break;
    case 28:
        break;
    case 23:
        newStatus = 0x20;
        break;
    case 25:
        newStatus = 0x80000020;
        break;
    case 24:
        break;
    case 32:
        newStatus = 0x08000000;
        func_pspeu_09242F98(1, 0x10);
        break;
    }

    if (g_Maria.timers[9]) {
        newStatus |= 0x400;
    }

    if (g_Maria.timers[10]) {
        newStatus |= 0x800;
    }

    if (g_Maria.timers[12]) {
        newStatus |= 0x1000;
    }

    if (g_unkGraphicsStruct.D_80097448 != 0) {
        newStatus |= 0x20000;
    }

    newStatus |= 0x10000000;
    g_Maria.status = newStatus;

    if ((g_Maria.unk08 & 0x10000) &&
        !(g_Maria.status & PLAYER_STATUS_UNK10000)) {
        func_pspeu_09242F98(1, 0x10);
        g_Maria.timers[4] = 0x10;
        g_Entities[64].palette = 0x8130;
    }

    if (newStatus & 0x08000000) {
        func_pspeu_09243020(1, 4);
    }

    if ((g_Maria.timers[13] | g_Maria.timers[14]) != 0) {
        g_Maria.status |= PLAYER_STATUS_INVINCIBLE;
    }

    g_api.UpdateAnim(D_pspeu_09285B00, D_pspeu_09285B20);
    if (g_Maria.status & PLAYER_STATUS_DEAD) {
        g_Entities[64].hitboxState = 0;
    } else {
        g_Entities[64].hitboxState = 0x33;
        if (g_Maria.status & PLAYER_STATUS_STONE) {
            g_Entities[64].hitboxState &= 0xFFCF;
        }

        if ((g_Maria.timers[13] | g_Maria.timers[14]) != 0) {
            g_Entities[64].hitboxState = 0;
        }
    }

    g_Entities[64].hitParams = 0;
    if (g_Entities[64].anim == &D_pspeu_09285A90) {
        g_Entities[64].palette = D_pspeu_09266DE0[g_Entities[64].pose];
    }

    if (!(g_Maria.status & (PLAYER_STATUS_UNK40 | PLAYER_STATUS_UNK10))) {
        func_pspeu_09242670();
        if (g_unkGraphicsStruct.D_80097448 > 0x28 &&
            !g_CurrentEntity->nFramesInvincibility) {
            g_Entities[64].velocityY = (g_Entities[64].velocityY * 3) / 4;
            g_Entities[64].velocityX = (g_Entities[64].velocityX * 3) / 4;
        }

        posX = g_Entities[64].posX.val;
        posY = g_Entities[64].posY.val;
        vramFlag = g_Maria.vram_flag;
        if (abs(g_Entities[64].velocityY) > 0x20000 ||
            abs(g_Entities[64].velocityX) > 0x20000) {
            g_Entities[64].velocityX >>= 2;
            g_Entities[64].velocityY >>= 2;

            if (g_Entities[64].posY.i.hi >= 0) {
                func_pspeu_0923A480(1);
            }

            if (g_Entities[64].posY.i.hi >= 0) {
                func_pspeu_0923A480(0);
            }

            if (g_Entities[64].posY.i.hi >= 0) {
                func_pspeu_0923A480(0);
            }

            if (g_Entities[64].posY.i.hi >= 0) {
                func_pspeu_0923A480(0);
            }

            if (g_Entities[64].posY.i.hi < 0) {
                g_Entities[64].posY.val = -0x10000;
            }

            g_Entities[64].velocityX *= 4;
            g_Entities[64].velocityY *= 4;
        } else {
            func_pspeu_0923A480(1);
        }

        g_Maria.unk04 = vramFlag;
        if (g_unkGraphicsStruct.D_80097448 > 0x28 &&
            !g_CurrentEntity->nFramesInvincibility) {
            g_Entities[64].velocityY = (g_Entities[64].velocityY * 4) / 3;
            g_Entities[64].velocityX = (g_Entities[64].velocityX * 4) / 3;
        }
        g_CurrentEntity->nFramesInvincibility = 0;
        func_pspeu_092428D8();
    }
}

void func_pspeu_0923C2F8(void) {
    g_CurrentEntity->flags = FLAG_POS_CAMERA_LOCKED;
    g_CurrentEntity->animSet = -0x7FFD;
    g_CurrentEntity->animCurFrame = 1;
    g_Entities[64].posX.i.hi = 0x138 - g_Tilemap.scrollX.i.hi;
    g_Entities[64].posY.i.hi = 0x1C7 - g_Tilemap.scrollY.i.hi;
    g_CurrentEntity->palette = 0x210;
    g_CurrentEntity->zPriority = 0x80;
    g_Maria.unk6E = g_Maria.unk6A = g_Maria.unk6C = g_Entities[64].hitPoints;
}

extern u16 D_pspeu_09276C10[];
extern s32 D_pspeu_09276C48;

void func_pspeu_0923C3B8(Entity* self) {
    s32 var_s1;
    s32 var_s0;

    switch (self->step) {
    case 0:
        self->flags = FLAG_KEEP_ALIVE_OFFCAMERA | FLAG_UNK_100000;
        self->unk5A = 0x55;
        self->zPriority = g_Entities[64].zPriority - 8;
        self->palette = 0x8137;
        self->animSet = -0x7FF8;
        func_pspeu_09242D28(&D_pspeu_09276C10);
        self->facingLeft = g_Entities[64].facingLeft;
        self->velocityX = 0;

        if (g_Entities[64].facingLeft) {
            var_s1 = 0x20;
        } else {
            var_s1 = -0x20;
        }

        self->posX.i.hi = g_Entities[64].posX.i.hi + var_s1;
        self->posY.i.hi = g_Entities[64].posY.i.hi - 8;
        self->ext.mariaCen.unk7E = self->posY.i.hi;
        self->hitboxWidth = 0x12;
        self->hitboxHeight = 0x14;
        self->hitboxOffX = 0;
        self->hitboxOffY = 0xC;
        self->ext.mariaCen.unkB0 = 0;
        func_pspeu_0924A148(self, 0x30, 0x1000, 0, 0x10, 0x10, 1, 0);
        g_Maria.unk7A = 1;
        self->ext.mariaCen.unk7C = 0;
        self->step = 3;
        // fallthrough
    case 3:
        self->ext.mariaCen.unk80 += 0x10;
        if (self->ext.mariaCen.unk80 >= 0x80) {
            func_pspeu_0924FA38(self, 0x80);
            self->step = 1;
            break;
        }
        func_pspeu_0924FA38(self, self->ext.mariaCen.unk80);
        break;
    case 1:
    case 2:
        if (self->step == 1) {
            self->posY.i.hi =
                self->ext.mariaCen.unk7E +
                ((-rsin(self->ext.mariaCen.unk7C << 6) * 0x2A) / 0x1000);
            if (self->ext.mariaCen.unk7C > D_pspeu_09276C48) {
                self->step = 2;
            }
        } else {
            self->posY.i.hi =
                self->ext.mariaCen.unk7E - 0x2A +
                ((-rsin(self->ext.mariaCen.unk7C << 6) * 0xC) / 0x1000);
        }

        self->velocityX += FIX(0.25);
        if (self->velocityX > FIX(3.0)) {
            self->velocityX = FIX(3.0);
        }

        if (self->facingLeft) {
            var_s0 = -self->velocityX;
        } else {
            var_s0 = self->velocityX;
        }

        self->posX.val = self->posX.val + var_s0;
        if (!(self->ext.mariaCen.unk7C % 60)) {
            g_api.PlaySfx(SFX_WING_FLAP_B);
        }

        self->ext.mariaCen.unk7C++;
        if (self->posX.i.hi < -0x10 || self->posX.i.hi > 0x110) {
            self->step = 4;
        }
        break;
    case 4:
        g_Maria.unk7A = 0;
        DestroyEntity(self);
        break;
    }
}

s32 func_pspeu_0923C818(s32 arg0) {
    arg0 %= 0x1000;

    if (arg0 < 0) {
        arg0 += 0x1000;
    }

    return arg0;
}

extern s32 D_pspeu_09294420[0x100];
extern s32 D_pspeu_09294528;

Entity* func_pspeu_0923C870(Entity* unused) {
    Entity* entity; // s0
    s32 i;          // s1
    s32 var_s2;
    s32 var_s3;

    var_s3 = 0;
    entity = &g_Entities[0];

    for (i = 0; i < 0x40; i++, entity++) {
        D_pspeu_09294420[i] = 0;
        if (entity->entityId && entity->hitboxState != 0 &&
            !(entity->flags & FLAG_UNK_00200000) &&
            (entity->posX.i.hi >= -0x10) && (entity->posX.i.hi < 0x111) &&
            (entity->posY.i.hi < 0xF1) && (entity->posY.i.hi >= 0) &&
            !(entity->hitboxState & 8) && (entity->hitPoints < 0x7000)) {
            if (entity->flags & FLAG_UNK_80000) {
                var_s3++;
                D_pspeu_09294420[i] = 1;
            } else {
                entity->flags |= FLAG_UNK_80000;
                return entity;
            }
        }
    }

    if (var_s3 != 0) {
        var_s2 = D_pspeu_09294528 % 0x40;
        for (i = 0; i < 0x40; i++) {
            if (D_pspeu_09294420[var_s2]) {
                entity = &g_Entities[var_s2];
                D_pspeu_09294528 = (var_s2 + 1) % 0x40;
                return entity;
            }

            var_s2 = (var_s2 + 1) % 0x40;
        }
    }
    return NULL;
}

bool func_pspeu_0923CAB8(Entity* entity) {
    if (entity == NULL) {
        return false;
    }
    if (entity->hitboxState == 0) {
        return false;
    }
    if (entity->posX.i.hi < -0x10) {
        return false;
    }
    if (entity->posX.i.hi > 0x110) {
        return false;
    }
    if (entity->posY.i.hi > 0xF0) {
        return false;
    }
    if (entity->posY.i.hi < 0) {
        return false;
    }
    if (entity->hitPoints >= 0x7000) {
        return false;
    }
    if (entity->hitPoints <= 0) {
        return false;
    }

    return true;
}

extern u16 D_pspeu_09276C40[];
extern s32 D_pspeu_09294418;
extern Point16 D_pspeu_09294530[];
extern char D_pspeu_09276C50[];

// EntityMariaDragonCrash
void func_pspeu_0923CBD8(Entity* self) {
    s32 x;
    s32 y;
    s32 angle;
    s32 i;

    switch (self->step) {
    case 0:
        self->flags = FLAG_POS_CAMERA_LOCKED | FLAG_KEEP_ALIVE_OFFCAMERA | FLAG_UNK_100000;
        self->unk5A = 0x55;
        self->zPriority = 0x1C0;
        self->palette = 0x8138;
        self->animSet = -0x7FF8;
        func_pspeu_09242D28(&D_pspeu_09276C40);
        self->facingLeft = 0;
        self->velocityX = 0;
        self->posX.i.hi = g_Entities[64].posX.i.hi;
        self->posY.i.hi = g_Entities[64].posY.i.hi - 0x20;
        self->rotate = 0;
        self->ext.mariaDragon.velocity = 1280;
        self->hitboxWidth = 0x18;
        self->hitboxHeight = 0x18;
        self->hitboxOffX = 0;
        self->hitboxOffY = 0;
        self->ext.mariaDragon.unkB0 = 0;
        func_pspeu_0924A148(self, 0x19, 0x5000, 2, 0x10, 0x10, 1, 0);
        D_pspeu_09294418 = 0;
        self->ext.mariaDragon.timer = 0;
        self->step = 1;
        func_pspeu_092508B8(4);
        self->ext.mariaDragon.opacity = 0;
        func_pspeu_0924FA38(self, 0);
        break;
    case 1:
        if (g_Maria.unk5C == 1) {
            func_pspeu_092474A8(self, 0xF, 0);
            self->step = 2;
            g_api.PlaySfx(0x906);
        }
        self->drawFlags |= ENTITY_ROTATE;
        break;
    case 2:
        if (self->ext.mariaDragon.opacity < 128) {
            self->ext.mariaDragon.opacity += 8;
            func_pspeu_0924FA38(self, self->ext.mariaDragon.opacity);
        } else {
            func_pspeu_0924FA38(self, 0x80);
        }
        self->drawFlags |= ENTITY_ROTATE;
        {
            MATRIX m;
            VECTOR vTranslate = {0, 0, 0};
            VECTOR vVelocity;
            SVECTOR svVelocity;
            long out;
            if (!func_pspeu_0923CAB8(self->ext.mariaDragon.target)) {
                self->ext.mariaDragon.target = func_pspeu_0923C870(self);
            }
            if (self->ext.mariaDragon.target) {
                x = self->ext.mariaDragon.target->posX.i.hi;
                y = self->ext.mariaDragon.target->posY.i.hi;
            } else {
                x = g_Entities[64].posX.i.hi;
                y = g_Entities[64].posY.i.hi;
            }
            self->rotate = func_pspeu_0923C818(self->rotate);
            angle = func_pspeu_0923C818(
                ratan2(y - self->posY.i.hi, x - self->posX.i.hi));
            if (self->rotate < angle) {
                if (angle - self->rotate < 0x800) {
                    self->rotate += 0x40;
                } else {
                    self->rotate -= 0x40;
                }
            } else if (self->rotate - angle < 0x800) {
                self->rotate -= 0x40;
            } else {
                self->rotate += 0x40;
            }
            SetGeomOffset(0, 0);
            func_89285A0(self->rotate, &m);
            TransMatrix(&m, &vTranslate);
            SetRotMatrix(&m);
            SetTransMatrix(&m);
            svVelocity.vx = self->ext.mariaDragon.velocity;
            svVelocity.vy = 0;
            svVelocity.vz = 0;
            RotTrans(&svVelocity, &vVelocity, &out);
            self->velocityX = vVelocity.vx << 8;
            self->velocityY = vVelocity.vy << 8;
            // "%08x : %08x\n"
            FntPrint(D_pspeu_09276C50, self->velocityX, self->velocityY);
            self->posX.val += self->velocityX;
            self->posY.val += self->velocityY;
            if ((self->ext.mariaDragon.timer % 120) == 0) {
                g_api.PlaySfx(SFX_HIPPOGRYPH_FIRE_ATTACK);
            }
            self->ext.mariaDragon.timer++;
            if (self->ext.mariaDragon.timer > 300) {
                self->step = 3;
            }
        }
        break;
    case 3:
        self->posX.val += self->velocityX;
        self->posY.val += self->velocityY;
        self->ext.mariaDragon.timer++;
        if (self->ext.mariaDragon.timer > 300 &&
            (self->posX.i.hi < -0x100 || self->posX.i.hi > 0x200 ||
             self->posY.i.hi < -0x100 || self->posY.i.hi > 0x200)) {
            self->step = 4;
        }
        break;
    case 4:
        g_Maria.unk5C = 0;
        D_pspeu_09294418 = 0;
        DestroyEntity(self);
        break;
    }

    if (g_Maria.unk5C == 1 && self->step != 4) {
        if (D_pspeu_09294418 == 0x80) {
            for (i = 0; i < 0x40; i++) {
                D_pspeu_09294530[i].x = D_pspeu_09294530[i + 0x40].x;
                D_pspeu_09294530[i].y = D_pspeu_09294530[i + 0x40].y;
            }
            D_pspeu_09294418 -= 0x40;
        }
        if (D_pspeu_09294418 < 0x80) {
            D_pspeu_09294530[D_pspeu_09294418].x =
                self->posX.i.hi + g_Tilemap.scrollX.i.hi;
            D_pspeu_09294530[D_pspeu_09294418].y =
                self->posY.i.hi + g_Tilemap.scrollY.i.hi;
            D_pspeu_09294418++;
        }
    }
}

extern s32 D_pspeu_09294418;
extern u8 D_pspeu_09294520[8];
extern Point16 D_pspeu_09294530[];

// EntityMariaDragonCrashBodyPart
void func_pspeu_0923D3E0(Entity* self) {
    s32 xStart;
    s32 xEnd;
    s32 yStart;
    s32 yEnd;
    s32 var_s6;
    s32 var_s5;
    s32 u;
    s32 v;
    s32 var_s2;
    s32 i;
    Primitive* prim;

    switch (self->step) {
    case 0:
        self->primIndex =
            g_api.AllocPrimitives(PRIM_GT4, LEN(D_pspeu_09294520));
        if (self->primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        self->flags = FLAG_KEEP_ALIVE_OFFCAMERA | FLAG_HAS_PRIMS;
        self->unk5A = 0x55;
        self->zPriority = 0x1C0;
        self->palette = 0x8138;
        self->animSet = -0x7FF8;
        func_pspeu_09242D28(D_pspeu_09276C40);
        self->facingLeft = g_Entities[64].facingLeft;
        self->velocityX = 0;
        self->posX.i.hi = g_Entities[64].posX.i.hi;
        self->posY.i.hi = g_Entities[64].posY.i.hi;
        self->hitboxWidth = 24;
        self->hitboxHeight = 24;
        self->hitboxOffX = 0;
        self->hitboxOffY = 0;
        self->ext.mariaDragon.unkB0 = 0;
        func_pspeu_0924A148(
            self, 0x20, ELEMENT_HOLY | ELEMENT_THUNDER, 2, 16, 16, 1, 0);
        for (i = 0; i < LEN(D_pspeu_09294520); i++) {
            D_pspeu_09294520[i] = 0;
        }
        self->step = 1;
        // fallthrough
    case 1:
        xStart = 0;
        xEnd = 0;
        yStart = 0;
        yEnd = 0;
        prim = &g_PrimBuf[self->primIndex];
        var_s2 = D_pspeu_09294418 - 1;
        for (i = 0; i < LEN(D_pspeu_09294520); i++) {
            prim->tpage = 0x15;
            prim->clut = self->palette & 0x3FF;
            prim->priority = self->zPriority;
            if (var_s2 >= 7) {
                var_s6 = D_pspeu_09294530[var_s2].x - g_Tilemap.scrollX.i.hi;
                var_s5 = D_pspeu_09294530[var_s2].y - g_Tilemap.scrollY.i.hi;
                var_s2 -= 7;
                if (var_s2 < 0) {
                    var_s2 = 0;
                }
                {
                    s32 spD4 =
                        D_pspeu_09294530[var_s2].x - g_Tilemap.scrollX.i.hi;
                    s32 spD0 =
                        D_pspeu_09294530[var_s2].y - g_Tilemap.scrollY.i.hi;
                    MATRIX m;
                    VECTOR spA0 = {0, 0, 0};
                    VECTOR vPosEnd;
                    VECTOR sp80;
                    VECTOR vPosStart;
                    VECTOR sp60;
                    SVECTOR svPosEnd;
                    SVECTOR sp50;
                    SVECTOR svPosStart;
                    SVECTOR sp40;
                    long out;
                    s32 angle;
                    angle = func_pspeu_0923C818(
                        ratan2(spD0 - var_s5, spD4 - var_s6));
                    func_89285A0(angle, &m);
                    SetGeomOffset(var_s6, var_s5);
                    TransMatrix(&m, &spA0);
                    SetRotMatrix(&m);
                    SetTransMatrix(&m);
                    if (i == 0) {
                        sp40.vx = -16;
                        sp40.vy = -16;
                        sp40.vz = 0;
                        svPosStart.vx = 16;
                        svPosStart.vy = -16;
                        svPosStart.vz = 0;
                        sp50.vx = -16;
                        sp50.vy = 16;
                        sp50.vz = 0;
                        svPosEnd.vx = 16;
                        svPosEnd.vy = 16;
                        svPosEnd.vz = 0;
                        RotTrans(&sp40, &sp60, &out);
                        RotTrans(&svPosStart, &vPosStart, &out);
                        RotTrans(&sp50, &sp80, &out);
                        RotTrans(&svPosEnd, &vPosEnd, &out);
                        prim->x0 = sp60.vx;
                        prim->y0 = sp60.vy;
                        prim->x1 = vPosStart.vx;
                        prim->y1 = vPosStart.vy;
                        prim->x2 = sp80.vx;
                        prim->y2 = sp80.vy;
                        prim->x3 = vPosEnd.vx;
                        prim->y3 = vPosEnd.vy;
                    } else {
                        svPosStart.vx = 16;
                        svPosStart.vy = -16;
                        svPosStart.vz = 0;
                        svPosEnd.vx = 16;
                        svPosEnd.vy = 16;
                        svPosEnd.vz = 0;
                        RotTrans(&svPosStart, &vPosStart, &out);
                        RotTrans(&svPosEnd, &vPosEnd, &out);
                        prim->x0 = xStart;
                        prim->y0 = yStart;
                        prim->x1 = vPosStart.vx;
                        prim->y1 = vPosStart.vy;
                        prim->x2 = xEnd;
                        prim->y2 = yEnd;
                        prim->x3 = vPosEnd.vx;
                        prim->y3 = vPosEnd.vy;
                    }
                    xStart = vPosStart.vx;
                    yStart = vPosStart.vy;
                    xEnd = vPosEnd.vx;
                    yEnd = vPosEnd.vy;
                }

                if (i == 7) {
                    u = 0xE0;
                    v = 0x20;
                } else if (i == 6) {
                    u = 0xE0;
                    v = 0;
                } else {
                    u = 0x80;
                    v = 0;
                }

                prim->u1 = u;
                prim->v1 = v + 0x1F;
                prim->u0 = u + 0x1F;
                prim->v0 = v + 0x1F;
                prim->u3 = u;
                prim->v3 = v;
                prim->u2 = u + 0x1F;
                prim->v2 = v;
                prim->drawMode = DRAW_DEFAULT;
                D_pspeu_09294520[i] += 0x10;
                if (D_pspeu_09294520[i] < 0x80) {
                    prim->drawMode |=
                        DRAW_TPAGE2 | DRAW_TPAGE | DRAW_COLORS | DRAW_TRANSP;
                } else {
                    D_pspeu_09294520[i] = 0x80;
                }
                prim->r0 = prim->r1 = prim->r2 = prim->r3 = D_pspeu_09294520[i];
                prim->g0 = prim->g1 = prim->g2 = prim->g3 = D_pspeu_09294520[i];
                prim->b0 = prim->b1 = prim->b2 = prim->b3 = D_pspeu_09294520[i];
            } else {
                prim->drawMode = DRAW_HIDE;
            }
            prim = prim->next;
        }

        if (g_Maria.unk5C != 0) {
            return;
        }
        self->step = 2;
        break;
    case 2:
        DestroyEntity(self);
        break;
    }
}

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria", func_pspeu_0923DB48);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria", func_pspeu_0923DB58);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria", func_pspeu_0923DBA0);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria", func_pspeu_0923DBF0);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria", func_pspeu_0923DED8);
