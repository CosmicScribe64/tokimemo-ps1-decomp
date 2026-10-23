#!/usr/bin/env python3
"""live_patch.py TRACE_C OUT_C: scratch diagnostic (T-25403), not a build step.

Input: the traced uopt.c that `tools/uopt_patch.py trace` writes (recomp 9c242ad). Output: the same
file with two more print points:

  R9LIVE / R9REACH  after every R9SAVE record (compute_save): the node numbers in the live range's
                    livebbs (+0x14) and reachingbbs (+0x0c) bit vectors, in graph-list order;
  R9ADJ             in addadjacents, for every successor tested while split() grows a new range:
                    bit, lr, node, shared, regsleft before/after, numintf and doheurab.

Build in the arm64 image from the recomp directory (objects from the normal recomp build):
  gcc -c -std=c11 -O1 -fno-strict-aliasing -I. -o live.o OUT_C
  gcc -o <scratch>/r9-uopt/variants/live live.o build/5.3/libc_impl.o build/5.3/version_info.o -lm
Use: TOKIMEMO_SCRATCH=<scratch> tools/docker.sh python3 tools/regdiff.py GEKO:func_80142C9C \
       --uopt /scratch/r9-uopt/variants/live    (the records land in build/scratch/regdiff/*/draft.log)
"""
import sys

text = open(sys.argv[1]).read()

save_anchor = """                MEM_U8(unit + 18), MEM_U8(unit + 21), MEM_U8(unit + 22), MEM_U8(unit + 23));
    }
}
"""
assert text.count(save_anchor) == 1, "R9SAVE print of uopt_patch.py trace not found"
text = text.replace(save_anchor, save_anchor + """{
    for (int which = 0; which < 2; which++) {
        uint32_t bv = s1 + (which ? 0x0c : 0x14);
        fprintf(stderr, which ? "R9REACH " : "R9LIVE ");
        if (MEM_U32(bv + 4) != 0)
            for (uint32_t n_ = MEM_U32(0x1001c8f8); n_; n_ = MEM_U32(n_ + 12))
                if (f_bvectin(mem, sp, MEM_U16(n_ + 8), bv)) fprintf(stderr, "%u,", MEM_U16(n_ + 8));
        fprintf(stderr, "\\n");
    }
}
""")

head = "static void f_addadjacents(uint8_t *mem, uint32_t sp, uint32_t a0, uint32_t a1, uint32_t a2, uint32_t a3) {"
start = text.index(head, text.index(head) + 1) if text.count(head) > 1 else text.index(head)
end = text.index("\n}\n", start)
body = text[start:end]
adj_anchor = "L46e2e0:\n"   # after updateforbidden/updatecolorsleft, before the doheurab test
assert body.count(adj_anchor) == 1
body = body.replace(adj_anchor, adj_anchor + """fprintf(stderr, "R9ADJ bit=%u lr=%u to=%u shared=%d oldleft=%u left=%u numintf=%u heur=%u\\n",
        MEM_U16(MEM_U32(s2) + 2), MEM_U32(s2 + 4), MEM_U16(MEM_U32(s1 + 0) + 8),
        (int)s6, s4, MEM_U8(s2 + 33), MEM_U32(s2 + 36), MEM_U8(0x1001eb10));
""")
text = text[:start] + body + text[end:]
open(sys.argv[2], "w").write(text)
