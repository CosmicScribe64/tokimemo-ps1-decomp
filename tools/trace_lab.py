"""Trace observatory (T-25015 tool 2).

One command renders a packet TU's compiler internals in diffable
text, replacing the one-off scratch scripts used in T-25013:

  trace_lab.py run --tree DIR --header F --body F --fn NAME --out DIR
                   [--ido-src DIR]

Outputs in DIR:
  ucode-in.txt / ucode-out.txt   the ucode entering and leaving uopt,
                                 rendered with the tree's own parser
  as1-weights.txt                as1 block weights and cross-block
                                 mover decisions, from the traced as1
  uopt-trace.txt                 the traced uopt's own output (when
                                 --ido-src points at a traced build)
  summary.txt                    counts and the weight table

  trace_lab.py diff DIR_A DIR_B  unified diff of two runs' artifacts

The tree must carry the capture hooks in its build shims
(TOKIMEMO_UCODE_DUMP in cvt_pass, TOKIMEMO_AS1CAP in frame_pass);
those are scratch-tree instruments, not pipeline changes.
"""
import argparse
import os
import subprocess
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))


def render_ucode(tree, bpath, out_path):
    sys.path.insert(0, os.path.join(tree, "tools"))
    import cvt_pass as cp
    with open(bpath, "rb") as f:
        insns = cp.parse(f.read())
    with open(out_path, "w") as f:
        for i, r in enumerate(insns):
            name = cp.OPS[r.opc] if r.opc < len(cp.OPS) else "op%d" % r.opc
            dt = cp.DTYPES[r.dtype] if r.dtype < len(cp.DTYPES) else str(r.dtype)
            extra = ""
            if name in ("lod", "str", "rlod", "rstr"):
                mt, blk, off, ln = r.location()
                extra = " mt=%d blk=%d off=0x%x len=%d" % (mt, blk, off, ln)
            elif name in ("lda", "ldc", "ilod", "istr", "ixa", "lab", "ent", "aent"):
                extra = " words=" + " ".join("%08x" % w for w in r.words[1:])
            f.write("%4d %-5s dt=%s%s\n" % (i, name, dt, extra))
    return len(insns)


def run(args):
    os.makedirs(args.out, exist_ok=True)
    tree = os.path.abspath(args.tree)
    tu_rel = "build/trace-tu.c"
    with open(os.path.join(tree, tu_rel), "w") as f:
        f.write(open(args.header).read())
        f.write(open(args.body).read())
    prefix = os.path.join(tree, "build", "trace-uc")
    capdir = os.path.join(tree, "build", "trace-as1cap")
    os.makedirs(capdir, exist_ok=True)
    env = dict(os.environ)
    env["TOKIMEMO_UCODE_DUMP"] = prefix
    env["TOKIMEMO_AS1CAP"] = capdir
    r = subprocess.run([sys.executable, "tools/cc.py", tu_rel,
                        "build/trace-cand.o", "ido", "5.3"],
                       cwd=tree, env=env, capture_output=True, text=True)
    if r.returncode != 0:
        print("compile failed")
        print(r.stderr[-2000:])
        return 2
    n_in = render_ucode(tree, prefix + ".in.B", os.path.join(args.out, "ucode-in.txt"))
    n_out = render_ucode(tree, prefix + ".out.B", os.path.join(args.out, "ucode-out.txt"))

    weights = []
    if args.ido_src:
        as1 = os.path.join(args.ido_src, "variants", "as1-trace")
        trace = subprocess.run(
            ["stdbuf", "-o0", as1, "-elf", "-G", "0", "-p0", "-EL", "-g0",
             "-O2", os.path.join(capdir, "in.G"), "-o",
             os.path.join(args.out, "as1.o"), "-t",
             os.path.join(capdir, "in.T"), "-R", "-xbbdbg", "8"],
            capture_output=True, text=True)
        with open(os.path.join(args.out, "as1-trace.txt"), "w") as f:
            f.write(trace.stdout)
        for line in trace.stdout.split("\n"):
            s = line.strip()
            if s.startswith("bb:") or s.startswith("XBB"):
                weights.append(s)
        uopt = os.path.join(args.ido_src, "variants", "trace")
        if os.path.exists(uopt):
            u = subprocess.run([uopt, prefix + ".in.B", "-o",
                                os.path.join(args.out, "uopt-out.B")],
                               capture_output=True, text=True)
            with open(os.path.join(args.out, "uopt-trace.txt"), "w") as f:
                f.write(u.stdout)
    with open(os.path.join(args.out, "as1-weights.txt"), "w") as f:
        f.write("\n".join(weights) + "\n")
    with open(os.path.join(args.out, "summary.txt"), "w") as f:
        f.write("function: %s\nucode records in: %d\nucode records out: %d\n"
                "as1 weight/mover lines: %d\n" % (args.fn, n_in, n_out, len(weights)))
    print("traced %s: %d ucode records in, %d out, %d as1 lines"
          % (args.fn, n_in, n_out, len(weights)))
    return 0


def diff(args):
    import difflib
    rc = 0
    for name in ("ucode-in.txt", "ucode-out.txt", "as1-weights.txt", "summary.txt"):
        pa, pb = os.path.join(args.a, name), os.path.join(args.b, name)
        if not (os.path.exists(pa) and os.path.exists(pb)):
            continue
        a = open(pa).read().split("\n")
        b = open(pb).read().split("\n")
        d = list(difflib.unified_diff(a, b, args.a + "/" + name, args.b + "/" + name))
        if d:
            rc = 1
            print("\n".join(d[:80]))
    return rc


def main():
    ap = argparse.ArgumentParser()
    sub = ap.add_subparsers(dest="cmd", required=True)
    r = sub.add_parser("run")
    r.add_argument("--tree", required=True)
    r.add_argument("--header", required=True)
    r.add_argument("--body", required=True)
    r.add_argument("--fn", required=True)
    r.add_argument("--out", required=True)
    r.add_argument("--ido-src")
    r.set_defaults(func=run)
    d = sub.add_parser("diff")
    d.add_argument("a")
    d.add_argument("b")
    d.set_defaults(func=diff)
    args = ap.parse_args()
    return args.func(args)


if __name__ == "__main__":
    sys.exit(main())
