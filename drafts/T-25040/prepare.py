"""Prepare annotated guarded/default dispatcher candidates from the exact replay."""
from pathlib import Path

directory = Path("build/t25040/final")
directory.mkdir(exist_ok=True)
body = Path("build/t25040/two-word-b8-dead-byte-view/body.c").read_text()
body = body.replace("    switch (gs->unk_03F) { default: break; }",
    "    /* FAKE: T-25040, the empty entry switch and flag/base test preserve the entry load.\n"
    "     * The empty year checks below retain the original branch layout. */\n"
    "    switch (gs->unk_03F) { default: break; }")
body = body.replace("            if (((CharFlags *)(gs->unk_F64 + ((u32)gs->unk_03F < 0)))->b7)",
    "            /* FAKE: T-25040, this zero offset folds after uopt, retaining the base-relative byte load. */\n"
    "            if (((CharFlags *)(gs->unk_F64 + ((u32)gs->unk_03F < 0)))->b7)")
body = body.replace("        if (gs->unk_03E == 0x62 + ((u32)gs->unk_03F < 0))",
    "        /* FAKE: T-25040, the late zero keeps constant 98 out of the allocator's interference graph. */\n"
    "        if (gs->unk_03E == 0x62 + ((u32)gs->unk_03F < 0))")
body = body.replace("            if ((u32)gs->unk_F64[1] < 0) {}",
    "            /* FAKE: T-25040, the dead byte view occupies uopt's static alias; the late-zero offset\n"
    "             * below keeps the real load base-relative. Both comparisons are false; no extra load emits. */\n"
    "            if ((u32)gs->unk_F64[1] < 0) {}")
body = body.replace(" {\n\n        }", " {\n        }")
(directory / "body.c").write_text(body)
source = Path("src/main/8006CB30.c").read_text()
needle = 'INCLUDE_ASM("asm/nonmatchings/main/8006CB30", func_8006D138);'
if needle not in source:
    start = source.index("void func_8006D138(void)")
    stop = source.index("void func_8006D4A0(void)", start)
    source = source[:start] + needle + "\n\n" + source[stop:]
assert source.count(needle) == 1
(directory / "guarded.c").write_text(source.replace(needle, "#ifdef NON_MATCHING\n" + body + "#else\n" + needle + "\n#endif"))
(directory / "default.c").write_text(source.replace(needle, body))
proof = Path("drafts/T-25036/prove.py").read_text().replace("t25036", "t25040").replace("func_8006D7EC", "func_8006D138")
Path("drafts/T-25040/prove.py").write_text(proof)
