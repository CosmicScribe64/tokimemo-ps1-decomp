"""T-25608 scratch: evaluate candidate ugen_pass.py lock rules on a census trace.

Input: build/ugen_census.log, one line per procedure with Umov: "<unit> <start> <tokens...>", written by a
temporary trace in tools/ugen_pass.py during a full build. Tokens: U istr/istv, S direct str, C call, M mov,
Rf/Rb signed positive-immediate `<` jump (forward/backward), Jf/Jb other fjp/tjp, Gf/Gb ujp.
Prints, per candidate rule, the procedures whose mov states differ from the current rule (R0).

    python3 census_eval.py build/ugen_census.log [extra_trace_file]
"""
import sys


def states(tokens, rule):
    seen_u = lock = False
    last = "ent"
    j_since_u = j_since_u_b = False
    out = []
    for t in tokens:
        k = t[0]
        if k == "U":
            seen_u, last, j_since_u = True, "U", False
        elif k == "S":
            last = "S"
        elif k in "RJ":
            if k == "R":
                if rule == "R0":
                    lock |= seen_u
                elif rule == "C1":
                    lock |= last == "U"
                elif rule == "C3":
                    lock |= seen_u and last != "J"
                elif rule == "C6":
                    lock |= seen_u and not j_since_u
                elif rule == "C7":  # backward R needs only seen_u; forward R needs no jump since the last U
                    lock |= seen_u and (t[1] == "b" or not j_since_u)
                elif rule == "C8":  # R0, but a backward ordinary jump after the last U and before R disarms
                    lock |= seen_u and not j_since_u_b
            if k == "J" and seen_u:
                j_since_u = True
            last = "J"
        elif k == "M":
            out.append("F" if lock or last == "U" else "B")
        if k == "U":
            j_since_u_b = False
        elif k == "J" and t[1] == "b":
            j_since_u_b = True
        elif k == "ent":
            pass
    return "".join(out)


def main():
    rows = []
    for path in sys.argv[1:]:
        for n, line in enumerate(open(path)):
            parts = line.split()
            if len(parts) < 2:
                continue
            unit = parts[0] if parts[0] != "?" else "%s:%d:?" % (path, n + 1)
            rows.append((unit, int(parts[1]), parts[2:]))
    seen = {}
    for unit, start, toks in rows:
        seen[(unit, start)] = toks
    print("procedures with Umov:", len(seen))
    nlock = sum(1 for t in seen.values() if states(t, "R0") != states(t, "NONE_LOCK"))
    print("procedures where the R0 lock changes a state:", nlock)
    for rule in ("C1", "C3", "C6", "C7", "C8"):
        diff = [(u, s, states(t, "R0"), states(t, rule)) for (u, s), t in seen.items() if states(t, "R0") != states(t, rule)]
        print("%s: %d differ" % (rule, len(diff)))
        for d in diff[:12]:
            print("   ", *d)


if __name__ == "__main__":
    main()
