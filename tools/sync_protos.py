#!/usr/bin/env python3
"""One home for the declarations of main-exe symbols: include/main_api.h (T-3340).

Every merge of overlay work used to hit header clashes: an overlay declares a main-exe function or
global with one type, include/game.h or another overlay with another, and each batch agent wrote
its own declarations. The scheme now is:

  include/main_api.h      the one declaration of every main-exe symbol (address 0x80041000-0x8012B537:
                          functions, SDK functions, globals) that more than one file may use. The type is
                          the one the main-exe definition has (matched C in src/main), else what
                          include/game.h had, else the call-site evidence of the overlay headers.
                          include/game.h and every include/ovl/<NAME>.h include it; nothing else
                          declares a main-exe symbol.
  MAIN_API_OVERRIDE_<sym> a documented per-file exception. An overlay (or a src/main .c file) that was
                          matched against a different view of a symbol (narrower parameter, a signed
                          byte of an unsigned global, an unprototyped call) defines the macro
                          BEFORE the first include that leads to main_api.h, with the reason, and
                          declares its own view next to its other declarations:

                            #define MAIN_API_OVERRIDE_func_80083440 /* s32 argument: u8 would mask it */
                            ...
                            void func_80083440(s32 arg0);

                          main_api.h wraps that symbol in `#ifndef MAIN_API_OVERRIDE_<sym>`.

Modes (run from the repo root, inside Docker like every tool):
  sync_protos.py --report            main-exe symbols referenced from overlays, and every conflicting view
                                     across all headers (which header, which type, used by which files)
  sync_protos.py --check             the rules above; exit 1 with one message per problem, each with its fix
                                     (tools/check_headers.py runs this too, so ninja and CI enforce it)
  sync_protos.py --write             create or update include/main_api.h: adds declarations that other
                                     headers hold, moves it to the definition's type, adds the override guards
  sync_protos.py --fix               --write, then edit the headers: delete declarations that main_api.h now
                                     carries (same type, or another type nobody uses), keep the others as
                                     explicit overrides with an automatic reason, add the main_api.h include
  sync_protos.py --snapshot FILE     save the type every .c file sees for every symbol it uses (JSON)
  sync_protos.py --compare FILE      list the symbols whose view changed since the snapshot; the proof that a
                                     header refactor changes what no matched function sees

Symbol names: `func_XXXXXXXX` and `D_XXXXXXXX` carry their address; renamed symbols are looked up in
config/symbol_addrs*.txt. The EVENT overlay (0x800F6000) shares addresses with the main bss: a symbol
at or above its load address is EVENT's own unless another header declares it as a main symbol.
"""
import argparse
import json
import os
import re
import subprocess
import sys
from collections import defaultdict

import check_headers as ch

MAIN_LO, MAIN_HI = 0x80041000, 0x8012B538   # main exe image: text, data and bss (wiki/executable.md)
API = "main_api.h"
OVERRIDE = "MAIN_API_OVERRIDE_"
SDK_HEADERS = ("libgpu.h", "libapi.h")       # the PsyQ SDK's own declarations, never moved
BUILTIN = {"void", "u8", "s8", "u16", "s16", "u32", "s32", "char", "int", "short", "long", "float",
           "double", "unsigned", "signed", "const", "volatile", "struct", "union", "enum", "F"}
ADDR_RE = re.compile(r"^(?:func|D)_([0-9A-Fa-f]{8})$")
DEFINE_RE = re.compile(r"^\s*#\s*define\s+" + OVERRIDE + r"(\w+)\s*(?:/\*(.*?)\*/)?\s*$", re.M)


# ---------------------------------------------------------------------------------------------
# config

def load_symbol_map(root):
    """{name: address} from config/symbol_addrs*.txt (renamed main-exe symbols)."""
    out = {}
    cfg = os.path.join(root, "config")
    if os.path.isdir(cfg):
        for f in sorted(os.listdir(cfg)):
            if f.startswith("symbol_addrs") and f.endswith(".txt"):
                with open(os.path.join(cfg, f), errors="replace") as fh:
                    for line in fh:
                        m = re.match(r"^\s*(\w+)\s*=\s*0x([0-9A-Fa-f]+)", line)
                        if m:
                            out[m.group(1)] = int(m.group(2), 16)
    return out


def load_overlay_bases(root):
    """{overlay: load address} from config/overlays.txt."""
    out = {}
    p = os.path.join(root, "config", "overlays.txt")
    if os.path.exists(p):
        with open(p) as fh:
            for line in fh:
                m = re.match(r"^(\w+)\s+0x([0-9A-Fa-f]+)", line)
                if m:
                    out[m.group(1)] = int(m.group(2), 16)
    return out


_CONFIG = {}


def is_main_symbol(root, name, unit="main"):
    """True when `name`, used by `unit` ('main' or an overlay name), is a main-exe symbol that
    belongs in include/main_api.h (used by tools/dupes.py to pick the header of a declaration)."""
    if root not in _CONFIG:
        _CONFIG[root] = (load_symbol_map(root), load_overlay_bases(root))
    symmap, bases = _CONFIG[root]
    m = ADDR_RE.match(name)
    addr = int(m.group(1), 16) if m else symmap.get(name)
    if addr is None or not MAIN_LO <= addr < MAIN_HI:
        return False
    base = bases.get(unit)
    return base is None or addr < base


# ---------------------------------------------------------------------------------------------
# model

class Model:
    """Everything read from include/ and src/ (nothing from the game data)."""

    def __init__(self, inc, src=None):
        for h in ch.header_files(inc).values():   # headers may have been edited since the last model
            ch.drop_cache(h)
        self.inc = inc
        self.src = src
        self.root = os.path.dirname(os.path.abspath(inc))
        self.headers = ch.header_files(inc)
        self.symmap = self._symbol_map()
        self.bases = self._overlay_bases()
        self.own = {}       # header -> [(name, type, raw)]
        for h, p in self.headers.items():
            self.own[h] = list(_declarations(_read(p)))
        self.sources = {}   # src .c path -> info
        if src:
            for root, _d, files in os.walk(src):
                for f in sorted(files):
                    if f.endswith(".c"):
                        self.sources[os.path.join(root, f)] = self._read_source(os.path.join(root, f))
        self.api_present = API in self.headers
        self.api = {n: (t, r) for n, t, r in self.own.get(API, [])}
        self.overrides = self._overrides()
        self.users = {}      # header -> .c files that include it
        self._others = None
        self._closure = {}   # header -> headers reached by #include; file -> same (forget() after a write)
        self._lines = {}     # header -> LineIndex of its text

    def forget(self):
        """Drop what depends on header text after a header was rewritten (the declarations read at
        construction stay, as before)."""
        self._closure.clear()
        self._lines.clear()

    # -- config
    def _symbol_map(self):
        return load_symbol_map(self.root)

    def _overlay_bases(self):
        return load_overlay_bases(self.root)

    # -- sources
    def _read_source(self, path):
        with open(path, errors="replace") as fh:
            text = fh.read()
        return {"text": text, "tokens": _tokens(text)}

    # -- symbols
    def addr(self, name):
        m = ADDR_RE.match(name)
        if m:
            return int(m.group(1), 16)
        return self.symmap.get(name)

    @staticmethod
    def owner(header):
        m = re.match(r"^ovl/(\w+)\.h$", header)
        return m.group(1) if m else None

    def in_main_range(self, name):
        a = self.addr(name)
        return a is not None and MAIN_LO <= a < MAIN_HI

    def others_declare(self):
        """{symbol: set of headers other than the overlay's own} used to settle the EVENT range."""
        if self._others is None:
            self._others = defaultdict(set)
            for h, ds in self.own.items():
                if h in SDK_HEADERS or h == API:
                    continue
                for n, _t, _r in ds:
                    self._others[n].add(h)
        return self._others

    def base_of(self, header):
        owner = self.owner(header)
        return self.bases.get(owner) if owner else None

    def is_main(self, name, header):
        """True when `name`, declared in `header`, is a main-exe symbol."""
        if not self.in_main_range(name) or header in SDK_HEADERS:
            return False
        base = self.base_of(header)
        if base is None or self.addr(name) < base:
            return True
        # overlay data over the main bss (EVENT): the overlay's own, unless main_api.h or a header
        # that cannot be that overlay declares the symbol
        if name in self.api:
            return True
        for h in self.others_declare().get(name, ()):
            b = self.base_of(h)
            if h != header and (b is None or self.addr(name) < b):
                return True
        return False

    # -- overrides
    def _overrides(self):
        """{header name or .c path: {symbol: reason}}."""
        out = {}
        texts = [(h, _read(p)) for h, p in self.headers.items()]
        texts += [(p, info["text"]) for p, info in self.sources.items()]
        for f, text in texts:
            found = {m.group(1): (m.group(2) or "").strip() for m in DEFINE_RE.finditer(text)}
            if found:
                out[f] = found
        return out

    def local_types(self, header):
        """typedef names defined in `header` itself."""
        text = _read(self.headers[header])
        text = re.sub(r"/\*.*?\*/", " ", text, flags=re.S)
        names = set(re.findall(r"}\s*(\w+)\s*;", text))
        names |= set(re.findall(r"typedef\s+[^;{]*?\b(\w+)\s*;", text))
        return names

    def api_types(self):
        """typedef names a declaration in main_api.h may use (everything main_api.h includes)."""
        names = set()
        todo, done = [API, "common.h", "libgpu.h"], set()
        while todo:
            h = todo.pop()
            if h in done or h not in self.headers:
                continue
            done.add(h)
            names |= self.local_types(h)
            todo += list(ch.includes(self.headers[h]))
        return names

    def needs_local_type(self, typ_raw, header):
        """True when the declaration uses a type that only `header` defines (it cannot move)."""
        idents = set(re.findall(r"[A-Za-z_]\w*", typ_raw)) - BUILTIN
        idents = {i for i in idents if not ADDR_RE.match(i) and i not in self.symmap}
        local = self.local_types(header) - self.api_types()
        return any(i in local for i in idents)


def _read(path):
    with open(path, errors="replace") as fh:
        return fh.read()


def _write(path, text):
    """Write a file and forget the parse caches of it (check_headers caches by mtime and size)."""
    with open(path, "w") as fh:
        fh.write(text)
    ch.drop_cache(path)


# parse results by exact text: the five Models of a --fix run read the same sources five times
_TOKENS, _DECLS = {}, {}


def _tokens(text):
    if text not in _TOKENS:
        _TOKENS[text] = tokens(text)
    return _TOKENS[text]


def _declarations(text):
    if text not in _DECLS:
        _DECLS[text] = tuple(ch.declarations_text(text))
    return _DECLS[text]


def tokens(text):
    """Identifiers of C text outside comments, strings and INCLUDE_ASM/INCLUDE_RODATA statements."""
    text = re.sub(r"/\*.*?\*/", " ", text, flags=re.S)
    text = re.sub(r'"(?:\\.|[^"\\\n])*"', '""', text)
    text = re.sub(r"^\s*#.*$", " ", text, flags=re.M)
    text = re.sub(r"INCLUDE_(?:ASM|RODATA)\s*\([^)]*\)\s*;", " ", text)
    return set(re.findall(r"[A-Za-z_]\w*", text))


def specificity(typ):
    """Order of function types: a prototype with parameters, then (void), then ()."""
    if "F(" not in typ:
        return 1
    params = typ.split("F(", 1)[1]
    if params == ")":
        return 0
    if params == "void)":
        return 1
    return 2


def effective(types):
    """The type one translation unit effectively sees when it holds these declarations."""
    best = None
    for t in types:
        if best is None or specificity(t) > specificity(best):
            best = t
    return best


def params_of(typ):
    return [p for p in typ.split("F(", 1)[1].rstrip(")").split(",") if p] if "F(" in typ else []


def narrow_params(typ):
    """True when a prototype of this type narrows an argument (u8, s16, float, ...)."""
    return any(p in ch.NARROW for p in params_of(typ))


def benign(view, canon):
    """True when `view` and `canon` generate the same call code: functions with one return type
    where one side is unprototyped `()` and the other is `(void)` or a prototype without narrow
    parameters."""
    if not (is_func(view) and is_func(canon)):
        return False
    if view.split("F(", 1)[0] != canon.split("F(", 1)[0]:
        return False
    if view == canon:
        return True
    sv, sc = specificity(view), specificity(canon)
    if 0 not in (sv, sc):
        return False
    other = canon if sv == 0 else view
    return not narrow_params(other)


def is_func(typ):
    return "F(" in typ


# ---------------------------------------------------------------------------------------------
# which units use what

def unit_walk(model, path):
    """[(name, type, file, raw)] a src .c file sees from its includes."""
    return ch.walk(path, model.inc, rel="<src>")


def unit_views(model):
    """{src path: {symbol: effective type}} for every symbol a .c file uses and declares."""
    out = {}
    for path, info in model.sources.items():
        seen = defaultdict(list)
        for name, typ, f, _raw in unit_walk(model, path):
            if name in info["tokens"]:
                seen[name].append(typ)
        out[path] = {n: effective(ts) for n, ts in seen.items()}
    return out


def users_of(model, header):
    """src .c files whose include closure contains `header`."""
    if header not in model.users:
        model.users[header] = [p for p in model.sources if header in closure_of_file(model, p)]
    return model.users[header]


def used_in(model, name, header):
    """True when a .c file that includes `header` uses `name`."""
    return any(name in model.sources[p]["tokens"] for p in users_of(model, header))


# ---------------------------------------------------------------------------------------------
# canonical choice

def definitions(model):
    """{function: (type, raw)} defined with a body in the main exe's C files (src/main)."""
    out = {}
    for path, info in model.sources.items():
        if os.sep + "main" + os.sep not in path.replace(os.path.sep, os.sep) and "/main/" not in path:
            continue
        for name, typ, raw in ch.declarations_text(info["text"], defs=True):
            if is_func(typ) and re.search(r"\b" + re.escape(name) + r"\s*\([^;{]*\)\s*\{", info["text"]):
                out[name] = (typ, raw)
    return out


def candidates(model):
    """{symbol: [(header, type, raw)]} for every main-exe symbol declared outside main_api.h."""
    out = defaultdict(list)
    for h, ds in model.own.items():
        if h == API or h in SDK_HEADERS:
            continue
        for n, t, r in ds:
            if model.is_main(n, h):
                out[n].append((h, t, r))
    return out


def overlaps(model, name, header):
    """True when `header` belongs to an overlay that is loaded over the address of `name`."""
    base = model.base_of(header)
    return base is not None and model.addr(name) >= base


def rank_view(model, name, header, typ, group):
    """Sort key of a view when no definition and no existing entry decides: used views first, then
    the most specific, then the most common, then game.h, then the widest scalar."""
    users = sum(1 for h, t, _r in group if t == typ and used_in(model, name, h))
    count = sum(1 for _h, t, _r in group if t == typ)
    return (-users, -specificity(typ), -count, 0 if header == "game.h" else 1, typ)


def choose(model, name, group, defs):
    """(type, raw, why) of the canonical declaration of `name`."""
    if name in defs:
        t, r = defs[name]
        if name in model.api and benign(model.api[name][0], t):
            # a K&R `()` kept on purpose: callers pass another number of arguments than the definition takes
            return model.api[name][0], model.api[name][1], "main_api.h"
        return t, r, "definition"
    if name in model.api:
        t, r = model.api[name]
        return t, r, "main_api.h"
    # game.h and main_only.h were the project's home for main-exe symbols: keep their view
    for home in ("game.h", "main_only.h"):
        views = [(t, r) for h, t, r in group if h == home]
        if views:
            t = effective([v[0] for v in views])
            if specificity(t) == 0 and is_func(t):
                # unprototyped in the home header: the call sites of the overlays are the evidence
                protos = [g for g in group if g[0] != home and specificity(g[1]) == 2 and not narrow_params(g[1])]
                if protos:
                    best = min(protos, key=lambda g: rank_view(model, name, g[0], g[1], protos))
                    return best[1], best[2], "call sites"
            return t, next(r for tt, r in views if tt == t), home
    # a view from an overlay that sits over this address (EVENT over the main bss) is that overlay's
    # own variable: it only decides when nothing else declares the symbol
    plain = [g for g in group if not overlaps(model, name, g[0])] or group
    best = min(plain, key=lambda g: rank_view(model, name, g[0], g[1], plain))
    return best[1], best[2], "overlay views"


def plan(model):
    """{symbol: (type, raw, why)} for every symbol main_api.h must hold."""
    defs = definitions(model) if model.sources else {}
    cands = candidates(model)
    out = {}
    for name in set(cands) | set(model.api):
        group = cands.get(name, [])
        if name not in cands and name not in defs:
            t, r = model.api[name]
            out[name] = (t, r, "main_api.h")
            continue
        if group and all(model.needs_local_type(r, h) for h, _t, r in group):
            continue
        group = [g for g in group if not model.needs_local_type(g[2], g[0])]
        out[name] = choose(model, name, group, defs if name in defs else {})
    return out


# ---------------------------------------------------------------------------------------------
# report

def short(t):
    return t.replace("F(", "(") if is_func(t) else t


def report(model, out=sys.stdout):
    cands = candidates(model)
    defs = definitions(model) if model.sources else {}
    plan_ = plan(model)
    ovl_users = defaultdict(set)   # symbol -> overlays whose sources use it
    for path, info in model.sources.items():
        m = re.search(r"src[/\\]ovl[/\\](\w+)[/\\]", path)
        if m:
            for n in info["tokens"]:
                if n in cands or n in model.api:
                    ovl_users[n].add(m.group(1))
    ovl_syms = {n for n in cands if any(model.owner(h) for h, _t, _r in cands[n])} | set(ovl_users)
    out.write("main-exe symbols declared in headers: %d (%d data, %d functions); in main_api.h: %d\n" % (
        len(set(cands) | set(model.api)),
        sum(1 for n in set(cands) | set(model.api) if not is_func((plan_.get(n) or model.api.get(n) or ("", ""))[0])),
        sum(1 for n in set(cands) | set(model.api) if is_func((plan_.get(n) or model.api.get(n) or ("", ""))[0])),
        len(model.api)))
    out.write("referenced from overlay headers or sources: %d\n" % len(ovl_syms))
    conflicts = []
    for name in sorted(cands, key=lambda n: (model.addr(n) or 0, n)):
        views = cands[name]
        canon = plan_.get(name, (None,))[0]
        distinct = {t for _h, t, _r in views} | ({canon} if canon else set())
        if len(distinct) > 1:
            conflicts.append(name)
    out.write("conflicting views: %d\n" % len(conflicts))
    for name in conflicts:
        canon = plan_.get(name)
        out.write("%s  canonical %s (%s)\n" % (name, short(canon[0]) if canon else "?", canon[2] if canon else "-"))
        for h, t, r in cands[name]:
            if canon and t == canon[0]:
                continue
            use = used_in(model, name, h) if model.sources else None
            ov = "override" if name in model.overrides.get(h, {}) else ""
            out.write("    %-16s %-24s %s %s\n" % (h, short(t), "used" if use else "unused", ov))
    return conflicts


# ---------------------------------------------------------------------------------------------
# check

def check_api(inc, src=None):
    """Problems (strings with the fix) for the main_api.h rules. Empty when main_api.h is absent."""
    model = Model(inc, src)
    if not model.api_present:
        return []
    problems = []
    api = model.api
    guarded = api_guards(model)
    defs = definitions(model) if model.sources else {}
    for h, ds in sorted(model.own.items()):
        if h in (API,) or h in SDK_HEADERS:
            continue
        ov = model.overrides.get(h, {})
        for name, typ, raw in ds:
            if not model.is_main(name, h) or model.needs_local_type(raw, h):
                continue
            if name not in api:
                problems.append("main symbol %s is declared in include/%s: declare it in include/%s instead "
                                "(python3 tools/sync_protos.py --write adds it from the headers)" % (name, h, API))
            elif api[name][0] == typ:
                problems.append("duplicate %s: include/%s declares '%s' like include/%s; delete it here "
                                "(python3 tools/sync_protos.py --fix does it)" % (name, h, short(typ), API))
            elif name in ov:
                if name not in guarded:
                    problems.append("override %s in %s is not guarded in include/%s: run python3 tools/sync_protos.py --write"
                                    % (name, h, API))
            else:
                problems.append("conflict %s: include/%s has '%s', include/%s has '%s'. Use the %s type (delete the "
                                "line) or, if this overlay was matched against its own view, add "
                                "`#define %s%s /* reason */` before the first include of %s and run "
                                "python3 tools/sync_protos.py --write"
                                % (name, h, short(typ), API, short(api[name][0]), API, OVERRIDE, name, h))
    # overrides: reason, target, redundancy
    for f, found in sorted(model.overrides.items()):
        for name, reason in sorted(found.items()):
            where = f if f in model.headers else os.path.relpath(f, model.root)
            if not reason:
                problems.append("override %s in %s has no reason: write `#define %s%s /* why this file keeps its own view */`"
                                % (name, where, OVERRIDE, name))
            if name not in api:
                problems.append("override %s in %s names a symbol that include/%s does not declare" % (name, where, API))
                continue
            if f in model.headers:
                mine = [t for n, t, _r in model.own[f] if n == name]
                if mine and mine[0] == api[name][0]:
                    problems.append("override %s in %s is redundant: its declaration equals include/%s; delete the define"
                                    % (name, where, API))
                elif not mine and not reason.lower().startswith("implicit"):
                    problems.append("override %s in %s has no declaration there: declare the view in the header, or "
                                    "start the reason with 'implicit' when the file relies on an implicit declaration"
                                    % (name, where))
            if name not in guarded:
                problems.append("override %s in %s is not guarded in include/%s: run python3 tools/sync_protos.py --write"
                                % (name, where, API))
    # a .c file that includes main_api.h must not declare its symbols again
    for path, info in sorted(model.sources.items()):
        decl = list(ch.declarations_text(info["text"]))
        if not decl or API not in {f for _n, _t, f, _r in unit_walk(model, path)} | closure_of_file(model, path):
            continue
        ov = model.overrides.get(path, {})
        for name, typ, _raw in decl:
            if name in api and name not in ov:
                where = os.path.relpath(path, model.root)
                if api[name][0] == typ:
                    problems.append("duplicate %s: %s declares '%s' like include/%s; delete the line"
                                    % (name, where, short(typ), API))
                else:
                    problems.append("conflict %s: %s has '%s', include/%s has '%s'. Use the %s type, or add "
                                    "`#define %s%s /* reason */` before the first include of %s"
                                    % (name, where, short(typ), API, short(api[name][0]), API, OVERRIDE, name, where))
    # every overlay header reaches main_api.h
    for h in sorted(model.headers):
        if model.owner(h) and API not in closure(model, h):
            problems.append("include/%s does not include %s (directly or through game.h): add `#include \"%s\"` "
                            "after common.h (python3 tools/sync_protos.py --fix does it)" % (h, API, API))
    # the definition is the source of truth
    for name, (typ, _raw) in sorted(defs.items()):
        if name in api and api[name][0] != typ and not benign(api[name][0], typ):
            problems.append("%s: include/%s has '%s' but the main exe defines it as '%s': update %s "
                            "(python3 tools/sync_protos.py --write)" % (name, API, short(api[name][0]), short(typ), API))
    return problems


def closure(model, header):
    """Headers reached from `header` by #include (itself included)."""
    key = ("h", header)
    if key not in model._closure:
        seen, stack = set(), [header]
        while stack:
            h = stack.pop()
            if h in seen or h not in model.headers:
                continue
            seen.add(h)
            stack += list(ch.includes(model.headers[h]))
        model._closure[key] = seen
    return model._closure[key]


def closure_of_file(model, path):
    """Headers reached from a .c file by #include."""
    key = ("f", path)
    if key not in model._closure:
        out = set()
        for i in ch.includes(path):
            out |= closure(model, i)
        model._closure[key] = out
    return model._closure[key]


def api_guards(model):
    """Symbols main_api.h wraps in `#ifndef MAIN_API_OVERRIDE_<sym>`."""
    if not model.api_present:
        return set()
    text = _read(model.headers[API])
    return set(re.findall(r"^\s*#\s*ifndef\s+" + OVERRIDE + r"(\w+)", text, flags=re.M))


# ---------------------------------------------------------------------------------------------
# write main_api.h

HEADER_TEXT = '''#ifndef MAIN_API_H
#define MAIN_API_H

/* The one declaration of every main-exe symbol (T-3340). Maintained with tools/sync_protos.py.
 *
 *   - A main-exe function or global (address 0x80041000-0x8012B537, or a name from
 *     config/symbol_addrs*.txt) is declared here and nowhere else. include/game.h and every
 *     include/ovl/<NAME>.h include this file; tools/check_headers.py (ninja, CI) rejects any other
 *     declaration and names the fix. New symbol: add it here (python3 tools/sync_protos.py --write
 *     copies it from the header that has it).
 *   - The type is the one the main-exe definition has (matched C in src/main). While the function is
 *     still INCLUDE_ASM the type comes from the call sites; change it here when a definition shows
 *     better. `()` is kept on purpose where callers pass other arguments than the definition takes.
 *   - An overlay (or src/main file) that was matched against another view of a symbol (signed byte of
 *     an unsigned global, scalar of an array, another return type, an implicit declaration) keeps it
 *     explicitly: it defines MAIN_API_OVERRIDE_<symbol> with the reason before its first include and
 *     declares its own type next to its other declarations; the symbol is then wrapped in the
 *     #ifndef below. An override changes what that file's functions see, so add one only when the
 *     matched code needs it: python3 tools/sync_protos.py --prune tries to delete each (ninja decides).
 *     Rules: CODING_STANDARDS 8a. */

#include "common.h"
#include "libgpu.h"
'''


def _norm_line(line):
    line = re.sub(r"/\*.*?\*/", " ", line)
    line = re.sub(r"\s+", " ", line).strip()
    line = re.sub(r"^extern\s+", "", line)
    return line.rstrip(";").strip()


class LineIndex:
    """The single-line declarations of a header text by normalised form: find() is a dict lookup
    (the scan of every line per symbol took minutes on the 1600-symbol main_api.h)."""

    def __init__(self, lines):
        self.lines = lines
        self.first = {}
        for i, line in enumerate(lines):
            if line.rstrip().endswith(("*/", ";")):
                self.first.setdefault(_norm_line(line), i)

    def find(self, raw):
        """(index, trailing comment) of the declaration `raw`, or (None, '')."""
        i = self.first.get(re.sub(r"\s+", " ", raw).strip())
        if i is None:
            return None, ""
        m = re.search(r"(/\*.*\*/)\s*$", self.lines[i])
        return i, (m.group(1) if m else "")


def find_line(lines, raw):
    """(index, trailing comment) of the single-line declaration `raw` in `lines`, or (None, '')."""
    return LineIndex(lines).find(raw)


def decl_text(name, typ, raw, comment=""):
    text = ("" if is_func(typ) else "extern ") + raw + ";"
    return text + (" " + comment if comment else "")


def sort_key(model, name):
    a = model.addr(name)
    return (a if a is not None else 1 << 40, name)


def render_api(model, entries, guards):
    """main_api.h text. entries: {name: (type, line text)}; guards: symbols wrapped in #ifndef."""
    data = sorted((n for n, (t, _l) in entries.items() if not is_func(t)), key=lambda n: sort_key(model, n))
    funcs = sorted((n for n, (t, _l) in entries.items() if is_func(t)), key=lambda n: sort_key(model, n))
    out = [HEADER_TEXT.rstrip("\n"), "", "/* ---- globals ---- */"]

    def emit(names):
        for n in names:
            line = entries[n][1]
            if n in guards:
                out.extend(["#ifndef " + OVERRIDE + n, line, "#endif"])
            else:
                out.append(line)

    emit(data)
    out += ["", "/* ---- functions ---- */"]
    emit(funcs)
    out += ["", "#endif /* MAIN_API_H */", ""]
    return "\n".join(out)


def line_index(model, header):
    if header not in model._lines:
        model._lines[header] = LineIndex(_read(model.headers[header]).split("\n"))
    return model._lines[header]


def existing_lines(model):
    """{symbol: line} of the declarations in main_api.h, comments kept."""
    out = {}
    if not model.api_present:
        return out
    idx = line_index(model, API)
    for n, _t, raw in model.own[API]:
        i, _c = idx.find(raw)
        if i is not None:
            out[n] = idx.lines[i].rstrip()
    return out


def source_comment(model, header, raw):
    return line_index(model, header).find(raw)[1]


def build_entries(model, plan_):
    keep = existing_lines(model)
    cands = candidates(model)
    entries = {}
    for name, (typ, raw, why) in plan_.items():
        if name in keep and model.api.get(name, (None,))[0] == typ:
            entries[name] = (typ, keep[name])
            continue
        comment = ""
        for h, t, r in cands.get(name, ()):
            if t == typ and r == raw:
                comment = source_comment(model, h, r)
                break
        entries[name] = (typ, decl_text(name, typ, raw, comment))
    return entries


def all_overrides(model):
    """Symbols some header or .c file overrides."""
    out = set()
    for found in model.overrides.values():
        out |= set(found)
    return out


def write_api(model, plan_, path=None):
    entries = build_entries(model, plan_)
    guards = all_overrides(model) & set(entries)
    text = render_api(model, entries, guards)
    path = path or os.path.join(model.inc, API)
    old = _read(path) if os.path.exists(path) else ""
    if old != text:
        _write(path, text)
    return old != text, len(entries), len(guards)


# ---------------------------------------------------------------------------------------------
# --fix: edit the headers

def reason_for(model, header, name, typ, canon):
    """One-line reason written next to an automatic override."""
    if overlaps(model, name, header):
        return "%s overlay data at this address, %s (main_api.h: %s)" % (model.owner(header), short(typ), short(canon))
    if is_func(typ):
        return "matched with %s (main_api.h: %s)" % (short(typ), short(canon))
    return "matched as %s (main_api.h: %s)" % (short(typ), short(canon))


def _declarators(stmt):
    """(prefix incl. base type, [declarator text]) of one `extern T a, *b[];` statement, or None."""
    body = re.sub(r"^\s*extern\s+", "", stmt)
    parts = ch.split_top(body, ",")
    mm = re.match(r"^((?:\w+[\s]+)*?)([\s*]*[A-Za-z_]\w*\s*(?:\[.*|\(.*)?)$", parts[0].strip(), flags=re.S)
    if not mm or not mm.group(1).strip():
        return None
    return mm.group(1), [mm.group(2).strip()] + [p.strip() for p in parts[1:]]


def rewrite_line(line, drop):
    """`line` without the declarations named in `drop`; None when nothing is left of it."""
    m = re.match(r"^(.*?)(\s*/\*.*\*/)?\s*$", line)
    body, comment = m.group(1), (m.group(2) or "")
    if body.count("/*") or ";" not in body or not any(re.search(r"\b%s\b" % re.escape(n), body) for n in drop):
        return line
    stmts = body.split(";")
    tail = stmts.pop()
    out = []
    for st in stmts:
        names = [n for n, _t, _r in ch.declarations_text(st + ";")]
        if not names or not any(n in drop for n in names):
            out.append(st.strip())
            continue
        if all(n in drop for n in names):
            continue
        base, decls = _declarators(st)
        keep = [d for d in decls if re.search(r"([A-Za-z_]\w*)\s*(?:\[.*|\(.*)?$", d).group(1) not in drop]
        ext = "extern " if st.strip().startswith("extern") else ""
        out.append(ext + base + ", ".join(keep))
    if not out:
        return None
    return ";".join(out) + ";" + tail + comment


def fix_header(model, plan_, h, dry=False, log=print):
    """Rewrite one header: drop declarations main_api.h carries, mark the used differing ones as
    overrides, include main_api.h. Returns the number of declarations changed."""
    path = model.headers[h]
    text = _read(path)
    is_home = h in ("game.h", "main_only.h")
    drop, overrides = set(), {}
    for name, typ, raw in model.own[h]:
        if not model.is_main(name, h) or model.needs_local_type(raw, h) or name not in plan_:
            continue
        canon = plan_[name][0]
        if typ == canon:
            drop.add(name)
        elif is_home or benign(typ, canon) or not used_in(model, name, h):
            drop.add(name)
            why = "home header" if is_home else ("benign: %s -> %s" % (short(typ), short(canon)) if benign(typ, canon) else "unused")
            log("  %s: dropped %s %s (%s)" % (h, name, short(typ), why))
        else:
            overrides[name] = reason_for(model, h, name, typ, canon)
    out = []
    for line in text.split("\n"):
        new = rewrite_line(line, drop) if drop and not line.lstrip().startswith("#") else line
        if new is None:
            # a one-line comment right above a removed declaration labelled it: drop it with it,
            # unless it says something about the symbol (kept in the log to move by hand)
            if out and re.match(r"^\s*/\*[^*]*\*/\s*$", out[-1]):
                if re.search(r"T-\d+|batch|Main-exe|overlay headers|defined in C|data and functions|used by|matched against", out[-1]):
                    out.pop()
                else:
                    log("  %s: kept comment above a removed declaration: %s" % (h, out[-1].strip()))
            continue
        out.append(new)
    # override defines go right after the include guard, before any include
    existing = set(model.overrides.get(h, {}))
    new_defs = sorted((n for n in overrides if n not in existing), key=lambda n: sort_key(model, n))
    if new_defs:
        defs = ["#define %s%s /* %s */" % (OVERRIDE, n, overrides[n]) for n in new_defs]
        at = next((i + 1 for i, l in enumerate(out) if re.search(OVERRIDE, l)), None)
        if at is not None:   # after the last existing define of the block
            at = max(i + 1 for i, l in enumerate(out) if re.search(r"#\s*define\s+" + OVERRIDE, l))
            out[at:at] = defs
        else:
            at = next((i + 1 for i, l in enumerate(out) if re.match(r"\s*#\s*define\s+\w+_H\b", l)), 0)
            out[at:at] = ["", "/* main_api.h overrides (T-3340, tools/sync_protos.py): the views this overlay was matched with. */"] + defs
    # the include
    if (model.owner(h) or h == "game.h") and API not in closure(model, h):
        at = max([i for i, l in enumerate(out) if re.match(r'\s*#\s*include\s+"(common|libgpu)\.h"', l)] or [-1]) + 1
        if h == "game.h" or "game.h" not in closure(model, h):
            out.insert(at, '#include "%s"' % API)
    new = re.sub(r"\n{3,}", "\n\n", "\n".join(out))
    if new != text and not dry:
        _write(path, new)
        model.forget()
    return len(drop) + len(overrides)


# ---------------------------------------------------------------------------------------------
# --prune: retire overrides the build does not need

BLOCK_COMMENT = re.compile(r"^\s*/\*\s*main_api\.h overrides\b.*\*/\s*$")


def tidy_override_block(lines):
    """Drop the explanatory comment of an override block whose defines are all gone."""
    out = []
    for i, l in enumerate(lines):
        if BLOCK_COMMENT.match(l):
            rest = [x for x in lines[i + 1:i + 3] if x.strip()]
            if not (rest and re.match(r"\s*#\s*define\s+" + OVERRIDE, rest[0])):
                continue
        out.append(l)
    return out


def remove_override(path, name):
    """Delete the override define of `name` in `path` and the file's own declaration of it."""
    lines = _read(path).split("\n")
    out = []
    for l in lines:
        if re.match(r"\s*#\s*define\s+" + OVERRIDE + re.escape(name) + r"\b", l):
            continue
        if l[:1].isalpha():   # a file-scope declaration starts in column 0; code and #-lines stay as they are
            l = rewrite_line(l, {name})
            if l is None:
                continue
        out.append(l)
    text = re.sub(r"\n{3,}", "\n\n", "\n".join(tidy_override_block(out))).lstrip("\n")
    _write(path, text)


def ninja_target(model, f):
    """The ninja target that proves the code behind override file `f` still matches."""
    if f in model.headers:
        return "build/ovl/%s.ok" % model.owner(f)
    rel = os.path.relpath(f, model.root).replace(os.sep, "/")
    m = re.match(r"src/ovl/(\w+)/", rel)
    return "build/ovl/%s.ok" % m.group(1) if m else "build/SLPM_86.053.ok"


def prune(model, run=None, log=print):
    """Try to remove every override (with the file's own declaration) and keep the removal when the
    unit still matches. `run(target)` returns True when the target builds (ninja by default).
    Returns (removed, kept) lists of (file, symbol)."""
    if run is None:
        def run(target):
            return subprocess.run(["ninja", target], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL).returncode == 0
    removed, kept = [], []
    for f, found in sorted(model.overrides.items()):
        path = model.headers.get(f, f)
        target = ninja_target(model, f)
        for name in sorted(found, key=lambda n: sort_key(model, n)):
            before = _read(path)
            remove_override(path, name)
            if run(target):
                log("  %s: %s not needed (%s still matches)" % (f if f in model.headers else os.path.relpath(f, model.root),
                                                                 name, target))
                removed.append((f, name))
            else:
                _write(path, before)
                kept.append((f, name))
    return removed, kept


# ---------------------------------------------------------------------------------------------
# --check-branch: disagreements that a single header does not show (T-5030)

KNOWN = os.path.join("config", "proto_known.txt")
JAL_RE = re.compile(r"^[ \t]*/\*[^*\n]*\*/[ \t]+jal[ \t]+(\w+)", re.M)
ASM_INSN_RE = re.compile(r"^\s*/\*\s*\w+\s+[0-9A-Fa-f]{8}\s+[0-9A-Fa-f]{8}\s*\*/\s+(\w+)\s*(.*?)\s*$")
ASM_LABEL_RE = re.compile(r"^\s*\.L[0-9A-Fa-f]{8}:")
READS_FIRST = {"sb", "sh", "sw", "swl", "swr", "swc1", "swc2", "mtc0", "mtc1", "mtc2", "ctc0", "ctc1", "ctc2",
               "jr", "jalr", "mult", "multu", "div", "divu", "b", "beq", "bne", "beqz", "bnez", "bgez", "bgtz",
               "blez", "bltz", "bgezal", "bltzal", "beql", "bnel", "beqzl", "bnezl", "bgezl", "bgtzl", "blezl",
               "bltzl"}
BRANCH_OPS = READS_FIRST - {"sb", "sh", "sw", "swl", "swr", "swc1", "swc2", "mtc0", "mtc1", "mtc2", "ctc0",
                            "ctc1", "ctc2", "mult", "multu", "div", "divu"}
WINDOW = 12   # instructions after a call in which the result is looked for


def result_read_after_call(lines):
    """True when the instructions after a `jal` (delay slot skipped: it runs before the callee) read
    $v0 before anything writes it, in straight-line code. `lines`: the asm lines after the jal."""
    insns = []
    for line in lines:
        if ASM_LABEL_RE.match(line):
            insns.append(None)
            continue
        m = ASM_INSN_RE.match(line)
        if m:
            insns.append((m.group(1), re.findall(r"\$(\w+)", m.group(2))))
    for item in insns[1:WINDOW]:
        if item is None:
            return False                  # another path joins here: not sure
        op, regs = item
        if op in ("jal", "jalr"):
            return op == "jalr" and "v0" in regs
        if op in READS_FIRST:
            if "v0" in regs:
                return True
            if op in BRANCH_OPS:
                return False
            continue
        if "v0" in regs[1:]:
            return True
        if regs[:1] == ["v0"]:
            return False
    return False


def asm_result_users(root):
    """{(unit, callee): (calls, calls whose result is read, example caller file)} over the splat asm
    (asm/**/*.s without data). Sequential reads: tools/queue.py shadows the standard queue module
    here, so no thread pool."""
    out = {}
    base = os.path.join(root, "asm")
    for dirpath, dirs, files in os.walk(base):
        dirs[:] = [d for d in dirs if d != "data"]
        rel = os.path.relpath(dirpath, base).replace(os.sep, "/")
        m = re.match(r"ovl/([^/]+)", rel)
        unit = m.group(1) if m else "main"
        for f in sorted(files):
            if not f.endswith(".s"):
                continue
            text = _read(os.path.join(dirpath, f))
            if "jal" not in text:
                continue
            body = text.split("\n")
            starts = [m.start() for m in JAL_RE.finditer(text)]
            if not starts:
                continue
            index, pos = {}, 0
            for n, line in enumerate(body):
                index[pos] = n
                pos += len(line) + 1
            for m in JAL_RE.finditer(text):
                n = index.get(text.rfind("\n", 0, m.start()) + 1)
                if n is None:
                    continue
                rec = out.get((unit, m.group(1)), (0, 0, ""))
                used = result_read_after_call(body[n + 1:n + 1 + WINDOW + 2])
                out[(unit, m.group(1))] = (rec[0] + 1, rec[1] + used, rec[2] or (f if used else ""))
    return out


def unit_types(model, unit):
    """{symbol: effective type} a C file of `unit` sees from its root header."""
    root = "game.h" if unit == "main" else "ovl/%s.h" % unit
    if root not in model.headers:
        return {}
    seen = defaultdict(list)
    for name, typ, _f, _raw in ch.walk(model.headers[root], model.inc, rel=root):
        seen[name].append(typ)
    return {n: effective(ts) for n, ts in seen.items()}


def load_renames(root):
    """{new name: address} of config/obin_renames.txt (`old new`, old = func_/D_<address>)."""
    out = {}
    path = os.path.join(root, "config", "obin_renames.txt")
    if os.path.exists(path):
        for line in _read(path).splitlines():
            parts = line.split()
            if len(parts) == 2 and not line.lstrip().startswith("#"):
                m = ADDR_RE.match(parts[0])
                if m:
                    out[parts[1]] = int(m.group(1), 16)
    return out


def disagree(a, b):
    """True when two views of one address differ in a way the call code or the data shows."""
    return a != b and not benign(a, b)


def alias_conflicts(model):
    """[(key, message)]: an address declared under several names whose types disagree."""
    renames = load_renames(model.root)
    groups = defaultdict(set)   # (namespace, address) -> {(name, type, header)}
    for h, ds in model.own.items():
        if h in SDK_HEADERS:
            continue
        for name, typ, _raw in ds:
            a = model.addr(name)
            if a is None:
                a = renames.get(name)
            if a is None:
                continue
            ns = "main" if model.is_main(name, h) or model.owner(h) is None else model.owner(h)
            groups[(ns, a)].add((name, typ, h))
    out = []
    for (ns, a), views in sorted(groups.items(), key=lambda kv: (kv[0][1], kv[0][0])):
        names = {n for n, _t, _h in views}
        if len(names) < 2:
            continue
        bad = sorted((x, y) for x in views for y in views if x[0] < y[0] and disagree(x[1], y[1]))
        if not bad:
            continue
        listing = "; ".join("%s '%s' (include/%s)" % (n, short(t), h) for n, t, h in sorted(views))
        out.append(("alias 0x%08X" % a,
                    "alias conflict at 0x%08X (%s): %s. One symbol has one declaration, under its current "
                    "name (config/obin_renames.txt, config/symbol_addrs*.txt): delete the other, or declare "
                    "the current name with the right type" % (a, ns, listing)))
    return out


def void_result_conflicts(model):
    """[(key, message)]: functions declared `void` whose result some asm caller reads."""
    if not os.path.isdir(os.path.join(model.root, "asm")):
        return []
    users = asm_result_users(model.root)
    types, out = {}, []
    for (unit, sym), (calls, used, example) in sorted(users.items()):
        if not used:
            continue
        if unit not in types:
            types[unit] = unit_types(model, unit)
        typ = types[unit].get(sym)
        if typ and is_func(typ) and typ.split("F(", 1)[0] == "void":
            out.append(("void-result %s:%s" % (unit, sym),
                        "%s: %s is declared void but %d of %d asm calls read $v0 afterwards (e.g. in %s): the C "
                        "of those callers will need its return value. Declare the return type they use in "
                        "include/main_api.h (or the overlay header), or keep void if matched callers need it and "
                        "ask the orchestrator to list the finding in %s"
                        % (unit, sym, used, calls, example, KNOWN)))
    return out


def read_known(root):
    path = os.path.join(root, KNOWN)
    out = set()
    if os.path.exists(path):
        for line in _read(path).splitlines():
            line = line.split("#", 1)[0].strip()
            if line:
                out.add(line)
    return out


def check_branch(inc, src, out=sys.stdout):
    """Run the branch guard; returns the number of new findings (see the module docstring)."""
    model = Model(inc, src)
    known = read_known(model.root)
    findings = [("headers", p) for p in ch.check(inc, src)]
    findings += alias_conflicts(model)
    findings += void_result_conflicts(model)
    new = [(k, m) for k, m in findings if k not in known or k == "headers"]
    tolerated = [k for k, _m in findings if k in known and k != "headers"]
    for _k, m in new:
        out.write(m + "\n")
    stale = sorted(known - {k for k, _m in findings})
    for k in stale:
        out.write("note: %s is listed in %s but no longer found; delete the line\n" % (k, KNOWN))
    out.write("%d new disagreement(s), %d known (%s)\n" % (len(new), len(tolerated), KNOWN))
    return len(new)


def update_known(inc, src, out=sys.stdout):
    model = Model(inc, src)
    findings = alias_conflicts(model) + void_result_conflicts(model)
    path = os.path.join(model.root, KNOWN)
    lines = ["# Findings of `tools/sync_protos.py --check-branch` that exist on the base tree (T-5030).",
             "# Orchestrator only: a branch never edits this file (it would conflict at every merge).",
             "# One finding key per line; delete a line when the disagreement is fixed."]
    lines += sorted({k for k, _m in findings})
    os.makedirs(os.path.dirname(path), exist_ok=True)
    _write(path, "\n".join(lines) + "\n")
    out.write("%s: %d finding(s)\n" % (KNOWN, len(findings)))


# ---------------------------------------------------------------------------------------------
# snapshot and compare

def snapshot(model):
    views = unit_views(model)
    return {os.path.relpath(p, model.root): v for p, v in sorted(views.items())}


def view_changes(old, new):
    """[(unit, symbol, kind, old type, new type, class)] for the views that differ. Class:
    benign (the same call code), check (an implicit `int f()` became a prototype with wide
    parameters but another return type: the build decides, IDO allocates registers differently
    for void and int calls), risky (narrow parameters, other types)."""
    out = []
    for unit in sorted(set(old) | set(new)):
        a, b = old.get(unit, {}), new.get(unit, {})
        for name in sorted(set(a) | set(b)):
            ta, tb = a.get(name), b.get(name)
            if ta == tb:
                continue
            if ta is None or tb is None:
                kind = "now declared (was implicit)" if ta is None else "no longer declared"
                if ta is None and is_func(tb) and not narrow_params(tb):
                    cls = "benign" if tb.split("F(", 1)[0] == "s32" else "check"
                else:
                    cls = "risky"
            else:
                kind = "type changed"
                cls = "benign" if benign(ta, tb) else "risky"
            out.append((unit, name, kind, ta, tb, cls))
    return out


def compare(old, new, out=sys.stdout):
    """Print the symbols whose view changed; returns (benign, check, risky) counts."""
    changes = view_changes(old, new)
    for unit, name, kind, ta, tb, cls in changes:
        if cls != "benign":
            out.write("%-6s %s %s: %s %s -> %s\n" % (cls.upper(), unit, name, kind,
                                                      short(ta) if ta else "-", short(tb) if tb else "-"))
    n = {c: sum(1 for x in changes if x[5] == c) for c in ("benign", "check", "risky")}
    out.write("view changes: %(benign)d benign (K&R against a prototype with wide parameters, implicit int against s32), "
              "%(check)d to confirm by build (implicit int against another return type), %(risky)d risky\n" % n)
    return n["benign"], n["check"], n["risky"]


def add_implicit_overrides(model, changes, log=print):
    """Mark the units that relied on an implicit declaration of a function that main_api.h now
    prototypes with narrow parameters: an `implicit` override in the overlay header (or the .c file)."""
    todo = defaultdict(set)   # file -> symbols
    for unit, name, kind, ta, tb, cls in changes:
        if cls != "risky" or ta is not None or not tb or not is_func(tb):
            continue
        m = re.match(r"src/ovl/(\w+)/", unit)
        todo[("ovl/%s.h" % m.group(1)) if m else unit].add(name)
    for f, names in sorted(todo.items()):
        in_header = f in model.headers
        path = model.headers[f] if in_header else os.path.join(model.root, f)
        text = _read(path)
        existing = set(model.overrides.get(f if in_header else path, {}))
        defs = ["#define %s%s /* implicit declaration, as matched; main_api.h has a prototype with narrow parameters */"
                % (OVERRIDE, n) for n in sorted(names - existing, key=lambda n: sort_key(model, n))]
        if not defs:
            continue
        lines = text.split("\n")
        marks = [i for i, l in enumerate(lines) if re.match(r"\s*#\s*define\s+" + OVERRIDE, l)]
        if marks:
            at = marks[-1] + 1
        elif in_header:
            at = next((i + 1 for i, l in enumerate(lines) if re.match(r"\s*#\s*define\s+\w+_H\b", l)), 0)
            defs = ["", "/* main_api.h overrides (T-3340, tools/sync_protos.py): the views this overlay was matched with. */"] + defs
        else:
            at = next((i for i, l in enumerate(lines) if re.match(r"\s*#\s*include", l)), 0)
            defs = ["/* main_api.h overrides (T-3340, tools/sync_protos.py): the views this file was matched with. */"] + defs + [""]
        lines[at:at] = defs
        _write(path, "\n".join(lines))
        log("  %s: implicit override for %s" % (f, ", ".join(sorted(names - existing))))


# ---------------------------------------------------------------------------------------------

def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("--inc", default="include")
    ap.add_argument("--src", default="src")
    g = ap.add_mutually_exclusive_group()
    g.add_argument("--report", action="store_true", help="list main-exe symbols and conflicting views (default)")
    g.add_argument("--check", action="store_true", help="enforce the main_api.h rules")
    g.add_argument("--write", action="store_true", help="create or update include/main_api.h")
    g.add_argument("--fix", action="store_true", help="--write, then clean the other headers")
    g.add_argument("--prune", action="store_true", help="drop the overrides the build does not need (runs ninja)")
    g.add_argument("--snapshot", metavar="FILE", help="save the view of every symbol per .c file")
    g.add_argument("--compare", metavar="FILE", help="compare the current views with a snapshot")
    g.add_argument("--check-branch", action="store_true",
                   help="before finishing: header rules plus alias and void-result disagreements (T-5030)")
    g.add_argument("--update-known", action="store_true", help="rewrite %s from the current findings" % KNOWN)
    a = ap.parse_args(argv)
    if a.check_branch:
        n = check_branch(a.inc, a.src)
        print("branch check FAILED" if n else "branch check OK")
        return 1 if n else 0
    if a.update_known:
        update_known(a.inc, a.src)
        return 0
    model = Model(a.inc, a.src)
    if a.check:
        problems = check_api(a.inc, a.src)
        for p in sorted(set(problems)):
            print(p)
        print("%d main_api problem(s)" % len(set(problems)) if problems else "main_api OK")
        return 1 if problems else 0
    if a.prune:
        if not os.path.exists("build.ninja"):
            print("run python3 configure.py && ninja first (--prune builds each unit)")
            return 1
        removed, kept = prune(model)
        write_api(Model(a.inc, a.src), plan(Model(a.inc, a.src)))
        print("overrides removed %d, kept %d" % (len(removed), len(kept)))
        return 0
    if a.snapshot:
        with open(a.snapshot, "w") as fh:
            json.dump(snapshot(model), fh, indent=0, sort_keys=True)
        print("snapshot of %d units written to %s" % (len(model.sources), a.snapshot))
        return 0
    if a.compare:
        with open(a.compare) as fh:
            old = json.load(fh)
        _b, _c, risky = compare(old, snapshot(model))
        return 1 if risky else 0
    if a.write or a.fix:
        before = snapshot(model) if a.fix else None
        plan_ = plan(model)
        changed, n, g = write_api(model, plan_)
        print("%s: %d symbols" % (os.path.join(a.inc, API), n))
        if a.fix:
            model = Model(a.inc, a.src)
            plan_ = plan(model)
            for h in sorted(model.headers):
                if h != API and h not in SDK_HEADERS:
                    fix_header(model, plan_, h)
            model = Model(a.inc, a.src)
            write_api(model, plan(model))
            model = Model(a.inc, a.src)
            add_implicit_overrides(model, view_changes(before, snapshot(model)))
            model = Model(a.inc, a.src)
        _c, n, g = write_api(model, plan(model))
        print("%s: %d symbols, %d guarded" % (os.path.join(a.inc, API), n, g))
        if a.fix:
            _b, _c, risky = compare(before, snapshot(Model(a.inc, a.src)))
            return 1 if risky else 0
        return 0
    report(model)
    return 0


if __name__ == "__main__":
    sys.exit(main())
