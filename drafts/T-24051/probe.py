"""Reproduce bounded T-24051 source diagnostics in Docker; never edit production."""
from pathlib import Path
import argparse
import re
import subprocess
import sys
sys.path.insert(0, "drafts/T-24011")
from diagnostic import report

NAME = "func_80144E20"
OUT = Path("build/date24051/archive")
OUT.mkdir(parents=True, exist_ok=True)
base = Path("drafts/T-24011/func_80144E20.c").read_text()
production = Path("src/ovl/DATE/80141EE0.c")
original = production.read_text()
fallback = 'INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80141EE0", ' + NAME + ');'
if original.count(fallback) != 1:
    raise SystemExit("assigned production fallback changed")
declarations = "extern u8 D_8015BF48;\n" + "\n".join(
    "void " + name + "(void);" for name in sorted(
        set(re.findall(r"func_801[0-9A-F]+", base)) - {NAME}))
variants = {"baseline": base}
view = base.replace("void (**base)();", "s32 (**base)();")
view = re.sub(r"base = (sp\w+)\.f;", r"base = (s32 (**)()) \1.f;", view)
for name in ("normal_date_move_place", "func_80146224"):
    view = re.sub(r"&?" + name + r"(?= (?:==|!=))", r"(s32 (*)()) " + name, view)
variants["callback-s32"] = view
variants["case0-aggregate"] = base.replace(
    "    case 0:\n        base = spDEC.f;",
    "    case 0:\n        base = (void (**)()) &spDEC;")
variants["s32-aggregate"] = view.replace(
    "    case 0:\n        base = (s32 (**)()) spDEC.f;",
    "    case 0:\n        base = (s32 (**)()) &spDEC;")
variants["array-address"] = base.replace(
    "    case 0:\n        base = spDEC.f;",
    "    case 0:\n        base = (void (**)()) &spDEC.f;")
variants["byte-aggregate"] = base.replace(
    "    case 0:\n        base = spDEC.f;",
    "    case 0:\n        base = (void (**)()) (u8 *)&spDEC;")
variants["case-index"] = base.replace(
    "    case 0:\n        base = spDEC.f;",
    "    case 0:\n        base = &spDEC.f[D_800CA2F8];").replace(
    "    case 20:\n        base = sp34.f;",
    "    case 20:\n        base = &sp34.f[D_800CA2F8 - 20];")
word = base.replace("void (**base)();", "u32 *base;")
word = re.sub(r"base = (sp\w+)\.f;", r"base = (u32 *) \1.f;", word)
variants["word-base"] = word.replace("base[g->unk_110A]", "((void (*)()) base[g->unk_110A])")
improved = Path("drafts/T-24051/func_80144E20.c").read_text()
variants["assigned-default"] = improved
variants["capture-below"] = Path("drafts/T-24051/func_80144E20-4rows.c").read_text()
variants["selector-neighbor"] = improved.replace("D_800CA2F8", "((u8 *)&D_800CA2F4)[4]")
variants["flag-neighbor"] = improved.replace("D_8015BF48", "((u8 *)&D_8015BF44)[4]")
variants["fade-neighbor"] = improved.replace("D_800B593C", "((u8 *)D_800B5920)[0x1C]")
variants["flag-fade-neighbor"] = variants["flag-neighbor"].replace(
    "D_800B593C", "((u8 *)D_800B5920)[0x1C]")
shade = improved.replace("    GameState *g", "    register u8 shade = 0x80;\n    GameState *g")
shade = shade.replace("D_800B593C == 0x80", "D_800B593C == shade")
variants["byte-shade"] = shade
variants["const-g-byte-shade"] = shade.replace("    GameState *g", "    GameState * const g")
parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument("--variant", choices=sorted(variants))
args = parser.parse_args()
for label, body in variants.items():
    if args.variant and args.variant != label:
        continue
    directory = OUT / label
    directory.mkdir(exist_ok=True)
    source = directory / "source.c"
    obj = directory / "source.o"
    changed = original.replace(fallback, body).replace(
        '#include "ovl/DATE.h"', '#include "ovl/DATE.h"\n' + declarations, 1)
    source.write_text(changed)
    subprocess.run([sys.executable, "tools/cc.py", str(source), str(obj), "ido", "5.3"], check=True)
    result = subprocess.run([sys.executable, "tools/funcdiff.py", "--built", str(obj),
                             "--no-build", "--resolve", "--version", "default",
                             "--unit", "DATE", NAME], capture_output=True, text=True)
    print(label + ": source-only diagnostic", flush=True)
    report(result, NAME, directory / "diagnostic.txt")
if production.read_text() != original:
    raise SystemExit("production changed during measurement")
print("Production unchanged; no verified default C match is claimed.")
