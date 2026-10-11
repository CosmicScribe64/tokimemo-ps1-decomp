#!/usr/bin/env python3
"""Evaluate a replacement uopt without changing the production compiler (T-9800).

Run in Docker. `regress OUT --uopt PATH` compiles all C-bearing IDO objects
through the normal cvt/frame/asm-processor/padding pipeline and compares each
C function with the already verified build. `cases MANIFEST OUT --uopt PATH`
does the same for standalone experimental sources. Manifest entries contain
src, unit, function, and expected (the original object), optionally provenance.
Results are resolved instruction words and relocation identities, never a
register-masked comparison. Missing functions and compile errors fail closed.
Scratch inputs, outputs and compiler binaries stay outside the repository.
"""

import argparse
import difflib
from functools import lru_cache
import hashlib
import json
import os
from pathlib import Path
import shlex
import subprocess
import sys
import tempfile
from types import SimpleNamespace

# tools/queue.py is a project CLI, not the standard library queue module.
_tools_path = sys.path.pop(0)
from concurrent.futures import ThreadPoolExecutor
import queue
sys.path.insert(0, _tools_path)

import cc
import funcdiff
import srcscan
import trailing_pad


def file_sha256(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


@lru_cache(maxsize=32)
def original_ranges(unit):
    """The verified original is immutable during one evaluation run."""
    return funcdiff.original_rodata(SimpleNamespace(unit=unit))


def compile_one(src, out, uopt, trace=False, toolchain=("ido", "5.3")):
    """Use cc.py's full pipeline, replacing only the target of its uopt shim."""
    out = Path(out).resolve()
    out.parent.mkdir(parents=True, exist_ok=True)
    scratch = out.parent / (out.stem + "-tmp")
    scratch.mkdir(exist_ok=True)
    tempfile.tempdir = str(scratch)
    os.environ["TMPDIR"] = str(scratch)
    original_env = cc.ido_frame_env

    def environment(ver, tmp, extra_shims=None):
        env = original_env(ver, tmp, extra_shims)
        shim = Path(tmp) / "uopt"
        text = shim.read_text()
        target = '"/opt/ido/5.3/uopt"'
        if text.count(target) != 1:
            raise ValueError("expected exactly one pinned IDO 5.3 uopt target in shim")
        text = text.replace(target, shlex.quote(str(Path(uopt).resolve())))
        # IDO names its unused default listing " ". Its ucode arguments and
        # our shim paths are absolute; keep that artifact in the job's scratch.
        text = text.replace("#!/bin/sh\n", "#!/bin/sh\ncd " + shlex.quote(str(scratch)) + " || exit\n", 1)
        if trace:
            text = text.replace('"$@"', '"$@" -zdbug:6 -l ' + shlex.quote(str(out) + '.trace'))
        shim.write_text(text)
        return env

    cc.ido_frame_env = environment
    cc.main(["cc.py", str(src), str(out)] + list(toolchain))


def compare(got, want):
    if got is None or want is None:
        return {"match": False, "edits": None, "missing": True}
    got = [key for key, _display in got]
    want = [key for key, _display in want]
    matcher = difflib.SequenceMatcher(None, want, got, autojunk=False)
    edits = sum(max(j - i, l - k) for tag, i, j, k, l in matcher.get_opcodes() if tag != "equal")
    return {"match": got == want, "edits": edits}


def case_function(obj, name, source, original=False, unit=None):
    """Bound isolated drafts by their ELF size, not object-end alignment zeros.

    Keep the original's explicit trailing padding and all in-function zeros.
    Jump-table identities still use funcdiff's normalization: an apparent case
    match needs a unit checksum before adoption, as with normal funcdiff.
    """
    symbols = trailing_pad.text_functions(obj)
    if name not in symbols:
        return None
    start, size = symbols[name]
    if size <= 0 or size % 4:
        raise ValueError("invalid function size for " + name)
    asm = trailing_pad.find_asm(source, name)
    # INCLUDE_ASM's ELF size already covers the original trailing words;
    # the C compiler's size excludes the words inserted by trailing_pad.py.
    padding = trailing_pad.trailing_words(asm) if asm and not original else 0
    result = subprocess.run(["mips-linux-gnu-objdump", "-drz", obj],
                            check=True, capture_output=True, text=True)
    functions = funcdiff.split_functions(result.stdout)
    if name not in functions:
        return None
    insns = [i for i in funcdiff.parse_insns(functions[name])
             if i[0] is not None and start <= i.offset < start + size + 4 * padding]
    if len(insns) != size // 4 + padding:
        raise ValueError("incomplete function disassembly for " + name)
    info = funcdiff.object_info(obj)
    if original:
        info = funcdiff.ObjInfo(info.rodata, info.symbols, info.tables,
                               original_ranges(unit),
                               info.text_symbols)
    address = funcdiff.sym_address(name)
    base = address - start if address is not None else None
    return funcdiff.resolve(insns, info, text_base=base)


def run_job(job, outdir, uopt, trace):
    source_digest = file_sha256(job["src"])
    if job.get("source_sha256", source_digest) != source_digest:
        raise ValueError("source differs from frozen manifest: " + job["src"])
    out = outdir / (job["id"] + ".o")
    out.parent.mkdir(parents=True, exist_ok=True)
    cmd = [sys.executable, str(Path(__file__).resolve()), "compile", job["src"],
           str(out), "--uopt", uopt]
    if trace:
        cmd.append("--trace")
    cmd += ["--toolchain"] + job.get("toolchain", ["ido", "5.3"])
    with out.with_suffix(".log").open("w") as log:
        result = subprocess.run(cmd, stdout=log, stderr=subprocess.STDOUT)
    names = job["functions"]
    record = dict(job, output=str(out), compile_exit=result.returncode,
                  source_sha256=source_digest)
    if file_sha256(job["src"]) != source_digest:
        raise ValueError("source changed during compilation: " + job["src"])
    if result.returncode:
        record["results"] = {n: {"match": False, "compile_error": True} for n in names}
        return record
    record["object_sha256"] = file_sha256(out)
    try:
        if job.get("original_source"):
            got = {n: case_function(str(out), n, job["original_source"]) for n in names}
            want = {n: case_function(job["expected"], n, job["original_source"],
                                     original=True, unit=job["unit"]) for n in names}
        else:
            got = funcdiff.resolved_functions(str(out), names)
            want = funcdiff.resolved_functions(job["expected"], names)
        record["results"] = {n: compare(got[n], want[n]) for n in names}
    except (Exception, SystemExit) as exc:
        record["results"] = {n: {"match": False, "comparison_error": str(exc)} for n in names}
    return record


def regression_jobs():
    jobs = []
    toolchains = {}
    for line in Path("config/toolchains.txt").read_text().splitlines():
        parts = line.split("#", 1)[0].split()
        if parts:
            toolchains[parts[0]] = parts[1:]
    for c in srcscan.c_files():
        names = sorted(srcscan.defined_functions(c.src) -
                       {name for _, name in srcscan.INCLUDE_ASM_RE.findall(c.src.read_text())})
        if not names:
            continue
        if not Path(c.obj).is_file():
            raise FileNotFoundError(c.obj)
        jobs.append({"id": c.name, "src": str(c.src), "unit": c.unit,
                     "expected": c.obj, "functions": names,
                     "toolchain": toolchains.get(c.name, ["ido", "5.3"])})
    return jobs


def main():
    evaluator_digest = file_sha256(__file__)
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("mode", choices=["compile", "regress", "cases"])
    parser.add_argument("paths", nargs="+")
    parser.add_argument("--uopt", required=True)
    parser.add_argument("--jobs", type=int, default=4)
    parser.add_argument("--trace", action="store_true")
    parser.add_argument("--toolchain", nargs="+", default=["ido", "5.3"])
    args = parser.parse_args()
    if args.mode == "compile":
        compile_one(*args.paths, args.uopt, args.trace, args.toolchain)
        return
    uopt_digest = file_sha256(args.uopt)
    if args.mode == "regress":
        jobs = regression_jobs()
        outdir = Path(args.paths[0]).resolve()
    else:
        entries = json.loads(Path(args.paths[0]).read_text())
        jobs = [dict(e, id=str(i), functions=[e["function"]]) for i, e in enumerate(entries)]
        outdir = Path(args.paths[1]).resolve()
    outdir.mkdir(parents=True, exist_ok=True)
    records = []
    with ThreadPoolExecutor(max_workers=args.jobs) as pool:
        for record in pool.map(lambda job: run_job(job, outdir, args.uopt, args.trace), jobs):
            records.append(record)
            matches = sum(x["match"] for x in record["results"].values())
            print("%s: %d/%d" % (record["id"], matches, len(record["functions"])), flush=True)
    values = [v for record in records for v in record["results"].values()]
    summary = {"total": len(values), "identical": sum(v["match"] for v in values),
               "compile_errors": sum(bool(v.get("compile_error")) for v in values),
               "comparison_errors": sum(bool(v.get("comparison_error") or v.get("missing")) for v in values)}
    if file_sha256(args.uopt) != uopt_digest:
        raise ValueError("uopt binary changed during evaluation")
    metadata = {"uopt": str(Path(args.uopt).resolve()), "uopt_sha256": uopt_digest,
                "evaluator_sha256": evaluator_digest, "trace": args.trace,
                "comparison": "resolved instructions; cases bounded by ELF size and original padding"}
    (outdir / "results.json").write_text(json.dumps(
        {"summary": summary, "metadata": metadata, "jobs": records}, indent=2) + "\n")
    print(json.dumps(summary), flush=True)
    if summary["compile_errors"] or summary["comparison_errors"]:
        sys.exit(2)
    if args.mode == "regress" and summary["identical"] != summary["total"]:
        sys.exit(1)


if __name__ == "__main__":
    main()
