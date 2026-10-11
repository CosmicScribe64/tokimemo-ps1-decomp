#!/usr/bin/env python3
"""Link a uopt regression sweep and compare literal function bytes (T-9800).

Run in Docker after uopt_eval.py regress and a verified normal build. Replaces
only object paths in copies of the normal linker scripts. All output belongs
in external scratch. A unit with a failed compile/link is unavailable, never
silently filled with stock objects. Function size, address, trailing padding
and literal relocated bytes must all agree. Whole-unit SHA1 equality is also
reported; this catches data and jump tables outside function spans.
"""

import argparse
import collections
import hashlib
import json
from pathlib import Path
import re
import subprocess

import trailing_pad


def run(*args):
    return subprocess.run(args, check=True, capture_output=True, text=True).stdout


def image_functions(elf, section, dump):
    headers = run("mips-linux-gnu-objdump", "-h", str(elf))
    rows = [line.split() for line in headers.splitlines()]
    matches = [int(row[3], 16) for row in rows if len(row) >= 7 and row[1] == section]
    if len(matches) != 1:
        raise ValueError("missing/ambiguous section " + section)
    base = matches[0]
    run("mips-linux-gnu-objcopy", "--dump-section", section + "=" + str(dump), str(elf), "/dev/null")
    data = dump.read_bytes()
    symbols = {}
    for line in run("mips-linux-gnu-nm", "-S", "--defined-only", str(elf)).splitlines():
        match = re.fullmatch(r"([0-9a-fA-F]+) ([0-9a-fA-F]+) [Tt] (\S+)", line)
        if match:
            # GNU nm sign-extends MIPS addresses even for this ELF32 target.
            address, size = int(match[1], 16) & 0xFFFFFFFF, int(match[2], 16)
            if base <= address and address + size <= base + len(data):
                symbols[match[3]] = (address, size)
    return base, data, symbols


def literal_match(got, want, name, padding):
    if name not in got[2] or name not in want[2]:
        return False
    ga, gs = got[2][name]
    wa, ws = want[2][name]
    if (ga, gs) != (wa, ws):
        return False
    go, wo, length = ga - got[0], wa - want[0], gs + padding
    g = got[1][go:go + length]
    w = want[1][wo:wo + length]
    return len(g) == length and len(w) == length and g == w


def link_unit(unit, jobs, out):
    names = [n for job in jobs for n in job["functions"]]
    record = {"unit": unit, "total": len(names)}
    if any(job["compile_exit"] for job in jobs):
        return dict(record, unavailable="compile failure", identical=0)
    stem = Path("build/SLPM_86.053" if unit == "main" else "build/ovl/" + unit)
    script = Path(str(stem) + ".ld")
    text = script.read_text()
    for job in jobs:
        anchor = job["expected"] + "("
        if anchor not in text:
            raise ValueError("object missing from linker script: " + job["expected"])
        target = str(Path(job["output"]).resolve()).replace("\\", "\\\\").replace('"', '\\"')
        text = text.replace(anchor, '"' + target + '"(')
    script_copy = out / (unit + ".ld")
    script_copy.write_text(text)
    if unit == "main":
        extra = ["build/undefined_funcs_auto.txt", "build/undefined_syms_auto.txt",
                 "build/SLPM_86.053_data_syms.ld"]
    else:
        extra = [str(stem) + suffix for suffix in ("_undefined_funcs_auto.txt",
                 "_undefined_syms_auto.txt", "_data_syms.ld")] + ["build/main_names.ld"]
    elf = out / (unit + ".elf")
    command = ["mips-linux-gnu-ld", "-EL", "-T", str(script_copy)]
    for item in extra:
        command += ["-T", item]
    command += ["-Map", str(out / (unit + ".map")), "-o", str(elf)]
    result = subprocess.run(command, capture_output=True, text=True)
    (out / (unit + ".log")).write_text(result.stdout + result.stderr)
    if result.returncode:
        return dict(record, unavailable="link failure", identical=0)
    section = ".main" if unit == "main" else "." + unit
    got = image_functions(elf, section, out / (unit + ".section"))
    want = image_functions(Path(str(stem) + ".elf"), section, out / (unit + ".baseline-section"))
    results = {}
    for job in jobs:
        for name in job["functions"]:
            asm = trailing_pad.find_asm(job["src"], name)
            if asm is None:
                raise ValueError("original function assembly missing: " + name)
            padding = 4 * trailing_pad.trailing_words(asm)
            results[name] = literal_match(got, want, name, padding)
    binary = out / (unit + ".bin")
    run("mips-linux-gnu-objcopy", "-O", "binary", str(elf), str(binary))
    actual_sha1 = hashlib.sha1(binary.read_bytes()).hexdigest()
    baseline_sha1 = hashlib.sha1(Path(str(stem) + ".bin").read_bytes()).hexdigest()
    return dict(record, identical=sum(results.values()), functions=results,
                sha1=actual_sha1, baseline_sha1=baseline_sha1,
                unit_identical=actual_sha1 == baseline_sha1)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("results", type=Path)
    parser.add_argument("output", type=Path)
    args = parser.parse_args()
    data = json.loads(args.results.read_text())
    groups = collections.defaultdict(list)
    for job in data["jobs"]:
        groups[job["unit"]].append(job)
    out = args.output.resolve()
    out.mkdir(parents=True, exist_ok=True)
    units = []
    for unit, jobs in groups.items():
        record = link_unit(unit, jobs, out)
        units.append(record)
        print(unit, record["identical"], "/", record["total"], record.get("unavailable", ""), flush=True)
    summary = {"total": sum(u["total"] for u in units),
               "identical": sum(u["identical"] for u in units),
               "unavailable": sum(u["total"] for u in units if u.get("unavailable")),
               "units_identical": sum(u.get("unit_identical", False) for u in units),
               "units_total": len(units)}
    (out / "results.json").write_text(json.dumps({"summary": summary, "units": units}, indent=2) + "\n")
    print(json.dumps(summary), flush=True)
    if summary["identical"] != summary["total"] or summary["units_identical"] != len(units):
        raise SystemExit(1)


if __name__ == "__main__":
    main()
