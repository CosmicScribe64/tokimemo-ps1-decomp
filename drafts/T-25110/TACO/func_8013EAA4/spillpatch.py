# spillpatch.py IN OUT: scratch diagnostic (T-25201). Adds one R9TEMP line per colored item to f_spilltemps of a
# traced uopt.c (uopt_patch.py trace output, recomp 9c242ad): bit, temporary index, displacement, the
# temporaries held by lower-bit items that share a node (spills=) and the item's region nodes.
import sys
t = open(sys.argv[1]).read()
start = t.index("static void f_spilltemps(uint8_t *mem, uint32_t sp) {\n", t.index("static void f_spilltemps(uint8_t *mem, uint32_t sp);") + 10)
end = t.index("\n}\n", start)
body = t[start:end]
anchor = "L46dbdc:\n"
assert body.count(anchor) == 1
inj = anchor + r'''{
    uint32_t tl_ = MEM_U32(sp + 148), bit_ = MEM_U32(sp + 188), ic_ = MEM_U32(sp + 160);
    fprintf(stderr, "R9TEMP bit=%u type=%u temp=%u disp=%d size=%u spills=", bit_, MEM_U8(ic_ + 0),
            MEM_U32(tl_ + 0), (int)MEM_U32(tl_ + 4), MEM_U32(tl_ + 8));
    for (uint32_t i_ = 0; i_ < MEM_U32(0x1001e680); i_++)
        if (f_bvectin(mem, sp, i_, s4)) fprintf(stderr, "%u,", i_);
    fprintf(stderr, " nodes=");
    for (uint32_t n_ = MEM_U32(0x1001c8f8); n_; n_ = MEM_U32(n_ + 12))
        if (f_bvectin(mem, sp, bit_, n_ + 0x15c)) fprintf(stderr, "%u,", MEM_U16(n_ + 8));
    fprintf(stderr, "\n");
}
'''
body = body.replace(anchor, inj)
open(sys.argv[2], "w").write(t[:start] + body + t[end:])
