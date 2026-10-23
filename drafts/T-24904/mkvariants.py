"""T-24904: write structural variants of func_80136C20 from the current unit copy.

Each variant keeps the instruction sequence (copies, defaults, calls) and only
changes the control-flow spelling around the two flag tests and the switch.
"""
import re
import sys

src = open("drafts/T-24904/v0.c").read()

start = src.index("void func_80136C20(void) {")
end = src.index("\n#else", start)
body = src[start:end]

HEAD = """void func_80136C20(void) {
    u8 girl;
    GekoWords11 all;
    GekoWords2 first;
    GekoWords2 second;
    GekoWords2 third;

    all = D_801451B8;
    first = D_801451E4;
    second = D_801451EC;
    third = D_801451F4;
    if (D_800E6280.unk_03F < 6 || D_800E6280.unk_03F >= 10) {
        all.v[0] = 0x544A;
        all.v[3] = 0x544A;
        all.v[5] = 0x544A;
        first.v[0] = 0x5D41;
        second.v[1] = 0x522E;
        third.v[1] = 0x58AF;
    }
"""

SWITCH = """    switch (girl) {
    case 0:
        func_80062CD0(all.v[D_800E6280.unk_0F4.h >> 12]);
        break;
    case 1:
        func_80062CD0(first.v[D_800E6280.unk_0F4.h >> 12]);
        break;
    case 2:
        func_80062CD0(second.v[(D_800E6280.unk_0F4.h >> 12) - 2]);
        break;
    case 3:
        func_80062CD0(third.v[(D_800E6280.unk_0F4.h >> 12) - 4]);
        break;
    case 4:
        func_80062CD0(0x66EC);
        break;
    case 5:
        func_80062CD0(0x5FE4);
        break;
    case 6:
        func_80062CD0(0x625A);
        break;
    default:
        func_80062CD0(0x6962);
        break;
    }
"""

TAIL = "}\n"

variants = {}

# v0: current guard, for reference
variants["v0.c"] = body

# v1: two early returns, selector load between them
variants["v1.c"] = HEAD + """    if (D_80145088 != 0) {
        read_bustup();
        return;
    }
    girl = D_800E6280.unk_F5F;
    if (D_80145090 == 0) {
        read_bustup_uniform();
        return;
    }
""" + SWITCH + """    func_8004284C();
""" + TAIL

# v2: outer if/else, inner if/else
variants["v2.c"] = HEAD + """    if (D_80145088 == 0) {
        girl = D_800E6280.unk_F5F;
        if (D_80145090 == 0) {
            read_bustup_uniform();
        } else {
""" + SWITCH + """            func_8004284C();
        }
    } else {
        read_bustup();
    }
""" + TAIL

# v3: if / else-if / else chain
variants["v3.c"] = HEAD + """    if (D_80145088 != 0) {
        read_bustup();
    } else if (D_80145090 == 0) {
        read_bustup_uniform();
    } else {
        girl = D_800E6280.unk_F5F;
""" + SWITCH + """        func_8004284C();
    }
""" + TAIL

# v4: goto out of both tests
variants["v4.c"] = HEAD + """    if (D_80145088 != 0) goto bustup;
    girl = D_800E6280.unk_F5F;
    if (D_80145090 == 0) goto uniform;
""" + SWITCH + """    func_8004284C();
    return;
uniform:
    read_bustup_uniform();
    return;
bustup:
    read_bustup();
""" + TAIL

# v5: outer if/else, inner early return
variants["v5.c"] = HEAD + """    if (D_80145088 == 0) {
        girl = D_800E6280.unk_F5F;
        if (D_80145090 == 0) {
            read_bustup_uniform();
            return;
        }
""" + SWITCH + """        func_8004284C();
        return;
    }
    read_bustup();
""" + TAIL

# v6: outer while/do split, inner early return
variants["v6.c"] = HEAD + """    while (D_80145088 == 0) {
        girl = D_800E6280.unk_F5F;
        if (D_80145090 == 0) {
            read_bustup_uniform();
            return;
        }
""" + SWITCH + """        func_8004284C();
        return;
    }
    read_bustup();
""" + TAIL

# v7: outer switch on D_80145088
variants["v7.c"] = HEAD + """    switch (D_80145088) {
    case 0:
        girl = D_800E6280.unk_F5F;
        if (D_80145090 == 0) {
            read_bustup_uniform();
            return;
        }
""" + SWITCH + """        func_8004284C();
        break;
    default:
        read_bustup();
        break;
    }
""" + TAIL

# v8: outer switch, inner switch-shape for the second flag
variants["v8.c"] = HEAD + """    switch (D_80145088) {
    case 0:
        girl = D_800E6280.unk_F5F;
        switch (D_80145090) {
        case 0:
            read_bustup_uniform();
            return;
        default:
""" + SWITCH + """            func_8004284C();
            return;
        }
    default:
        read_bustup();
        break;
    }
""" + TAIL

# v9: plain if forms, D_80145088 tested with truthiness
variants["v9.c"] = HEAD + """    if (D_80145088) {
        read_bustup();
        return;
    }
    girl = D_800E6280.unk_F5F;
    if (!D_80145090) {
        read_bustup_uniform();
        return;
    }
""" + SWITCH + """    func_8004284C();
""" + TAIL

# v10: select the girl through the flag test blocks, tail shared via goto
variants["v10.c"] = HEAD + """    if (D_80145088) goto bustup;
    girl = D_800E6280.unk_F5F;
    if (!D_80145090) goto uniform;
""" + SWITCH + """    func_8004284C();
    goto out;
uniform:
    read_bustup_uniform();
    goto out;
bustup:
    read_bustup();
out:
    ;
""" + TAIL

for name, text in variants.items():
    open("drafts/T-24904/" + name, "w").write(src[:start] + text + src[end:])
    print(name)
