#!/usr/bin/env python3
"""Build the English disc and its xdelta patch in one command (T-39507, wiki/english-build.md).

Docker only, in the image of tools/Dockerfile.patch (xdelta3):
    IMG=tokimemo-patch tools/docker.sh python3 -m tools.patch.build_english

Inputs: the original data track (default disc/orig/track1.bin, with track2.bin next to it), the
original files (disc/files), the text units (.asset-cache/text/units/units.jsonl, see
wiki/english-text-containers.md) and the English records (translations/en/). Outputs go to
build/en/disc/: track1.bin, tokimemo-en.cue (track 2 next to it), tokimemo-en.xdelta and
report.json (ids, offsets, sizes and counts; no game text). Exit status 1 when a step fails;
records that cannot be placed are reported, not fatal.

Steps:
 1. configure and build the font metrics (build/en/font_metrics.json).
 2. Encode every English record (tools/patch/text/english.py) and wrap it for its window
    (windows.py, wrap.py).
 3. C literals: write the English sources to build/en/src (c_literals.rewrite_sources; a sized array
    grows with its literal) and compile them. The English link relinks every unit freely (T-39505,
    wiki/relink.md), so objects may grow; the fit loop of the old pinned link only runs with
    PINNED_LINK. Literals of EVENT and GYOZI, which the game never loads, stay Japanese.
 4. Write the talk scripts and tables (Image.write) and place them on the disc
    (tools/patch/disc.py): a file that needs more sectors moves to the end of the data track,
    after the checks of tools/patch/cd_layout.py (read sites, load buffers); a file that may not
    grow keeps its original bytes.
 5. Write build/en/cd_layout.json (the moved files) and run `ninja english`: the remap table of
    src/english/cd.c sends reads of moved files to their new sectors.
 4b. The English art (tools/patch/art/): every registered image into its sprite sheet or BS still,
    placed like a data file; the MAINROM part records go into the built executable before step 6.
 6. Write the in-place records into the built executable and overlays (text/inplace.py), at the
    place the free link moved each slot to (english_link.translator), put them on the disc (the
    executable last, moved to the end when it needs more sectors), write track 1 and the cue,
    then the xdelta patch against the original track. A route run on this disc needs
    `tools/emu/run.sh route run --relink build/en`.
"""
from __future__ import annotations

import argparse
import json
import os
import re
import shutil
import subprocess
import sys
import time
from collections import Counter, defaultdict
from pathlib import Path

from tools import english_link
from tools.patch import cd_layout
from tools.patch import disc as disc_mod
from tools.patch import make_patch
from tools.patch.art import hook as art_hook
from tools.patch.text import c_literals, retarget
from tools.patch.text.containers import EXE_PATH, build_containers, load_units
from tools.patch.text.image import SECTOR
from tools.patch.text.inplace import patch_built
from tools.patch.text.layout import english_payloads
from tools.patch.text.translations import load_jsonl, load_targets
from tools.patch.text.wrap import FontMetrics

REPOSITORY = Path(__file__).resolve().parents[2]
EN_DIR = Path("build/en")
EN_SRC = EN_DIR / "src"
CD_LAYOUT = EN_DIR / "cd_layout.json"
FONT_METRICS = EN_DIR / "font_metrics.json"
EXE_OUT = EN_DIR / "SLPM_86.053.bin"
OVERLAY_DIR = EN_DIR / "ovl"
DATA_KINDS = ("talk_script", "talk_script_supplemental", "binary_table")
BINARY_KINDS = ("main_executable", "overlay")
# The English link relinks every unit freely (T-39505, wiki/relink.md): objects may grow, so the literal
# fit loop of the pinned link is off. Set it to True only to reproduce the pinned-link report.
PINNED_LINK = False
NINJA_OBJECT = re.compile(r"^build (build/en/\S+\.o): cctext (src/\S+\.c)", re.M)
SECTIONS = (".text", ".rodata", ".data", ".sdata", ".bss", ".sbss")


class BuildError(RuntimeError):
    """A build step failed; the message says which and why."""


class Steps:
    def __init__(self):
        self.timings: dict[str, float] = {}
        self._start = time.monotonic()

    def run(self, name, function, *args, **kwargs):
        began = time.monotonic()
        print("== %s" % name, flush=True)
        try:
            return function(*args, **kwargs)
        finally:
            self.timings[name] = round(time.monotonic() - began, 1)

    def total(self) -> float:
        return round(time.monotonic() - self._start, 1)


def run(cmd: list[str], *, check: bool = True, quiet: bool = False) -> subprocess.CompletedProcess:
    proc = subprocess.run(cmd, cwd=REPOSITORY, text=True, capture_output=True)
    if check and proc.returncode:
        raise BuildError("%s failed (exit %d):\n%s" % (" ".join(cmd[:3]), proc.returncode,
                                                       failure_lines(proc.stdout + proc.stderr)))
    if not quiet and proc.stdout.strip():
        print(proc.stdout.strip().splitlines()[-1], flush=True)
    return proc


def failure_lines(output: str, limit: int = 30) -> str:
    """The lines of a ninja run that say what failed (compiler warnings left out)."""
    lines = output.strip().splitlines()
    keep, after = [], 0
    for line in lines:
        if line.startswith("FAILED:"):
            after = 12
        if after or "rror" in line or "cannot" in line:
            if "Warning" not in line:
                keep.append(line)
            after = max(0, after - 1)
    return "\n".join((keep or lines)[-limit:])


def configure() -> None:
    run(["python3", "configure.py"], quiet=True)


def ninja(targets: list[str], *, keep_going: bool = False, check: bool = True) -> subprocess.CompletedProcess:
    return run(["ninja"] + (["-k", "0"] if keep_going else []) + targets, check=check)


# ---------------------------------------------------------------- C literals

def literal_payloads(containers, text) -> dict[str, bytes]:
    out = {}
    for path, container in containers.items():
        for record in container.records.values():
            if record.kind == "source_literal" and record.id in text.payloads.get(path, {}):
                out[record.id] = text.payloads[path][record.id]
    return out


def write_sources(sites, payloads: dict[str, bytes]) -> list[str]:
    """Generate build/en/src/<path> for every file with an English literal; returns the paths."""
    if EN_SRC.exists():
        shutil.rmtree(REPOSITORY / EN_SRC)
    files = c_literals.rewrite_sources(REPOSITORY, sites, payloads)
    for path, source in files.items():
        out = REPOSITORY / EN_DIR / path
        out.parent.mkdir(parents=True, exist_ok=True)
        out.write_text(source, encoding="utf-8")
    return sorted(files)


def english_objects() -> dict[str, str]:
    """src path -> English object, for every file compiled from build/en/src."""
    text = re.sub(r"\$\n\s*", "", (REPOSITORY / "build.ninja").read_text())  # ninja_syntax line wraps
    return {src: obj for obj, src in NINJA_OBJECT.findall(text)}


def _sections(obj: Path) -> dict[str, int]:
    out = run(["mips-linux-gnu-objdump", "-h", str(obj)], quiet=True).stdout
    sizes = {}
    for line in out.splitlines():
        words = line.split()
        if len(words) >= 3 and words[0].isdigit() and words[1] in SECTIONS:
            sizes[words[1]] = int(words[2], 16)
    return sizes


def _symbols(obj: Path) -> dict[str, tuple[str, str]]:
    out = run(["mips-linux-gnu-nm", "--defined-only", str(obj)], quiet=True).stdout
    syms = {}
    for line in out.splitlines():
        words = line.split()
        if len(words) == 3 and not words[2].startswith("$"):
            syms[words[2]] = (words[0], words[1])
    return syms


def object_fit(english: Path, default: Path) -> str | None:
    """None when the English object links at the default object's address, else why not."""
    if not english.is_file():
        return "does not compile"
    new, old = _sections(english), _sections(default)
    for name, size in new.items():
        if size > old.get(name, 0):
            return "%s grows from %d to %d bytes" % (name, old.get(name, 0), size)
    new_syms, old_syms = _symbols(english), _symbols(default)
    moved = [name for name, value in old_syms.items() if name in new_syms and new_syms[name] != value]
    if moved:
        return "%d symbols move (%s)" % (len(moved), ", ".join(sorted(moved)[:3]))
    return None


def unloaded_sources() -> set[str]:
    """C files of the overlays the game never loads (EVENT, GYOZI): their literals stay Japanese."""
    sys.path.insert(0, str(REPOSITORY / "tools"))
    import srcscan  # noqa: E402  (tools/srcscan.py)
    old_dir = os.getcwd()
    os.chdir(REPOSITORY)
    try:
        return {str(cf.src) for name in english_link.UNLOADED for cf in srcscan.unit_c_files(name)}
    finally:
        os.chdir(old_dir)


def build_literals(sites, payloads: dict[str, bytes]) -> dict:
    """Generate and compile the English sources; with a pinned link keep only what fits."""
    by_file = defaultdict(list)
    for site in sites:
        by_file[site.path].append(site)
    skipped = unloaded_sources()
    unloaded = {s.id: "%s: overlay never loaded" % s.path for s in sites if s.path in skipped and s.id in payloads}
    active = {rid: payload for rid, payload in payloads.items() if rid not in unloaded}
    reverted: dict[str, str] = {}
    failed_files: dict[str, str] = {}
    rounds = 0
    while True:
        rounds += 1
        files = write_sources(sites, active)
        configure()
        objects = english_objects()
        targets = [objects[path] for path in files if path in objects]
        for target in targets:
            (REPOSITORY / target).unlink(missing_ok=True)
        if targets:
            ninja(targets, keep_going=True, check=False)
        if not PINNED_LINK:
            break
        bad = {}
        for path in files:
            obj = objects.get(path)
            if obj is None:
                continue
            reason = object_fit(REPOSITORY / obj, REPOSITORY / ("build/" + obj[len("build/en/"):]))
            if reason:
                bad[path] = reason
        if not bad:
            break
        for path, reason in bad.items():
            ids = [s.id for s in by_file[path] if s.id in active]
            growing = [rid for rid in ids if len(active[rid]) > len(next(s for s in by_file[path] if s.id == rid).payload)]
            # first round: drop the growing literals of the file; later: the whole file
            drop = growing if rounds == 1 and growing and len(growing) < len(ids) else ids
            for rid in drop:
                active.pop(rid)
                reverted[rid] = "%s: %s" % (path, reason)
            if drop is ids:
                failed_files[path] = reason
        if rounds >= 3:
            break
    files = write_sources(sites, active)
    configure()
    return {"files": files, "literals_written": len(active), "literals_reverted": reverted,
            "literals_unloaded": unloaded,
            "files_reverted": failed_files, "rounds": rounds}


# ---------------------------------------------------------------- disc layout

def place_data_files(disc, containers, text, files_root: Path, sites, checksum_sites) -> dict:
    """Write every data container, check it, and place it on the disc."""
    limits = cd_layout.buffer_limits(sites)
    files_by_lba = {e.lba: (e.path, e.sectors) for e in disc.iso.files()}
    out = {"placed": [], "kept_original": [], "unchanged": 0, "overflows": {}, "problems": {}, "warnings": {}}
    for path, container in sorted(containers.items()):
        if container.kind not in DATA_KINDS or container.image is None:
            continue
        payloads = text.payloads.get(path, {})
        result = container.image.write(payloads)
        if result.overflows:
            out["overflows"][path] = result.overflows
        if result.data == container.image.data:
            out["unchanged"] += 1
            continue
        entry = disc.iso.find(path)
        lba, size = entry.lba, entry.size  # disc.replace updates the entry
        plan = cd_layout.plan_file(path, lba, size, len(result.data), sites, limits, checksum_sites, files_by_lba)
        if plan.problems:
            out["problems"][path] = plan.problems
        if plan.warnings:
            out["warnings"][path] = plan.warnings
        if not plan.accept:
            out["kept_original"].append({"path": path, "reason": plan.reason, "records": len(payloads)})
            continue
        new_lba = disc.replace(path, result.data)
        out["placed"].append({"path": path, "lba": lba, "sectors": cd_layout.sectors_of(size), "new_lba": new_lba,
                              "new_sectors": cd_layout.sectors_of(len(result.data)), "bytes": size,
                              "new_bytes": len(result.data), "moved_strings": sum(p.moved for p in
                                                                                  result.placements.values())})
    return out


def write_layout(placed: list[dict]) -> list[dict]:
    remap = cd_layout.remap_entries(placed)
    path = REPOSITORY / CD_LAYOUT
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps({"remap": remap}, indent=1) + "\n")
    return remap


def relinked_offsets(disc_path: str, built_path: Path, base: int):
    """Original file offset -> offset in the freely relinked English binary (None: no such place)."""
    unit = "main" if disc_path == EXE_PATH else built_path.stem
    if unit in english_link.UNLOADED:
        return lambda offset: offset
    old_dir = os.getcwd()
    os.chdir(REPOSITORY)
    try:
        moved = english_link.translator(unit, str(EN_DIR))
    finally:
        os.chdir(old_dir)
    new_base = base if unit != "main" else english_link.EXE_VRAM - english_link.EXE_HEADER

    def locate(offset):
        if unit == "main" and offset < english_link.EXE_HEADER:
            return offset
        address = moved(base + offset)
        return None if address is None else address - new_base
    return locate


def retargeter(disc_path: str, built_path: Path, built: bytes) -> retarget.Retargeter:
    """Moves over-capacity slots of one English binary (tools/patch/text/retarget.py, T-39506)."""
    unit = "main" if disc_path == EXE_PATH else built_path.stem
    if unit == "main":
        elf, map_path = EN_DIR / ("%s.elf" % english_link.EXE), EN_DIR / ("%s.map" % english_link.EXE)
        vram = english_link.EXE_VRAM - english_link.EXE_HEADER
    else:
        elf, map_path = EN_DIR / "ovl" / ("%s.elf" % unit), EN_DIR / "ovl" / ("%s.map" % unit)
        vram = english_link.OVERLAY_SLOT
    old_dir = os.getcwd()
    os.chdir(REPOSITORY)  # map paths are relative to the repository
    try:
        refs = retarget.References(str(map_path), str(elf))
    finally:
        os.chdir(old_dir)
    elf = REPOSITORY / elf
    pool = retarget.main_pool(built, str(elf), vram) if unit == "main" else retarget.overlay_pool(built, str(elf), vram)
    default_elf = REPOSITORY / ("build/%s.elf" % english_link.EXE if unit == "main" else "build/ovl/%s.elf" % unit)
    return retarget.Retargeter(refs, pool, str(default_elf))


def place_binaries(disc, containers, text, files_root: Path) -> dict:
    """In-place records into the built overlays and executable, then onto the disc."""
    out = {"overlays_replaced": [], "in_place": {}}
    jobs = []
    for bin_path in sorted((REPOSITORY / OVERLAY_DIR).glob("*.bin")):
        if bin_path.stem not in english_link.UNLOADED:  # never loaded: the original file stays
            jobs.append(("CDROM/EXEDIR/%s.EXN" % bin_path.stem, bin_path))
    jobs.append((EXE_PATH, REPOSITORY / EXE_OUT))
    for disc_path, built_path in jobs:
        built = built_path.read_bytes()
        container = containers.get(disc_path)
        if container is not None and container.kind in BINARY_KINDS:
            mover = retargeter(disc_path, built_path, built)
            result = patch_built(container, text.payloads.get(disc_path, {}), built,
                                 relinked_offsets(disc_path, built_path, container.image.base), mover)
            built = result.data
            out["in_place"][disc_path] = {"written": len(result.written), "overflows": result.overflows,
                                          "moved_in_build": result.moved_in_build,
                                          "retargeted": mover.moved, "not_retargeted": mover.refused}
        if built == (files_root / disc_path).read_bytes():
            continue
        lba = disc.replace(disc_path, built)
        if disc_path == EXE_PATH:
            out["exe"] = {"lba": lba, "bytes": len(built), "sectors": cd_layout.sectors_of(len(built))}
        else:
            out["overlays_replaced"].append(disc_path)
    return out


# ---------------------------------------------------------------- report

def record_report(text, containers, literals: dict, data: dict, binaries: dict) -> dict:
    """Final status of every record, by container."""
    status = {rid: note["status"] for rid, note in text.notes.items()}
    reasons = {rid: note.get("reason") for rid, note in text.notes.items() if note.get("reason")}
    for rid, why in literals["literals_reverted"].items():
        status[rid], reasons[rid] = "needs_relink", why
    for rid, why in literals.get("literals_unloaded", {}).items():
        status[rid], reasons[rid] = "kept_original", why
    for path, overflows in data["overflows"].items():
        for item in overflows:
            if item.get("id"):
                status[item["id"]], reasons[item["id"]] = "overflow", item["reason"]
    kept_files = {item["path"]: item["reason"] for item in data["kept_original"]}
    for rid, note in text.notes.items():
        if note["path"] in kept_files and status[rid] in ("placed", "unwrappable"):
            status[rid], reasons[rid] = "file_kept_original", kept_files[note["path"]]
    for path, item in binaries["in_place"].items():
        for over in item["overflows"]:
            status[over["id"]] = "needs_relink"
            reasons[over["id"]] = "in place: %d bytes, capacity %d" % (over["needed"], over["capacity"])
        for rid in item["moved_in_build"]:
            status[rid], reasons[rid] = "moved_in_build", "slot bytes differ in the English binary"
    unloaded = {"CDROM/EXEDIR/%s.EXN" % name for name in english_link.UNLOADED}
    for rid, note in text.notes.items():
        if note["path"] in unloaded and status[rid] != "kept_original":
            status[rid], reasons[rid] = "kept_original", "overlay never loaded"
    by_container = defaultdict(Counter)
    by_kind = defaultdict(Counter)
    lists = defaultdict(list)
    for rid, state in status.items():
        note = text.notes[rid]
        by_container[note["path"]][state] += 1
        by_kind[note["container"]][state] += 1
        if state != "placed":
            item = {"id": rid, "reason": reasons.get(rid)}
            if state == "unwrappable":
                item.update(window=note.get("window"), overflow_px=note.get("overflow_px"),
                            too_many_lines=note.get("too_many_lines"))
            lists[state].append(item)
    unwrapped_lines = Counter(note.get("window") for note in text.notes.values() if note.get("breaks_added"))
    return {"totals": dict(Counter(status.values())), "by_container_kind": {k: dict(v) for k, v in by_kind.items()},
            "by_container": {k: dict(v) for k, v in sorted(by_container.items())},
            "records": {k: sorted(v, key=lambda i: i["id"]) for k, v in lists.items()},
            "records_with_breaks_added_by_window": dict(unwrapped_lines),
            "rows_over_row_buffer": sum(1 for n in text.notes.values() if n.get("rows_over_row_buffer"))}


def headline(report: dict) -> list[str]:
    totals = report["records"]["totals"]
    lines = ["records: " + ", ".join("%s %d" % kv for kv in sorted(totals.items()))]
    data = report["data_files"]
    grown = sum(p["new_bytes"] - p["bytes"] for p in data["placed"])
    moved = [p for p in data["placed"] if p["new_lba"] != p["lba"]]
    lines.append("data files: %d written (+%d bytes), %d moved, %d kept original, %d unchanged"
                 % (len(data["placed"]), grown, len(moved), len(data["kept_original"]), data["unchanged"]))
    lit = report["c_literals"]
    lines.append("C literals: %d written in %d files, %d need the relink (%d files reverted)"
                 % (lit["literals_written"], len(lit["files"]), len(lit["literals_reverted"]),
                    len(lit["files_reverted"])))
    lines.append("characters: %d unmapped kinds, %d kept as Shift-JIS kinds"
                 % (len(report["characters"]["unmapped"]), len(report["characters"]["kept_shift_jis"])))
    disc = report["disc"]
    lines.append("disc: track1 %s (%d sectors added), patch %s bytes"
                 % (disc["track1_sha1"][:12], disc["sectors_added"], disc.get("xdelta_bytes")))
    art = report.get("art")
    if art:
        lines.append("art: " + ", ".join("%s %d" % kv for kv in sorted(art["totals"].items()))
                     + "; %d containers written, %d MAINROM records" % (len(art["files_placed"]),
                                                                         art.get("exe_records_written", 0)))
    lines.append("time: %s s total" % report["timings"]["total"])
    return lines


# ---------------------------------------------------------------- main

def build(args) -> dict:
    steps = Steps()
    out_dir = REPOSITORY / args.out
    out_dir.mkdir(parents=True, exist_ok=True)
    files_root = REPOSITORY / args.disc
    (REPOSITORY / CD_LAYOUT).unlink(missing_ok=True)

    def prepare():
        configure()
        ninja([str(FONT_METRICS)])
    steps.run("font metrics", prepare)
    metrics = FontMetrics.load(REPOSITORY / FONT_METRICS)

    def load():
        units = load_units(args.units)
        supplemental = load_jsonl(args.supplemental)
        return units, build_containers(files_root, units, supplemental), load_targets(args.returned,
                                                                                      args.supplemental)
    units, containers, targets = steps.run("load records", load)
    text = steps.run("encode and wrap", english_payloads, containers, targets, metrics)

    sites = c_literals.locate_sites(REPOSITORY, units)
    literals = steps.run("C literals", build_literals, sites, literal_payloads(containers, text))

    read_sites = cd_layout.scan_read_sites(REPOSITORY / "src") + cd_layout.tour_table_sites(
        files_root, REPOSITORY / "config")
    checksum_sites = cd_layout.scan_checksum_sites(REPOSITORY / "src")
    with disc_mod.Disc(str(REPOSITORY / args.orig)) as disc:
        data = steps.run("data files", place_data_files, disc, containers, text, files_root, read_sites,
                         checksum_sites)
        art = None if args.no_art else steps.run("art", art_hook.place_files, disc, REPOSITORY, files_root,
                                                   read_sites, checksum_sites, data, EXE_PATH)
        remap = write_layout(data["placed"])

        def english_build():
            configure()
            try:
                ninja(["english"])
                return None
            except BuildError as exc:
                if not literals["files"] or os.environ.get("ENGLISH_NO_FALLBACK"):
                    raise
                # keep the disc build going without the English sources and say why
                shutil.rmtree(REPOSITORY / EN_SRC)
                configure()
                ninja(["english"])
                return str(exc)
        link_error = steps.run("ninja english", english_build)
        if link_error:
            literals["link_error_without_literals"] = link_error
            literals["literals_reverted"].update({rid: "English link failed" for rid in literal_payloads(
                containers, text) if rid not in literals["literals_reverted"]})
            literals["files"], literals["literals_written"] = [], 0
        if art:
            art["report"]["exe_records_written"] = art_hook.patch_exe(REPOSITORY / EXE_OUT, files_root / EXE_PATH, art)
        binaries = steps.run("executable and overlays", place_binaries, disc, containers, text, files_root)
        track1 = out_dir / "track1.bin"
        track2 = out_dir / "track2.bin"
        if not track2.exists():
            source2 = (REPOSITORY / args.orig).with_name("track2.bin")
            try:
                os.link(source2, track2)
            except OSError:
                shutil.copyfile(source2, track2)
        cue = out_dir / "tokimemo-en.cue"
        added = len(disc.added)
        sha1 = steps.run("write track 1", disc.write, str(track1), str(cue), str(track2))
    if art:
        steps.run("verify art", art_hook.verify, REPOSITORY, track1, files_root, art)
    report = {"command": "IMG=tokimemo-patch tools/docker.sh python3 -m tools.patch.build_english",
              "disc": {"track1": str(args.out + "/track1.bin"), "track1_sha1": sha1, "sectors_added": added,
                       "cue": str(args.out + "/tokimemo-en.cue")}}
    if not args.no_patch:
        patch = out_dir / "tokimemo-en.xdelta"
        steps.run("xdelta", make_patch.make, str(REPOSITORY / args.orig), str(track1), str(patch))
        report["disc"]["xdelta"] = str(args.out + "/tokimemo-en.xdelta")
        report["disc"]["xdelta_bytes"] = patch.stat().st_size
    encoder = text.encoder
    report.update({
        "records": record_report(text, containers, literals, data, binaries),
        "characters": {"unmapped": {"U+%04X" % ord(c): n for c, n in encoder.unmapped.most_common()},
                       "kept_shift_jis": {"U+%04X" % ord(c): n for c, n in encoder.kept_shift_jis.most_common()},
                       "folded": {"U+%04X" % ord(c): n for c, n in encoder.folded.most_common()}},
        "c_literals": literals,
        "data_files": data,
        "cd_remap": remap,
        "binaries": binaries,
        "art": art["report"] if art else None,
    })
    report["timings"] = dict(steps.timings, total=steps.total())
    report["layout"] = layout_report(text)
    (out_dir / "report.json").write_text(json.dumps(report, indent=1, sort_keys=True) + "\n")
    (out_dir / "layout.json").write_text(json.dumps(layout_records(text), indent=1, sort_keys=True) + "\n")
    return report


def layout_records(text) -> dict:
    """Per reflowed record: rows before and after, breaks kept and why, orphans, pages over the window
    (T-39506); ids and counts only."""
    return {rid: dict(note["reflow"], window=note.get("window")) for rid, note in text.notes.items()
            if "reflow" in note}


def layout_report(text) -> dict:
    """Totals of the reflow (T-39506): breaks seen, forced (reflowed), kept by rule or category,
    ambiguous; rows before and after; orphans and pages longer than their window."""
    totals = Counter()
    kept_reasons = Counter()
    for note in text.notes.values():
        flow = note.get("reflow")
        if not flow:
            continue
        totals["records"] += 1
        for key in ("breaks", "forced", "kept_rule", "kept_category", "ambiguous", "rows_before", "rows_after",
                    "orphans", "pages_over_lines"):
            totals[key] += flow.get(key, 0)
        totals["records_shorter"] += flow.get("rows_after", 0) < flow.get("rows_before", 0)
        for reason, _fill in flow["kept"]:
            kept_reasons[reason] += 1
    return {"totals": dict(totals), "kept_by_reason": dict(kept_reasons), "per_record": "layout.json"}


def main(argv=None) -> int:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--orig", default="disc/orig/track1.bin", help="original data track (MODE2/2352)")
    parser.add_argument("--disc", default="disc/files", help="original files")
    parser.add_argument("--units", default=".asset-cache/text/units/units.jsonl")
    parser.add_argument("--returned", default="translations/en/returned-v3.jsonl")
    parser.add_argument("--supplemental", default="translations/en/supplemental.jsonl")
    parser.add_argument("--out", default="build/en/disc")
    parser.add_argument("--no-patch", action="store_true", help="skip the xdelta patch")
    parser.add_argument("--no-art", action="store_true", help="leave the images Japanese (T-39508)")
    args = parser.parse_args(argv)
    os.chdir(REPOSITORY)
    try:
        report = build(args)
    except (BuildError, disc_mod.DiscError, make_patch.PatchError, art_hook.ArtBuildError) as exc:
        print("build_english: %s" % exc, file=sys.stderr)
        return 1
    for line in headline(report):
        print(line)
    print("report: %s/report.json" % args.out)
    return 0


if __name__ == "__main__":
    sys.exit(main())
