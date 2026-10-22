"""Replay named MAIN flag-dispatch candidates in the full TU, Docker only."""
from pathlib import Path
import json
import re
import subprocess
import sys
sys.path.insert(0, "tools")
import funcdiff as fd
import funcdiff_verify as fv

name = "func_8006D138"
c = fd.locate(name, "main")
base = Path(c.src).read_text()
needle = 'INCLUDE_ASM("asm/nonmatchings/main/8006CB30", ' + name + ');'
old = Path("drafts/T-23003/main/func_8006D138/hand.c").read_text()
old = old[old.index("void " + name):]
old = old.replace("(((Rec38Flags *)gs->unk_F64)->f.", "(((CharFlags *)gs->unk_F64)->")
old = old.replace("((Rec38Flags *)gs->unk_F64)->f.", "((CharFlags *)gs->unk_F64)->")
old = old.replace("D_800E6280.unk_F64", "gs->unk_F64")
snap = old.replace("    GameState *gs = &D_800E6280;", "    GameState *gs = &D_800E6280;\n    u32 flags;\n\n    flags = *(u32 *)gs->unk_F64;")
first, second = snap.split("    switch (D_800E6280.unk_03F)", 2)[1:]
parts = re.split(r"(    case (?:6|9|1|12|3):\n)", first)
for i in range(2, len(parts), 2):
    parts[i] = re.sub(r"\(\(CharFlags \*\)gs->unk_F64\)->b(\d)",
        lambda m: "((s32)(flags << %d) < 0)" % (31-int(m[1])), parts[i], count=1)
snap = snap.split("    switch (D_800E6280.unk_03F)", 1)[0] + "    switch (D_800E6280.unk_03F)" + "".join(parts) + "    switch (D_800E6280.unk_03F)" + second
variants = {
    "typed": old,
    "snapshot": snap,
    "snapshot-base-entry": snap.replace("    flags = *(u32 *)gs->unk_F64;", "    flags = *(u32 *)gs->unk_F64;\n    if (gs) {}"),
    "snapshot-base-tail": snap.rstrip()[:-1] + "    if (gs) {}\n}\n",
}
all_fields = snap.replace("D_800E6280.", "gs->")
variants["typed-all"] = old.replace("D_800E6280.", "gs->")
variants["snapshot-all"] = all_fields
variants["snapshot-all-entry"] = all_fields.replace("    flags = *(u32 *)gs->unk_F64;", "    flags = *(u32 *)gs->unk_F64;\n    if (gs) {}")
variants["snapshot-all-combined"] = all_fields.replace("    flags = *(u32 *)gs->unk_F64;", "    flags = *(u32 *)gs->unk_F64;\n    if (flags | (u32)gs) {}")
bits = re.sub(r"\(\(s32\)\(flags << (\d+)\) < 0\)", lambda m: "((CharFlags *)&flags)->b%d" % (31-int(m[1])), variants["snapshot-all-combined"])
variants["snapshot-bits"] = bits
variants["snapshot-bits-separated"] = bits.replace("if (flags | (u32)gs) {}", "if (flags) {}\n    if (gs) {}")
opaque = "(GameState *)((u8 *)&D_800E6280 + ((u32)D_800E6280.unk_03F < 0))"
variants["opaque-base"] = bits.replace("GameState *gs = &D_800E6280;", "GameState *gs = " + opaque + ";")
variants["opaque-base-flags-test"] = variants["opaque-base"].replace("if (flags | (u32)gs) {}", "if (flags) {}")
flag_only = re.sub(r"gs->(unk_03F|unk_03E|unk_120)", r"D_800E6280.\1", variants["opaque-base"])
variants["opaque-flags-only"] = flag_only
union = flag_only.replace("u32 flags;", "Rec38Flags flags;").replace("flags = *(u32 *)", "flags.u = *(u32 *)").replace("if (flags | (u32)gs)", "if (flags.u | (u32)gs)").replace("((CharFlags *)&flags)->", "flags.f.")
variants["opaque-flags-union"] = union
variants["opaque-flags-scalar"] = re.sub(r"\(\(CharFlags \*\)&flags\)->b(\d)", lambda m: "((s32)(flags << %d) < 0)" % (31-int(m[1])), flag_only)
variants["opaque-union-notest"] = union.replace("    if (flags.u | (u32)gs) {}\n", "")
direct = variants["typed-all"].replace("\n\n    switch", "\n    if (*(u32 *)gs->unk_F64) {}\n\n    switch", 1)
variants["direct-flags-entry"] = direct
variants["direct-flags-base-entry"] = direct.replace("if (*(u32 *)gs->unk_F64) {}", "if (*(u32 *)gs->unk_F64 | (u32)gs) {}")
variants["direct-flags-signed-entry"] = direct.replace("if (*(u32 *)gs->unk_F64)", "if (*(s32 *)gs->unk_F64)")
variants["direct-flags-unsigned-bound"] = direct.replace("if (*(u32 *)gs->unk_F64)", "if ((u32)*(s32 *)gs->unk_F64 < 0)")
signed = bits.replace("u32 flags;", "s32 flags;").replace("flags = *(u32 *)", "flags = *(s32 *)")
variants["snapshot-signed"] = signed
variants["snapshot-signed-separated"] = signed.replace("if (flags | (u32)gs) {}", "if (flags) {}\n    if (gs) {}")
variants["snapshot-union-signed"] = signed.replace("s32 flags;", "Rec38Flags flags;").replace("flags = *(s32 *)", "flags.w = *(s32 *)").replace("if (flags | (u32)gs)", "if (flags.w | (u32)gs)").replace("((CharFlags *)&flags)->", "flags.f.")
variants["snapshot-local-selector"] = bits.replace("    u32 flags;", "    u32 flags;\n    u8 month;").replace("    flags = *(u32 *)gs->unk_F64;", "    month = gs->unk_03F;\n    flags = *(u32 *)gs->unk_F64;").replace("switch (gs->unk_03F)", "switch (month)", 1)
variants["snapshot-opaque-selector"] = bits.replace("switch (gs->unk_03F)", "switch (gs->unk_03F + ((u32)gs->unk_03F < 0))", 1)
variants["snapshot-entry-year"] = bits.replace("    flags = *(u32 *)gs->unk_F64;", "    (void)gs->unk_03E;\n    flags = *(u32 *)gs->unk_F64;")
variants["snapshot-local-month-first"] = variants["snapshot-local-selector"].replace("    if (flags | (u32)gs) {}", "    if (flags) {}\n    if (gs) {}")
variants["snapshot-empty-switch"] = bits.replace("    flags = *(u32 *)gs->unk_F64;", "    switch (gs->unk_03F) { default: break; }\n    flags = *(u32 *)gs->unk_F64;")
variants["snapshot-empty-switch-case"] = bits.replace("    flags = *(u32 *)gs->unk_F64;", "    switch (gs->unk_03F) { case 1: break; default: break; }\n    flags = *(u32 *)gs->unk_F64;")
variants["snapshot-selector-zero-use"] = bits.replace("    if (flags | (u32)gs) {}\n", "").replace("switch (gs->unk_03F)", "switch (gs->unk_03F + ((u32)flags < 0) + ((u32)gs < 0))", 1)
near = variants["snapshot-empty-switch"].replace("(s8) gs->unk_F64[0] < 0", "((CharFlags *)gs->unk_F64)->b7").replace("(s8) gs->unk_F64[0] >= 0", "!((CharFlags *)gs->unk_F64)->b7")
variants["entry-switch-bits"] = near
variants["entry-switch-separated"] = near.replace("if (flags | (u32)gs) {}", "if (flags) {}\n    if (gs) {}")
variants["entry-switch-flagonly"] = near.replace("if (flags | (u32)gs) {}", "if (flags) {}")
variants["entry-switch-nosnapshot"] = variants["direct-flags-base-entry"].replace("    if (*(u32 *)gs->unk_F64 | (u32)gs) {}", "    switch (gs->unk_03F) { default: break; }\n    if (*(u32 *)gs->unk_F64 | (u32)gs) {}")
near = variants["entry-switch-flagonly"]
pos = near.rindex("    switch (gs->unk_03F)")
variants["flag-boundary-live"] = near[:pos] + "    if (flags) {}\n" + near[pos:]
variants["flag-base-boundary-live"] = near[:pos] + "    if (flags | (u32)gs) {}\n" + near[pos:]
variants["flag-initial-pair"] = near.replace("    if (flags) {}", "    if (flags) {}\n    if (flags) {}", 1)
variants["entry-switch-return-int"] = near.replace("void func_8006D138(void)", "s32 func_8006D138(void)")
variants["flag-late-loop-entry"] = variants["flag-boundary-live"].replace("    if (flags) {}", "    do { if (flags) {} } while ((u32)gs->unk_03E < 0);", 1)
first_switch = near.index("    switch (gs->unk_03F)", near.index("    flags ="))
last_switch = near.rindex("    switch (gs->unk_03F)")
variants["first-switch-single-loop"] = near[:first_switch] + "    do {\n" + near[first_switch:last_switch] + "    } while ((u32)gs->unk_03E < 0);\n" + near[last_switch:]
variants["flag-all-case-ends"] = near[:last_switch].replace("        break;", "        if (flags) {}\n        break;") + near[last_switch:]
near = variants["entry-switch-nosnapshot"].replace("(s8) gs->unk_F64[0] < 0", "((CharFlags *)gs->unk_F64)->b7").replace("(s8) gs->unk_F64[0] >= 0", "!((CharFlags *)gs->unk_F64)->b7")
variants["direct-entry-switch-bits"] = near
variants["indirect-store-zero"] = re.sub(r"gs->unk_120\[([^\]]+)\]", r"gs->unk_120[\1 + ((u32)gs->unk_03F < 0)]", near)
variants["indirect-store-localzero"] = re.sub(r"gs->unk_120\[([^\]]+)\]", r"gs->unk_120[\1 + z]", near).replace("    GameState *gs = &D_800E6280;", "    GameState *gs = &D_800E6280;\n    s32 z = (u32)D_800E6280.unk_03F < 0;")
variants["indirect-absolute-stores"] = variants["indirect-store-zero"].replace("gs->unk_120", "D_800E6280.unk_120")
variants["absolute-stores-control"] = near.replace("gs->unk_120", "D_800E6280.unk_120")
variants["absolute-date-control"] = re.sub(r"gs->(unk_03E|unk_03F)", r"D_800E6280.\1", near)
variants["all-absolute-control"] = variants["absolute-stores-control"].replace("gs->unk_03E", "D_800E6280.unk_03E").replace("gs->unk_03F", "D_800E6280.unk_03F")
variants["volatile-output-stores"] = near.replace("gs->unk_120[", "((volatile u8 *)gs->unk_120)[")
variants["volatile-absolute-stores"] = near.replace("gs->unk_120[", "((volatile u8 *)D_800E6280.unk_120)[")
live_snapshot = variants["entry-switch-bits"]
variants["snapshot-store-latezero"] = re.sub(r"(gs->unk_120\[[^\]]+\] = )(1|0x80);", r"\g<1>\2 + ((u32)flags < 0);", live_snapshot)
variants["snapshot-first-store-latezero"] = variants["snapshot-store-latezero"][:variants["snapshot-store-latezero"].rindex("    switch (gs->unk_03F)")] + live_snapshot[live_snapshot.rindex("    switch (gs->unk_03F)"):]
variants["snapshot-store-identity"] = re.sub(r"(gs->unk_120\[[^\]]+\] = )(1|0x80);", r"\g<1>\2 | (flags & 0);", live_snapshot)
split = live_snapshot.rindex("    switch (gs->unk_03F)")
variants["snapshot-dead-return-join"] = live_snapshot[:split] + "    if ((u32)flags < 0) { return; }\n" + live_snapshot[split:]
variants["snapshot-dead-return-cases"] = live_snapshot[:split] + re.sub(r"(    case \d+:\n)", r"\1        if ((u32)flags < 0) { return; }\n", live_snapshot[split:])
variants["snapshot-dead-store-exits"] = re.sub(r"(            return;|        return;|        break;)", r"        if ((u32)flags < 0) { gs->unk_120[0] = 1; }\n\1", live_snapshot)
variants["snapshot-register-local"] = re.sub(r"\(\(CharFlags \*\)&flags\)->b(\d)", lambda m: "((s32)(flags << %d) < 0)" % (31-int(m[1])), live_snapshot.replace("u32 flags;", "register u32 flags;"))
local_year = live_snapshot.replace("u32 flags;", "u32 flags;\n    u8 year;").replace("gs->unk_03E", "year")
local_year = re.sub(r"(    case (?:7|10|1|12|4|5|8|11|2):\n)", r"\1        year = gs->unk_03E;\n", local_year)
variants["snapshot-local-year-arms"] = local_year
variants["direct-local-year-arms"] = local_year.replace("    u32 flags;\n", "").replace("    flags = *(u32 *)gs->unk_F64;\n", "").replace("if (flags | (u32)gs)", "if (*(u32 *)gs->unk_F64 | (u32)gs)").replace("((CharFlags *)&flags)->", "((CharFlags *)gs->unk_F64)->")
variants["indirect-field-address"] = re.sub(r"gs->unk_120\[([^\]]+)\]", r"*(&D_800E6280.unk_120[\1] + ((u32)gs->unk_03F < 0))", near)
variants["indirect-array-object"] = re.sub(r"gs->unk_120\[([^\]]+)\]", r"(*(u8 (*)[15])((u8 *)&D_800E6280.unk_120 + ((u32)gs->unk_03F < 0)))[\1]", near)
dead_outputs = "    if ((u32)flags < 0) {\n" + "".join("        gs->unk_120[%d] = 1;\n" % i for i in (0, 2, 4, 8, 9, 12)) + "    }\n"
variants["snapshot-dead-output-webs"] = live_snapshot.replace("    if (flags | (u32)gs) {}\n", "    if (flags | (u32)gs) {}\n" + dead_outputs)
variants["snapshot-dead-output-one"] = live_snapshot.replace("    if (flags | (u32)gs) {}\n", "    if (flags | (u32)gs) {}\n    if ((u32)flags < 0) { gs->unk_120[0] = 1; }\n")
variants["snapshot-dead-output-rmw"] = variants["snapshot-dead-output-webs"].replace("gs->unk_120[0] = 1;", "gs->unk_120[0]++;").replace("gs->unk_120[2] = 1;", "gs->unk_120[2]++;").replace("gs->unk_120[4] = 1;", "gs->unk_120[4]++;").replace("gs->unk_120[8] = 1;", "gs->unk_120[8]++;").replace("gs->unk_120[9] = 1;", "gs->unk_120[9]++;").replace("gs->unk_120[12] = 1;", "gs->unk_120[12]++;")
discard_sum = "flags + " + " + ".join("gs->unk_120[%d]" % i for i in (0, 2, 4, 8, 9, 12))
variants["snapshot-dead-read-webs"] = live_snapshot.replace("    if (flags | (u32)gs) {}\n", "    if (flags | (u32)gs) {}\n    if ((u32)(" + discard_sum + ") < 0) { return; }\n")
late_b7 = "((CharFlags *)(gs->unk_F64 + ((u32)gs->unk_03F < 0)))->b7"
variants["direct-first-b7-late-address"] = near.replace("((CharFlags *)gs->unk_F64)->b7", late_b7, 1)
variants["direct-all-b7-late-address"] = near.replace("((CharFlags *)gs->unk_F64)->b7", late_b7)
variants["direct-first-b7-volatile"] = near.replace("((CharFlags *)gs->unk_F64)->b7", "((volatile CharFlags *)gs->unk_F64)->b7", 1)
variants["direct-98-latezero"] = near.replace("gs->unk_03E == 0x62", "gs->unk_03E == 0x62 + ((u32)gs->unk_03F < 0)")
variants["direct-first-128-latezero"] = near.replace("= 0x80;", "= 0x80 + ((u32)gs->unk_03F < 0);", 1)
last_near = variants["direct-98-latezero"]
variants["near-b7-word-shift"] = last_near.replace("((CharFlags *)gs->unk_F64)->b7", "((s32)(*(u32 *)gs->unk_F64 << 24) < 0)", 1)
variants["near-b7-byte-signed"] = last_near.replace("((CharFlags *)gs->unk_F64)->b7", "(*(s8 *)gs->unk_F64 < 0)", 1)
variants["near-b7-late-address"] = last_near.replace("((CharFlags *)gs->unk_F64)->b7", late_b7, 1)
expected, _ = fd.expected_object(c)
want = fd.resolved_functions(expected, [name], c)[name]
for label in sys.argv[1:] or variants:
    directory = Path("build/t25040") / label
    directory.mkdir(parents=True, exist_ok=True)
    body = variants[label]
    (directory / "body.c").write_text(body)
    source = directory / "unit.c"
    source.write_text(base.replace(needle, body))
    obj, _ = fv.compile_unit(c, directory, source=source, non_matching=False)
    got = fd.resolved_functions(obj, [name], c)[name]
    rows = fd.diff_lines(want, got)
    (directory / "diff.txt").write_text("\n".join(rows) + "\n")
    dump = subprocess.check_output(["mips-linux-gnu-objdump", "-drz", "--disassemble=" + name, obj], text=True)
    (directory / "disassembly.txt").write_text(dump)
    result = dict(target_words=len(want), candidate_words=len(got), printed_lines=len(rows), aligned_cost=fd.diff_score(want, got))
    (directory / "measurement.json").write_text(json.dumps(result, indent=2) + "\n")
    print(label, result, flush=True)
assert Path(c.src).read_text() == base
