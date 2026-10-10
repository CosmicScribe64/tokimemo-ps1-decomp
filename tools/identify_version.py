#!/usr/bin/env python3
"""Identify which release of Tokimeki Memorial a disc image or folder is.

Usage (inside Docker):
    tools/docker.sh python3 tools/identify_version.py <path> [--versions config/versions.txt]

<path> is one of:
  - a folder holding the boot exe (SLPM_86.053 or PSX.EXE) and optionally
    CDROM/EXEDIR/*.EXN and O.BIN (for example disc/files after extract_disc.py);
  - a raw .bin (MODE2/2352) or .iso (2048-byte sectors) data track, or a .cue
    whose first FILE is such a track;
  - a .zip, .7z or .chd archive of such a disc (T-3300). A zip is read in place with Python's
    zipfile (only the sectors needed are decompressed). A 7z or CHD is unpacked to a temporary
    directory first (about 700 MB, removed afterwards) with 7-Zip (7zz, 7z, 7za or 7zr) or
    chdman; both are in the Docker image. Without the tool the tool says so and asks for the
    disc to be unpacked first.

The boot exe is found through SYSTEM.CNF, hashed, and looked up in
config/versions.txt (facts only: ids, sizes, SHA-1; see wiki/versions.md). The
overlays found next to it are compared as well, so a patched or mixed disc is
reported as such. Nothing is written; no game data leaves the machine.
Exit status: 0 known exe, 1 unknown exe, 2 usage or read error.
"""
import contextlib
import hashlib
import os
import re
import shutil
import struct
import subprocess
import sys
import tempfile
import zipfile

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import extract_disc  # noqa: E402

DEFAULT_DB = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "config", "versions.txt")
BOOT_NAMES = ("SLPM_86.053", "PSX.EXE")
ARCHIVE_EXTS = (".zip", ".7z", ".chd")
SEVENZIP = ("7zz", "7z", "7za", "7zr")


def load_db(path):
    """Return (versions, exes, ovls): versions[id]=label, exes[sha1]=[ids], ovls[id][name]=sha1."""
    versions, exes, ovls = {}, {}, {}
    with open(path) as f:
        for line in f:
            p = line.split()
            if not p or p[0].startswith("#"):
                continue
            if p[0] == "version":
                versions[p[1]] = p[2].replace("_", " ")
            elif p[0] == "exe":
                exes.setdefault(p[3], []).append(p[1])
            elif p[0] == "ovl":
                ovls.setdefault(p[1], {})[p[2]] = p[3]
    return versions, exes, ovls


class IsoImage(extract_disc.Image):
    """Cooked 2048-byte-sector image (`f`: an open file-like object, `size`: its length)."""

    def __init__(self, f, size):
        self.f = f
        self.sectors = size // 2048

    def read_sector(self, lba):
        self.f.seek(lba * 2048)
        data = self.f.read(2048)
        if len(data) < 2048:
            raise EOFError("LBA %d past end of image" % lba)
        return data, False


class RawImage(extract_disc.Image):
    """Raw 2352-byte-sector image (`f`: an open file-like object, `size`: its length)."""

    def __init__(self, f, size):
        self.f = f
        self.sectors = size // extract_disc.SECTOR


class ArchiveError(ValueError):
    """An archive cannot be read (missing tool, no disc track inside, tool failure)."""


def image_of(f, size):
    """Image over a seekable binary file, raw or cooked by its sync header."""
    f.seek(0)
    head = f.read(12)
    f.seek(0)
    return (RawImage if head == b"\x00" + b"\xff" * 10 + b"\x00" else IsoImage)(f, size)


def cue_track(text):
    """File name of the first FILE entry of a cue sheet, or None."""
    m = re.search(r'FILE\s+"([^"]+)"', text)
    return m.group(1) if m else None


def open_image(path):
    if path.lower().endswith(".cue"):
        with open(path) as f:
            name = cue_track(f.read())
        if not name:
            raise ValueError("no FILE entry in cue")
        path = os.path.join(os.path.dirname(path), name)
    f = open(path, "rb")
    f.seek(0, 2)
    return image_of(f, f.tell())


def pick_track(names, cue_text=None):
    """The data track among archive member names: the cue's first FILE, else the first .bin/.iso."""
    base = {os.path.basename(n).lower(): n for n in names}
    if cue_text:
        want = cue_track(cue_text)
        if want and os.path.basename(want).lower() in base:
            return base[os.path.basename(want).lower()]
    for n in sorted(names):
        if n.lower().endswith((".bin", ".iso", ".img")):
            return n
    raise ArchiveError("no .bin or .iso disc track in the archive")


def find_tool(candidates, what, path):
    for c in candidates:
        found = shutil.which(c)
        if found:
            return found
    raise ArchiveError("%s needs %s on PATH, which is not installed here. Run this through "
                       "tools/docker.sh (the image has it), or unpack %s first and pass the "
                       ".cue, .bin or folder" % (os.path.splitext(path)[1], what, os.path.basename(path)))


def run_tool(cmd):
    proc = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
    if proc.returncode != 0:
        raise ArchiveError("%s failed (exit %d):\n%s" % (os.path.basename(cmd[0]), proc.returncode,
                                                         proc.stdout.strip()[-600:]))
    return proc.stdout


def sevenzip_members(tool, path):
    out = run_tool([tool, "l", "-slt", "-ba", path])
    names, cur = [], {}
    for line in out.splitlines() + [""]:
        if line.strip() == "":
            if cur.get("Path") and cur.get("Folder") != "+":
                names.append(cur["Path"])
            cur = {}
        elif " = " in line:
            k, v = line.split(" = ", 1)
            cur[k.strip()] = v
    return names


@contextlib.contextmanager
def open_archive(path):
    """Context manager giving the Image of the data track of a .zip, .7z or .chd disc archive."""
    ext = os.path.splitext(path)[1].lower()
    if ext == ".zip":
        try:
            z = zipfile.ZipFile(path)
        except zipfile.BadZipFile as e:
            raise ArchiveError("%s is not a readable zip: %s" % (path, e))
        with z:
            names = [i.filename for i in z.infolist() if not i.is_dir()]
            cue = next((n for n in sorted(names) if n.lower().endswith(".cue")), None)
            track = pick_track(names, z.read(cue).decode("latin1") if cue else None)
            with z.open(track) as f:
                yield image_of(f, z.getinfo(track).file_size)
        return
    tmp = tempfile.mkdtemp(prefix="identify_version")
    try:
        if ext == ".7z":
            tool = find_tool(SEVENZIP, "7-Zip (7zz, 7z, 7za or 7zr)", path)
            names = sevenzip_members(tool, path)
            cue = next((n for n in sorted(names) if n.lower().endswith(".cue")), None)
            text = None
            if cue:
                run_tool([tool, "e", "-y", "-bd", "-o" + tmp, path, cue])
                with open(os.path.join(tmp, os.path.basename(cue)), errors="replace") as f:
                    text = f.read()
            track = pick_track(names, text)
            run_tool([tool, "e", "-y", "-bd", "-o" + tmp, path, track])
            img_path = os.path.join(tmp, os.path.basename(track))
        else:
            tool = find_tool(("chdman",), "chdman (MAME tools)", path)
            run_tool([tool, "extractcd", "-i", path, "-o", os.path.join(tmp, "disc.cue"),
                      "-ob", os.path.join(tmp, "disc.bin")])
            img_path = os.path.join(tmp, "disc.bin")
        img = open_image(img_path)
        try:
            yield img
        finally:
            img.f.close()
    finally:
        shutil.rmtree(tmp, ignore_errors=True)


def read_tree(img, wanted):
    """Return {path: bytes} for the wanted paths (e.g. 'SYSTEM.CNF', 'CDROM/EXEDIR/O.BIN')."""
    pvd, _ = img.read_sector(16)
    if pvd[1:6] != b"CD001" and img.read_sector(16)[0][1:6] != b"CD001":
        raise ValueError("no ISO9660 primary volume descriptor")
    root = pvd[156:190]
    out = {}

    def walk(lba, size, prefix, depth):
        if depth > extract_disc.MAX_DEPTH:
            return
        for name, r_lba, r_size, flags in img.read_dir(lba, size):
            if name in ("\x00", "\x01"):
                continue
            path = prefix + name.split(";")[0]
            if flags & 2:
                if any(w.startswith(path + "/") for w in wanted):
                    walk(r_lba, r_size, path + "/", depth + 1)
            elif path in wanted and r_lba < img.sectors:
                data = img.read_file(r_lba, r_size)
                if data is not None:
                    out[path] = data

    walk(struct.unpack("<I", root[2:6])[0], struct.unpack("<I", root[10:14])[0], "", 0)
    return out


def read_folder(path):
    """Collect the same files from a folder (exe at top level, overlays in CDROM/EXEDIR)."""
    out = {}
    for n in BOOT_NAMES + ("SYSTEM.CNF",):
        p = os.path.join(path, n)
        if os.path.isfile(p):
            with open(p, "rb") as f:
                out[n] = f.read()
    d = os.path.join(path, "CDROM", "EXEDIR")
    if os.path.isdir(d):
        for n in os.listdir(d):
            if n.endswith(".EXN") or n == "O.BIN":
                with open(os.path.join(d, n), "rb") as f:
                    out["CDROM/EXEDIR/" + n] = f.read()
    return out


def boot_name(files):
    cnf = files.get("SYSTEM.CNF", b"").decode("latin1")
    m = re.search(r"BOOT\s*=\s*cdrom:\\?([^;\s]+)", cnf, re.I)
    if m and m.group(1) in files:
        return m.group(1)
    return next((n for n in BOOT_NAMES if n in files), None)


def identify(files, db):
    """Return a result dict for the collected files."""
    versions, exes, ovls = db
    boot = boot_name(files)
    if boot is None:
        raise ValueError("no boot exe found (looked for %s)" % ", ".join(BOOT_NAMES))
    sha = hashlib.sha1(files[boot]).hexdigest()
    ov = {n.split("/")[-1]: hashlib.sha1(d).hexdigest() for n, d in files.items() if n.startswith("CDROM/EXEDIR/")}
    scores = {v: sum(1 for n, h in ov.items() if t.get(n) == h) for v, t in ovls.items()}
    best = max(scores, key=scores.get) if scores and ov else None
    return {"boot": boot, "exe_sha1": sha, "exe_size": len(files[boot]), "versions": exes.get(sha, []), "version": exes.get(sha, [None])[0],
            "overlays": len(ov), "overlay_matches": scores, "closest_overlays": best,
            "system_cnf": files.get("SYSTEM.CNF", b"").decode("latin1").strip().splitlines()[:1]}


def report(res, db):
    versions = db[0]
    lines = ["boot exe: %s (%d bytes) sha1 %s" % (res["boot"], res["exe_size"], res["exe_sha1"])]
    if res["version"]:
        lines.append("exe is: %s" % " / ".join("%s [%s]" % (versions.get(v, v), v) for v in res["versions"]))
        if len(res["versions"]) > 1:
            lines.append("(these releases share the same exe and overlays; they differ only in disc metadata)")
    else:
        lines.append("exe is UNKNOWN (not in the version table)")
    if res["overlays"]:
        lines.append("overlays found: %d" % res["overlays"])
        for v, n in sorted(res["overlay_matches"].items(), key=lambda x: -x[1]):
            lines.append("  matches %-8s %d of %d" % (v, n, res["overlays"]))
        if res["version"] and res["closest_overlays"] not in res["versions"] and \
                res["overlay_matches"][res["closest_overlays"]] > max(res["overlay_matches"].get(v, 0) for v in res["versions"]):
            lines.append("warning: overlays look like %s, not %s (mixed or patched disc)" %
                         (res["closest_overlays"], "/".join(res["versions"])))
    else:
        lines.append("no overlays found (exe-only identification)")
    return "\n".join(lines)


def main(argv):
    args = argv[1:]
    db_path = DEFAULT_DB
    if "--versions" in args:
        i = args.index("--versions")
        db_path = args[i + 1]
        del args[i:i + 2]
    if len(args) != 1:
        sys.exit(__doc__)
    path = args[0]
    try:
        db = load_db(db_path)
        if os.path.isdir(path):
            files = read_folder(path)
        else:
            wanted = {"SYSTEM.CNF", *BOOT_NAMES} | {"CDROM/EXEDIR/%s" % n for n in db_overlay_names(db)}
            if path.lower().endswith(ARCHIVE_EXTS):
                with open_archive(path) as img:
                    files = read_tree(img, wanted)
            else:
                files = read_tree(open_image(path), wanted)
        res = identify(files, db)
    except (OSError, ValueError, EOFError) as e:
        print("error: %s" % e, file=sys.stderr)
        return 2
    print(report(res, db))
    return 0 if res["version"] else 1


def db_overlay_names(db):
    return {n for t in db[2].values() for n in t}


if __name__ == "__main__":
    sys.exit(main(sys.argv))
