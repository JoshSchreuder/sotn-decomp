// SPDX-License-Identifier: AGPL-3.0-or-later
// Weapon ID #40. Used by weapons:
// Monster vial 1
#include "weapon_private.h"
extern u16* g_WeaponCluts[];
extern s32 g_HandId;
#include "shared.h"
#include "gen/w_040_1.h"
#include "gen/w_040_2.h"
#define g_Animset w_040_1
#define g_Animset2 w_040_2
#include "sfx.h"

extern AnimationFrame D_11C000_8017A724[];
extern AnimationFrame D_11C000_8017A748[];
extern AnimationFrame D_11C000_8017A7DC[];
extern AnimationFrame D_11C000_8017A7B8[];
extern AnimationFrame D_11C000_8017A804[];
extern AnimationFrame D_11C000_8017A80C[];
extern FrameProperty D_11C000_8017A844[];
extern s32 D_11C000_8017A85C[];
extern s32 D_11C000_8017B540;

void func_11C000_8017AC14(void) {
    RECT rect;
    RECT rectDummy;
    s16 color;

    color = (g_GameTimer >> 1) % 2 ? 0x039C : 0x199D;
    D_8006EDCC[g_HandId][10] = color;

    rect.x = 0;
    rect.y = 0xF1;
    rect.w = 0x100;
    rect.h = 3;
    LoadImage(&rect, D_8006EDCC);
}

static void EntityWeaponAttack(Entity* self) {
    Collider col;
    s32 var_a2;
    s16 xMod;

    switch (self->step) {
    case 0:
        SetSpriteBank1(g_Animset);
        self->animSet = ANIMSET_OVL(16);
        self->palette = 0x110;
        self->unk5A = 0x64;
        if (g_HandId != 0) {
            self->palette += 0x18;
            self->unk5A += 2;
            self->animSet += 2;
        }

        self->zPriority = PLAYER.zPriority + 2;
        self->facingLeft = PLAYER.facingLeft;
        self->flags = FLAG_POS_CAMERA_LOCKED | FLAG_UNK_100000;
        self->anim = D_11C000_8017A804;
        self->posY.i.hi -= 4;

        D_11C000_8017B540 %= 4;
        SetSpeedX(D_11C000_8017A85C[D_11C000_8017B540]);
        self->velocityY = -FIX(2.5);
        g_Player.timers[10] = 4;
        D_11C000_8017B540++;
        self->step++;
        break;
    case 1:
        self->posX.val += self->velocityX;
        self->posY.val += self->velocityY;
        self->velocityY += FIX(0.15625);
        g_api.CheckCollision(self->posX.i.hi, self->posY.i.hi, &col, 0);
        if (col.effects & EFFECT_SOLID) {
            self->posY.i.hi += col.unk18;
            self->anim = D_11C000_8017A7DC;
            self->poseTimer = 0;
            self->pose = 0;
            self->drawMode = DRAW_TPAGE2 | DRAW_TPAGE;
            g_api.PlaySfxVolPan(SFX_GLASS_BREAK_A, 0x50, 0);
            // TODO: FACTORY()
            g_api.CreateEntFactoryFromEntity(self, WFACTORY(56, 0), 0);
            self->step++;
            return;
        }

        if (self->velocityX < 0) {
            xMod = -4;
        } else {
            xMod = 4;
        }
        g_api.CheckCollision(
            (s16)(xMod + self->posX.i.hi), self->posY.i.hi, &col, 0);
        if (col.effects & EFFECT_UNK_0002) {
            if (xMod < 0) {
                self->posX.i.hi += col.unkC;
            } else {
                self->posX.i.hi += col.unk4;
            }
            self->velocityX /= -2;
        }

        g_api.CheckCollision(
            self->posX.i.hi, (s16)(self->posY.i.hi - 8), &col, 0);
        if (col.effects & EFFECT_SOLID) {
            self->posY.i.hi += col.unk20 + 1;
            self->velocityY = FIX(1);
            self->velocityX /= 2;
            return;
        }
        break;
    case 2:
        if (self->poseTimer < 0) {
            DestroyEntity(self);
        }
        break;
    }
}

s32 func_ptr_80170004(Entity* self) {
    switch (self->step) {
    case 0:
        self->animSet = self->ext.weapon.parent->animSet;
        self->unk5A = self->ext.weapon.parent->unk5A;
        self->palette = self->ext.weapon.parent->palette;
        self->facingLeft = (self->facingLeft + 1) & 1;
        self->flags = FLAG_POS_CAMERA_LOCKED;
        self->zPriority = self->ext.weapon.parent->zPriority - 2;
        self->anim = D_11C000_8017A724;
        self->drawFlags |= FLAG_DRAW_SCALEY | FLAG_DRAW_SCALEX;
        self->scaleY = 0;
        self->scaleX = 0;
        self->rotPivotY = 0x16;
        self->posY.i.hi -= 0x16;
        self->step++;
        break;
    case 1:
        self->scaleX += 4;
        if (self->scaleX >= 0x100) {
            self->scaleX = 0x100;
            self->anim = D_11C000_8017A748;
            self->pose = 0;
            self->poseTimer = 0;
            self->ext.weapon.equipId =
                self->ext.weapon.parent->ext.weapon.equipId;
            SetWeaponProperties(self, 0);
            self->step++;
        }
        self->scaleY = self->scaleX;
        break;
    case 2:
        if (self->poseTimer == 1) {
            if ((self->pose == 0xB || self->pose == 0xF ||
                 self->pose == 0x13) &&
                // TODO: FACTORY()
                g_api.CreateEntFactoryFromEntity(self, WFACTORY(62, 0), 0) !=
                    NULL) {
                g_api.PlaySfx(SFX_EXPLODE_B);
            }
        }
        if (self->poseTimer < 0) {
            self->anim = D_11C000_8017A80C;
            self->pose = 0;
            self->poseTimer = 0;
            g_api.PlaySfx(SFX_VO_ALU_PAIN_A);
            self->step++;
        }
        break;
    case 3:
        if (self->scaleX == 0x40) {
            // TODO: FACTORY()
            g_api.CreateEntFactoryFromEntity(self, WFACTORY(70, 0), 0);
        }
        if (self->pose >= 5) {
            self->scaleX -= 4;
        }
        if (self->scaleX < 0) {
            DestroyEntity(self);
            return;
        }
        self->scaleY = self->scaleX;
        break;
    }
    g_api.UpdateAnim(D_11C000_8017A844, NULL);
    func_11C000_8017AC14();
}

static void func_ptr_80170008(Entity* self) {
    s32 var_a1;

    if (self->step == 0) {
        if (self->ext.weapon.parent->entityId == 0) {
            DestroyEntity(self);
            return;
        }
        self->animSet = self->ext.weapon.parent->animSet;
        self->unk5A = self->ext.weapon.parent->unk5A;
        self->palette = self->ext.weapon.parent->palette;
        self->flags = FLAG_POS_CAMERA_LOCKED;
        self->zPriority = self->ext.weapon.parent->zPriority - 2;
        self->anim = D_11C000_8017A724;
        self->posY.i.hi -= 0xA;
        var_a1 = 0x18;
        if (self->facingLeft == 0) {
            var_a1 = -0x18;
        }
        self->posX.i.hi = var_a1 + self->posX.i.hi;
        SetSpeedX(-FIX(2.5));
        self->anim = D_11C000_8017A7B8;
        self->ext.weapon.equipId = self->ext.weapon.parent->ext.weapon.equipId;
        self->attackElement |= ELEMENT_FIRE;
        SetWeaponProperties(self, 0);
        self->step++;
    } else {
        self->posX.val += self->velocityX;
        if (self->hitFlags != 0) {
            DestroyEntity(self);
            return;
        }
    }

    g_api.UpdateAnim(D_11C000_8017A844, NULL);
}

static void func_ptr_8017000C(Entity* self) {
    if (self->step == 0) {
        self->animSet = self->ext.weapon.parent->animSet;
        self->unk5A = self->ext.weapon.parent->unk5A;
        self->palette = self->ext.weapon.parent->palette;
        self->flags = FLAG_POS_CAMERA_LOCKED | FLAG_UNK_100000;
        self->zPriority = self->ext.weapon.parent->zPriority + 2;
        self->anim = D_11C000_8017A7DC;
        self->drawMode = DRAW_TPAGE2 | DRAW_TPAGE;
        self->posY.i.hi += 0x10;
        self->step++;
    } else if (self->poseTimer < 0) {
        DestroyEntity(self);
    }
}

static s32 func_ptr_80170010(Entity* self) {}

static s32 func_ptr_80170014(Entity* self) {}

static int GetWeaponId(void) { return 40; }

static void EntityWeaponShieldSpell(Entity* self) {}

static void func_ptr_80170024(Entity* self) {}

static void func_ptr_80170028(Entity* self) {}

static void WeaponUnused2C(void) {}

static void WeaponUnused30(void) {}

static void WeaponUnused34(void) {}

static void WeaponUnused38(void) {}

static void WeaponUnused3C(void) {}
