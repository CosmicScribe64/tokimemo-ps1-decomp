#!/usr/bin/env python3
"""T-16510: detect and rewrite recurring m2c draft defects (post-processing for any draft body).

Agents kept fixing the same m2c output by hand (wiki/matching-notes.md; T-12701, T-15300, T-15400,
T-15600, T-16210). This module finds those defects in a function body and, where the fix is
mechanical, writes the alternative. A rewrite is a candidate, not a verdict: score the full
translation unit (`rescore`) and keep the original unless the score improves.

Detectors (`analyze`; R = has a rewrite):

  goto-shared-store  R  m2c shares one store through a temporary and a `goto block_N`:
                          case 0: var = 5; block_N: D = var; break;   case 2: var = 6; goto block_N;
                        becomes a direct store `D = 5; break;` in every case (T-12701). The tail after
                        the label must be plain memory stores ending in `break;`/`return`; each jump
                        assigns the temporaries right before it (or past up to three stores that cannot
                        change the value); a temporary that is never assigned in the switch is read where
                        it is; one call may move into the only store. Every other label is reported as
                        `goto-shared-store-blocked` with its reason (loop labels, shared calls and ifs,
                        a temporary used elsewhere, ...). A block the original really shares stays a
                        `goto` (matching-notes, TT `func_80132000`): the score decides.
  sjis-escape        R  Shift-JIS bytes printed as C escapes (`"\\x83\\\\"` is 0x83 0x5C, or octal) become the
                        text the project writes: UTF-8 in the source, which tools/cc.py converts back to
                        Shift-JIS octal escapes. IDO's K&R lexer reads `\\x83` as the letters `x83` (T-15300,
                        T-15400). The bytes must round-trip through the `shift_jis` codec, else the
                        string is reported (`sjis-escape-blocked`) and left alone.
  postinc-compare    R  `D += 1; if (D >= N)` where the asm compares the register that holds the OLD
                        value becomes `if (D++ >= N)` (T-15600); also when the asm increments in place
                        (`addiu $v1, $v1, 1` after the compare), which m2c.py's own pass missed.
  postinc-wrong      R  the opposite: a `D++` in an `if` where the asm compares the NEW value becomes
                        `D += 1;` before the condition (never seen in the T-14500 archive).
  void-callers-read-v0  report: the draft is `void`, callers read $v0 right after the call.
  s32-no-return      report: the draft is `void` and the asm leaves a dead copy or constant in $v0 at
                        an exit (T-16210; matching-notes "Implicit int"): try `s32 f(void)` with no
                        `return` value. Checked against the matched sources: 207 of 334 `s32` functions
                        without a value return show it, 32 of 5383 `void` ones do.

Usage (in Docker, repo root as cwd):
  python3 tools/draft_fixes.py check BODY.c [--scope UNIT:func | --asm FILE.s]
  python3 tools/draft_fixes.py fix BODY.c [--scope UNIT:func | --asm FILE.s]       # new body on stdout
  python3 tools/draft_fixes.py survey --archive drafts/T-14500 [--json OUT.json]    # hits per detector
  python3 tools/draft_fixes.py rescore --archive drafts/T-14500 --top 30 --out drafts/T-16510 [--jobs 2]
  python3 tools/draft_fixes.py rescore --list                                       # the hits, no compile
`tools/m2c.py --fix-defects` runs the rewrites on its output. Without an asm file the asm-driven
detectors (postinc, s32/void reports) are skipped. `rescore` takes the archive's selected bodies, tries
all rewrites of a body together (with two to six: also each alone and a greedy build-up), compiles each
full translation unit (the exhaustive_drafts scoring path, at most --jobs at once), and saves bodies that
beat the unmodified one under
--out with provenance. Limits: line-oriented (one statement per line, as m2c prints it); the callers scan
follows the fall-through path of the call only and does not see indirect calls; nothing here proves a match.
"""
import argparse
import collections
import hashlib
import json
import os
import re
import sys
import time

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import m2c_args  # noqa: E402

TICKET = "T-16510"
REWRITES = ("goto-shared-store", "sjis-escape", "postinc-compare", "postinc-wrong")
REPORTS = ("void-callers-read-v0", "s32-no-return")


class Finding(collections.namedtuple("Finding", "kind line detail fixed n", defaults=(0,))):
    """kind: a detector name or `<name>-blocked`; line: 1-based line (of the body as rewritten so far);
    fixed: the rewrite was applied; n: ordinal among the rewritable findings of the kind, the key for
    `select` in analyze()."""
    __slots__ = ()


class Result(collections.namedtuple("Result", "body findings")):
    """The (possibly rewritten) body and every Finding."""
    __slots__ = ()

    @property
    def applied(self):
        return [f for f in self.findings if f.fixed]


def mask(text):
    """Text with comments and literal contents blanked, offsets kept (autotweak.mask)."""
    import autotweak
    return autotweak.mask(text)


# ---------------------------------------------------------------------------------------------
# goto-shared-store

LOCAL_RE = re.compile(r"^(?:var|temp|phi)_\w+$")
DECL_RE = re.compile(r"^\s+((?:[A-Za-z_]\w*[ \t]+)+)[ \t\*]*(\w+)\s*;\s*$")
LABEL_RE = re.compile(r"^([A-Za-z_]\w*):\s*(?:/\*.*\*/)?\s*$")
GOTO_RE = re.compile(r"^\s*goto\s+([A-Za-z_]\w*)\s*;\s*$")
ASSIGN_RE = re.compile(r"^\s*([A-Za-z_]\w*)\s*=\s*([^=;][^;]*?)\s*;\s*$")
TERMINATOR_RE = re.compile(r"^\s*(break|return\b[^;]*)\s*;\s*$")
STATEMENT_RE = re.compile(r"^\s*[^{};]+;\s*$")
CALL_RE = re.compile(r"\b[A-Za-z_]\w*\s*\(|\+\+|--")
CONSTANT_RE = re.compile(r"^(?:-?(?:0x[0-9A-Fa-f]+|\d+)U?|func_[0-9A-Fa-f]+|&D_[0-9A-Fa-f]+)$")
MOVE_OVER = 3     # plain stores an assignment may be moved past
NARROW = {"s8": (-0x80, 0x7F), "u8": (0, 0xFF), "s16": (-0x8000, 0x7FFF), "u16": (0, 0xFFFF)}


def block_info(masked_lines):
    """(frames, closes): for each line, the blocks open at its start as a tuple of (kind, line that opened
    it), kind being `func`, `switch`, `loop` or `if` (any other brace); and {opening line: closing line}."""
    stack, out, closes = [], [], {}
    for n, line in enumerate(masked_lines):
        out.append(tuple(stack))
        for i, ch in enumerate(line):
            if ch == "{":
                head = line[:i].strip()
                if not stack:
                    kind = "func"
                elif re.match(r"(?:\}\s*)?switch\b", head):
                    kind = "switch"
                elif re.match(r"(?:\}\s*)?(?:for|while)\b", head) or head.endswith("do"):
                    kind = "loop"
                else:
                    kind = "if"
                stack.append((kind, n))
            elif ch == "}" and stack:
                closes[stack.pop()[1]] = n
    return out, closes


def breakable(frames):
    """Opening line of the innermost switch or loop, or None."""
    for kind, n in reversed(frames):
        if kind in ("switch", "loop"):
            return n
    return None


FNPTR_DECL_RE = re.compile(r"^\s+[\w \t\*]+\(\s*\*+\s*(\w+)\s*\)\s*\([^;]*\)\s*;\s*$")


def decl_name(line):
    """(name, type) when the line declares one m2c temporary, else None."""
    m = DECL_RE.match(line)
    if m and LOCAL_RE.match(m.group(2)):
        return m.group(2), m.group(1).split()[-1]
    m = FNPTR_DECL_RE.match(line)
    if m and LOCAL_RE.match(m.group(1)):
        return m.group(1), "fnptr"
    return None


def local_types(lines):
    """{name: declared type} of the m2c temporaries declared in the body."""
    return dict(d for d in map(decl_name, lines) if d)


def words(text):
    return re.findall(r"[A-Za-z_]\w*", text)


def split_store(stmt):
    """(lhs, rhs) when the statement is an assignment (also `op=`), else None (call, compare, ...)."""
    s = stmt.strip().rstrip(";")
    depth = 0
    for i, ch in enumerate(s):
        if ch in "([":
            depth += 1
        elif ch in ")]":
            depth -= 1
        elif ch == "=" and depth == 0:
            prev = s[i - 1] if i else ""
            nxt = s[i + 1] if i + 1 < len(s) else ""
            if nxt == "=" or prev == "=" or prev == "!" or (prev in "<>" and s[i - 2:i] not in ("<<", ">>")):
                continue
            lhs = re.sub(r"(?:<<|>>|[-+*/%|&^])$", "", s[:i].rstrip()).strip()
            if lhs and not re.match(r"^\w+\s*\(.*\)$", lhs):
                return lhs, s[i + 1:].strip()
            return None
    return None


def is_memory_store(stmt):
    parts = split_store(stmt)
    return parts is not None and not LOCAL_RE.match(parts[0])


def substitute(text, name, value):
    """`text` with the identifier `name` replaced by `value`: bare when `name` is the whole right-hand
    side of an assignment or `value` is atomic, else in parentheses."""
    whole = re.match(r"^(\s*[^;]*?[^=!<>]=\s*)%s(\s*;\s*)$" % re.escape(name), text)
    if whole and name not in words(whole.group(1)):
        return whole.group(1) + value + whole.group(2)
    atom = value if re.match(r"^(?:-?\w+|\([^()]*\))$", value) else "(" + value + ")"
    return re.sub(r"\b%s\b" % re.escape(name), lambda m: atom, text)


def plan_label(lines, masked, info, closes, types, li):
    """(plan, None) when the tail after the label on line `li` can be written out at every jump to it,
    else (None, reason)."""
    name = LABEL_RE.match(masked[li]).group(1)
    if name.startswith("loop_"):
        return None, "loop-label"
    sites = [i for i, l in enumerate(masked) if GOTO_RE.match(l) and GOTO_RE.match(l).group(1) == name]
    if not sites:
        return None, "unreferenced"
    tail, i = [], li + 1
    while i < len(masked) and STATEMENT_RE.match(masked[i]) and not TERMINATOR_RE.match(masked[i]):
        tail.append(i)
        i += 1
    if i >= len(masked) or not TERMINATOR_RE.match(masked[i]):
        return None, "tail-not-straight-line"
    term = i
    if not tail:
        return None, "empty-tail"
    if not all(is_memory_store(masked[t]) for t in tail):
        return None, "tail-not-store"
    if masked[term].strip().startswith("break"):
        here = breakable(info[li])
        if here is None or any(breakable(info[s]) != here for s in sites):
            return None, "break-leaves-another-construct"
    tail_text = [masked[t] for t in tail + [term]]
    used = [v for v in types if any(re.search(r"\b%s\b" % re.escape(v), t) for t in tail_text)]

    def preceding(first):
        """({var: value}, [lines of the assignments]) of the `used` temporaries assigned above line `first`:
        right above it, or past at most MOVE_OVER plain stores that cannot change the value."""
        got, drops, passed, k = {}, [], [], first - 1
        while k >= 0 and len(passed) <= MOVE_OVER and set(got) != set(used):
            m = ASSIGN_RE.match(masked[k])
            if m and m.group(1) in used and m.group(1) not in got:
                value = ASSIGN_RE.match(lines[k]).group(2)
                if not all(can_move_past(value, stmt) for stmt in passed):
                    break
                got[m.group(1)] = value
                drops.append(k)
            elif is_memory_store(masked[k]) and not CALL_RE.search(masked[k]) and \
                    not any(re.search(r"\b%s\b" % re.escape(v), masked[k]) for v in used):
                passed.append(masked[k])
            else:
                break
            k -= 1
        return got, drops

    jumps = []
    for s in sites:
        got, drops = preceding(s)
        jumps.append((s, got, drops))
    prev = li - 1
    while prev >= 0 and not masked[prev].strip():
        prev -= 1
    fall = None
    if prev >= 0 and not (TERMINATOR_RE.match(masked[prev]) or GOTO_RE.match(masked[prev])):
        got, drops = preceding(li)
        fall = (li, got, drops)
    # A temporary assigned at one jump is assigned at every jump; any other one the tail reads must keep
    # its value through the whole switch, so the tail may read it where it is.
    assigned = [v for v in used if any(v in g for _, g, _ in jumps + ([fall] if fall else []))]
    if any(set(g) != set(assigned) for _, g, _ in jumps + ([fall] if fall else [])):
        return None, "temporary-not-assigned-at-jump"
    scope = breakable(info[li])
    if scope is not None:
        end = closes.get(scope, len(masked) - 1)
        for v in used:
            if v not in assigned and any(assigns(masked[k], v) for k in range(scope + 1, end)):
                return None, "tail-reads-temporary-assigned-in-switch"
    every = jumps + ([fall] if fall else [])
    values = [v for _, g, _ in every for v in g.values()]
    if any(set(words(v)) & set(assigned) for v in values):
        return None, "value-uses-temporary"
    if any(CALL_RE.search(v) for v in values):
        # A call may only move into the first store, and only when it is the sole temporary.
        uses = sum(len(re.findall(r"\b%s\b" % re.escape(v), t)) for v in assigned for t in tail_text)
        if len(assigned) != 1 or uses != 1 or not re.search(r"\b%s\b" % re.escape(assigned[0]), masked[tail[0]]):
            return None, "value-has-side-effects"
    body = "\n".join(masked)
    for v in assigned:
        decls = sum(1 for l in lines if (decl_name(l) or ("",))[0] == v)
        in_tail = sum(len(re.findall(r"\b%s\b" % re.escape(v), t)) for t in tail_text)
        if len(re.findall(r"\b%s\b" % re.escape(v), body)) != decls + len(every) + in_tail:
            return None, "temporary-used-elsewhere"
    # A temporary read after the first store must not see a different value than before: only
    # constants move behind another store.
    for t in tail[1:] + [term]:
        if any(re.search(r"\b%s\b" % re.escape(v), masked[t]) for v in assigned) and \
                not all(CONSTANT_RE.match(v) for v in values):
            return None, "value-read-after-store"
    for v in assigned:
        bounds = NARROW.get(types[v])
        if not bounds:
            continue
        for _, g, _ in every:
            if re.match(r"^-?(?:0x[0-9A-Fa-f]+|\d+)$", g[v]):
                if not bounds[0] <= int(g[v], 0) <= bounds[1]:
                    return None, "literal-out-of-range-of-temporary"
            else:
                g[v] = "(%s) %s" % (types[v], g[v] if re.match(r"^\w+$", g[v]) else "(%s)" % g[v])
    return dict(name=name, label=li, tail=tail, term=term, jumps=jumps, fall=fall, assigned=assigned), None


def assigns(stmt, name):
    """True when the statement sets the variable `name` itself (not what it points to)."""
    parts = split_store(stmt)
    return (parts is not None and parts[0] == name) or \
        re.search(r"\b%s\s*(?:\+\+|--)|(?:\+\+|--)\s*%s\b" % (re.escape(name), re.escape(name)), stmt) is not None


def can_move_past(value, stmt):
    """True when evaluating `value` after the store `stmt` gives the same result: the value is a constant,
    or it names no object the store writes and the store has no indirection."""
    if CONSTANT_RE.match(value):
        return True
    lhs = split_store(stmt)[0]
    return not re.search(r"[*>\[]", lhs) and not set(words(lhs)) & set(words(value))


def apply_values(text, got):
    for var, value in got.items():
        text = substitute(text, var, value)
    return text


def rewrite_label(lines, plan):
    """The body lines with the stores of the label written out at each jump (and at the label when
    code falls into it); the label and the assignments to the temporaries disappear."""
    li, term = plan["label"], plan["term"]
    shared = [lines[t] for t in plan["tail"] + [term]]
    ops = []
    for s, got, drops in plan["jumps"]:
        indent = re.match(r"^\s*", lines[s]).group()
        ops += [(k, k, []) for k in drops]
        ops.append((s, s, [indent + apply_values(l.strip(), got) for l in shared]))
    if plan["fall"] is None:
        ops.append((li, term, []))
    else:
        _, got, drops = plan["fall"]
        ops += [(k, k, []) for k in drops]
        ops.append((li, li, []))
        ops.append((li + 1, term, [apply_values(l, got) for l in shared]))
    out = list(lines)
    for start, end, new in sorted(ops, key=lambda o: -o[0]):
        out[start:end + 1] = new
    return out


def goto_findings(body, rewrite, select=None):
    """(body, [Finding]) for the labels of `body`; the shared-store ones are rewritten when `rewrite`
    (and `select("goto-shared-store", n)` for the n-th of them)."""
    findings, seen = [], {}
    lines = body.split("\n")
    for _ in range(256):
        masked = mask("\n".join(lines)).split("\n")
        info, closes = block_info(masked)
        types = local_types(lines)
        for li, line in enumerate(masked):
            m = LABEL_RE.match(line)
            key = (m.group(1), "".join(lines[li:li + 3])) if m else None
            if not m or m.group(1) in ("default", "else") or key in seen:
                continue
            seen[key] = None
            plan, why = plan_label(lines, masked, info, closes, types, li)
            if plan is None:
                findings.append(Finding("goto-shared-store-blocked", li + 1, "%s: %s" % (m.group(1), why), False))
                continue
            n = sum(1 for f in findings if f.kind == "goto-shared-store")
            chosen = rewrite and (select is None or select("goto-shared-store", n))
            detail = "%s: %d jumps share %d stores" % (m.group(1), len(plan["jumps"]), len(plan["tail"]))
            findings.append(Finding("goto-shared-store", li + 1, detail, bool(chosen), n))
            if chosen:
                lines = rewrite_label(lines, plan)
                for v in plan["assigned"]:
                    lines = [l for l in lines if (decl_name(l) or ("",))[0] != v]
                break
        else:
            break
    return "\n".join(lines), findings


# ---------------------------------------------------------------------------------------------
# sjis-escape

SIMPLE_ESCAPES = {"n": 10, "t": 9, "r": 13, "a": 7, "b": 8, "f": 12, "v": 11, "\\": 92, '"': 34, "'": 39, "?": 63}
RENDER_ESCAPES = {10: "\\n", 9: "\\t", 13: "\\r", 92: "\\\\", 34: '\\"'}


def literal_bytes(inner):
    """(bytes, hit) for the inside of a C string literal: escapes give their byte (a hex escape takes at
    most two digits, as m2c prints them), other characters their Shift-JIS bytes; hit is True when a hex
    escape or an escape of a byte >= 0x80 occurs. (None, False) when the text cannot be read."""
    out, hit, i = bytearray(), False, 0
    while i < len(inner):
        ch = inner[i]
        if ch != "\\":
            try:
                out += ch.encode("shift_jis")
            except UnicodeEncodeError:
                return None, False
            i += 1
            continue
        nxt = inner[i + 1:i + 2]
        if nxt == "x":
            m = re.match(r"[0-9A-Fa-f]{1,2}", inner[i + 2:])
            if not m:
                return None, False
            value, i, hit = int(m.group(), 16), i + 2 + len(m.group()), True
        elif nxt and nxt in "01234567":
            m = re.match(r"[0-7]{1,3}", inner[i + 1:])
            value, i = int(m.group(), 8), i + 1 + len(m.group())
            if value > 255:
                return None, False
            hit = hit or value >= 0x80
        elif nxt in SIMPLE_ESCAPES:
            value, i = SIMPLE_ESCAPES[nxt], i + 2
        else:
            return None, False
        out.append(value)
    return bytes(out), hit


def render_literal(data):
    """The literal text for the bytes `data` (Shift-JIS text kept as characters), or None."""
    try:
        text = data.decode("shift_jis")
    except UnicodeDecodeError:
        return None
    out = []
    for ch in text:
        c = ord(ch)
        if c >= 0x80:
            out.append(ch)
        elif c in RENDER_ESCAPES:
            out.append(RENDER_ESCAPES[c])
        elif c < 0x20 or c == 0x7F:
            out.append("\\%03o" % c)
        else:
            out.append(ch)
    rendered = "".join(out)
    if literal_bytes(rendered)[0] != data:
        return None
    return rendered


def string_literals(text):
    """[(start, end)] of the string literals of `text` (opening quote to closing quote)."""
    masked = mask(text)
    spans, i = [], 0
    while i < len(text):
        if masked[i] == '"':
            j = masked.find('"', i + 1)
            if j < 0:
                break
            spans.append((i, j + 1))
            i = j + 1
        else:
            i += 1
    return spans


def sjis_findings(body, rewrite, select=None):
    """(body, [Finding]) for string literals that spell Shift-JIS text as escapes."""
    findings, out, last = [], [], 0
    for start, end in string_literals(body):
        inner = body[start + 1:end - 1]
        if "\\" not in inner:
            continue
        data, hit = literal_bytes(inner)
        line = body.count("\n", 0, start) + 1
        if data is None:
            if re.search(r"\\x[0-9A-Fa-f]|\\[0-7]{3}", inner):
                findings.append(Finding("sjis-escape-blocked", line, "cannot read %s" % inner[:30], False))
            continue
        if not hit:
            continue
        text = render_literal(data)
        if text is None or text == inner:
            findings.append(Finding("sjis-escape-blocked", line, "bytes do not round-trip: %s" % inner[:30], False))
            continue
        n = sum(1 for f in findings if f.kind == "sjis-escape")
        chosen = rewrite and (select is None or select("sjis-escape", n))
        findings.append(Finding("sjis-escape", line, "%s -> %s" % (inner[:24], text[:24]), bool(chosen), n))
        out.append(body[last:start + 1] + (text if chosen else inner))
        last = end - 1
    out.append(body[last:])
    return "".join(out), findings


# ---------------------------------------------------------------------------------------------
# postinc-compare, postinc-wrong

def postinc_findings(body, asm_text, rewrite, select=None):
    kinds = m2c_args.post_increment_compares(asm_text)
    findings, take = [], set()
    for n, sym in enumerate(sorted(s for s, k in kinds.items() if "old" in k)):
        chosen = rewrite and (select is None or select("postinc-compare", n))
        for m in m2c_args.postinc_pattern(sym).finditer(body):
            findings.append(Finding("postinc-compare", body.count("\n", 0, m.start()) + 1,
                                    "%s %s= 1 compared before the store" % (sym, m.group(2)), bool(chosen), n))
        if chosen:
            take.add(sym)
    if take:
        body = m2c_args.rewrite_post_increment(body, take)[0]
    masked = mask(body)
    for n, sym in enumerate(sorted(s for s, k in kinds.items() if k == {"new"})):
        chosen = rewrite and (select is None or select("postinc-wrong", n))
        pat = re.compile(r"^([ \t]*)(if[ \t]*\([^\n]*?)\b%s(\+\+|--)" % re.escape(sym), re.M)
        for m in list(pat.finditer(masked))[::-1]:
            findings.append(Finding("postinc-wrong", body.count("\n", 0, m.start()) + 1,
                                    "%s%s but the asm compares the incremented value" % (sym, m.group(3)), bool(chosen), n))
            if chosen:
                step_line = m.group(1) + sym + (" += 1;" if m.group(3) == "++" else " -= 1;") + "\n"
                body = body[:m.start()] + step_line + m.group(1) + m.group(2) + sym + body[m.end(3):]
    return body, findings


# ---------------------------------------------------------------------------------------------
# asm: v0 at exits, v0 read after calls

ASM_LABEL_RE = re.compile(r"^\s*(?:(jlabel)\s+)?(\.L\w+)\s*:?\s*$")
COND_BRANCHES = {"beq", "bne", "beqz", "bnez", "bgez", "bgtz", "blez", "bltz", "bc1t", "bc1f"}
JUMPS = {"b", "j", "jr"}
CALLS = {"jal", "jalr"}
CONTROL = COND_BRANCHES | JUMPS | CALLS


class Asm(collections.namedtuple("Asm", "insns labels jtargets starts")):
    """insns: [(mnemonic, [operands])]; labels: {name: instruction index}; jtargets: indices of jump
    table labels (`jlabel`); starts: indices that carry any label."""
    __slots__ = ()


def parse_asm(text):
    insns, labels, jtargets, pending = [], {}, set(), []
    for line in text.splitlines():
        m = m2c_args.INSN_RE.match(line)
        if m:
            ops = [o.strip() for o in re.split(r",(?![^()]*\))", m.group(2)) if o.strip()]
            for name, is_jump in pending:
                labels[name] = len(insns)
                if is_jump:
                    jtargets.add(len(insns))
            pending = []
            insns.append((m.group(1), ops))
            continue
        m = ASM_LABEL_RE.match(line)
        if m:
            pending.append((m.group(2), bool(m.group(1))))
    return Asm(insns, labels, jtargets, set(labels.values()))


def def_kind(mn, ops):
    """How an instruction that writes $v0 defines it: call, zero, copy (register move), const, load, other."""
    if mn in CALLS:
        return "call"
    if mn in m2c_args.LOADS:
        return "load"
    srcs = [o for o in ops[1:] if o != "$zero"]
    if mn in ("or", "addu", "move", "addiu", "ori", "li", "daddu"):
        if not srcs or srcs == ["0"] or srcs == ["0x0"]:
            return "zero"
        if mn in ("addiu", "ori", "li"):
            return "const" if "$zero" in ops[1:2] or mn == "li" else "other"
        if len(srcs) == 1 and srcs[0].startswith("$") and srcs[0] != "$v0":
            return "copy"
    return "other"


def step(state, idx, mn, ops):
    reads, writes = m2c_args.effects(mn, ops)
    if "v0" in reads:
        state = frozenset()
    if "v0" in writes:
        state = frozenset([(idx, def_kind(mn, ops))])
    return state


def exit_v0_defs(asm):
    """[(index, kind, in_delay_slot)] of the writes of $v0 that reach a `jr $ra` with no read of $v0 in
    between (calls excluded): the value the function leaves in $v0 without using it."""
    insns, n = asm.insns, len(asm.insns)
    if not n:
        return []
    states, work, exits = {0: frozenset()}, [0], set()

    def push(target, state):
        if target is None or target >= n:
            return
        have = states.get(target)
        if have is None or not state <= have:
            states[target] = (have or frozenset()) | state
            work.append(target)

    while work:
        i = work.pop()
        state = states[i]
        mn, ops = insns[i]
        if mn not in CONTROL:
            push(i + 1, step(state, i, mn, ops))
            continue
        state = step(state, i, mn, ops) if mn not in CALLS else state
        if i + 1 < n:
            state = step(state, i + 1, *insns[i + 1])
        if mn in CALLS:
            push(i + 2, frozenset([(i, "call")]))
        elif mn == "jr":
            if ops[:1] == ["$ra"] or not asm.jtargets:
                exits.add(state)
            else:
                for t in asm.jtargets:
                    push(t, state)
        else:
            push(asm.labels.get(ops[-1]) if ops else None, state)
            if mn in COND_BRANCHES:
                push(i + 2, state)
    found = set()
    for state in exits:
        for idx, kind in state:
            if kind != "call":
                found.add((idx, kind, idx > 0 and insns[idx - 1][0] in CONTROL))
    return sorted(found)


def reads_v0_after_call(asm, i):
    """True when the instructions after the call at `i` (and its delay slot) read $v0 before anything
    writes it: the fall-through path only, up to a label, a call or a jump."""
    insns, n, j = asm.insns, len(asm.insns), i + 2
    while j < n and j not in asm.starts:
        mn, ops = insns[j]
        reads, writes = m2c_args.effects(mn, ops)
        if "v0" in reads:
            return True
        if mn in CALLS or mn in JUMPS or mn in COND_BRANCHES:
            if j + 1 < n:
                sreads, swrites = m2c_args.effects(*insns[j + 1])
                if "v0" in sreads:
                    return True
                if "v0" in swrites:
                    return False
            if mn in CALLS or mn in JUMPS:
                return False
            j += 2
            continue
        if "v0" in writes:
            return False
        j += 1
    return False


def call_sites(asm):
    """[(callee, reads_v0)] of the `jal` instructions of one function."""
    return [(ops[0], reads_v0_after_call(asm, i)) for i, (mn, ops) in enumerate(asm.insns) if mn == "jal" and ops]


def returns_void(body):
    """True when the first line of the body declares a `void` function."""
    return re.match(r"\s*(?:static\s+)?void\b", body) is not None


def has_value_return(body):
    return re.search(r"\breturn\s+[^;\s]", mask(body)) is not None


def asm_files(root="."):
    """{(unit, function): path} of the per-function asm files (matchings and nonmatchings)."""
    found = {}
    for top, _, names in os.walk(os.path.join(root, "asm")):
        p = top.replace(os.sep, "/") + "/"
        if "/matchings/" not in p and "/nonmatchings/" not in p:
            continue
        m = re.search(r"asm/ovl/([^/]+)/", p)
        for name in names:
            if name.endswith(".s"):
                found[(m.group(1) if m else "main", name[:-2])] = os.path.join(top, name)
    return found


def read_asm(path):
    """Text of a file (asm, body or catalog); undecodable bytes are replaced."""
    with open(path, errors="replace") as fh:
        return fh.read()


def build_callers(files):
    """{(unit, callee): [(caller, reads_v0)]} over every asm function of `files` (asm_files()); a `jal`
    target is the function of the caller's own unit when that unit has it (overlays share 0x80132000),
    else of `main`."""
    index = collections.defaultdict(list)
    for (unit, name), path in files.items():
        text = read_asm(path)
        if "jal " not in text:
            continue
        for callee, reads in call_sites(parse_asm(text)):
            target = unit if (unit, callee) in files else "main" if ("main", callee) in files else None
            if target:
                index[(target, callee)].append((unit + ":" + name, reads))
    return index


# ---------------------------------------------------------------------------------------------
# driver

def analyze(body, asm_text=None, rewrite=True, callers=None, scope=None, select=None):
    """Run every detector on `body`. `asm_text`: the original function's asm (enables the asm-driven
    detectors); `callers`: build_callers() output (enables void-callers-read-v0, needs `scope`
    `UNIT:func`); rewrite=False only reports; `select(kind, n)` picks which rewritable findings are
    applied (default all). Returns Result(body, findings)."""
    findings = []
    body, found = goto_findings(body, rewrite, select)
    findings += found
    body, found = sjis_findings(body, rewrite, select)
    findings += found
    if asm_text:
        body, found = postinc_findings(body, asm_text, rewrite, select)
        findings += found
        asm = parse_asm(asm_text)
        if returns_void(body) and not has_value_return(body):
            for idx, kind, slot in exit_v0_defs(asm):
                if kind in ("copy", "zero", "const"):
                    findings.append(Finding("s32-no-return", 0, "%s of $v0 at instruction %d%s reaches the return unread"
                                            % (kind, idx, " (delay slot)" if slot else ""), False))
                    break
    if callers is not None and scope and returns_void(body):
        sites = callers.get(tuple(scope.split(":", 1)), [])
        reading = sorted({c for c, reads in sites if reads})
        if reading:
            findings.append(Finding("void-callers-read-v0", 0, "%d of %d call sites read $v0 (%s)"
                                    % (len([1 for _, r in sites if r]), len(sites), ", ".join(reading[:3])), False))
    return Result(body, findings)


def asm_for(scope, files):
    """Asm text of `UNIT:func` from asm_files(), or None."""
    path = files.get(tuple(scope.split(":", 1)))
    return read_asm(path) if path else None


def summarize(results):
    """{kind: {"bodies": n, "findings": n}} over [(scope, Result)]."""
    out = collections.defaultdict(lambda: {"bodies": 0, "findings": 0})
    for _, r in results:
        for kind in {f.kind for f in r.findings}:
            out[kind]["bodies"] += 1
        for f in r.findings:
            out[f.kind]["findings"] += 1
    return dict(sorted(out.items()))


def digest(text):
    return hashlib.sha256(text.encode()).hexdigest()


def load_selected(archive):
    """[(row, body text)] of the selected body of each catalog row of the archive."""
    rows = [json.loads(l) for l in read_asm(os.path.join(archive, "best-quality-catalog.jsonl")).splitlines() if l.strip()]
    out = []
    for row in rows:
        path = os.path.join(row["selected_path"], "body.c")
        if os.path.exists(path):
            out.append((row, read_asm(path)))
    return out


def survey(archive, json_out=None, callers=True, root="."):
    rows = load_selected(archive)
    files = asm_files(root)
    index = build_callers(files) if callers else None
    results, missing = [], 0
    for row, body in rows:
        asm_text = asm_for(row["scope"], files)
        missing += asm_text is None
        results.append((row["scope"], analyze(body, asm_text, rewrite=False, callers=index, scope=row["scope"])))
    blocked = collections.Counter(f.detail.split(": ", 1)[1] for _, r in results for f in r.findings
                                  if f.kind == "goto-shared-store-blocked")
    summary = dict(archive=archive, bodies=len(rows), asm_missing=missing, detectors=summarize(results),
                   goto_blocked_reasons=dict(sorted(blocked.items())))
    if json_out:
        detail = [dict(scope=s, findings=[f._asdict() for f in r.findings if not f.kind.endswith("-blocked")])
                  for s, r in results if any(not f.kind.endswith("-blocked") for f in r.findings)]
        with open(json_out, "w") as fh:
            json.dump(dict(summary=summary, detail=detail), fh, indent=1, sort_keys=True)
            fh.write("\n")
    return summary, results


# ---------------------------------------------------------------------------------------------
# rescore

def make_scorer(row):
    """score(text, work) -> diff rows of the function in the full translation unit `text`, 10**6 when
    it does not compile: the scoring path of tools/exhaustive_drafts.py."""
    import funcdiff
    import funcloc
    import order_search
    c = funcloc.locate(row["func"], row["src"])
    expected, problem = funcdiff.expected_object(c)
    if problem:
        raise RuntimeError(problem)
    want = funcdiff.resolved_functions(expected, [row["func"]], c)[row["func"]]
    if want is None:
        raise RuntimeError("target missing from expected object")

    def score(text, work):
        try:
            obj = order_search.compile_variant(c, text, work, False)
            got = funcdiff.resolved_functions(obj, [row["func"]])[row["func"]]
            if got is None:
                return 10 ** 6
            return 0 if funcdiff.keys(got) == funcdiff.keys(want) else len(funcdiff.diff_lines(want, got))
        except (SystemExit, ValueError, OSError, RuntimeError):
            return 10 ** 6
    return score


def rewrite_hits(archive, root=".", only=None):
    """[(row, body, Result)] of the selected bodies that a rewrite changes, lowest positive archived score
    first (a score of 0 is already a candidate; a missing score last)."""
    hits, files = [], asm_files(root)
    for row, body in load_selected(archive):
        if only and row["scope"] not in only:
            continue
        result = analyze(body, asm_for(row["scope"], files), rewrite=True)
        if result.body != body and result.applied:
            hits.append((row, body, result))
    hits.sort(key=lambda h: (h[0]["score"] == 0, h[0]["score"] if h[0]["score"] is not None else 10 ** 9, h[0]["scope"]))
    return hits


def fresh_context(row, body, folder):
    """The full translation unit of the current source file with `body` in place of the function's
    INCLUDE_ASM, or None. Used when the archived unit no longer compiles (another function of the file
    has been matched since, so its INCLUDE_ASM path moved)."""
    import autotweak
    import bulk_m2c
    func = row["scope"].split(":")[1]
    decls = os.path.join(folder, "declarations.h")
    lines = [l for l in read_asm(decls).splitlines() if l.strip()] if os.path.exists(decls) else []
    placed = bulk_m2c.insert_draft(read_asm(row["src"]), func, body, lines)
    return autotweak.rewrite_guard(placed, func, body) if placed else None


def best_rewrite(row, body, asm_text, context, scorer, scratch, fallback=None):
    """(baseline score, best score, chosen findings, body, context, tried scores) over the rewrites of
    `body`: all of them, each alone, and a greedy build-up from the best single when there are two to six.
    Scores come from `scorer(full translation unit text, work dir)`; the baseline is the unmodified
    `context`, or `fallback()` when that does not compile."""
    import shutil
    base = scorer(context, scratch / "baseline")
    if base >= 10 ** 6 and fallback:
        context = fallback() or context
        base = scorer(context, scratch / "baseline2")
    cands = sorted({(f.kind, f.n) for f in analyze(body, asm_text, rewrite=False).findings if f.kind in REWRITES})
    tried = {}

    def attempt(subset):
        key = frozenset(subset)
        if key not in tried:
            result = analyze(body, asm_text, rewrite=True, select=lambda k, n: (k, n) in key)
            text = context.replace(body, result.body, 1)
            score = base if result.body == body else scorer(text, scratch / ("t%d" % len(tried)))
            tried[key] = (score, result.body, text)
        return tried[key]

    attempt(cands)
    if 2 <= len(cands) <= 6:
        singles = sorted(cands, key=lambda c: attempt([c])[0])
        current = [singles[0]]
        for c in singles[1:]:
            if attempt(current + [c])[0] < attempt(current)[0]:
                current.append(c)
    best = min(tried, key=lambda k: (tried[k][0], -len(k), sorted(k)))
    shutil.rmtree(scratch, ignore_errors=True)
    return base, tried[best][0], sorted(best), tried[best][1], tried[best][2], {
        "+".join("%s#%d" % c for c in sorted(k)): v[0] for k, v in tried.items()}


def rescore(archive, top, out, jobs=2, root=".", only=None):
    """Rewrite the selected bodies, score the `top` hits with the lowest positive archived score in
    their full translation unit (baseline and rewrite in the current tree), save the improved ones
    under `out` with provenance. At most `jobs` compiles at once."""
    import pathlib
    import threading
    hits = rewrite_hits(archive, root, only)
    hits = hits[:top] if top else hits
    files = asm_files(root)

    def work(item):
        n, (row, body, result) = item
        folder = pathlib.Path(row["selected_path"])
        context = (folder / "context.c").read_text()
        record = dict(rank=n + 1, scope=row["scope"], unit=row["scope"].split(":")[0],
                      func=row["scope"].split(":")[1], src=row["src"], archived_score=row["score"],
                      source_path=str(folder),
                      draft_hash=digest(body), tool=TICKET + " tools/draft_fixes.py",
                      harness_hash=digest(read_asm(os.path.abspath(__file__))))
        if body not in context:
            record.update(status="context-mismatch", baseline=None, score=None)
            return record, None, None
        scorer = make_scorer(dict(row, func=record["func"]))
        rebuilt = []

        def fallback():
            rebuilt.append(True)
            return fresh_context(row, body, folder)

        base, score, chosen, new_body, new_context, tried = best_rewrite(
            row, body, asm_for(row["scope"], files), context, scorer, pathlib.Path(out) / "_work" / ("%03d" % n),
            fallback)
        record["context"] = "rebuilt from the current source (archived unit no longer compiles)" if rebuilt else "archived"
        record.update(baseline=base, score=score, chosen=["%s#%d" % c for c in chosen], tried=tried,
                      new_draft_hash=digest(new_body))
        record["status"] = ("compile-failed" if score >= 10 ** 6 else "zero" if score == 0 and base != 0
                            else "improved" if score < base else "same" if score == base else "worse")
        return record, new_body, new_context

    done, lock, todo = [None] * len(hits), threading.Lock(), iter(list(enumerate(hits)))

    def worker():
        while True:
            with lock:
                item = next(todo, None)
            if item is None:
                return
            try:
                done[item[0]] = work(item)
            except Exception as exc:     # one bad draft must not end the run
                row = item[1][0]
                done[item[0]] = (dict(scope=row["scope"], archived_score=row["score"], baseline=None, score=None,
                                      status="error", reason=str(exc)[:200]), None, None)

    threads = [threading.Thread(target=worker) for _ in range(max(1, jobs))]
    for t in threads:
        t.start()
    for t in threads:
        t.join()
    records = []
    for record, new_body, new_context in done:
        records.append(record)
        if record["status"] in ("improved", "zero"):
            folder = pathlib.Path(out) / record["unit"] / record["func"]
            folder.mkdir(parents=True, exist_ok=True)
            (folder / "body.c").write_text(new_body)
            (folder / "context.c").write_text(new_context)
            record["created"] = time.strftime("%Y-%m-%d", time.gmtime())
            (folder / "result.json").write_text(json.dumps(record, sort_keys=True) + "\n")
    with open(os.path.join(out, "results.jsonl"), "w") as fh:
        for record in records:
            fh.write(json.dumps(record, sort_keys=True) + "\n")
    import shutil
    shutil.rmtree(os.path.join(out, "_work"), ignore_errors=True)
    return records


def main(argv):
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    sub = ap.add_subparsers(dest="cmd", required=True)
    for name in ("check", "fix"):
        p = sub.add_parser(name)
        p.add_argument("body")
        p.add_argument("--scope", help="UNIT:func, to read the function's asm")
        p.add_argument("--asm", help="asm file of the function")
    p = sub.add_parser("survey")
    p.add_argument("--archive", default="drafts/T-14500")
    p.add_argument("--json")
    p.add_argument("--no-callers", action="store_true")
    p = sub.add_parser("rescore")
    p.add_argument("--archive", default="drafts/T-14500")
    p.add_argument("--top", type=int, default=30)
    p.add_argument("--out", default="drafts/T-16510")
    p.add_argument("--jobs", type=int, default=2)
    p.add_argument("--only", action="append", help="scope to rescore (repeatable)")
    p.add_argument("--list", action="store_true", help="list the hits and the rewrites, compile nothing")
    args = ap.parse_args(argv)
    if args.cmd in ("check", "fix"):
        body = read_asm(args.body)
        asm_text = None
        if args.asm:
            asm_text = read_asm(args.asm)
        elif args.scope:
            asm_text = asm_for(args.scope, asm_files())
            if asm_text is None:
                sys.exit("draft_fixes.py: no asm file for %s" % args.scope)
        result = analyze(body, asm_text, rewrite=args.cmd == "fix")
        if args.cmd == "fix":
            sys.stdout.write(result.body)
        for f in result.findings:
            sys.stderr.write("%s:%d: %s%s: %s\n" % (args.body, f.line, f.kind, " (fixed)" if f.fixed else "", f.detail))
    elif args.cmd == "survey":
        summary, _ = survey(args.archive, args.json, not args.no_callers)
        print(json.dumps(summary, indent=1, sort_keys=True))
    else:
        if args.list:
            for row, _, result in rewrite_hits(args.archive, only=set(args.only or [])):
                print("%-28s score %-5s %s" % (row["scope"], row["score"], ", ".join(sorted({f.kind for f in result.applied}))))
            return
        os.makedirs(args.out, exist_ok=True)
        records = rescore(args.archive, args.top, args.out, args.jobs, only=set(args.only or []))
        for r in records:
            print("%s archived %s baseline %s new %s %s" % (r["scope"], r["archived_score"], r["baseline"], r["score"], r["status"]))
        print(json.dumps(dict(collections.Counter(r["status"] for r in records))))


if __name__ == "__main__":
    main(sys.argv[1:])
