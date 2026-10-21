"""Unit tests for the T-25014 transform library (tools/shape_transforms.py).

Each transform must fire on a representative sample, keep braces
balanced, and leave the input unchanged. Run: python3 test_transforms.py
"""
import shape_transforms as transforms

SAMPLE = """void func_80062948(void) {
    s32 a;
    u8 flag;
    s16 b;

    a = D_800E6280;
    if (flag == 1) {
        a += 1;
    }
    if (D_80145088 == 0) {
        switch (a) {
        case 0:
            break;
        }
    }
}
"""


def balanced(text):
    return text.count("{") == text.count("}")


def check(fn, needle=None):
    vs = list(fn(SAMPLE))
    assert vs, fn.__name__ + " produced no variants"
    for name, text in vs:
        assert text != SAMPLE, (fn.__name__, name)
        assert balanced(text), (fn.__name__, name)
        if needle:
            assert needle in name or needle in text, (fn.__name__, name)
    return vs


def main():
    assert len(check(transforms.decl_swaps)) >= 2
    check(transforms.empty_if_probes, "if (")
    check(transforms.cmp_variants)
    flips = [n for n, _ in transforms.cmp_variants(SAMPLE)]
    assert any("cmp_flip" in n for n in flips), flips
    assert any("cmp_mul1" in n for n in flips), flips
    check(transforms.type_views, "type_view")
    check(transforms.copy_intro, "shapeTmp")
    gv = check(transforms.goto_forms, "goto shape_out")
    assert "shape_out: ;" in gv[0][1]
    allv = list(transforms.variants(SAMPLE))
    assert len(allv) >= 10, len(allv)
    print("all transform tests pass;", len(allv), "variants on the sample")
    check2()



SAMPLE2 = """void f(s32 arg0) {
    if (D_800E6280.unk_03F != 2) {
        D_800E6280.unk_F64[1] |= 0x40;
    }
    switch (D_800E6280.unk_03F) {
    case 2:
        break;
    }
}
"""

def check2():
    fc = list(transforms.field_copy_intro(SAMPLE2))
    assert fc and any("shapeTmp" in t for _, t in fc), fc
    for _, t in fc:
        assert balanced(t)
    sl = list(transforms.switch_local(SAMPLE2))
    assert len(sl) == 2, sl
    assert "switch (shapeSw)" in sl[0][1]
    print("field/switch transform tests pass")
if __name__ == "__main__":
    main()
