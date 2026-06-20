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
extern u16 D_pspeu_09266DE0[];
extern AnimationFrame D_pspeu_09285730[];
extern AnimationFrame D_pspeu_09285A90;
extern FrameProperty D_pspeu_09285B00[];
extern AnimationFrame* D_pspeu_09285B20[];
extern s32 D_pspeu_09294300;

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_0923DFF0);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_0923E2D8);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_0923E388);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_0923EA08);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_0923EAB0);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_0923F040);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_0923F088);

extern AnimationFrame D_pspeu_09285978;
extern AnimationFrame D_pspeu_092859A0;
extern AnimationFrame D_pspeu_092859C8;
extern s32 D_pspeu_09295B50;

void func_pspeu_092401F8(
    s32 damageEffect, s32 damageKind, s16 step, s16 step_s);
void func_pspeu_09242D00(s16 arg0);
void func_pspeu_0923F0D0(
    s32 damageEffect, u32 damageKind, s16 step, s16 step_s) {
    DamageParam damageParam; // sp20
    s32 i;                   // s0
    s32 damageResult;        // s1
    s16 posX;                // s2
    s32 step_s_zero;         // s3

    if (g_Maria.unk7A == 2) {
        g_Maria.unk7A = 0;
        g_Maria.unk66 = 0;
    }

    step_s_zero = 0;
    if (D_pspeu_09295B50) {
        D_pspeu_09295B50--;
    }

    switch (g_Entities[64].step_s) {
    case 0:
        step_s_zero = true;
        func_pspeu_0923F040();
        if (damageKind < 0x10) {
            func_pspeu_0923F088();
        } else {
            g_Entities[64].entityRoomIndex = g_Entities[64].facingLeft;
        }

        if (damageEffect & 0x4000) {
            func_pspeu_092474A8(g_CurrentEntity, 0x30, 0);
            g_Entities[64].velocityY = -0x40000;
            func_pspeu_09242F50(FIX(-1.25));
            g_Entities[64].step_s = 1;
            g_Entities[64].anim = &D_pspeu_09285A90;
            g_Maria.damagePalette = 0x8130;
            func_pspeu_092474A8(g_CurrentEntity, 0x30, 0);
            func_pspeu_092474A8(g_CurrentEntity, 0x460021, 0);
            g_Maria.timers[2] = 6;
            g_api.PlaySfx(0x908);
        } else {
            if (damageEffect & 0x2000) {
                damageKind = 3;
            }

            switch (damageKind) {
            case 2:
                switch (step) {
                case 0:
                case 1:
                    g_Entities[64].velocityY = 0;
                    func_pspeu_09242F50(FIX(-1.25));
                    g_Entities[64].step_s = 6;
                    g_Entities[64].anim = &D_pspeu_09285978;
                    g_api.PlaySfx(0x907);
                    func_pspeu_092474A8(g_CurrentEntity, 0, 0);
                    break;
                case 2:
                    g_Entities[64].velocityY = 0;
                    func_pspeu_09242F50(FIX(-1.25));
                    g_Entities[64].step_s = 7;
                    g_Entities[64].anim = &D_pspeu_092859C8;
                    func_pspeu_092474A8(g_CurrentEntity, 0, 0);
                    g_api.PlaySfx(0x908);
                    break;
                case 4:
                case 3:
                    g_Entities[64].velocityY = -0x30000;
                    func_pspeu_09242F50(FIX(-1.25));
                    g_Entities[64].step_s = 1;
                    g_Entities[64].anim = &D_pspeu_092859A0;
                    g_api.PlaySfx(0x907);
                    break;
                }
                break;
            case 5:
            case 4:
                g_Maria.damageTaken = g_Entities[64].hitPoints;
                g_Entities[64].posY.val -= 1;
                g_Entities[64].velocityY = -0x8000;
                func_pspeu_09242F50(0xFFF80000);
                g_Entities[64].step_s = 2;
                g_Entities[64].anim = &D_pspeu_09285978;
                g_Maria.timers[2] = 0x200;
                g_Entities[64].facingLeft = g_Entities[64].entityRoomIndex;
                break;
            default:
            case 16:
            case 3:
                switch (step) {
                default:
                case 0:
                case 1:
                    g_Entities[64].velocityY = -0x40000;
                    func_pspeu_09242F50(FIX(-1.25));
                    g_Entities[64].step_s = 1;
                    g_Entities[64].anim = &D_pspeu_09285978;
                    g_api.PlaySfx(0x907);
                    break;
                case 2:
                    g_Entities[64].velocityY = 0;
                    func_pspeu_09242F50(FIX(-1.25));
                    g_Entities[64].step_s = 7;
                    g_Entities[64].anim = &D_pspeu_092859C8;
                    func_pspeu_092474A8(g_CurrentEntity, 0, 0);
                    g_api.PlaySfx(0x908);
                    break;
                case 4:
                case 3:
                    g_Entities[64].velocityY = -0x30000;
                    func_pspeu_09242F50(FIX(-1.25));
                    g_Entities[64].step_s = 1;
                    g_Entities[64].anim = &D_pspeu_092859A0;
                    g_api.PlaySfx(0x907);
                    break;
                }
                break;
            }

            g_Maria.damagePalette = 0x8166;
            g_Maria.timers[2] = 6;
            if (damageEffect & 0x8000) {
                func_pspeu_092474A8(g_CurrentEntity, 0x10008, 0);
                func_pspeu_092474A8(g_CurrentEntity, 9, 0);
                func_pspeu_092474A8(g_CurrentEntity, 0x430021, 0);
                g_Maria.damagePalette = 0x8160;
                g_Maria.timers[2] = 0x10;
            } else if (damageEffect & 0x40) {
                func_pspeu_092474A8(g_CurrentEntity, 0x5002E, 0);
                func_pspeu_092474A8(g_CurrentEntity, 0x440021, 0);
                g_Maria.damagePalette = 0x8166;
                g_Maria.timers[2] = 0x10;
            } else if (damageEffect & 0x2000) {
                func_pspeu_092474A8(g_CurrentEntity, 0x2F, 0);
                g_Maria.timers[2] = 0xC;
                g_Maria.damagePalette = 0x8169;
            } else {
                if (damageEffect & 0x800) {
                    func_pspeu_092474A8(g_CurrentEntity, 0x46, 0);
                    func_pspeu_092474A8(g_CurrentEntity, 0x560021, 0);
                    g_Maria.timers[2] = 0x10;
                    g_Maria.damagePalette = 0x8164;
                }
                if (damageEffect & 0x1000) {
                    func_pspeu_092474A8(g_CurrentEntity, 0x47, 0);
                    func_pspeu_092474A8(g_CurrentEntity, 0x570021, 0);
                    g_Maria.timers[2] = 8;
                    g_Maria.damagePalette = 0x8168;
                }
                if (!(damageEffect & 0xF840)) {
                    func_pspeu_092474A8(g_CurrentEntity, 0x530021, 0);
                }
            }
        }
        break;
    case 1:
        if ((g_Maria.vram_flag & TOUCHING_CEILING) &&
            g_Entities[64].velocityY < -0x10000) {
            g_Entities[64].velocityY = -0x10000;
        }

        if (func_pspeu_09243118(0x20280)) {
            return;
        }

        break;
    case 2:
        if ((g_Maria.unk04 & 0x8000) &&
            !(g_Maria.vram_flag & TOUCHING_ANY_SLOPE)) {
            goto block_6dc;
        }

        if ((g_Maria.vram_flag & TOUCHING_ANY_SLOPE) && !(g_GameTimer & 1)) {
            func_pspeu_092474A8(g_CurrentEntity, 0xA004A, 0);
        }

        if (!(g_Maria.vram_flag &
              (TOUCHING_L_WALL | TOUCHING_R_WALL | TOUCHING_CEILING))) {
            break;
        }

        if (g_Maria.vram_flag & TOUCHING_CEILING) {
            func_pspeu_0923DED8(1);
            g_Entities[64].velocityX /= 2;
            g_Entities[64].velocityY = 0;
            D_pspeu_09295B50 = 0x18;
            g_Entities[64].step_s = 5;
            damageParam.effects = EFFECT_NONE;
            damageParam.damageKind = 1;
            damageParam.damageTaken = g_Maria.damageTaken;
            damageResult = g_api.CalcPlayerDamageMaria(&damageParam);
            if (damageResult) {
                func_pspeu_09242D00(0x10);
                func_pspeu_092401F8(0, 2, 0xA, 2);
                return;
            }
            break;
        }

        if (g_StageId != STAGE_BO6 && g_StageId != STAGE_RBO6 &&
            g_StageId != STAGE_DRE) {
            for (i = 2; i < NUM_VERTICAL_SENSORS; i++) {
                if (g_Maria.colWall[i].effects & EFFECT_UNK_0002) {
                    break;
                }
            }

            if (i == NUM_VERTICAL_SENSORS) {
                for (i = NUM_VERTICAL_SENSORS + 2; i < NUM_VERTICAL_SENSORS * 2;
                     i++) {
                    if (g_Maria.colWall[i].effects & EFFECT_UNK_0002) {
                        break;
                    }
                }
            }

            if (i == NUM_VERTICAL_SENSORS * 2) {
            block_6dc:
                g_Entities[64].velocityY = -0x40000;
                func_pspeu_09242F50(FIX(-1.25));
                posX = -3;
                if (g_Entities[64].velocityX != 0) {
                    posX = -posX;
                }
                g_Entities[64].posY.i.hi += 0x14;
                g_Entities[64].posX.i.hi += posX;
                func_pspeu_092474A8(g_CurrentEntity, 0x90004, 0);
                g_Entities[64].posY.i.hi -= 0x14;
                g_Entities[64].posX.i.hi -= posX;
                g_api.PlaySfx(SFX_WALL_DEBRIS_B);
                g_api_func_80102CD8(2);
                g_Entities[64].step_s = 1;
                damageParam.effects = EFFECT_NONE;
                damageParam.damageKind = 1;
                damageParam.damageTaken = g_Maria.damageTaken;
                damageResult = g_api.CalcPlayerDamageMaria(&damageParam);
                if (damageResult) {
                    func_pspeu_09242D00(0x10);
                    func_pspeu_092401F8(0, 2, 0xA, 2);
                    return;
                }

                break;
            }
        }

        D_pspeu_09295B50 = 8;
        g_api.PlaySfx(SFX_WALL_DEBRIS_B);
        g_Entities[64].velocityY = -0x28000;
        g_api_func_80102CD8(2);
        g_Entities[64].step_s = 3;
        func_pspeu_092474A8(g_CurrentEntity, 0x8004B, 0);
        damageParam.effects = EFFECT_NONE;
        damageParam.damageKind = 1;
        damageParam.damageTaken = g_Maria.damageTaken;
        damageResult = g_api.CalcPlayerDamageMaria(&damageParam);
        if (damageResult) {
            func_pspeu_09242D00(0x10);
            func_pspeu_092401F8(0, 2, 0xA, 2);
            return;
        }

        break;
    case 3:
        if (D_pspeu_09295B50 == 0) {
            func_pspeu_09242F00(0xC000);
            if (func_pspeu_09243118(0x20280)) {
                return;
            }
        } else {
            break;
        }
        break;
    case 5:
        func_pspeu_09242D68(0x2000);
        if (D_pspeu_09295B50 != 0) {
            if ((g_Maria.vram_flag & TOUCHING_CEILING) && !(g_GameTimer & 3)) {
                func_pspeu_0923DED8(0);
            }
        } else if (
            (g_Maria.vram_flag & (TOUCHING_L_WALL | TOUCHING_R_WALL)) &&
            !(g_Maria.vram_flag &
              (TOUCHING_ANY_SLOPE | TOUCHING_RAISING_SLOPE | VRAM_FLAG_UNK2000 |
               TOUCHING_SLIGHT_SLOPE | TOUCHING_CEILING_SLOPE |
               VRAM_FLAG_UNK400 | VRAM_FLAG_UNK200 | VRAM_FLAG_UNK100 |
               TOUCHING_CEILING | TOUCHING_GROUND))) {
            g_Entities[64].velocityY += 0x1800;
            if (g_Entities[64].velocityY > 0x70000) {
                g_Entities[64].velocityY = 0x70000;
            }

            if (!(g_GameTimer & 3)) {
                func_pspeu_092474A8(g_CurrentEntity, 0x4004C, 0);
            }
        } else {
            g_Entities[64].step_s = 1;
            g_Entities[64].poseTimer = g_Entities[64].pose = 0;
        }
        break;
    case 6:
        func_pspeu_09242D68(0x2000);
        if (!(g_Maria.vram_flag & TOUCHING_GROUND)) {
            func_pspeu_09241688();
        }

        if (g_Entities[64].poseTimer < 0) {
            func_pspeu_09241558(g_Entities[64].velocityX);
        }
        break;
    case 7:
        func_pspeu_09242D68(0x2000);
        if (!(g_Maria.vram_flag & TOUCHING_GROUND)) {
            func_pspeu_09241688();
        }

        if (g_Entities[64].poseTimer < 0) {
            func_pspeu_09241480(0, g_Entities[64].velocityX);
        }
        break;
    case 4:
        break;
    }

    if (step_s_zero && g_Maria.unk72) {
        g_Entities[64].velocityY = 0;
    }
}

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_0923FFA8);

extern RECT D_pspeu_09276C70;
extern u16 D_pspeu_09285798[];
extern u16 D_pspeu_09285860[];
extern u16 D_pspeu_09285A30[];
extern s16 D_pspeu_09294738;
extern s16 D_pspeu_09294740;
extern u_long D_pspeu_09294748;

void func_pspeu_092401F8(
    s32 damageEffect, s32 damageKind, s16 step, s16 step_s) {
    s32 var_s3;       // s3
    PlayerDraw* draw; // s2
    s32 scrollX;      // s0
    s32 scrollY;      // s1

    var_s3 = 0;
    draw = &g_PlayerDraw[5];

    switch (g_Entities[64].step_s) {
    case 0:
        g_CutsceneFlags |= 0x200;
        func_pspeu_0923F040();
        func_pspeu_0923F088();
        func_pspeu_09242F50(0xFFFF4000);
        func_pspeu_09242D28(&D_pspeu_09285A30);
        g_Entities[64].velocityY = -0x1A000;
        g_api_PlaySfx(0x90A);
        g_Entities[64].step_s += 1;
        break;
    case 1:
        if (g_Entities[64].pose == 3) {
            func_pspeu_09242D28(&D_pspeu_09285798);
            g_Entities[64].step_s += 1;
        }
        break;
    case 2:
        g_Entities[64].velocityY += 0xB00;
        if (g_Entities[64].velocityY > 0x1000) {
            StoreImage(&D_pspeu_09276C70, &D_pspeu_09294748);
            D_pspeu_09294738 = 0;
            D_pspeu_09294740 = 0x40;
            g_Entities[64].step_s += 1;
        }
        break;
    case 3:
        g_Entities[64].velocityY += 0xB00;
        if (g_Maria.vram_flag & TOUCHING_GROUND) {
            func_pspeu_09242D28(&D_pspeu_09285860);
            g_Entities[64].step_s = 4;
            if (PLAYER.posX.i.hi > g_Entities[64].posX.i.hi) {
                g_Entities[64].facingLeft = 0;
                break;
            }
            g_Entities[64].facingLeft = 1;
        }
        break;
    case 4:
        if (g_Entities[64].velocityX > 0) {
            // nb. Weird no-op here
        }
        g_Entities[64].step_s++;
        // fallthrough
    case 5:
        scrollX = g_Tilemap.scrollX.i.hi + g_Entities[64].posX.i.hi;
        scrollY = g_Tilemap.scrollY.i.hi + g_Entities[64].posY.i.hi;
        if (g_Entities[64].velocityX > 0) {
            if (scrollX > 0x1AC) {
                g_Entities[64].step_s = 0x80;
                g_Entities[64].velocityX = 0;
                break;
            }
        } else if (scrollX < 0x153) {
            g_Entities[64].step_s = 0x80;
            g_Entities[64].velocityX = 0;
        }
        break;
    case 128:
        g_CutsceneFlags |= 0x20;
        break;
    }
}

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09240530);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09240590);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_092405E0);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09240630);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09240680);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_092406D0);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09240710);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_092409D0);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09240C18);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09240D60);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09241050);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_092411A0);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09241400);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09241480);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09241558);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09241608);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09241688);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_092417A0);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_092418B0);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09241928);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_092419E0);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09241A50);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09241B28);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09241BD0);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09241C80);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09241EC8);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09242240);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09242538);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_092425D0);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09242670);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_092428D8);

void func_pspeu_09242D00(s16 arg0) {
    g_Entities[64].step = arg0;
    g_Entities[64].step_s = 0;
}

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09242D28);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09242D68);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09242E08);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09242F00);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09242F50);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09242F98);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09243020);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_092430E0);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09243118);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09243A78);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09244358);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09244BA0);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09244EE0);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09245230);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09245268);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09245600);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09245978);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_092459D0);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09245C78);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09245D98);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09245E38);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09245ED8);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09246A88);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09246B30);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09246BE8);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09246CB0);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09246DA0);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09246EA8);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", CEN_Unused09247108);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09247118);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_092474A8);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09247598);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09247C48);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09247DA0);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09247EB0);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09247FA0);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_092486C0);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09249160);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09249550);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09249A88);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09249DB0);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", CEN_Unused0924A138);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_0924A148);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_0924A240);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_0924A298);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_0924AAB0);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_0924AB68);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_0924C808);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_0924D5E8);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_0924E1B8);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_0924EBC0);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_0924EFC8);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_0924EFE8);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_0924FA38);

Entity* func_pspeu_09246A88(s32, s32);
s32 func_pspeu_0924FAB0(s16 arg0) {
    Entity* entity = func_pspeu_09246A88(0x78, 0x80);

    if (entity == NULL) {
        return -1;
    }

    DestroyEntity(entity);
    entity->entityId = 0x10;
    entity->posX.val = g_Entities[64].posX.val;
    entity->posY.val = g_Entities[64].posY.val;
    entity->ext.mariaCen.unk80 = arg0;
    entity->ext.mariaCen.unk82 = 1;
    return 0;
}

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_0924FB40);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_092508B8);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09250950);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09250D30);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09250ED8);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09250FA0);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09250FE0);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09251068);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09251160);

void func_pspeu_09251270(void) {}

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09251278);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09251378);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_092513A8);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_092513E8);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09246B88_from_st0_09251428);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09251478);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09246B88_from_st0_092514B8);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09251508);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09251548);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09251580);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_092515B8);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09251600);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09251658);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09251698);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09251700);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09251750);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09251768);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09251780);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_092517D0);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_092518C8);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_092519E8);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09251A28);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09251AC0);

static s32 func_pspeu_09251B40(void) {
    if (g_Maria.vram_flag & TOUCHING_GROUND) {
        return func_pspeu_09251AC0();
    }
    g_Maria.padSim |= 0x8000;
    D_pspeu_09295E00 = 0x3C;
    return 0;
}

static s32 func_pspeu_09251BA0(void) {
    s32 ret = 0;

    if (g_Entities[64].facingLeft != D_pspeu_09295DF0) {
        if (D_pspeu_09295DF0 != 0) {
            g_Maria.padSim = 0x80;
            ret = 1;
        } else {
            g_Maria.padSim = 0x20;
            ret = 1;
        }
    }
    return ret;
}

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09251C18);

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_09251EB0);

static void func_pspeu_092521A8(void) {
    if (D_pspeu_09295E18 == 0) {
        func_pspeu_09251C18();
        return;
    }
    func_pspeu_09251EB0();
}

INCLUDE_ASM("st/cen_psp/nonmatchings/cen_psp/e_maria_step", func_pspeu_092521E8);

void EntityUnused(Entity* self);

void EntityMaria(Entity* self) {
    Entity* entity = self;
    self = &g_Entities[64];
    if (g_PlayableCharacter != PLAYER_ALUCARD) {
        DestroyEntity(self);
        return;
    }

    if (g_CastleFlags[INVERTED_CASTLE_UNLOCKED]) {
        DestroyEntity(self);
        return;
    }
    if (!self->ext.mariaCen.unkA0) {
        if (g_CastleFlags[MET_MARIA_IN_CEN]) {
            D_pspeu_09295D88 = 1;
            InitializeEntity(g_EInitSpawner);
            self->flags = FLAG_POS_CAMERA_LOCKED;
            self->animSet = -0x7FFF;
            self->animCurFrame = 0xA;
            self->unk5A = 0x48;
            self->palette = 0x210;
            self->zPriority = 0x80;
            D_pspeu_09295DB0 = 4;
            D_pspeu_09295DA0 = 0;
        } else {
            func_pspeu_09239FA0(0);
        }
        self->ext.mariaCen.unkA0 = 1;
    }

    if (D_pspeu_09295D88) {
        EntityUnused(self);
        return;
    }

    if (!self->ext.mariaCen.unkA2 && (g_CutsceneFlags & 0x10)) {
        if (!(D_pspeu_09295D90 & 0x10)) {
            D_pspeu_09295E10 = 0;
            self->ext.mariaCen.unkA2 = 1;
        }
    } else if (!self->ext.mariaCen.unkA2) {
        g_Maria.demo_timer = 2;
        g_Maria.padSim = 0;
        D_pspeu_09295DF8 = 0;
        D_pspeu_09295E10 = 0x15;
        D_pspeu_09295E18 = 0;
    }

    if (D_psp_091CE570 != 0) {
        func_pspeu_09251378();
        func_pspeu_092513A8();
        func_pspeu_092513E8();
        func_pspeu_09246B88_from_st0_09251428();
        func_pspeu_09251478();
        func_pspeu_09246B88_from_st0_092514B8();
        func_pspeu_09251508();
    }

    func_pspeu_092521E8();
    func_pspeu_0923B190();
    func_pspeu_09247118();

    g_Maria.unk6C = g_Maria.unk6A;
    D_pspeu_09295D90 = g_CutsceneFlags;
}

void EntityUnused(Entity* self) {}

static void func_pspeu_09253240(s32 arg0) { D_pspeu_09295D98 = arg0; }

static s32 func_pspeu_09253260(void) { return D_pspeu_09295D98; }
