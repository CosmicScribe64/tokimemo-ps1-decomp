#!/usr/bin/env python3
"""Redraft unmatched functions with the recovered types and score every draft like the census (T-20800).

One tool for what 21 per-unit drivers (drafts/T-16550 .. T-18800/redraft.py) did by copy: for each target
function it runs m2c with the current headers (several variants), applies the type-driven rewrites of
tools/redraft_rewrites.py and tools/redraft_extras.py, compiles each draft as the whole translation unit and
scores it with tools/exhaustive_drafts.py's process() (m2c, bulk cleanup, cheap declaration fixes,
order_search's resolved diff), keeps the best, tries the tools/draft_fixes.py rewrites on it, and writes

    drafts/<TICKET>/<UNIT>/<func>/{body.c, context.c, declarations.h, result.json}
    drafts/<TICKET>/catalog.jsonl

result.json: scope, score (diff rows of the function in the full unit; 0 is a candidate, not a verified
match), census_score (the T-14500 / T-16510 best), variant, rewrites, variant_scores, provenance (tool,
ticket, git head, header hashes, archived source), best_archived_score (the best score any archive holds).

Run in Docker from the repo root (tools/docker.sh python3 tools/redraft.py ...):
    redraft.py --ticket T-20810 UNIT:func [UNIT:func ...]     the named functions (a bare name: every unit)
    redraft.py --ticket T-20810 --unit GEKO [--unit DATE]     every unmatched function of the units
    redraft.py --ticket T-20810 --all-unmatched --jobs 2      the whole tree, TT excluded
    redraft.py --list ...                                      print the targets and stop
    redraft.py --unit GEKO --rewrite BODY.c                    print the rewrites of one body and stop

Variants (--variants a,b,c or all; default typed,typed-late,plain,archived,guarded):
  plain        m2c with the current headers, nothing else (what the census did).
  typed        the asm is folded first (a label inside a typed aggregate becomes base+offset; every %hi takes the
               bare base, T-16550; the %hi of an indexed access keeps its offset, T-17000), the record views are
               typed in the m2c context, then the PRE and POST rewrites run. The fold of 10 of the 21 drivers.
  typed-late   the fold runs after tools/m2c.py's fix_shared_hi and every %hi keeps its offset (T-17700; the fold
               of GEKO, NAME_ENT, SHUGAKU, EN_NICHI, ENDING, KANGEI, MASTER and BUNKASAI).
  typed-keep   (opt-in) typed, and a %hi whose %lo uses all name its own symbol keeps its offset (T-17300, ETC).
  typed-named  (opt-in) typed, but only labels at a named field (T-17100).
  typed-pass1  (opt-in) typed without stack-array typing in work-prim (T-17300 typed1).
  archived     the best archived body (drafts/*, lowest score; hand.c where its score is better) through the
               same rewrites and the cleanup; archived-raw is the same body untouched (the rescoring of today).
  guarded      the NON_MATCHING body in src, scored as it is (only for guarded functions).
The best score wins; ties go to the variant earlier in that list. Then draft_fixes.best_rewrite and the
GameState-local form (gs-local) are tried on it. Nothing under src/ or include/ is edited.

TT is skipped unless --include-tt (a contributor's overlay; `--skip-unit U` adds more). The run is resumable:
a variant with a result in the work directory (build/redraft/<ticket>, --work) is not run again, a function
with a result in drafts/<ticket>/ is not finished again (--force redoes it). --jobs N bounds the compile jobs.
"""
import argparse
import hashlib
import json
import os
import re
import shutil
import signal
import subprocess
import sys
import time
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import redraft_rewrites as rw  # noqa: E402

BIG = 10 ** 6
VARIANTS = {
    "plain": {},
    "typed": {"fold": "bare", "rewrites": True},
    "typed-late": {"fold": "late", "rewrites": True},
    "typed-keep": {"fold": "keep", "rewrites": True},
    "typed-named": {"fold": "named", "rewrites": True},
    "typed-pass1": {"fold": "bare", "rewrites": True, "flags": ("pass1",)},
    "archived": {"source": "archived", "rewrites": True},
    "archived-raw": {"source": "archived"},
    "guarded": {"source": "guarded"},
}
DEFAULT_VARIANTS = ("typed", "typed-late", "plain", "archived", "guarded")
SKIP_UNITS = ("TT",)
CENSUS = "drafts/T-14500/best-quality-catalog.jsonl"
T16510 = "drafts/T-16510/results.jsonl"
ARCHIVE_TICKETS = re.compile(r"^T-1[5-9]\d{3}$|^T-2\d{4}$")


def digest(text):
    return hashlib.sha256(text.encode()).hexdigest()


def git_head():
    """HEAD from git, else $TOKIMEMO_GIT_HEAD (a worktree's .git file points outside the Docker mount)."""
    try:
        got = subprocess.run(["git", "rev-parse", "HEAD"], capture_output=True, text=True)
    except OSError:
        return os.environ.get("TOKIMEMO_GIT_HEAD", "")
    return got.stdout.strip() if got.returncode == 0 else os.environ.get("TOKIMEMO_GIT_HEAD", "")


# ------------------------------------------------------------------------------------------ targets

def split_target(arg):
    """(unit or None, func) of `UNIT:func` or a bare function name."""
    if ":" in arg:
        unit, func = arg.split(":", 1)
        return unit, func
    return None, arg


def select_targets(rows, targets=(), units=(), all_unmatched=False, skip_units=SKIP_UNITS):
    """(selected rows, messages): the census rows named by `targets`, in `units`, or all; units in
    `skip_units` are left out, and a target inside one of them is reported, not run."""
    wanted = [split_target(t) for t in targets]
    chosen, messages, seen = [], [], set()
    for row in rows:
        hit = (all_unmatched or row["unit"] in units
               or any(f == row["func"] and (u is None or u == row["unit"]) for u, f in wanted))
        if not hit or row["scope"] in seen:
            continue
        if row["unit"] in skip_units:
            if row["unit"] in units or any(f == row["func"] and u == row["unit"] for u, f in wanted):
                messages.append("%s: unit %s is skipped (--include-tt or --skip-unit to change)"
                                % (row["scope"], row["unit"]))
            continue
        seen.add(row["scope"])
        chosen.append(row)
    for unit, func in wanted:
        if not any(r["func"] == func and (unit is None or r["unit"] == unit) for r in rows):
            messages.append("%s: not an unmatched function (no INCLUDE_ASM line)" % (func if unit is None else "%s:%s" % (unit, func)))
    return chosen, messages


# ------------------------------------------------------------------------------------------ archives

class Archive:
    """The drafts earlier passes archived: scope -> the best (lowest score, newest ticket) body, and the census
    score of T-14500 / T-16510 that every pass compared with."""

    def __init__(self, root=".", exclude=()):
        self.root, self.exclude = Path(root), set(exclude)    # exclude: tickets whose folders are not archives
        self.census, self.best = {}, {}
        self._load()

    def offer(self, scope, score, ticket, folder, name="body.c"):
        if score is None or not (folder / name).exists():
            return
        key = (score, -int(ticket[2:]) if ticket[2:].isdigit() else 0)
        cur = self.best.get(scope)
        if cur is None or key < cur["key"]:
            self.best[scope] = dict(key=key, score=score, path=str(folder), file=name, ticket=ticket)

    def _load(self):
        cat = self.root / CENSUS
        if cat.exists():
            for line in cat.read_text().splitlines():
                r = json.loads(line)
                self.census[r["scope"]] = dict(score=r["score"], path=r["selected_path"])
                self.offer(r["scope"], r["score"], "T-14500", self.root / r["selected_path"])
        t16510 = self.root / T16510
        if t16510.exists():
            for line in t16510.read_text().splitlines():
                r = json.loads(line)
                if r.get("score") is not None and r.get("status") in ("improved", "zero"):
                    if r["score"] < (self.census.get(r["scope"], {}).get("score") or BIG):
                        self.census[r["scope"]] = dict(score=r["score"], path="drafts/T-16510/%s/%s" % (r["unit"], r["func"]))
                    self.offer(r["scope"], r["score"], "T-16510", self.root / "drafts/T-16510" / r["unit"] / r["func"])
        for res in sorted((self.root / "drafts").glob("T-*/*/*/result.json")):
            ticket, unit, func = res.parts[-4], res.parts[-3], res.parts[-2]
            if not ARCHIVE_TICKETS.match(ticket) or ticket in self.exclude or unit.startswith(("_", ".")):
                continue
            try:
                r = json.loads(res.read_text())
            except ValueError:
                continue
            scope = r.get("scope") or "%s:%s" % (unit, func)
            if scope != "%s:%s" % (unit, func):
                continue
            self.offer(scope, r.get("score"), ticket, res.parent)
            self.offer(scope, r.get("hand_score"), ticket, res.parent, "hand.c")

    def census_of(self, scope):
        return self.census.get(scope, {})

    def best_of(self, scope):
        return self.best.get(scope)


# ------------------------------------------------------------------------------------------ the worker

def keep_errors():
    """funcdiff_verify keeps the last 12 lines of a failed compile; cfe warnings (three lines each) can push
    the error out, and with it the "redeclaration" line exhaustive_drafts repairs on. Drop the warnings (T-17300)."""
    import funcdiff_verify as fv

    def command(args):
        result = subprocess.run(args, capture_output=True, text=True)
        if result.returncode:
            keep, skip = [], 0
            for line in (result.stderr or result.stdout).splitlines():
                if line.startswith("cfe: Warning"):
                    skip = 2
                    continue
                if skip:
                    skip -= 1
                    continue
                keep.append(line)
            raise ValueError("%s failed:\n%s" % (args[0], "\n".join(keep[-12:])))
        return result.stdout
    fv.command = command


def install_compile_cache(directory):
    """Cache the compiled object of every translation unit text by hash (T-20800): two variants that end in the
    same draft (a function that touches no folded label gives the same C in plain and typed) share one IDO run.
    A compile error is cached too, with its message, so the cheap-fix loop sees the same text. The key holds
    the hash of all headers, so a header change misses."""
    import order_search
    directory = Path(directory)
    directory.mkdir(parents=True, exist_ok=True)
    heads = sorted(Path("include").rglob("*.h"))
    env_hash = digest("".join(h.read_text(errors="replace") for h in heads))
    orig = order_search.compile_variant

    def cached(c, text, work, non_matching):
        key = digest("\0".join((env_hash, str(c.src), "nm" if non_matching else "", text)))
        obj, err = directory / (key + ".o"), directory / (key + ".err")
        work.mkdir(parents=True, exist_ok=True)
        if obj.exists():
            dest = work / "guarded.o"
            shutil.copyfile(obj, dest)
            return str(dest)
        if err.exists():
            raise ValueError(err.read_text())
        try:
            built = orig(c, text, work, non_matching)
        except ValueError as exc:
            tmp = directory / ("%s.tmp%d" % (key, os.getpid()))
            tmp.write_text(str(exc))
            os.replace(tmp, err)
            raise
        tmp = directory / ("%s.tmp%d" % (key, os.getpid()))
        shutil.copyfile(built, tmp)
        os.replace(tmp, obj)
        return built
    order_search.compile_variant = cached


def archived_source(env, folder, name):
    """The text run_m2c returns for an archived body: its declarations (minus what the headers declare now,
    and local typedefs moved into the body, T-17500) and the body."""
    import migrate_globals as mg
    folder = Path(folder)
    decls = (folder / "declarations.h").read_text() if (folder / "declarations.h").exists() else ""
    declared = set(mg.declarations(env.decl_text))
    # the census declared some table globals itself; the header now has their real type
    decls = "\n".join(l for l in decls.splitlines() if not (set(re.findall(r"\bD_[0-9A-F]{8}\b", l)) & declared))
    body = (folder / name).read_text()
    local = [l for l in decls.splitlines() if l.startswith("typedef")]
    names = [m.group(1) for l in local for m in [re.search(r"\}\s*(\w+)\s*;", l)] if m]
    local += [l for l in decls.splitlines() if any(re.match(r"extern %s\b" % n, l) for n in names)]
    if local:
        decls = "\n".join(l for l in decls.splitlines() if l not in local)
        body = re.sub(r"\{\n", lambda m: "{\n" + "".join("    %s\n" % l for l in local), body, count=1)
    return decls + "\n" + body


def install(env, spec, applied, ctx, archived=None):
    """Patch m2c and bulk_m2c for one variant: the fold and the typed context, the PRE rewrites on m2c's
    text, the POST rewrites after the cleanup, an archived body in place of m2c's output."""
    import bulk_m2c as bulk
    import m2c
    if spec.get("source") == "archived":
        folder, name = archived
        m2c.run_m2c = lambda root, scope, *a, **k: archived_source(env, folder, name)
    if spec.get("fold"):
        orig_hi = m2c.fix_shared_hi
        if spec["fold"] == "late":
            m2c.fix_shared_hi = lambda t: rw.fold(env, orig_hi(t), "late")
        else:
            m2c.fix_shared_hi = lambda t: orig_hi(rw.fold(env, t, spec["fold"]))
        orig_ctx = m2c.build_context

        def build_context(root, heads, out_path, *a, **k):
            orig_ctx(root, heads, out_path, *a, **k)
            with open(out_path) as f:
                text = f.read()
            with open(out_path, "w") as f:
                f.write(env.context(text))
        m2c.build_context = build_context
    if spec.get("rewrites"):
        orig_split = m2c.split_draft

        def split_draft(text, name):
            decls, body = orig_split(text, name)
            return decls, rw.pre_steps(env, body, applied)
        m2c.split_draft = split_draft
        plain_cleanup = bulk.cleanup

        def cleanup(decls, body, glob=None):
            body, known, problem = plain_cleanup(decls, body, glob)
            if problem:
                return body, known, problem
            body, known = rw.apply_post(env, body, known, ctx, applied)
            return body, known, None
        bulk.cleanup = cleanup


def read_inventory(work):
    return {r["scope"]: r for r in json.loads((Path(work) / "inventory.json").read_text())}


def prune_context(folder, spec):
    """Drop the full unit text of a variant run: finish() rebuilds it from body.c and declarations.h. A guarded
    body lives in src/ and cannot be inserted again, so its unit text stays."""
    if spec.get("source") != "guarded":
        (Path(folder) / "context.c").unlink(missing_ok=True)


def run_worker(scope, variant, work, archived):
    """One (function, variant): the census path of exhaustive_drafts.process() with the variant installed;
    result.json lands in <work>/runs/<variant>/<UNIT>/<func>/."""
    import exhaustive_drafts as ex
    import m2c
    row = {k: v for k, v in read_inventory(work)[scope].items() if k not in ("census", "archived")}
    spec = VARIANTS[variant]
    ex.BASE = Path(work) / "runs" / variant
    if spec.get("source") != "guarded":
        os.environ["TOKIMEMO_REGENERATE_GUARDS"] = "1"
    keep_errors()
    install_compile_cache(Path(work) / "objcache")
    applied = []
    env = rw.Env(row["unit"])
    env.flags.update(spec.get("flags", ()))
    asm_text = None
    try:
        asm_path, _overlay = m2c.locate(".", scope)
        asm_text = Path(asm_path).read_text(errors="replace")
    except (LookupError, OSError):
        pass
    arch = None
    if spec.get("source") == "archived":
        folder, _, name = (archived or "").partition("|")
        arch = (folder, name or "body.c")
    install(env, spec, applied, {"row": row, "asm": asm_text}, arch)
    got = ex.process(row)
    got["rewrites"] = sorted(set(applied))
    got["redraft_variant"] = variant
    folder = ex.BASE / row["unit"] / row["func"]
    ex.save(folder / "result.json", json.dumps(got, sort_keys=True) + "\n")
    prune_context(folder, spec)


# ------------------------------------------------------------------------------------------ finishing

def choose_variant(scored, order):
    """Name of the best variant: lowest score, then earliest in `order`."""
    return min(scored, key=lambda v: (scored[v], order.index(v) if v in order else len(order)))


def header_hashes(unit):
    heads = list(rw.TYPE_HEADERS) + (["include/ovl/%s.h" % unit] if unit != "main" else [])
    out = {}
    for h in heads:
        if os.path.exists(h):
            out[h] = digest(Path(h).read_text(errors="replace"))
    return out


def harness_hash():
    here = Path(__file__).resolve().parent
    return digest("".join((here / n).read_text() for n in ("redraft.py", "redraft_rewrites.py", "redraft_extras.py")))


def result_record(row, ticket, variant_scores, chosen, rewrites, census, best, fixes=None, extra=None,
                  score=None, name=None):
    """The result.json of a finished function. `chosen` is the variant whose draft it is; `score` and `name`
    default to its score and its name and differ after draft_fixes or gs-local."""
    if score is None and chosen:
        score = variant_scores.get(chosen)
    record = dict(scope=row["scope"], unit=row["unit"], func=row["func"], src=row["src"], size=row["size"],
                  guarded=row["guarded"], ticket=ticket, score=score, variant=name or chosen,
                  census_score=census.get("score"), census_path=census.get("path"),
                  best_archived_score=best["score"] if best else None, best_archived_path=best["path"] if best else None,
                  variant_scores=variant_scores, rewrites=rewrites, draft_fixes=fixes or [],
                  created=time.strftime("%Y-%m-%d", time.gmtime()))
    record["status"] = "failed" if score is None else "candidate-zero" if score == 0 else "scored"

    def versus(other):
        return None if score is None or other is None else "better" if score < other else "same" if score == other else "worse"
    record["vs_census"] = versus(record["census_score"])
    record["vs_archived"] = versus(record["best_archived_score"])
    record["provenance"] = dict(tool="tools/redraft.py (T-20800)", ticket=ticket, git=git_head(),
                                harness_hash=harness_hash(), headers=header_hashes(row["unit"]),
                                archived=best, **(extra or {}))
    return record


def unit_text(row, body, folder, guard_body=False):
    """The whole source file of `row` with `body` as the definition, as exhaustive_drafts.process() scored it:
    in place of the function's INCLUDE_ASM line or, for a guarded function, of its NON_MATCHING block.
    `guard_body`: `body` is the block's own definition (the guarded variant); otherwise the block is first
    reduced to its asm line. None when the file has no place for the body."""
    import autotweak
    import bulk_m2c as bulk
    func = row["func"]
    src = Path(row["src"]).read_text(errors="replace")
    guard = next((g for g in autotweak.guards(src) if g.func == func), None)
    if guard is not None:
        if guard_body:
            return autotweak.rewrite_guard(src, func, body)
        include = re.search(bulk.INCLUDE_ASM_TMPL % re.escape(func), src[guard.else_end:guard.end], re.M)
        if include is None:
            return None
        src = autotweak.rewrite_guard(src, func, include.group(0))
        if src is None:
            return None
    decls = Path(folder) / "declarations.h"
    lines = [l for l in decls.read_text(errors="replace").splitlines() if l.strip()] if decls.exists() else []
    placed = bulk.insert_draft(src, func, body, lines)
    return autotweak.rewrite_guard(placed, func, body) if placed else None


def write_draft(dest, body, context, declarations, record):
    """The result folder of one function; result.json goes last, so it is the mark of a finished function."""
    dest.mkdir(parents=True, exist_ok=True)
    (dest / "body.c").write_text(body)
    (dest / "context.c").write_text(context)
    if declarations is not None and Path(declarations).exists():
        shutil.copy(declarations, dest / "declarations.h")
    record["draft_hash"], record["context_hash"] = digest(body), digest(context)
    tmp = dest / "result.json.tmp"
    tmp.write_text(json.dumps(record, sort_keys=True) + "\n")
    os.replace(tmp, dest / "result.json")


def finish(scope, work, out, ticket, variants, fixes=True):
    """Pick the best variant of one function and write the result folder; then try draft_fixes and gs-local on
    it and rewrite the folder if one is better (the first write stands if the search is cut short)."""
    import draft_fixes
    import m2c
    install_compile_cache(Path(work) / "objcache")
    row = read_inventory(work)[scope]
    runs, scores = {}, {}
    for v in variants:
        res = Path(work) / "runs" / v / row["unit"] / row["func"] / "result.json"
        if res.exists():
            runs[v] = (json.loads(res.read_text()), res.parent)
            if runs[v][0].get("score") is not None:
                scores[v] = runs[v][0]["score"]
    dest = Path(out) / row["unit"] / row["func"]
    census, best = row.get("census") or {}, row.get("archived")
    variant_scores = {v: runs[v][0].get("score") for v in runs}
    if not scores:
        reason = " | ".join("%s: %s" % (v, (r[0].get("reason") or r[0].get("status") or "")[:120]) for v, r in runs.items())
        record = result_record(row, ticket, variant_scores, None, [], census, best, extra={"reason": reason[:400]})
        for v in ("typed", "typed-late", "plain", "archived"):
            if v in runs and (runs[v][1] / "body.c").exists():
                write_draft(dest, (runs[v][1] / "body.c").read_text(), "", runs[v][1] / "declarations.h", record)
                (dest / "context.c").unlink()
                return record
        dest.mkdir(parents=True, exist_ok=True)
        (dest / "result.json").write_text(json.dumps(record, sort_keys=True) + "\n")
        return record
    chosen = choose_variant(scores, list(variants))
    got, folder = runs[chosen]
    body = (folder / "body.c").read_text()
    context = unit_text(row, body, folder, VARIANTS[chosen].get("source") == "guarded")
    if context is None:
        context = (folder / "context.c").read_text() if (folder / "context.c").exists() else ""
    rewrites = got.get("rewrites", [])
    record = result_record(row, ticket, variant_scores, chosen, rewrites, census, best)
    write_draft(dest, body, context, folder / "declarations.h", record)
    score = scores[chosen]
    if score == 0 or not fixes or not context:
        return record
    scorer = draft_fixes.make_scorer(dict(row))
    files = {}
    try:
        files[(row["unit"], row["func"])] = m2c.locate(".", scope)[0]
    except (LookupError, OSError):
        pass
    scratch = Path(work) / "fixes" / row["unit"] / row["func"]
    _base, new_score, picked, new_body, new_context, tried = draft_fixes.best_rewrite(
        row, body, draft_fixes.asm_for(scope, files), context, scorer, scratch)
    name, chosen_fixes = chosen, []
    if new_score < score:
        body, context, score, chosen_fixes = new_body, new_context, new_score, ["%s#%d" % c for c in picked]
        name = chosen + "+draft-fixes"
    if score > 0 and "D_800E6280" in body:
        gbody = rw.gs_local(body)
        if gbody != body and body in context:
            work_dir = scratch / "gs"
            work_dir.mkdir(parents=True, exist_ok=True)
            gcontext = context.replace(body, gbody, 1)
            gscore = scorer(gcontext, work_dir)
            variant_scores["gs-local"] = gscore
            if gscore < score:
                body, context, score = gbody, gcontext, gscore
                name += "+gs-local"
    shutil.rmtree(scratch, ignore_errors=True)
    if name != chosen:
        record = result_record(row, ticket, variant_scores, chosen, rewrites, census, best, chosen_fixes,
                               extra={"draft_fixes_tried": tried}, score=score, name=name)
        write_draft(dest, body, context, folder / "declarations.h", record)
    else:
        record["variant_scores"] = variant_scores
        record["provenance"]["draft_fixes_tried"] = tried
        (dest / "result.json").write_text(json.dumps(record, sort_keys=True) + "\n")
    return record


# ------------------------------------------------------------------------------------------ scheduling

class Task:
    """A subprocess with a result file; `after` names tasks that must have finished first."""

    def __init__(self, name, cmd, result, timeout, after=(), on_fail=None, log=None):
        self.name, self.cmd, self.result, self.timeout = name, cmd, Path(result), timeout
        self.after, self.on_fail = set(after), on_fail
        self.log = Path(log) if log else self.result.parent / (name.replace("/", "_").replace(":", "_") + ".log")


def run_pool(tasks, jobs, log=print, poll=0.2):
    """Run the tasks, at most `jobs` at a time, respecting `after`. A task that ends without its result file
    gets `on_fail(reason)` called (it writes a failure result, so a resumed run does not retry it forever)."""
    pending = [t for t in tasks if not t.result.exists()]
    done = {t.name for t in tasks if t.result.exists()}
    running = []
    try:
        _pool_loop(pending, done, running, jobs, log, poll)
    except BaseException:           # an interrupt must not leave compiles running in the background
        for t, proc, _t0, logf in running:
            try:
                os.killpg(proc.pid, signal.SIGKILL)
            except OSError:
                pass
            logf.close()
        raise


def _pool_loop(pending, done, running, jobs, log, poll):
    while pending or running:
        for t in list(pending):
            if len(running) >= jobs:
                break
            if t.after <= done:
                pending.remove(t)
                t.log.parent.mkdir(parents=True, exist_ok=True)
                logf = open(t.log, "w")
                proc = subprocess.Popen(t.cmd, stdout=logf, stderr=subprocess.STDOUT, start_new_session=True)
                running.append((t, proc, time.monotonic(), logf))
        if not running and pending and not any(t.after <= done for t in pending):
            raise RuntimeError("tasks wait for tasks that never run: %s" % ", ".join(t.name for t in pending))
        time.sleep(poll)
        for item in list(running):
            t, proc, t0, logf = item
            if proc.poll() is None and time.monotonic() - t0 < t.timeout:
                continue
            reason = "exit %s" % proc.returncode
            if proc.poll() is None:
                os.killpg(proc.pid, signal.SIGKILL)
                proc.wait()
                reason = "timeout"
            logf.close()
            running.remove(item)
            if not t.result.exists() and t.on_fail:
                t.on_fail(reason)
            done.add(t.name)
            log("%s %s%s" % (t.name, "done" if t.result.exists() else "FAILED", "" if reason == "exit 0" else " (%s)" % reason))


def write_failure(row, variant, work):
    def fail(reason):
        folder = Path(work) / "runs" / variant / row["unit"] / row["func"]
        folder.mkdir(parents=True, exist_ok=True)
        status = "timeout" if reason == "timeout" else "worker-error"
        (folder / "result.json").write_text(json.dumps(dict(
            row, status=status, score=None, reason=reason, redraft_variant=variant), sort_keys=True) + "\n")
    return fail


def applicable(row, variant):
    """Whether `variant` can run for `row` (guarded needs a guarded draft, archived an archived body)."""
    if variant == "guarded":
        return row["guarded"]
    if VARIANTS[variant].get("source") == "archived":
        return bool(row.get("archived"))
    return True


def log_path(work, scope, step):
    return Path(work) / "logs" / ("%s_%s.log" % (scope.replace(":", "_"), step))


def build_tasks(rows, variants, work, out, ticket, timeout, fixes, force):
    script = str(Path(__file__).resolve())
    tasks = []
    for row in rows:
        scope = row["scope"]
        final = Path(out) / row["unit"] / row["func"] / "result.json"
        if final.exists():
            if not force:
                continue
            final.unlink()            # the finish task counts as done when its result exists
        names = []
        for v in variants:
            if not applicable(row, v):
                continue
            arch = row.get("archived")
            cmd = [sys.executable, script, "--worker", scope, "--variant", v, "--work", str(work)]
            if VARIANTS[v].get("source") == "archived":
                cmd += ["--archived", "%s|%s" % (arch["path"], arch["file"])]
            result = Path(work) / "runs" / v / row["unit"] / row["func"] / "result.json"
            tasks.append(Task("%s/%s" % (scope, v), cmd, result, timeout, on_fail=write_failure(row, v, work),
                              log=log_path(work, scope, v)))
            names.append("%s/%s" % (scope, v))
        cmd = [sys.executable, script, "--finish", scope, "--work", str(work), "--out", str(out), "--ticket", ticket,
               "--variants", ",".join(variants)] + ([] if fixes else ["--no-fixes"])
        tasks.append(Task("%s/finish" % scope, cmd, final, timeout * 4, after=names, log=log_path(work, scope, "finish")))
    return tasks


# ------------------------------------------------------------------------------------------ command line

def environment(work):
    """work/environment.json: the git head and the header hashes ex.process() records as environment_hash."""
    path = Path(work) / "environment.json"
    if not path.exists():
        files = sorted(Path("include").glob("*.h")) + sorted(Path("include/ovl").glob("*.h"))
        env = dict(base=git_head(), files={str(f): digest(f.read_text(errors="replace")) for f in files})
        path.write_text(json.dumps(env, indent=1, sort_keys=True) + "\n")
    return path


def summary_line(r):
    sc = r["variant_scores"]
    return "%-28s %-22s score %-7s census %-5s archived %-5s %s" % (
        r["scope"], r["variant"] or "-", r["score"], r["census_score"], r["best_archived_score"],
        " ".join("%s=%s" % kv for kv in sorted(sc.items())))


def main(argv=None):
    p = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    p.add_argument("targets", nargs="*", help="UNIT:func or func")
    p.add_argument("--unit", action="append", default=[], help="every unmatched function of this unit (repeatable)")
    p.add_argument("--all-unmatched", action="store_true")
    p.add_argument("--ticket", default="T-20800", help="output goes to drafts/<ticket>/ (default T-20800)")
    p.add_argument("--out", help="output directory (default drafts/<ticket>)")
    p.add_argument("--work", help="work directory (default build/redraft/<ticket>)")
    p.add_argument("--variants", default=",".join(DEFAULT_VARIANTS), help="comma list or 'all'; default %(default)s")
    p.add_argument("--jobs", type=int, default=2, help="parallel compile jobs (default 2)")
    p.add_argument("--timeout", type=int, default=300, help="seconds per variant (finish: 4x)")
    p.add_argument("--no-fixes", action="store_true", help="skip the draft_fixes search and gs-local")
    p.add_argument("--force", action="store_true", help="finish functions that already have a result")
    p.add_argument("--include-tt", action="store_true")
    p.add_argument("--skip-unit", action="append", default=[], help="also skip this unit (repeatable)")
    p.add_argument("--limit", type=int, help="at most N targets")
    p.add_argument("--base", default="", help="git head to record in provenance (git is not reachable in Docker worktrees)")
    p.add_argument("--list", action="store_true", help="print the targets and stop")
    p.add_argument("--rewrite", help="print the rewrites of one body file (needs --unit) and stop")
    p.add_argument("--worker", help=argparse.SUPPRESS)
    p.add_argument("--finish", help=argparse.SUPPRESS)
    p.add_argument("--variant", help=argparse.SUPPRESS)
    p.add_argument("--archived", help=argparse.SUPPRESS)
    a = p.parse_args(argv)
    work = a.work or "build/redraft/%s" % a.ticket
    out = a.out or "drafts/%s" % a.ticket
    variants = tuple(VARIANTS) if a.variants == "all" else tuple(v for v in a.variants.split(",") if v)
    for v in variants:
        if v not in VARIANTS:
            p.error("unknown variant %s (have %s)" % (v, ", ".join(VARIANTS)))
    if a.worker:
        run_worker(a.worker, a.variant, work, a.archived)
        return 0
    if a.finish:
        finish(a.finish, work, out, a.ticket, variants, not a.no_fixes)
        return 0
    if a.rewrite:
        if len(a.unit) != 1:
            p.error("--rewrite needs exactly one --unit")
        body, log = rw.rewrite_all(rw.Env(a.unit[0]), Path(a.rewrite).read_text())
        sys.stdout.write(body)
        print(log, file=sys.stderr)
        return 0
    if not (a.targets or a.unit or a.all_unmatched):
        p.error("name functions, --unit U or --all-unmatched")
    import exhaustive_drafts as ex
    skip = tuple(s for s in SKIP_UNITS if not a.include_tt) + tuple(a.skip_unit)
    rows, messages = select_targets(ex.census(), a.targets, tuple(a.unit), a.all_unmatched, skip)
    for m in messages:
        print("redraft:", m, file=sys.stderr)
    if a.limit:
        rows = rows[:a.limit]
    if a.list:
        for r in rows:
            print(r["scope"], r["size"], "guarded" if r["guarded"] else "")
        print(len(rows), "functions", file=sys.stderr)
        return 0
    if not rows:
        print("redraft: no targets", file=sys.stderr)
        return 1
    if a.base:
        os.environ["TOKIMEMO_GIT_HEAD"] = a.base
    workp = Path(work)
    workp.mkdir(parents=True, exist_ok=True)
    inventory = workp / "inventory.json"
    known = {r["scope"]: r for r in json.loads(inventory.read_text())} if inventory.exists() else {}
    archive = Archive(exclude=(a.ticket,))
    for r in rows:   # the workers read the archive's view from here, not by scanning drafts/ again
        r["census"], r["archived"] = archive.census_of(r["scope"]), archive.best_of(r["scope"])
        if r["archived"]:
            r["archived"] = {k: v for k, v in r["archived"].items() if k != "key"}
    known.update({r["scope"]: r for r in rows})
    ex.save(inventory, json.dumps(sorted(known.values(), key=lambda r: r["scope"]), indent=1) + "\n")
    env_file = environment(workp)
    for v in variants:
        (workp / "runs" / v).mkdir(parents=True, exist_ok=True)
        shutil.copy(env_file, workp / "runs" / v / "environment.json")
    tasks = build_tasks(rows, variants, workp, out, a.ticket, a.timeout, not a.no_fixes, a.force)
    print("redraft: %d functions, %d tasks, %d jobs" % (len(rows), len(tasks), a.jobs), flush=True)
    run_pool(tasks, a.jobs, log=lambda m: print(m, flush=True))
    results = []
    for r in rows:
        res = Path(out) / r["unit"] / r["func"] / "result.json"
        if res.exists():
            results.append(json.loads(res.read_text()))
    catalog = Path(out) / "catalog.jsonl"
    merged = {}
    if catalog.exists():
        for line in catalog.read_text().splitlines():
            rec = json.loads(line)
            merged[rec["scope"]] = rec
    merged.update({rec["scope"]: rec for rec in results})
    Path(out).mkdir(parents=True, exist_ok=True)
    catalog.write_text("".join(json.dumps(merged[k], sort_keys=True) + "\n" for k in sorted(merged)))
    for rec in results:
        print(summary_line(rec))
    scored = [r for r in results if r["score"] is not None]
    print("redraft: %d of %d scored, %d at 0, %d better than the census, %d better than the best archive" % (
        len(scored), len(results), sum(r["score"] == 0 for r in scored),
        sum(r["vs_census"] == "better" for r in scored), sum(r["vs_archived"] == "better" for r in scored)))
    return 0


if __name__ == "__main__":
    sys.exit(main())
