"""gen.py VARIANT... : write drafts/T-24905/vARIANT.c variants of the guarded func_8013B460 body."""
import sys

body = open("drafts/T-24905/body.txt").read()
pre = open("drafts/T-24905/pre.txt").read()
post = open("drafts/T-24905/post.txt").read()

Y3_LINE = ("    p->y3 = g->unk_018[D_8011ECA0] + ((0x64 - arg0) << 6) / 100 + (arg0 * 0xF0) / 100 - 0x78;\n")
UV_ARGS = ("p->u0 = 0, p->v0 = 0, p->u1 = 0xFF, p->v1 = 0, p->u2 = 0, p->v2 = 0xF0, "
           "p->u3 = 0xFF, p->v3 = 0xF0")
UV_LINE = "    " + UV_ARGS + ";\n"
EXPR = "g->unk_018[D_8011ECA0] + ((0x64 - arg0) << 6) / 100 + (arg0 * 0xF0) / 100 - 0x78"
assert body.count(Y3_LINE) == 2 and body.count(UV_LINE) == 2


def two(body, fn):
    """apply fn to the first and second quad separately"""
    first, second = body.split(Y3_LINE, 1)[0], body.split(Y3_LINE, 1)[1]
    return fn(body)


vs = {}


def v1(body):
    # s16 temp: chain before the UV statement, store after it
    body = body.replace("    s32 pad;", "    s32 pad;\n    s16 y3;")
    body = body.replace(Y3_LINE, "    y3 = " + EXPR + ";\n")
    body = body.replace(UV_LINE, UV_LINE + "    p->y3 = y3;\n")
    return body


def v2(body):
    # y3 store first operand of the UV comma statement
    body = body.replace(Y3_LINE, "")
    body = body.replace(UV_LINE, "    p->y3 = " + EXPR + ", " + UV_ARGS + ";\n")
    return body


def v3(body):
    # y3 store last operand of the UV comma statement
    body = body.replace(Y3_LINE, "")
    body = body.replace(UV_LINE, "    " + UV_ARGS + ", p->y3 = " + EXPR + ";\n")
    return body


def v4(body):
    # y3 + UV + AddPrim on one source line
    body = body.replace(Y3_LINE, "")
    body = body.replace(UV_LINE + "    AddPrim((D_8011ECA0 << 0xA) + 0x3FC + D_800E8CA0, p);\n",
                        "    p->y3 = " + EXPR + " " + UV_ARGS + " "
                        "AddPrim((D_8011ECA0 << 0xA) + 0x3FC + D_800E8CA0, p);\n")
    return body


def v5(body):
    # y3 statement after the UV line, no temp
    body = body.replace(Y3_LINE + "    /* one comma", "    /* one comma")
    body = body.replace(UV_LINE, UV_LINE + Y3_LINE)
    return body


def v6(body):
    # temp assigned in one comma chain with the UV stores (y3 first), store after
    body = body.replace("    s32 pad;", "    s32 pad;\n    s16 y3;")
    body = body.replace(Y3_LINE, "    y3 = " + EXPR + ";\n")
    body = body.replace(UV_LINE, "    y3 = y3, " + UV_ARGS + ";\n")
    body = body.replace(UV_LINE + "    p->y3 = y3;\n", UV_LINE + "    p->y3 = y3;\n")
    return body


def v7(body):
    # y3 temp: chain before the UV statement, store between UV and AddPrim; u32 temp
    body = body.replace("    s32 pad;", "    s32 pad;\n    s32 y3;")
    body = body.replace(Y3_LINE, "    y3 = " + EXPR + ";\n")
    body = body.replace(UV_LINE, UV_LINE + "    p->y3 = y3;\n")
    return body


def v8(body):
    # temp assigned before, stored after AddPrim
    body = body.replace("    s32 pad;", "    s32 pad;\n    s16 y3;")
    body = body.replace(Y3_LINE, "    y3 = " + EXPR + ";\n")
    body = body.replace(UV_LINE + "    AddPrim((D_8011ECA0 << 0xA) + 0x3FC + D_800E8CA0, p);\n",
                        UV_LINE + "    AddPrim((D_8011ECA0 << 0xA) + 0x3FC + D_800E8CA0, p);\n"
                        "    p->y3 = y3;\n")
    return body


vs = {"1": v1, "2": v2, "3": v3, "4": v4, "5": v5, "6": v6, "7": v7, "8": v8}

for name in sys.argv[1:]:
    out = vs[name](body)
    assert out != body, name
    open("drafts/T-24905/v%s.c" % name, "w").write(pre + out + post)
    print("drafts/T-24905/v%s.c" % name)
