"""T-25608 scratch ugen shim: tools/ugen_pass.py with a per-Umov log and optional forced B/F states.

Env: UGLOG=<file> appends one line per procedure with Umovs: proc index, then state/last-op per Umov.
     UGFORCE=<proc>:<states>  e.g. 57:BFFB forces the states of that procedure's Umovs in order.
"""
import os, sys
sys.path.insert(0, "/work/tools")
import ugen_pass as up


def rewrite(insns):
    if not any(i.opc == up.OP["mov"] for i in insns):
        return insns, False
    force = os.environ.get("UGFORCE", "")
    fproc, fstates = (force.split(":") + [""])[:2] if force else (None, "")
    insertions, log = {}, []
    for pi, (start, end) in enumerate(up._procedures(insns)):
        if not any(insns[k].opc == up.OP["mov"] for k in range(start, end)):
            continue
        proc_block = insns[start].words[1]
        seen_uist = uist_rill = False
        last = "ent"
        states, n, nj, nu = [], 0, 0, 0
        for k in range(start + 1, end):
            ins = insns[k]
            opc = ins.opc
            if opc in (up.OP["istr"], up.OP["istv"]):
                seen_uist = True
                nu += 1
                last = "uist"
            elif opc == up.OP["str"] and ins.mtype != up.MT_R:
                last = "ustr"
            elif opc in (up.OP["fjp"], up.OP["tjp"]):
                if seen_uist and up._is_signed_blt_pos_imm_jump(insns, start, k):
                    if not uist_rill:
                        states.append("RILL@%d(jumps=%d,uist=%d)" % (k - start, nj, nu))
                        if os.environ.get("UGDUMP"):
                            with open(os.environ["UGDUMP"], "a") as f:
                                for q in range(start, k + 1):
                                    o = insns[q]
                                    f.write("%d %s dt=%d %s\n" % (q - start, up.OPS[o.opc], o.dtype, " ".join("%x" % w for w in o.words[1:5])))
                    uist_rill = True
                nj += 1
                last = "jump"
            elif opc == up.OP["mov"]:
                want_f = uist_rill or last == "uist"
                if fproc is not None and str(pi) == fproc and n < len(fstates):
                    want_f = fstates[n] == "F"
                n += 1
                states.append("j%d/u%d:" % (nj, nu) + ("F" if want_f else "B") + "/" + last)
                idx = up._mov_insert_index(insns, start, k)
                insertions.setdefault(idx, []).extend(up._gadget_f() if want_f else up._gadget_b(proc_block))
        log.append("%d %s" % (pi, " ".join(states)))
    if os.environ.get("UGLOG"):
        with open(os.environ["UGLOG"], "a") as f:
            f.write("\n".join(log) + "\n")
    out = []
    for k, ins in enumerate(insns):
        out.extend(insertions.get(k, []))
        out.append(ins)
    return out, True


up.rewrite_ucode = rewrite
if __name__ == "__main__":
    up.main(sys.argv)
