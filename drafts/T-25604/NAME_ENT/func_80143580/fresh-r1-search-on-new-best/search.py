"""T-25604 search-lens scratch driver: single-edit search on a body of NAME_ENT func_80143580.

    tools/docker.sh python3 drafts/T-25604/NAME_ENT/func_80143580/fresh-r1-search-on-new-best/search.py BASE.c OUT \
        [--kinds a,b] [--custom emptyall,split] [--names index,address,i,step] [--limit N] [--rev1-below N]

Edits: tools/autotweak.py's variants() of the given kinds, plus custom kinds:
  emptyall  `if (NAME) {}` before every statement (autotweak caps it at 3 per name)
  split     `goto sN; sN: ;` after every statement (empty-label block split, T-25314)
Every variant is compiled in the whole unit (score2.unit_text, the funcdiff pipeline) and scored by
funcdiff --resolve rows; variants at or below --rev1-below rows (default: base rows - 1) are also
compiled with the rev1 delta and aligned with build/T-25604/ref/rev1.s. Results are appended to
build/T-25604/search/OUT/results.tsv (rows, score, rev1 hunks, rev1 regs, colours, label) and the
bodies of the rev1-scored variants are kept there. Scratch, not a project tool.
"""
from pathlib import Path
import re, sys
HERE = Path(__file__).parent
sys.path.insert(0, str(HERE))
sys.path.insert(0, "tools")
import score2, autotweak, funcloc, funcdiff, order_search, rev1

HEAD = "void func_80143580(void) {\n"


def opt(name, default=None):
    for a in sys.argv:
        if a.startswith(name + "="):
            return a.split("=", 1)[1]
    return default


def custom(func_text, kinds, names):
    f = autotweak.Func(func_text)
    out = []
    starts = [i for i in range(f.stmt_line, f.end_line) if f.statement_start(i)]
    if "emptyall" in kinds:
        for name in names:
            for i in starts:
                lines = list(f.lines)
                lines.insert(i, "%sif (%s) {}" % (f.indent(i), name))
                out.append(autotweak.Variant("\n".join(lines), "emptyall %s L%d" % (name, i), True))
    if "split" in kinds:
        for n, i in enumerate(starts):
            lines = list(f.lines)
            lines.insert(i, "%sgoto s%d; s%d: ;" % (f.indent(i), n, n))
            out.append(autotweak.Variant("\n".join(lines), "split L%d" % i, True))
    return out


def main():
    base_path, out_name = sys.argv[1], sys.argv[2]
    body = Path(base_path).read_text()
    k = body.index(HEAD)
    prefix, func_text = body[:k], body[k:]
    kinds = set(filter(None, (opt("--kinds") or "").split(",")))
    ckinds = set(filter(None, (opt("--custom") or "").split(",")))
    names = (opt("--names") or "index,address,i,step").split(",")
    vs = []
    if kinds:
        vs += autotweak.variants(func_text, kinds=kinds)
    vs += custom(func_text, ckinds, names)
    lim = opt("--limit")
    if lim: vs = vs[:int(lim)]
    c = funcloc.locate(score2.NAME, "NAME_ENT")
    expected, problem = funcdiff.expected_object(c)
    assert not problem, problem
    want = funcdiff.resolved_functions(expected, [score2.NAME], c)[score2.NAME]
    R = score2.parse_splat(score2.REV1)
    out = Path("build/T-25604/search") / out_name
    out.mkdir(parents=True, exist_ok=True)
    work = out / "work"

    def best_rows(text):
        obj = order_search.compile_variant(c, text, work, True)
        got = funcdiff.resolved_functions(obj, [score2.NAME])[score2.NAME]
        return len(got), len(funcdiff.diff_lines(want, got)), funcdiff.diff_score(want, got), score2.dis(obj)

    base_text = score2.unit_text(c, body)
    w0, r0, s0, d0 = best_rows(base_text)
    below = int(opt("--rev1-below", r0 - 1))
    res = open(out / "results.tsv", "a")
    res.write("# base %s rows %d score %d words %d variants %d\n" % (base_path, r0, s0, w0, len(vs)))
    res.flush()
    print("base rows", r0, "variants", len(vs), flush=True)
    for n, v in enumerate(vs):
        b = prefix + v.text
        try:
            text = score2.unit_text(c, b)
            w, r, s, d = best_rows(text)
        except Exception as e:
            res.write("ERR\t\t\t\t\t%s\t%s\n" % (v.label, str(e)[:80])); res.flush(); continue
        h = rr = ""
        if r <= below:
            try:
                obj1 = order_search.compile_variant(c, rev1.apply(text), work / "r1", True)
                d1 = score2.parse_dis(score2.dis(obj1))
                hh, pairs = score2.compare(d1, R)
                h, rr = str(hh), str(sum(pairs.values()))
            except Exception as e:
                h = "E"
            safe = re.sub(r"[^\w]+", "_", v.label)
            (out / ("v%04d_%s.c" % (n, safe))).write_text(b)
        res.write("%d\t%d\t%d\t%s\t%s\t%s\t%s\n" % (r, s, w, h, rr, score2.colours(d), v.label))
        res.flush()
        if r < r0:
            print("GAIN", r, s, h, rr, v.label, flush=True)


if __name__ == "__main__":
    main()
