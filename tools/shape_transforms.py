"""Source-to-source transforms for packet variant search (T-25014).

Each transform takes the text of one C function (the draft body,
including its signature line and closing brace) and yields
(name, new_text) variants, one change per variant. The transforms are
distilled from the project's matching history (wiki/matching-notes.md):
lifetime shaping for allocator artifacts, comparison respellings and
type views from the cheat sheet, and CFG shapes aimed at the as1
weight/window behaviour identified in T-25013 lever 4.

Text handling is line based and conservative: a transform that cannot
identify its site cleanly yields nothing.
"""
import re

TYPES = ("s8", "u8", "s16", "u16", "s32", "u32")


def _lines(text):
    return text.split("\n")


def _decl_block(lines):
    """Indices of the leading declaration lines inside the function body."""
    out = []
    started = False
    for i, ln in enumerate(lines):
        s = ln.strip()
        if s.endswith("{") and not started and i < 3:
            started = True
            continue
        if not started:
            continue
        if re.match(r"^(s8|u8|s16|u16|s32|u32|s64|u64|bool|MATRIX|VECTOR|[\w]+)\s+[\w\[\]]+(\s*=[^;]*)?;$", s) and "=" not in s:
            out.append(i)
        elif s == "":
            continue
        else:
            break
    return out


def decl_swaps(text):
    lines = _lines(text)
    decls = _decl_block(lines)
    for a, b in zip(decls, decls[1:]):
        if b != a + 1:
            continue
        new = list(lines)
        new[a], new[b] = new[b], new[a]
        name = "decl_swap:%s<->%s" % (lines[a].strip().rstrip(";"), lines[b].strip().rstrip(";"))
        yield name, "\n".join(new)


def _body_vars(lines, decls):
    vars_ = []
    for i in decls:
        m = re.search(r"\b([A-Za-z_]\w*)\s*(\[|;)", lines[i])
        if m:
            vars_.append(m.group(1))
    return vars_


def empty_if_probes(text):
    """Insert `if (v) {}` for each declared variable: at body start, and
    right before the first statement using it (the priority probe)."""
    lines = _lines(text)
    decls = _decl_block(lines)
    if not decls:
        return
    vars_ = _body_vars(lines, decls)
    indent = "    "
    first_stmt = decls[-1] + 1
    for v in vars_:
        for pos, tag in ((first_stmt, "start"),):
            new = list(lines)
            new.insert(pos, "%sif (%s) {}" % (indent, v))
            yield "empty_if:%s@%s" % (v, tag), "\n".join(new)
        for i in range(first_stmt, len(lines)):
            if re.search(r"\b%s\b" % re.escape(v), lines[i]) and "if (%s) {}" % v not in lines[i]:
                new = list(lines)
                new.insert(i, "%sif (%s) {}" % (indent, v))
                yield "empty_if:%s@use%d" % (v, i), "\n".join(new)
                break


_CMP = re.compile(r"([^;{}()]+?)\s*(==|!=)\s*([^;{}()]+?)(?=[;){]|&&|\|\|)")


def cmp_variants(text):
    """Per comparison site: flip operands, and the `* 1` wrapper forms
    that fix bne operand order in the project's cheat sheet."""
    out = []
    for m in _CMP.finditer(text):
        a, op, b = m.group(1).strip(), m.group(2), m.group(3).strip()
        if not a or not b or a.startswith("=") or "return" in a:
            continue
        start, end = m.span()
        flipped = "%s %s %s" % (b, op, a)
        out.append(("cmp_flip@%d" % start, text[:start] + flipped + text[end:]))
        if op == "==":
            out.append(("cmp_mul1_lhs@%d" % start, text[:start] + "%s * 1 %s %s" % (a, op, b) + text[end:]))
            out.append(("cmp_mul1_rhs@%d" % start, text[:start] + "%s %s %s * 1" % (a, op, b) + text[end:]))
    return out


def type_views(text):
    """Change one scalar local's declared type to each other width/sign."""
    lines = _lines(text)
    decls = _decl_block(lines)
    for i in decls:
        m = re.match(r"^(\s*)(s8|u8|s16|u16|s32|u32)(\s+)([A-Za-z_]\w*);", lines[i])
        if not m:
            continue
        for t in TYPES:
            if t == m.group(2):
                continue
            new = list(lines)
            new[i] = "%s%s%s%s;" % (m.group(1), t, m.group(3), m.group(4))
            yield "type_view:%s->%s" % (m.group(4), t), "\n".join(new)


def copy_intro(text):
    """Introduce a local copy of a global read in one statement:
    `tmpN = GLOB;` before the statement, that one read replaced by tmpN.
    Models the `cur = best` lifetime splits in matching-notes."""
    lines = _lines(text)
    decls = _decl_block(lines)
    if not decls:
        return
    insert_at = decls[-1] + 1
    n = 0
    for i in range(insert_at, len(lines)):
        for m in re.finditer(r"\b(D_[0-9A-F]{8})\b", lines[i]):
            sym = m.group(1)
            tmp = "shapeTmp%d" % n
            n += 1
            new = list(lines)
            new[i] = new[i][:m.start()] + tmp + new[i][m.end():]
            new.insert(insert_at, "    s32 %s;" % tmp)
            new.insert(i + 1 if False else insert_at + 1, "    %s = %s;" % (tmp, sym))
            yield "copy_intro:%s@line%d" % (sym, i), "\n".join(new)
            break


def goto_forms(text):
    """CFG shape: `if (cond) { ... }` whose body contains a switch and
    ends the function is rewritten to a goto-skip form, giving the
    block before the switch a successor outside the switch region
    (the as1 +1 weight condition, T-25013 lever 4)."""
    lines = _lines(text)
    for i, ln in enumerate(lines):
        m = re.match(r"^(\s*)if \((.+)\) \{\s*$", ln)
        if not m:
            continue
        indent, cond = m.group(1), m.group(2)
        depth = 1
        j = i + 1
        has_switch = False
        while j < len(lines) and depth:
            if "switch" in lines[j]:
                has_switch = True
            depth += lines[j].count("{") - lines[j].count("}")
            j += 1
        if not has_switch or depth != 0:
            continue
        close = j - 1
        new = list(lines)
        new[i] = "%sif (!(%s)) goto shape_out;" % (indent, cond)
        del new[close]
        new.insert(close, "%sshape_out: ;" % indent)
        yield "goto_form@if%d" % i, "\n".join(new)


_FIELD = re.compile(r"\b(D_[0-9A-F]{8}(?:\.\w+)+(?:\[\d+\])?)")


def field_copy_intro(text):
    """Introduce a local copy of one global field read:
    `s32 shapeTmpN = SYM.field;` before the enclosing statement, that
    one read replaced by the local. The promoted-global lifetime split."""
    lines = _lines(text)
    # declarations are hoisted just after the opening brace so the
    # assignment statement is C89-legal at any site
    open_idx = next(i for i, ln in enumerate(lines) if ln.rstrip().endswith("{"))
    n = 0
    for i, ln in enumerate(lines):
        for m in _FIELD.finditer(ln):
            expr = m.group(1)
            tmp = "shapeTmp%d" % n
            n += 1
            new = list(lines)
            new[i] = ln[:m.start()] + tmp + ln[m.end():]
            indent = re.match(r"^\s*", ln).group(0)
            new.insert(i, "%s%s = %s;" % (indent, tmp, expr))
            new.insert(open_idx + 1, "    s32 %s;" % tmp)
            yield "field_copy:%s@line%d" % (expr, i), "\n".join(new)


def switch_local(text):
    """Switch on a fresh local holding the scrutinee expression:
    `s32 shapeSw = EXPR; switch (shapeSw)`, and a second variant that
    also rewrites later reads of EXPR to the local."""
    m = re.search(r"switch \((\w[\w.\[\]]*)\)", text)
    if not m:
        return
    expr = m.group(1)
    if expr.startswith("shapeSw"):
        return
    line_start = text.rfind("\n", 0, m.start()) + 1
    indent = re.match(r"\s*", text[line_start:]).group(0)
    decl = "%ss32 shapeSw = %s;\n" % (indent, expr)
    base = text[:line_start] + decl + text[line_start:m.start()] + "switch (shapeSw)" + text[m.end():]
    yield "switch_local:scrutinee", base
    yield "switch_local:all", base.replace(expr, "shapeSw").replace(
        "s32 shapeSw = shapeSw;", "s32 shapeSw = %s;" % expr)


ALL = (decl_swaps, empty_if_probes, cmp_variants, type_views, copy_intro,
       field_copy_intro, switch_local, goto_forms)


def variants(text):
    seen = set()
    for fn in ALL:
        for name, new_text in fn(text):
            if new_text == text:
                continue
            key = hash(new_text)
            if key in seen:
                continue
            seen.add(key)
            yield name, new_text
