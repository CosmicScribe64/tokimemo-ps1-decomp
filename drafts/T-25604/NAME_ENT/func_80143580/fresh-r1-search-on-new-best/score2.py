"""T-25604 search-lens scratch helper: score bodies of NAME_ENT func_80143580 on Best and Rev 1.

    tools/docker.sh python3 drafts/T-25604/NAME_ENT/func_80143580/fresh-r1-search-on-new-best/score2.py BODY.c [BODY.c ...]

For each body (the format of ../fresh-r1-best.c) it compiles the whole unit twice: as is (funcdiff
--resolve rows and Levenshtein score against the target) and with the rev1 source delta
(../fresh-r1/rev1.py), aligned with build/T-25604/ref/rev1.s (a local copy of the gitignored
versions/ asm, never committed): structural hunks and register substitutions. Also prints the key
colours of ../fresh-r1/colours.py. One line per body. Build output: build/T-25604/s2/<stem>.
Option --no-rev1 skips the second compile. No game data is read from or written to tracked paths.
"""
from pathlib import Path
import collections, difflib, os, re, subprocess, sys
HERE = Path(__file__).parent
sys.path.insert(0, "tools")
sys.path.insert(0, str(HERE.parent / "fresh-r1"))
import funcloc, funcdiff, order_search
import rev1

NAME = "func_80143580"
ASM = 'INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80142A80", func_80143580);\n'
REV1 = Path("build/T-25604/ref/rev1.s")


def unit_text(c, body):
    """The unit with BODY in the NON_MATCHING guard. Body lines `#define MAIN_API_OVERRIDE_...` go to the
    top of the file (before the first include), as the project's override mechanism needs."""
    # SCORE2_UNIT: a guarded copy of the unit to use as the template (once src holds the plain C)
    text = Path(os.environ.get("SCORE2_UNIT", c.src)).read_text()
    top = [l for l in body.splitlines(True) if l.startswith("#define MAIN_API_OVERRIDE_")]
    body = "".join(l for l in body.splitlines(True) if not l.startswith("#define MAIN_API_OVERRIDE_"))
    text = "".join(top) + text
    if ASM + "#endif" not in text:
        view = "void func_80143498(void) {\n"
        text = text.replace(view, "s32 func_80143498(void) {\n")
        return text.replace(ASM, "#ifdef NON_MATCHING\n" + body + "#else\n" + ASM + "#endif\n")
    s = text.rindex("#ifdef NON_MATCHING\n", 0, text.index(ASM + "#endif"))
    e = text.index("#else\n" + ASM)
    return text[:s] + "#ifdef NON_MATCHING\n" + body + text[e:]


def canon(op, args):
    a = [x.strip().lstrip('$') for x in args.split(',')] if args.strip() else []
    if op in ('or', 'addu') and len(a) == 3 and a[2] == 'zero': op, a = 'move', a[:2]
    if op == 'addiu' and len(a) == 3 and a[1] == 'zero': op, a = 'li', [a[0]]
    if op == 'ori' and len(a) == 3 and a[1] == 'zero': op, a = 'li', [a[0]]
    if op == 'beq' and len(a) == 3 and a[1] == 'zero': op, a = 'beqz', a[:1]
    if op == 'bne' and len(a) == 3 and a[1] == 'zero': op, a = 'bnez', a[:1]
    if op in ('b', 'j', 'jal'): a = []
    regs = [re.sub(r'.*\((\w+)\)', r'\1', x) for x in a]
    regs = [r for r in regs if re.fullmatch(r'(v[01]|a[0-3]|t\d|s\d|ra|sp|at|zero|fp|gp|k[01])', r)]
    return op, regs


def dis(obj):
    out = subprocess.run(["mips-linux-gnu-objdump", "-d", "--disassemble=" + NAME, obj],
                         capture_output=True, text=True, check=True).stdout
    return out


def parse_dis(text):
    out = []
    for line in text.splitlines():
        m = re.match(r"\s+[0-9a-f]+:\s+[0-9a-f]{8}\s+(\S+)\s*([^<]*)", line)
        if m: out.append(canon(m.group(1), m.group(2)))
    return out


def parse_splat(path):
    out = []
    for line in open(path):
        m = re.match(r"\s*/\* [0-9A-F]+ [0-9A-F]+ [0-9A-F]{8} \*/\s+(\S+)\s*(.*?)\s*(#.*)?$", line)
        if m and not m.group(1).startswith('.'):
            out.append(canon(m.group(1), re.sub(r'%(hi|lo)\([^)]*\)', '0', m.group(2))))
    return out


def compare(D, R):
    sm = difflib.SequenceMatcher(None, [o for o, _ in D], [o for o, _ in R], autojunk=False)
    pairs = collections.Counter(); hunks = 0
    for tag, i1, i2, j1, j2 in sm.get_opcodes():
        if tag != 'equal': hunks += 1; continue
        for k in range(i2 - i1):
            for x, y in zip(D[i1 + k][1], R[j1 + k][1]):
                if x != y: pairs[(x, y)] += 1
    return hunks, pairs


def colours(text):
    c = collections.defaultdict(collections.Counter)
    for l in text.splitlines():
        m = re.match(r"\s+[0-9a-f]+:\s+[0-9a-f]{8}\s+(\S+)\s*([^<]*)", l)
        if not m: continue
        op, a = m.group(1), [x.strip() for x in m.group(2).split(",")]
        if op == "lbu" and a[1].startswith("4365("): c["sel"][a[0]] += 1
        elif op == "lbu" and re.match(r"(18|54)\(", a[1]): c["f0" if a[1].startswith("18") else "f1"][a[0]] += 1
        elif op == "lui" and a[1] == "0x8000": c["k8"][a[0]] += 1
    return " ".join(f"{k}=" + "/".join(r for r, _ in c[k].most_common(2)) for k in ("sel", "f0", "f1", "k8"))


def main():
    args = [a for a in sys.argv[1:] if not a.startswith("--")]
    do_rev1 = "--no-rev1" not in sys.argv
    c = funcloc.locate(NAME, "NAME_ENT")
    expected, problem = funcdiff.expected_object(c)
    assert not problem, problem
    want = funcdiff.resolved_functions(expected, [NAME], c)[NAME]
    R = parse_splat(REV1) if do_rev1 else None
    for path in args:
        body = Path(path).read_text()
        tag = Path(path).stem
        out = Path("build/T-25604/s2") / tag
        try:
            text = unit_text(c, body)
            obj = order_search.compile_variant(c, text, out / "best", True)
            got = funcdiff.resolved_functions(obj, [NAME])[NAME]
            lines = funcdiff.diff_lines(want, got)
            (out / "diff.txt").write_text("\n".join(lines))
            d = dis(obj)
            (out / "best.dis").write_text(d)
            line = f"{tag}: words {len(got)} rows {len(lines)} score {funcdiff.diff_score(want, got)} | {colours(d)}"
            if do_rev1:
                obj1 = order_search.compile_variant(c, rev1.apply(text), out / "rev1", True)
                d1 = dis(obj1)
                (out / "rev1.dis").write_text(d1)
                h, pairs = compare(parse_dis(d1), R)
                line += f" | rev1 {len(parse_dis(d1))}w {h}h {sum(pairs.values())}r"
        except Exception as e:  # a body that does not compile
            line = f"{tag}: ERROR {type(e).__name__}: {str(e)[:120]}"
        print(line, flush=True)


if __name__ == "__main__":
    main()
