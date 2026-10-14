#!/usr/bin/env python3
"""Search statement orders and operand orders of a marked C draft (T-11530).

IDO's scheduling and ugen temp numbering depend on the source order of independent statements (packet
stores, RECT fields, increments) and on the operand order of sums. Batches T-11000..T-11520 found many
matches by trying those orders by hand; decomp-permuter samples them poorly. This tool enumerates them.

Mark the draft in its C file (in the plain definition or in the NON_MATCHING draft):

    /* ORDER */
    q->x = 0;
    q->y = 0; q->h = 4;
    q->w = 8;
    /* END ORDER */

permutes the lines between the markers (each non-blank line is one unit, so statements written on one
line move together; that grouping matters to IDO too), and

    v = ORDER_SWAP(*(s16 *)s, +, func_80141234() / 64);

tries both `A + B` and `B + A`. Several blocks and swaps combine. Every combination is compiled with the
same full-translation-unit path as funcdiff's guarded compile (gcc -E -DNON_MATCHING, tools/cc.py IDO 5.3,
trailing padding, object layout) and scored by the number of funcdiff diff rows against the original, with
relocations resolved (funcdiff --resolve; without it a matching variant still scores above 0 wherever a
symbol+offset is named differently). NON_MATCHING is defined only when the function is a guarded draft, as
in funcdiff, so the other guarded drafts of the file keep the normal build's rodata layout.

Usage (in Docker, repo root):
    python3 tools/order_search.py func_80147150 [--unit TT] [-j 8] [--max-exhaustive 2000]
                                  [--budget 3000] [--no-resolve] [--top 5] [--dry-run]

Exhaustive when the number of combinations is at most --max-exhaustive, otherwise a hill-climb from the
written order (moves: put one unit at another position, flip one swap) until no neighbour improves or
--budget compiles are spent.

The C file is never left with markers: at the end (also on an error or Ctrl-C) it gets the best
variant found, written as plain C, or with --dry-run the written order without markers. The written
order is one of the variants, so the result is never worse than the input. A score of 0 is a
candidate: confirm it with `ninja` (TT.bin: OK) and `funcdiff.py`, as for permuter outputs.
"""
import argparse
import hashlib
import itertools
import os
import random
import re
import sys
import threading
import time
from pathlib import Path

BEGIN_RE = re.compile(r"^\s*/\*\s*ORDER\s*\*/\s*$")
END_RE = re.compile(r"^\s*/\*\s*END ORDER\s*\*/\s*$")
SWAP_TOKEN = "ORDER_SWAP("
PLACEHOLDER = "\x00SWAP%d\x00"


class MarkerError(ValueError):
    pass


def split_args(text):
    """Split `text` at commas outside parentheses/brackets/braces."""
    parts, depth, cur = [], 0, []
    for ch in text:
        if ch in "([{":
            depth += 1
        elif ch in ")]}":
            depth -= 1
        if ch == "," and depth == 0:
            parts.append("".join(cur))
            cur = []
        else:
            cur.append(ch)
    parts.append("".join(cur))
    return [p.strip() for p in parts]


def extract_swaps(text):
    """Replace every ORDER_SWAP(A, op, B) by a placeholder, innermost first, so A and B may hold other swaps
    (as placeholders). Returns (text, [(A, op, B)]); an outer swap has a higher index than the swaps it holds."""
    swaps = []
    while True:
        found = None
        start = text.find(SWAP_TOKEN)
        while start >= 0:
            k, depth = start + len(SWAP_TOKEN), 1
            while k < len(text) and depth:
                if text[k] == "(":
                    depth += 1
                elif text[k] == ")":
                    depth -= 1
                k += 1
            if depth:
                raise MarkerError("unbalanced ORDER_SWAP( at offset %d" % start)
            inner = text[start + len(SWAP_TOKEN):k - 1]
            if SWAP_TOKEN not in inner:
                found = (start, k, inner)
                break
            start = text.find(SWAP_TOKEN, start + len(SWAP_TOKEN))
        if found is None:
            return text, swaps
        start, k, inner = found
        args = split_args(inner)
        if len(args) != 3 or not all(args):
            raise MarkerError("ORDER_SWAP takes (A, op, B), got: ORDER_SWAP(%s)" % inner)
        text = text[:start] + PLACEHOLDER % len(swaps) + text[k:]
        swaps.append(tuple(args))


class Template:
    """A source text with ORDER blocks and ORDER_SWAP sites.

    segments: list of str (verbatim text) and int (index into blocks); blocks: list of unit-line lists;
    swaps: list of (A, op, B).
    """

    def __init__(self, text):
        body, self.swaps = extract_swaps(text)
        self.segments, self.blocks = [], []
        cur, block = [], None
        for line in body.splitlines(keepends=True):
            if BEGIN_RE.match(line):
                if block is not None:
                    raise MarkerError("nested /* ORDER */ block")
                self.segments.append("".join(cur))
                cur, block = [], []
            elif END_RE.match(line):
                if block is None:
                    raise MarkerError("/* END ORDER */ without /* ORDER */")
                if len(block) < 2:
                    raise MarkerError("an ORDER block needs at least two non-blank lines")
                self.segments.append(len(self.blocks))
                self.blocks.append(block)
                block = None
            elif block is not None:
                if line.strip():
                    block.append(line if line.endswith("\n") else line + "\n")
            else:
                cur.append(line)
        if block is not None:
            raise MarkerError("/* ORDER */ without /* END ORDER */")
        self.segments.append("".join(cur))
        if not self.blocks and not self.swaps:
            raise MarkerError("no /* ORDER */ block and no ORDER_SWAP( in the file")

    def identity(self):
        return (tuple(tuple(range(len(b))) for b in self.blocks), tuple(0 for _ in self.swaps))

    def size(self):
        n = 1
        for b in self.blocks:
            n *= _factorial(len(b))
        return n * (2 ** len(self.swaps))

    def all_states(self):
        perms = [list(itertools.permutations(range(len(b)))) for b in self.blocks]
        for p in itertools.product(*perms):
            for s in itertools.product((0, 1), repeat=len(self.swaps)):
                yield (tuple(p), tuple(s))

    def neighbours(self, state):
        perms, bits = state
        for bi, perm in enumerate(perms):
            n = len(perm)
            for i in range(n):
                for j in range(n):
                    if i == j:
                        continue
                    p = list(perm)
                    p.insert(j, p.pop(i))
                    if tuple(p) != perm:
                        yield (perms[:bi] + (tuple(p),) + perms[bi + 1:], bits)
        for si in range(len(bits)):
            yield (perms, bits[:si] + (1 - bits[si],) + bits[si + 1:])

    def render(self, state):
        perms, bits = state
        out = []
        for seg in self.segments:
            if isinstance(seg, int):
                out.extend(self.blocks[seg][k] for k in perms[seg])
            else:
                out.append(seg)
        text = "".join(out)
        # Outer swaps (higher index) first: their arguments hold the placeholders of the inner ones.
        for i in reversed(range(len(self.swaps))):
            a, op, b = self.swaps[i]
            text = text.replace(PLACEHOLDER % i, ("%s %s %s" % (b, op, a)) if bits[i] else ("%s %s %s" % (a, op, b)))
        return text

    def describe(self, state):
        """Short description of a state relative to the written order."""
        perms, bits = state
        parts = []
        for bi, perm in enumerate(perms):
            if perm != tuple(range(len(perm))):
                parts.append("block %d order %s" % (bi + 1, ",".join(str(k + 1) for k in perm)))
        for si, bit in enumerate(bits):
            if bit:
                parts.append("swap %d flipped" % (si + 1))
        return "; ".join(parts) or "as written"


def _factorial(n):
    r = 1
    for k in range(2, n + 1):
        r *= k
    return r


def search(template, score, jobs=1, max_exhaustive=2000, budget=3000, progress=print, seed=0):
    """Evaluate states with `score(state) -> int` (lower is better). Returns [(score, state)] sorted, best first.

    Exhaustive when template.size() <= max_exhaustive, else hill-climb from the identity. `score` is called
    at most once per distinct state.
    """
    seen = {}

    def evaluate(states):
        todo = [s for s in dict.fromkeys(states) if s not in seen]
        if not todo:
            return
        if jobs > 1:
            # Plain threads: concurrent.futures imports `queue`, which tools/queue.py shadows on this path.
            results, lock, pending = {}, threading.Lock(), list(todo)

            def worker():
                while True:
                    with lock:
                        if not pending:
                            return
                        s = pending.pop()
                    v = score(s)
                    with lock:
                        results[s] = v
            threads = [threading.Thread(target=worker) for _ in range(min(jobs, len(todo)))]
            for th in threads:
                th.start()
            for th in threads:
                th.join()
            seen.update(results)
        else:
            for s in todo:
                seen[s] = score(s)

    total = template.size()
    if total <= max_exhaustive:
        progress("order_search: exhaustive, %d variant(s)" % total)
        states = list(template.all_states())
        for i in range(0, len(states), max(jobs, 1) * 4):
            evaluate(states[i:i + max(jobs, 1) * 4])
            best = min(seen.values())
            progress("order_search: %d/%d compiled, best %d" % (len(seen), total, best))
            if best == 0:
                break
    else:
        progress("order_search: %d variants, hill-climb with a budget of %d compiles" % (total, budget))
        rng = random.Random(seed)
        current = template.identity()
        evaluate([current])
        while len(seen) < budget:
            neigh = [s for s in template.neighbours(current) if s not in seen]
            rng.shuffle(neigh)
            neigh = neigh[:max(budget - len(seen), 0)]
            if not neigh:
                break
            evaluate(neigh)
            best = min(neigh, key=lambda s: seen[s])
            progress("order_search: %d compiled, current %d, best neighbour %d" % (len(seen), seen[current], seen[best]))
            if seen[best] >= seen[current]:
                break
            current = best
            if seen[current] == 0:
                break
    # On equal scores the written order comes first, so a tie never rewrites the source.
    ident = template.identity()
    return sorted(((v, s) for s, v in seen.items()), key=lambda t: (t[0], t[1] != ident))


# ---- compile and score (needs the Docker toolchain) ----

def compile_variant(c, text, work, non_matching):
    """Compile `text` as the whole translation unit of `c` the way funcdiff does: with NON_MATCHING when the
    function is a guarded draft (funcdiff_verify.compile_guarded), without it otherwise, so the other guarded
    drafts of the file stay asm and the rodata layout is the normal build's. Returns the object path."""
    import cc
    import funcdiff_verify as proof
    import object_layout
    import trailing_pad
    work.mkdir(parents=True, exist_ok=True)
    variant = work / "variant.c"
    variant.write_text(text)
    pre = proof.command(["gcc", "-E", "-P", "-Iinclude", "-I" + os.path.dirname(c.src), "-D__sgi"] +
                        (["-DNON_MATCHING"] if non_matching else []) +
                        ["-D__attribute__(x)=", "-DINCLUDE_ASM(F,N)=INCLUDE_ASM(F,N)",
                         "-DINCLUDE_RODATA(F,N)=INCLUDE_RODATA(F,N)", str(variant)])
    source = work / "guarded.c"
    source.write_text(pre)
    obj = work / "guarded.o"
    proof.command(["python3", "tools/cc.py", str(source), str(obj), "ido", "5.3"])
    includes = {name for _, name in cc.INCLUDE_ASM_RE.findall(pre)}
    trailing_pad.pad_functions(str(obj), str(c.src), includes)
    obj.write_bytes(object_layout.trim(obj.read_bytes(), object_layout.source_extent(c.src)))
    return str(obj)


def make_scorer(c, name, template, work_root, resolve=False):
    import funcdiff
    import funcdiff_verify as proof
    non_matching = proof.guarded(c.src, name)
    expected, problem = funcdiff.expected_object(c)
    if problem:
        raise SystemExit("order_search.py: %s" % problem)
    want = (funcdiff.resolved_functions(expected, [name], c) if resolve else funcdiff.functions(expected, [name]))[name]
    if want is None:
        raise SystemExit("order_search.py: %s is missing from the original object" % name)
    get = funcdiff.resolved_functions if resolve else funcdiff.functions

    def score(state):
        text = template.render(state)
        # One directory per state: two states can render the same text (identical lines), and they compile
        # in parallel.
        work = work_root / hashlib.sha1(repr(state).encode()).hexdigest()[:16]
        try:
            got = get(compile_variant(c, text, work, non_matching), [name])[name]
        except (OSError, ValueError, SystemExit) as err:
            print("order_search: variant failed to compile (%s): %s" % (template.describe(state), str(err).splitlines()[0]))
            return 10 ** 6
        if got is None:
            return 10 ** 6
        if funcdiff.keys(got) == funcdiff.keys(want):
            return 0
        return len(funcdiff.diff_lines(want, got))

    return score


def main(argv):
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("func")
    ap.add_argument("--unit", help="unit (main or an overlay name) or C/asm path that holds the function")
    ap.add_argument("-j", type=int, default=os.cpu_count() or 1, help="parallel compiles")
    ap.add_argument("--max-exhaustive", type=int, default=2000, help="exhaustive up to this many variants")
    ap.add_argument("--budget", type=int, default=3000, help="compile budget of the hill-climb")
    ap.add_argument("--no-resolve", dest="resolve", action="store_false",
                    help="compare symbol names instead of resolved words (faster, but relocation naming noise keeps "
                         "even a matching variant above 0)")
    ap.add_argument("--top", type=int, default=5, help="variants to list")
    ap.add_argument("--dry-run", action="store_true", help="write back the written order (markers removed), not the best")
    args = ap.parse_args(argv)
    if not os.path.isdir("config"):
        sys.exit("order_search.py: run it from the repository root")
    import funcloc
    name, scope = funcloc.split_scope(args.func, args.unit)
    c = funcloc.locate(name, scope)
    src = Path(c.src)
    template = Template(src.read_text())
    work_root = Path("build/order_search") / name
    keep = template.identity()
    try:
        score = make_scorer(c, name, template, work_root, args.resolve)
        start = time.time()
        results = search(template, score, args.j, args.max_exhaustive, args.budget)
        written = dict((s, v) for v, s in results).get(template.identity())
        print("order_search: %d variant(s) compiled in %.0f s; written order scores %s" %
              (len(results), time.time() - start, written))
        for v, s in results[:args.top]:
            print("  score %d: %s" % (v, template.describe(s)))
        if results and not args.dry_run:
            keep = results[0][1]
    finally:
        src.write_text(template.render(keep))
        print("order_search: wrote %s (%s), markers removed" % (src, template.describe(keep)))
    if results and results[0][0] == 0:
        print("order_search: score 0 is a candidate; confirm with `ninja` (unit .bin: OK) and funcdiff.py")
    return 0


if __name__ == "__main__":
    sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
    sys.exit(main(sys.argv[1:]))
