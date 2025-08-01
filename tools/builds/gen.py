# usage
# python3 ./tools/build/gen.py && ninja

import ninja_syntax
import os
import yaml

# global dictionary that collects all source entries
# useful to avoid adding multiple rules for the same output file
entries = dict()

psp_o4_files = [
    "3250.c",
    "menu.c",
    "186E8.c",
    "1DDC0.c",
    "23138.c",
    "4AEA4.c",
    "4CE2C.c",
    "4DA70.c",
    "5087C.c",
    "624DC.c",
    "628AC.c",
    "62EB8.c",
    "62FE0.c",
    "63C08.c",
    "66590.c",
    "collider.c",
    "game_handlers.c",
]


def is_psp(version: str) -> bool:
    return version.startswith("psp")


def is_hd(version: str) -> bool:
    return version == "hd"


def is_weapon(ovl_name: str) -> bool:
    return ovl_name == "weapon"


def is_servant(ovl_name: str) -> bool:
    return ovl_name.startswith("tt_")


def is_base_ovl(ovl_name: str) -> bool:
    return (
        ovl_name == "main"
        or ovl_name == "dra"
        or ovl_name == "ric"
        or ovl_name == "maria"
    )


def is_boss(ovl_name: str) -> bool:
    ovl_name = ovl_name.lower()
    return ovl_name.startswith("bo") or ovl_name.startswith("rbo") or ovl_name == "mar"


def is_stage(ovl_name: str) -> bool:
    return (
        is_weapon(ovl_name) == False
        and is_servant(ovl_name) == False
        and is_base_ovl(ovl_name) == False
        and is_boss(ovl_name) == False
    )


def get_cc_flags_for_exceptional_files(version: str, file_name: str):
    if version == "us" and file_name == "src/weapon/w_029.c":
        return "-O1"
    if not is_psp(version):
        return ""  # PSX is almost always -O2
    if file_name.startswith("src/dra") and os.path.basename(file_name) in psp_o4_files:
        return "-O4,p"
    return "-Op"


def add_c_psx(
    nw: ninja_syntax.Writer, ver: str, file_name: str, ld_path: str, cpp_flags: str
):
    output = f"build/{ver}/{file_name}.o"
    if output in entries:
        return
    entries[output] = {}
    rule = "psx-cc"
    if (
        file_name == "src/main/psxsdk/libgpu/sys.c"
        or file_name == "src/main/psxsdk/libgpu/font.c"
    ):
        rule = "psx-cc-2_21"
    nw.build(
        rule=rule,
        outputs=output,
        inputs=file_name,
        implicit=[
            f"src/.assets_build_done_{ver}",
            ld_path,
            "include/types.h",
            "include/common.h",
            "include/game.h",
            "include/entity.h",
            "include/sfx.h",
        ],
        variables={
            "version": version,
            "cpp_flags": cpp_flags,
            "cc_flags": get_cc_flags_for_exceptional_files(version, file_name),
        },
    )
    nw.build(
        rule="phony",
        outputs=file_name,
        implicit=[f"src/.assets_build_done_{ver}"],
    )
    return output


def add_s_psx(nw: ninja_syntax.Writer, ver: str, file_name: str, ld_path: str):
    output = f"build/{ver}/{file_name}.o"
    if output in entries:
        return
    entries[output] = {}
    nw.build(
        rule="psx-as",
        outputs=[output],
        inputs=[file_name],
    )
    nw.build(
        rule="phony",
        outputs=[file_name],
        implicit=[ld_path],
    )
    return output


def add_copy_psx(
    nw: ninja_syntax.Writer,
    version: str,
    in_file_name: str,
    out_file_name: str,
    ld_script_path: str,
):
    output = f"build/{version}/{out_file_name}.o"
    if output in entries:
        return
    entries[output] = {}
    nw.build(
        rule="psx-copy",
        outputs=[output],
        inputs=[in_file_name],
    )
    nw.build(
        rule="phony",
        outputs=[in_file_name],
        implicit=[ld_script_path],
    )
    return output


def add_memcard_img_psx(
    nw: ninja_syntax.Writer,
    version: str,
    in_file_name: str,
    out_file_name: str,
    ld_script_path: str,
):
    output_gfx = f"build/{version}/{out_file_name}.png"
    output_pal = f"build/{version}/{out_file_name}.pal"
    if output_gfx in entries:
        return
    if output_pal in entries:
        return
    entries[output_gfx] = {}
    entries[output_pal] = {}
    slot_id = out_file_name.split("memcard_")[1]
    nw.build(
        rule="psx-as",
        outputs=[f"{output_gfx}.o"],
        inputs=[f"{output_gfx}.s"],
    )
    nw.build(
        rule="psx-as",
        outputs=[f"{output_pal}.o"],
        inputs=[f"{output_pal}.s"],
    )
    nw.build(
        rule="memcard-encode",
        outputs=[f"{output_gfx}.s", f"{output_pal}.s"],
        inputs=[in_file_name],
        variables={
            "out_gfx": f"{output_gfx}.s",
            "out_pal": f"{output_pal}.s",
            "sym_gfx": f"g_saveIcon{slot_id}",
            "sym_pal": f"g_saveIconPal{slot_id}",
        },
    )
    nw.build(
        rule="phony",
        outputs=[in_file_name],
        implicit=[ld_script_path],
    )
    return [f"{output_gfx}.o", f"{output_pal}.o"]


def add_c_psp(
    nw: ninja_syntax.Writer, ver: str, file_name: str, ld_path: str, cpp_flags: str
):
    output = f"build/{ver}/{file_name}.o"
    if output in entries:
        return
    entries[output] = {}
    nw.build(
        rule="psp-cc",
        outputs=output,
        inputs=file_name,
        implicit=[
            f"src/.assets_build_done_{ver}",
            ld_path,
            "include/types.h",
            "include/common.h",
            "include/game.h",
            "include/entity.h",
        ],
        variables={
            "version": version,
            "cpp_flags": cpp_flags,
            "opt_level": get_cc_flags_for_exceptional_files(version, file_name),
            "src_dir": os.path.dirname(file_name),
        },
    )
    nw.build(
        rule="phony",
        outputs=file_name,
        implicit=[f"src/.assets_build_done_{ver}"],
    )
    return output


def add_s_psp(nw: ninja_syntax.Writer, ver: str, file_name: str, ld_path: str):
    output = f"build/{ver}/{file_name}.o"
    if output in entries:
        return
    entries[output] = {}
    nw.build(
        rule="psp-as",
        outputs=[output],
        inputs=[file_name],
    )
    nw.build(
        rule="phony",
        outputs=[file_name],
        implicit=[ld_path],
    )
    return output


def add_assets_config(nw: ninja_syntax.Writer, version: str):
    nw.build(
        rule="assets-build",
        inputs=f"config/assets.{version}.yaml",
        outputs=f"src/.assets_build_done_{version}",
        implicit=f"assets/done_{version}",
    )


def add_gfx_stage(
    nw: ninja_syntax.Writer, target_path: str, asset_path: str, output_name: str
):
    nw.build(
        rule="gfxstage-decode",
        outputs=f"{asset_path}_0.png",
        inputs=target_path,
        variables={
            "path": asset_path,
        },
    )
    nw.build(
        rule="gfxstage-encode",
        outputs=[output_name],
        inputs=[f"{asset_path}_0.png"],
        variables={
            "path": asset_path,
        },
    )


def add_gfx_weapon(nw: ninja_syntax, version: str, base_file: str, linker_path: str):
    ovl_id = os.path.basename(base_file).strip("f_")
    in_file = f"{base_file}.png"
    step_raw = os.path.join("build", version, f"{base_file}.bin")
    step_elf = f"build/{version}/weapon/f0_{ovl_id}.elf"
    step_bin = f"build/{version}/weapon/f0_{ovl_id}.bin"
    nw.build(
        rule="psx-strip",
        outputs=step_bin,
        inputs=step_elf,
    )
    nw.build(
        rule="psx-copy",
        outputs=step_elf,
        inputs=step_raw,
    )
    nw.build(
        rule="gfxweapon-encode",
        outputs=step_raw,
        inputs=in_file,
    )
    nw.build(
        rule="phony",
        outputs=in_file,
        implicit=linker_path,
    )
    return step_bin


def add_weapon_splat_config(nw: ninja_syntax.Writer, ver: str, splat_config):
    hand_id = 0  # hardcoded for the time being
    platform = str(splat_config["options"]["platform"])
    asset_path = str(splat_config["options"]["asset_path"])
    ld_path = str(splat_config["options"]["ld_script_path"])
    symbol_paths = list[str](splat_config["options"]["symbol_addrs_path"])
    undefined_funcs_auto_path = str(
        splat_config["options"]["undefined_funcs_auto_path"]
    )
    undefined_syms_auto_path = str(splat_config["options"]["undefined_syms_auto_path"])
    asm_path = str(splat_config["options"]["asm_path"])
    src_path = str(splat_config["options"]["src_path"])

    nw.build(
        rule="splat",
        outputs=[ld_path],
        inputs=[entry.path],
        implicit=symbol_paths,
    )
    if platform == "psx":
        add_c = add_c_psx
        add_s = add_s_psx
    elif platform == "psp":
        add_c = add_c_psp
        add_s = add_s_psp
    else:
        raise Exception(f"platform {platform} not recognized")

    weapons = []
    weapon_gfx = ""  # horrible hack as code segments for weapon do not have a name
    for segment in splat_config["segments"]:
        if not "type" in segment:
            continue
        if segment["type"] == "grey4":
            weapon_gfx = segment["name"]
            weapons.append(
                add_gfx_weapon(nw, ver, os.path.join(asset_path, weapon_gfx), ld_path)
            )
            continue
        if segment["type"] != "code":
            continue
        weapon_id = weapon_gfx[2:]
        cpp_flags = f"-DW_{weapon_id} -DWEAPON{hand_id}"
        objs = []
        for subsegment in segment["subsegments"]:
            kind = subsegment[1]
            if kind == "data":
                obj = add_s(nw, ver, f"{asm_path}/data/w_{weapon_id}.data.s", ld_path)
                objs.append(obj)
            elif kind == "sbss":
                obj = add_s(nw, ver, f"{asm_path}/data/w_{weapon_id}.sbss.s", ld_path)
                objs.append(obj)
            elif kind == "c":
                obj = add_c(nw, ver, f"{src_path}/w_{weapon_id}.c", ld_path, cpp_flags)
                objs.insert(0, obj)  # the C file needs to always be linked first
            elif kind == ".data":
                continue
            elif kind == ".rodata":
                continue
            elif kind == ".bss":
                continue
            elif kind == "animset":
                continue
            else:
                raise Exception(f"unknown subsegment type {kind}")
        step_elf = f"build/{ver}/weapon/w{hand_id}_{weapon_id}.elf"
        symbols_lists = [
            f"-T config/undefined_syms.{ver}.txt",
            f"-T {undefined_funcs_auto_path}",
            f"-T {undefined_syms_auto_path}",
        ]
        nw.build(
            rule="psx-ld",
            outputs=step_elf,
            inputs=f"weapon{hand_id}.ld",
            implicit=objs,
            variables={
                "version": ver,
                "obj_files": objs,
                "ld_flags": "--gc-sections" if platform == "psp" else "",
                "map_out": f"build/{ver}/weapon/w{hand_id}_{weapon_id}.map",
                "symbols_arg": str.join(" ", symbols_lists),
            },
        )
        step_bin = f"build/{ver}/weapon/w{hand_id}_{weapon_id}.bin"
        nw.build(
            rule="psx-strip-weapon",
            outputs=step_bin,
            inputs=step_elf,
        )
        weapons.append(step_bin)
    nw.build(
        rule="concat",
        outputs=f"build/{ver}/WEAPON0.BIN",
        inputs=weapons,
    )


def add_splat_config(nw: ninja_syntax.Writer, version: str, file_name: str):
    with open(file_name) as f:
        splat_config = yaml.load(f, Loader=yaml.SafeLoader)
    platform = str(splat_config["options"]["platform"])
    ovl_name = str(splat_config["options"]["basename"])
    if ovl_name == "weapon":
        add_weapon_splat_config(nw, version, splat_config)
        return
    target_path = str(splat_config["options"]["target_path"])
    asset_path = str(splat_config["options"]["asset_path"])
    ld_path = str(splat_config["options"]["ld_script_path"])
    symbol_paths = list[str](splat_config["options"]["symbol_addrs_path"])
    undefined_funcs_auto_path = str(
        splat_config["options"]["undefined_funcs_auto_path"]
    )
    undefined_syms_auto_path = str(splat_config["options"]["undefined_syms_auto_path"])
    asm_path = str(splat_config["options"]["asm_path"])
    src_path = str(splat_config["options"]["src_path"])
    nw.build(
        rule="splat",
        outputs=[ld_path],
        inputs=[entry.path],
        implicit=symbol_paths,
    )
    if platform == "psx":
        add_c = add_c_psx
        add_s = add_s_psx
    elif platform == "psp":
        add_c = add_c_psp
        add_s = add_s_psp
    else:
        raise Exception(f"platform {platform} not recognized")
    objs = []
    if ovl_name == "main":
        objs.append(add_s(nw, version, f"{asm_path}/header.s", ld_path))
    for segment in splat_config["segments"]:
        if not "type" in segment:
            continue
        if segment["type"] == "data":
            asm_name = f"{asm_path}/data/{segment["name"]}.data.s"
            objs.append(add_s(nw, version, asm_name, ld_path))
            continue
        for subsegment in segment["subsegments"]:
            if isinstance(subsegment, dict):  # handle PSP BSS
                if not "start" in subsegment:  # skip malformed BSS
                    continue
                subsegment_as_obj = subsegment
                subsegment = [  # normalize subsegment
                    subsegment["start"],
                    subsegment["type"],
                ]
                if "name" in subsegment_as_obj:
                    subsegment.append(subsegment_as_obj["name"])
            offset = int(subsegment[0])
            if len(subsegment) < 2:  # for subsegment entries without a kind
                kind = "data"
                name = segment["name"]
            else:
                kind = str(subsegment[1])
                if len(subsegment) > 2:
                    name = str(subsegment[2])
                else:
                    name = str.format("{0:X}", offset)
            if kind == "c" or kind == ".data" or kind == ".rodata" or kind == ".bss":
                objs.append(add_c(nw, version, f"{src_path}/{name}.c", ld_path, ""))
            elif kind == "data" or kind == "rodata" or kind == "bss" or kind == "sbss":
                obj = add_s(nw, version, f"{asm_path}/data/{name}.{kind}.s", ld_path)
                objs.append(obj)
            elif kind == "asm":
                objs.append(add_s(nw, version, f"{asm_path}/{name}.s", ld_path))
            elif kind == "raw" or kind == "cmp":
                objs.append(
                    add_copy_psx(
                        nw,
                        version,
                        f"{asset_path}/{name}.bin",
                        f"{asset_path}/{name}",
                        ld_path,
                    )
                )
            elif kind == "cmpgfx":
                objs.append(
                    add_copy_psx(
                        nw,
                        version,
                        f"{asset_path}/{name}.gfxbin",
                        f"{asset_path}/{name}",
                        ld_path,
                    )
                )
            elif kind == "pal":
                objs.append(
                    add_copy_psx(
                        nw,
                        version,
                        f"{asset_path}/{name}.palbin",
                        f"{asset_path}/{name}",
                        ld_path,
                    )
                )
            elif kind == "palette":
                objs_memcard = add_memcard_img_psx(
                    nw,
                    version,
                    f"{asset_path}/{name}.png",
                    f"{asset_path}/{name}",
                    ld_path,
                )
                objs += objs_memcard
            else:
                continue
    if platform == "psp" and file_name != "PS.ELF":
        mwo = os.path.join(asset_path, "mwo_header.bin")
        objs.append(add_copy_psx(nw, version, mwo, mwo, ld_path))
    output_elf = f"build/{version}/{ovl_name}.elf"
    sym_version = version
    if ovl_name == "stmad":
        sym_version = "beta"
    symbols_lists = [
        f"-T config/undefined_syms.{sym_version}.txt",
        f"-T {undefined_syms_auto_path}",
    ]
    if ovl_name != "main":
        symbols_lists.append(f"-T {undefined_funcs_auto_path}")
    if platform == "psp":
        symbols_lists.append(f"-T config/symexport.{version}.{ovl_name}.txt")
    nw.build(
        rule="psx-ld",
        outputs=[output_elf],
        inputs=[ld_path],
        implicit=[x for x in objs if x],
        variables={
            "ld_flags": "--gc-sections" if platform == "psp" else "",
            "map_out": f"build/{version}/{ovl_name}.map",
            "symbols_arg": str.join(" ", symbols_lists),
        },
    )
    strip_rule = "psx-strip"
    if platform != "psp" and not is_hd(version) and is_servant(ovl_name):
        strip_rule = "psx-strip-servant"
    output_name = os.path.basename(target_path)
    if ovl_name == "main":
        output_name = "main.exe"
    nw.build(
        rule=strip_rule,
        outputs=[f"build/{version}/{output_name}"],
        inputs=[output_elf],
    )
    if platform == "psx":
        if (is_stage(ovl_name) or is_boss(ovl_name)) and ovl_name != "stsel":
            gfx_name = f"F_{os.path.basename(target_path)}"
            if is_hd(version):
                gfx_name = f"f_{os.path.basename(target_path)}"
            stage_gfx_path = f"build/{version}/{gfx_name}"
            target_f_path = os.path.join(os.path.dirname(target_path), gfx_name)
            add_gfx_stage(nw, target_f_path, asset_path, stage_gfx_path)
    return


def add_checksum(nw: ninja_syntax.Writer, version: str, file_name: str):
    with open(file_name) as f:
        lines = f.readlines()
    binaries = [line.split(" ")[2].strip() for line in lines]
    if version == "us":
        dirt = "build/us/dra.dirt.done"
        binaries.append(dirt)
        nw.build(
            rule="dirt",
            outputs=dirt,
            inputs="config/dirt.us.json",
            implicit="build/us/DRA.BIN",
        )
    nw.build(
        rule="check",
        outputs=["🆗"],
        inputs=file_name,
        implicit=binaries,
    )


with open("build.ninja", "w") as f:
    nw = ninja_syntax.Writer(f)
    nw.rule(
        "splat",
        # 'touch' circumnavigates a bug where splat would not update the
        # mtime of the linker script if it is already up-to-date.
        command=".venv/bin/splat split $in > /dev/null && touch $out",
        description="splat $in",
    )
    nw.rule(
        "concat",
        command="cat $in > $out",
        description="concat to $out",
    )
    cpp_defs = "-Dmips -D__GNUC__=2 -D__OPTIMIZE__ -D__mips__ -D__mips -Dpsx -D__psx__ -D__psx -D_PSYQ -D__EXTENSIONS__ -D_MIPSEL -D_LANGUAGE_C -DLANGUAGE_C -DNO_LOGS -DHACKS -DUSE_INCLUDE_ASM -D_internal_version_$version -DSOTN_STR"
    cc_command = (
        "VERSION=$version"
        f" mipsel-linux-gnu-cpp $cpp_flags -lang-c -Iinclude -Iinclude/psxsdk -undef -Wall -fno-builtin {cpp_defs} $in"
        " | tools/sotn_str/target/release/sotn_str process"
        " | iconv --from-code=UTF-8 --to-code=Shift-JIS"
        " | bin/cc1-psx-26 -G0 -w -O2 -funsigned-char -fpeephole -ffunction-cse -fpcc-struct-return -fcommon -fverbose-asm -msoft-float -g -quiet -mcpu=3000 -fgnu-linker -mgas -gcoff $cc_flags"
        " | python3 tools/maspsx/maspsx.py  --expand-div --aspsx-version=2.34"
        " | mipsel-linux-gnu-as -Iinclude -march=r3000 -mtune=r3000 -no-pad-sections -O1 -G0 -o $out"
    )
    nw.rule(
        "psx-cc",
        command=cc_command,
        description="psx cc $in",
    )
    nw.rule(
        "psx-cc-2_21",
        command=cc_command.replace("--aspsx-version=2.34", "--aspsx-version=2.21"),
        description="psx cc $in",
    )
    nw.rule(
        "psx-as",
        command="mipsel-linux-gnu-as -Iinclude -march=r3000 -mtune=r3000 -no-pad-sections -O1 -G0 -o $out $in",
        description="psx as $in",
    )
    nw.rule(
        "psx-copy",
        command="mipsel-linux-gnu-ld -r -b binary -o $out $in",
        description="psx copy $in",
    )
    nw.rule(
        "psx-ld",
        command="mipsel-linux-gnu-ld -nostdlib --no-check-sections $ld_flags -Map $map_out -T $in $symbols_arg -o $out $obj_files",
        description="mipsel-linux-gnu-ld -nostdlib --no-check-sections -Map $map_out -T $in $symbols_arg -o $out",
    )
    nw.rule(
        "psx-strip",
        command="mipsel-linux-gnu-objcopy -O binary $in $out",
        description="psx strip $in",
    )
    nw.rule(
        "psx-strip-servant",
        command="mipsel-linux-gnu-objcopy -O binary $in $out && truncate -c -s 40960 $out",
        description="psx strip $in",
    )
    nw.rule(
        "psx-strip-weapon",
        command="mipsel-linux-gnu-objcopy -O binary $in $out && truncate -c -s 12288 $out",
        description="psx strip $in",
    )
    nw.rule(
        "psp-cc",
        command=(
            "VERSION=$version"
            " tools/sotn_str/target/release/sotn_str process -p -f $in"
            " | .venv/bin/python3 tools/mwccgap/mwccgap.py $out --src-dir $src_dir"
            " --mwcc-path bin/mwccpsp.exe --use-wibo --wibo-path bin/wibo --as-path bin/allegrex-as"
            " --asm-dir-prefix asm/pspeu --target-encoding sjis --macro-inc-path include/macro.inc"
            " -gccinc -Iinclude -D_internal_version_$version -DSOTN_STR -c -lang c -sdatathreshold 0 -char unsigned -fl divbyzerocheck"
            " $opt_level -opt nointrinsics"
        ),
        description="psp cc $in",
    )
    nw.rule(
        "psp-as",
        command="bin/allegrex-as -EL -I include/ -G0 -march=allegrex -mabi=eabi -o $out $in",
        description="psp as $in",
    )
    nw.rule(
        "dirt",
        command=".venv/bin/python3 tools/dirt_patcher.py $in && touch $out",
        description="dirt $in",
    )
    nw.rule(
        "assets-extract",
        command="bin/sotn-assets extract $in && touch $out",
        description="extract $in",
    )
    nw.rule(
        "assets-build",
        command="bin/sotn-assets build $in && mkdir -p $$(dirname $out) && touch $out",
        description="build $in",
    )
    nw.rule(
        "gfxstage-decode",
        command=".venv/bin/python3 tools/gfxstage.py d $in $path",
        description="gfxstage decode $in",
    )
    nw.rule(
        "gfxstage-encode",
        command=".venv/bin/python3 tools/gfxstage.py e $path $out",
        description="gfxstage encode $in",
    )
    nw.rule(
        "gfxweapon-encode",
        command=".venv/bin/python3 tools/png2bin.py $in $out",
        description="gfxweapon encode $in",
    )
    nw.rule(
        "memcard-encode",
        command=".venv/bin/python3 tools/png2s.py encode $in $out_gfx $sym_gfx $out_pal $sym_pal",
        description="memcard icon encode $in",
    )
    nw.rule(
        "check",
        command=".venv/bin/python3 tools/builds/check.py $in",
        description="check $in",
    )

    actual_version = os.getenv("VERSION")
    if not actual_version:
        actual_version = "us"
    for version in [actual_version]:
        for entry in os.scandir("config/"):
            if not entry.name.startswith(f"splat.{version}."):
                continue
            add_splat_config(nw, version, entry.path)
        nw.build(
            rule="assets-extract",
            outputs=f"assets/done_{version}",
            inputs=f"config/assets.{version}.yaml",
            implicit="bin/sotn-assets",
        )
        add_assets_config(nw, version)
        add_checksum(nw, version, f"config/check.{version}.sha")
    nw.close()
