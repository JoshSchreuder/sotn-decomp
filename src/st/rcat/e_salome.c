// SPDX-License-Identifier: AGPL-3.0-or-later
#include "rcat.h"

extern EInit g_EInitSalome;
extern EInit D_us_80181078;
extern EInit D_us_80181084;
extern EInit D_us_80181090;
extern EInit D_us_8018109C;

static s16 D_us_80182190[] = {0, 7, 8, 0};
static u8 D_us_80182198[] = {
    0x02, 0x01, 0x06, 0x02, 0x06, 0x03, 0xFF, 0x00,
};
static u8 D_us_801821A0[] = {
    0x04, 0x03, 0x04, 0x02, 0x04, 0x01, 0xFF, 0x00,
};
static u8 D_us_801821A8[] = {
    0x03, 0x04, 0x03, 0x05, 0x03, 0x06, 0x03, 0x07, 0x03, 0x08, 0x00, 0x00,
};
static u8 D_us_801821B4[] = {
    0x06, 0x09, 0x06, 0x0A, 0x06, 0x0B, 0x03, 0x0C,
    0x02, 0x0D, 0x02, 0x0E, 0x01, 0x0F, 0xFF, 0x00,
};
static u8 D_us_801821C4[] = {
    0x04, 0x01, 0x08, 0x10, 0x03, 0x11, 0x03, 0x12, 0x03, 0x13,
    0x1C, 0x14, 0x02, 0x15, 0x02, 0x16, 0x02, 0x17, 0x02, 0x18,
    0x02, 0x19, 0x06, 0x1A, 0x04, 0x01, 0xFF, 0x00,
};
static u8 D_us_801821E0[] = {
    0x02, 0x1B, 0x02, 0x1C, 0x00, 0x00, 0x00, 0x00,
};
static u8 D_us_801821E8[] = {
    0x02, 0x1E, 0x02, 0x1F, 0x02, 0x20, 0x02, 0x21, 0x00, 0x00, 0x00, 0x00,
};
static u8 D_us_801821F4[] = {
    0x02, 0x22, 0x02, 0x23, 0x02, 0x24, 0x02, 0x25, 0x02, 0x26,
    0x02, 0x27, 0x02, 0x28, 0x02, 0x29, 0x00, 0x00, 0x00, 0x00,
};
static u8 D_us_80182208[] = {
    0x01, 0x22, 0x01, 0x23, 0x01, 0x24, 0x01, 0x25, 0x01, 0x26, 0x01, 0x27,
    0x02, 0x28, 0x02, 0x29, 0x02, 0x22, 0x02, 0x23, 0x03, 0x24, 0x03, 0x25,
    0x03, 0x26, 0x04, 0x27, 0x04, 0x28, 0x04, 0x29, 0x04, 0x22, 0x05, 0x23,
    0x05, 0x24, 0x05, 0x25, 0x05, 0x26, 0x05, 0x27, 0x05, 0x28, 0x05, 0x29,
    0x05, 0x22, 0x05, 0x23, 0x05, 0x24, 0x05, 0x25, 0x05, 0x26, 0x05, 0x27,
    0x05, 0x28, 0x05, 0x29, 0x00, 0x00, 0x00, 0x00,
};
static u8 D_us_8018224C[] = {
    0x02, 0x2A, 0x01, 0x2B, 0x03, 0x2C, 0x03, 0x2D,
    0x03, 0x2E, 0x03, 0x2F, 0x00, 0x00, 0x00, 0x00,
};
static u8 D_us_8018225C[] = {
    0x02, 0x33, 0x01, 0x34, 0x03, 0x35, 0x03, 0x36,
    0x03, 0x37, 0x03, 0x38, 0x00, 0x00, 0x00, 0x00,
};
static u8 D_us_8018226C[] = {
    0x02, 0x31, 0x02, 0x32, 0x00, 0x00, 0x00, 0x00,
};
static Point16 D_us_80182274[] = {
    {.x = 0x0016, .y = 0x0003}, {.x = 0x0016, .y = 0xFFFF},
    {.x = 0xFFED, .y = 0xFFFE}, {.x = 0xFFE8, .y = 0x0009},
    {.x = 0xFFE8, .y = 0x0009}, {.x = 0xFFE8, .y = 0x0009},
    {.x = 0xFFE8, .y = 0x0009}, {.x = 0xFFE8, .y = 0x0009},
    {.x = 0xFFF1, .y = 0x0003}, {.x = 0xFFEC, .y = 0xFFFE},
    {.x = 0x0014, .y = 0xFFFE}, {.x = 0x001A, .y = 0x0009},
    {.x = 0x001A, .y = 0x0009}, {.x = 0x001A, .y = 0x0009},
    {.x = 0x001A, .y = 0x0009}, {.x = 0x0019, .y = 0xFFFE},
    {.x = 0x0019, .y = 0xFFFD}, {.x = 0x0019, .y = 0xFFFC},
    {.x = 0x0019, .y = 0xFFFB}, {.x = 0x0019, .y = 0xFFFB},
    {.x = 0x0019, .y = 0xFFFC}, {.x = 0x0019, .y = 0xFFFD},
    {.x = 0x0019, .y = 0xFFFE}, {.x = 0x0019, .y = 0xFFFE},
    {.x = 0x0018, .y = 0xFFFF}, {.x = 0x0017, .y = 0x0000},
};

void func_us_801C1804(Entity*);
void func_us_801C0990(Entity* self) {
    Entity* entity;      // s0
    s32 posY;            // s1
    s16 angle;           // s2
    s32 facingDirection; // s3

    if ((GetDistanceToPlayerX() < 0x50) && (GetDistanceToPlayerY() < 0x40)) {
        self->ext.salome.unk86 = true;
    } else {
        self->ext.salome.unk86 = false;
    }

    self->hitboxState = 3;
    entity = &PLAYER;
    if (((GetSideToPlayer() & 1) ^ 1) == self->facingLeft &&
        self->ext.salome.unk86) {
        self->hitboxState = 1;
    }

    if (self->hitFlags & 3) {
        PlaySfxPositional(SFX_SALOME_PAIN);
    }

    if (self->flags & FLAG_DEAD && self->step < 8) {
        SetStep(8);
    }

    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitSalome);
        self->facingLeft = (GetSideToPlayer() & 1) ^ 1;
        self->ext.salome.unk85 = Random() & 1;
        self->ext.salome.unk88 = 0x30;
        self->zPriority = 0xAA;
        entity = self + 1;
        CreateEntityFromCurrentEntity(E_UNK_29, entity);
        entity->zPriority = self->zPriority + 1;
        entity->params = 0;

        entity = self + 2;
        CreateEntityFromCurrentEntity(E_UNK_29, entity);
        entity->zPriority = self->zPriority - 1;
        entity->params = 1;
        SetStep(2);
        // fallthrough
    case 2:
        switch (self->step_s) {
        case 0:
            self->animCurFrame = 1;
            MoveEntity();
            angle = self->ext.salome.unk82 += 0x20;
            angle &= 0xFFF;
            self->velocityX = 0;
            self->velocityY = rsin(angle) * 8;
            if (GetDistanceToPlayerX() < 0x60) {
                self->step_s++;
            }
            break;
        case 1:
            if (AnimateEntity(&D_us_80182198, self) == 0) {
                SetStep(4);
            }
            break;
        }
        break;
    case 4:
        if (!self->step_s) {
            self->step_s++;
        }
        AnimateEntity(D_us_801821A8, self);
        MoveEntity();
        if (self->facingLeft ^ self->ext.salome.unk84) {
            self->velocityX += 0xC00;
            if (self->velocityX >= FIX(1.25)) {
                self->velocityX = FIX(1.25);
            }
        } else {
            self->velocityX -= 0xC00;
            if (self->velocityX <= FIX(-1.25)) {
                self->velocityX = FIX(-1.25);
            }
        }

        entity = &PLAYER;
        posY = entity->posY.i.hi;
        posY = posY - 0x48 - self->posY.i.hi;
        if (posY < -8) {
            self->velocityY -= 0x600;
            if (self->velocityY <= FIX(-0.625)) {
                self->velocityY = FIX(-0.625);
            }
        }

        if (posY > 8) {
            self->velocityY += 0xC00;
            if (self->velocityY >= FIX(0.625)) {
                self->velocityY = FIX(0.625);
            }
        }

        if (!self->ext.salome.unk88) {
            if (GetDistanceToPlayerX() > 0x40) {
                SetStep(6);
            } else {
                PlaySfxPositional(SFX_SALOME_MAGIC_ATTACK);
                entity = AllocEntity(&g_Entities[160], &g_Entities[192]);
                if (entity != NULL) {
                    CreateEntityFromEntity(E_UNK_2A, self, entity);
                    if (self->facingLeft) {
                        entity->posX.i.hi += 0x1C;
                    } else {
                        entity->posX.i.hi -= 0x1C;
                    }
                    entity->posY.i.hi += 4;
                    entity->zPriority = self->zPriority + 1;
                }
            }
            self->ext.salome.unk88 = 0x30;
        } else {
            self->ext.salome.unk88--;
        }

        facingDirection = ((GetSideToPlayer() & 1) ^ 1);
        if (self->facingLeft != facingDirection) {
            SetStep(5);
        }
        break;
    case 5:
        if (!AnimateEntity(D_us_801821B4, self)) {
            self->animCurFrame = 4;
            self->facingLeft ^= 1;
            SetStep(4);
        }
        MoveEntity();
        self->velocityX -= self->velocityX / 32;
        self->velocityY -= self->velocityY / 32;
        return;
    case 6:
        switch (self->step_s) {
        case 0:
            MoveEntity();
            self->velocityX -= self->velocityX / 4;
            self->velocityY -= self->velocityY / 4;
            if (!AnimateEntity(&D_us_801821A0, self)) {
                SetSubStep(1);
            }
            break;
        case 1:
            if (!AnimateEntity(D_us_801821C4, self)) {
                SetSubStep(2);
            }
            if (!self->poseTimer && self->pose == 5) {
                self->ext.salome.unk85 ^= 1;
                entity = AllocEntity(&g_Entities[160], &g_Entities[192]);
                if (entity != NULL) {
                    if (self->ext.salome.unk85) {
                        PlaySfxPositional(SFX_SALOME_MEOW_SHORT);
                        CreateEntityFromEntity(E_UNK_2C, self, entity);
                    } else {
                        PlaySfxPositional(SFX_SALOME_ATTACK);
                        CreateEntityFromEntity(E_UNK_2B, self, entity);
                    }

                    entity->zPriority = self->zPriority + 1;
                    entity->facingLeft = self->facingLeft;
                    if (self->facingLeft) {
                        entity->posX.i.hi += 0x14;
                    } else {
                        entity->posX.i.hi -= 0x14;
                    }
                    entity->posY.i.hi -= 4;
                }
            }
            break;
        case 2:
            if (!AnimateEntity(D_us_80182198, self)) {
                SetStep(4);
            }
            break;
        }
        break;
    case 8:
        switch (self->step_s) {
        case 0:
            self->hitboxState = 0;

            entity = self + 1;
            DestroyEntity(entity);

            entity = self + 2;
            entity->step = 8;

            self->blendMode = BLEND_NO;
            self->drawFlags = ENTITY_OPACITY | ENTITY_SCALEY | ENTITY_SCALEX;
            self->scaleX = self->scaleY = 0x100;
            self->opacity = 0x80;
            self->step_s++;
            // fallthrough
        case 1:
            self->scaleX = self->scaleY -= 8;
            if (self->opacity) {
                self->opacity -= 4;
            }

            if (self->scaleX < 0x40) {
                self->blendMode = BLEND_ADD | BLEND_TRANSP;
            }

            if (!self->scaleX) {
                PlaySfxPositional(SFX_SALOME_MEOW);
                entity = self + 2;
                entity->step = 0;
                entity->step_s = 0;
                entity->entityId = 0x2C;
                entity->pfnUpdate = func_us_801C1804;
                entity->ext.salome.unk9E = 1;

                entity = self;
                DestroyEntity(entity);
            }
            break;
        }
        break;

    case 0xFF:
#include "../pad2_anim_debug.h"
    }
}

void func_us_801C1194(Entity* self) {
    Entity* entity;
    s32 animCurFrame;

    switch (self->step) {
    case 0:
        InitializeEntity(D_us_80181078);
        self->hitboxState = 2;
        self->blendMode = BLEND_ADD | BLEND_TRANSP;
        if (!self->params) {
            self->drawFlags = ENTITY_OPACITY;
            self->hitboxWidth = 0x14;
            self->hitboxHeight = 0x30;
            self->hitboxOffX = -0x11;
            // fallthrough
        } else {
            self->blendMode = BLEND_ADD | BLEND_TRANSP;
            self->hitboxState = 0;
            self->step = 2;
            break;
        }
    case 1:
        AnimateEntity(D_us_801821E0, self);
        entity = self - 1;
        self->facingLeft = entity->facingLeft;
        self->posX.i.hi = entity->posX.i.hi;
        self->posY.i.hi = entity->posY.i.hi;

        if (entity->ext.salome.unk86) {
            if (!self->ext.salome.unk87) {
                PlaySfxPositional(SFX_MAGIC_NOISE_SWEEP);
                self->ext.salome.unk87 = true;
            }
            self->hitboxState = 2;
            self->opacity += 16;
            if (self->opacity > 0x80) {
                self->opacity = 0x80;
            }
            break;
        }

        self->ext.salome.unk87 = false;
        self->hitboxState = 0;
        if (self->opacity) {
            self->opacity -= 16;
            break;
        }

        self->animCurFrame = 0;
        break;
    case 2:
        AnimateEntity(&D_us_8018226C, self);
        entity = self - 2;
        self->facingLeft = entity->facingLeft;
        self->posX.i.hi = entity->posX.i.hi;
        self->posY.i.hi = entity->posY.i.hi;
        animCurFrame = entity->animCurFrame;
        if (self->facingLeft) {
            self->posX.i.hi -= D_us_80182274[animCurFrame].x;
        } else {
            self->posX.i.hi += D_us_80182274[animCurFrame].x;
        }
        self->posY.i.hi += D_us_80182274[animCurFrame].y;
        break;
    case 8:
        self->drawFlags = ENTITY_DEFAULT;
        self->blendMode = BLEND_NO;

        entity = self - 2;
        self->facingLeft = entity->facingLeft;
        self->posX.i.hi = entity->posX.i.hi;
        self->posY.i.hi = entity->posY.i.hi;
        self->animCurFrame = 0x38;
        break;
    }
}

void func_us_801C13B4(Entity* self) {
    Entity* player;
    s16 angle;

    if (!self->step) {
        InitializeEntity(D_us_80181084);
        player = &PLAYER;
        angle = GetAngleBetweenEntities(player, self);
        self->velocityX = (rcos(angle) * -0x10000) >> 0xC;
        self->velocityY = (rsin(angle) * -0x10000) >> 0xC;
    }
    AnimateEntity(D_us_801821E8, self);
    MoveEntity();

    if (self->hitFlags || self->flags & FLAG_DEAD) {
        DestroyEntity(self);
    }
}

void func_us_801C148C(Entity* self) {
    Collider collider;
    Entity* entity;
    s32 posX;
    s32 posY;

    if (self->flags & FLAG_DEAD) {
        entity = AllocEntity(&g_Entities[224], &g_Entities[TOTAL_ENTITY_COUNT]);
        if (entity != NULL) {
            CreateEntityFromEntity(E_EXPLOSION, self, entity);
            entity->params = EXPLOSION_FIREBALL;
        }
        DestroyEntity(self);
        return;
    }

    switch (self->step) {
    case 0:
        InitializeEntity(D_us_80181090);
        if (self->facingLeft) {
            self->velocityX = FIX(1.25);
        } else {
            self->velocityX = FIX(-1.25);
        }
        self->velocityY = FIX(-1.0);
        // fallthrough
    case 1:
        AnimateEntity(D_us_801821F4, self);
        MoveEntity();
        self->velocityY += FIX(0.09375);
        posX = self->posX.i.hi;
        posY = self->posY.i.hi + 2;
        g_api.CheckCollision(posX, posY, &collider, 0);
        if (!(collider.effects & EFFECT_SOLID)) {
            self->step++;
        }
        break;
    case 2:
        if (self->ext.salome.unk9C) {
            AnimateEntity(D_us_80182208, self);
        } else {
            AnimateEntity(D_us_801821F4, self);
        }
        MoveEntity();
        self->velocityY += FIX(0.09375);
        posX = self->posX.i.hi;
        posY = self->posY.i.hi;
        if (self->velocityX > 0) {
            posX += 8;
            g_api.CheckCollision(posX, posY, &collider, 0);
            if (collider.effects & EFFECT_SOLID) {
                PlaySfxPositional(SFX_SKULL_KNOCK_B);
                self->posX.i.hi += collider.unk14;
                self->velocityX = -self->velocityX;
            }
        } else {
            posX -= 8;
            g_api.CheckCollision(posX, posY, &collider, 0);
            if (collider.effects & EFFECT_SOLID) {
                PlaySfxPositional(SFX_SKULL_KNOCK_B);
                self->posX.i.hi += collider.unk1C;
                self->velocityX = -self->velocityX;
            }
        }
        posX = self->posX.i.hi;
        posY = self->posY.i.hi + 8;
        g_api.CheckCollision(posX, posY, &collider, 0);
        if (collider.effects & EFFECT_SOLID) {
            PlaySfxPositional(SFX_SKULL_KNOCK_B);
            self->posY.i.hi += collider.unk18;
            self->velocityY = -self->velocityY;
            self->velocityY -= self->velocityY / 8;

            if (!self->ext.salome.unk9C) {
                self->pose = 0;
                self->poseTimer = 0;
            }

            self->ext.salome.unk9C |= 1;
            if (abs(self->velocityY) < 0x4000) {
                self->flags |= FLAG_DEAD;
            }
        }

        if (self->velocityY < 0) {
            posY -= 0x10;
            g_api.CheckCollision(posX, posY, &collider, 0);
            if (collider.effects & EFFECT_SOLID) {
                PlaySfxPositional(SFX_SKULL_KNOCK_B);
                self->posY.i.hi += collider.unk20;
                self->velocityY = 0;
            }
        }
        break;
    }
}

void func_us_801C1804(Entity* self) {
    Collider collider;
    Entity* entity;
    s32 var_s1;
    s32 var_s3;
    s32 posX;
    s32 posY;

    var_s3 = self->hitFlags;
    if (self->flags & FLAG_DEAD) {
        var_s3 |= 3;
    }

    if (var_s3 && !(var_s3 & 0x80)) {
        self->ext.salome.unk9D = 1;
        if (self->step != 1) {
            self->velocityY = FIX(-2.0);
            SetStep(1);
        }
    }

    switch (self->step) {
    case 0:
        InitializeEntity(D_us_8018109C);
        self->attackElement = 7;
        self->drawFlags |= ENTITY_ROTATE;
        if (self->facingLeft) {
            self->velocityX = FIX(0.5);
        } else {
            self->velocityX = FIX(-0.5);
        }
        self->velocityY = 0;
        self->ext.salome.unk9C = 0;
        if (self->ext.salome.unk9E) {
            self->hitPoints <<= 2;
        }
        // fallthrough
    case 1:
        MoveEntity();
        self->rotate = 0;
        if (self->ext.salome.unk9E) {
            self->animCurFrame = 0x38;
        } else {
            self->animCurFrame = 0x2F;
        }

        if (self->facingLeft) {
            self->velocityX = FIX(0.5);
        } else {
            self->velocityX = FIX(-0.5);
        }
        self->velocityY += FIX(0.09375);

        if (self->ext.salome.unk9D) {
            self->velocityX = -self->velocityX;
            self->animCurFrame = 0x30;
            if (self->ext.salome.unk9E) {
                self->animCurFrame = 0x39;
            }
        }
        posX = self->posX.i.hi;
        posY = self->posY.i.hi;
        if (self->velocityX > 0) {
            if (!self->ext.salome.unk9C) {
                posX += 2;
            } else {
                posX += 8;
            }
            g_api.CheckCollision(posX, posY, &collider, 0);
            if (collider.effects & EFFECT_SOLID) {
                if (self->ext.salome.unk9C & 1) {
                    self->posX.i.hi += collider.unk14;
                    self->velocityX = -self->velocityX;
                }
            } else {
                self->ext.salome.unk9C |= 1;
            }
        } else {
            if (!self->ext.salome.unk9C) {
                posX -= 2;
            } else {
                posX -= 8;
            }
            g_api.CheckCollision(posX, posY, &collider, 0);
            if (collider.effects & EFFECT_SOLID) {
                if (self->ext.salome.unk9C & 1) {
                    self->posX.i.hi += collider.unk1C;
                    self->velocityX = -self->velocityX;
                }
            } else {
                self->ext.salome.unk9C |= 1;
            }
        }

        posX = self->posX.i.hi;
        if (self->ext.salome.unk9C & 2) {
            posY = self->posY.i.hi + 8;
        } else {
            posY = self->posY.i.hi + 2;
        }

        g_api.CheckCollision(posX, posY, &collider, 0);
        if (collider.effects & EFFECT_SOLID) {
            if (self->ext.salome.unk9C & 2) {
                self->posY.i.hi += collider.unk18;
                self->velocityY = 0;
                if (self->ext.salome.unk9D) {
                    self->ext.salome.unk9D = 0;
                    if (self->flags & FLAG_DEAD) {
                        PlaySfxPositional(SFX_STUTTER_EXPLODE_LOW);
                        entity = AllocEntity(
                            &g_Entities[224], &g_Entities[TOTAL_ENTITY_COUNT]);
                        if (entity != NULL) {
                            CreateEntityFromEntity(E_EXPLOSION, self, entity);
                            entity->params = EXPLOSION_FIREBALL;
                        }
                        DestroyEntity(self);
                        break;
                    }
                }
                PlaySfxPositional(SFX_STOMP_SOFT_B);
                SetStep(2);
            }
        } else {
            self->ext.salome.unk9C |= 2;
        }
        break;
    case 2:
        if (self->facingLeft) {
            self->velocityX = FIX(2.0);
        } else {
            self->velocityX = FIX(-2.0);
        }

        posX = self->posX.i.hi;
        posY = self->posY.i.hi + 8;
        g_api.CheckCollision(posX, posY, &collider, 0);
        if (collider.effects & EFFECT_SOLID) {
            var_s1 = 1;
            if (collider.effects & EFFECT_UNK_8000) {
                if (collider.effects & EFFECT_UNK_4000) {
                    if (self->facingLeft) {
                        var_s1 = 4;
                    } else {
                        var_s1 = 2;
                    }
                } else {
                    if (self->facingLeft) {
                        var_s1 = 2;
                    } else {
                        var_s1 = 4;
                    }
                }
            }
        }

        self->rotate = 0;
        if (var_s1 == 2) {
            self->velocityX -= self->velocityX / 2;
            self->rotate = 0x200;
            if (collider.effects & EFFECT_UNK_1000) {
                self->rotate = 0x100;
            }
            if (collider.effects & EFFECT_UNK_2000) {
                self->rotate = 0x80;
            }
        }

        if (var_s1 == 4) {
            self->velocityX += self->velocityX / 8;
            self->rotate = -0x200;
            if (collider.effects & EFFECT_UNK_1000) {
                self->rotate = -0x100;
            }
            if (collider.effects & EFFECT_UNK_2000) {
                self->rotate = -0x80;
            }
        }

        if (self->ext.salome.unk9E) {
            var_s3 = AnimateEntity(D_us_8018225C, self);
        } else {
            var_s3 = AnimateEntity(D_us_8018224C, self);
        }

        if (!var_s3) {
            PlaySfxPositional(SFX_QUIET_STEPS);
        }

        var_s1 = UnkCollisionFunc2(&D_us_80182190);
        if (var_s1 == 0xFF) {
            self->facingLeft ^= 1;
        }

        if (var_s1 == 0x80) {
            self->velocityY = FIX(-2.0);
            SetStep(1);
        }

        break;
    }
}
