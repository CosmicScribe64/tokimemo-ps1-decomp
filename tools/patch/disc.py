#!/usr/bin/env python3
"""Rebuild track 1 of the disc with replaced or relocated files (T-39501).

Usage (in Docker):
    python3 tools/patch/disc.py list TRACK1 [PATH_PREFIX]
    python3 tools/patch/disc.py rebuild TRACK1 OUT_TRACK1 [--cue OUT.cue --track2 TRACK2]
                                [--replace DISC_PATH=FILE ...] [--relocate DISC_PATH=FILE ...]

The library API (used by the patch build):
    disc = Disc(track1_path)                                     # also a context manager
    lba = disc.replace("CDROM/EXEDIR/DATE.EXN", data)            # in place if it fits, else moved
    lba = disc.replace("SLPM_86.053", data, relocate=True)       # always moved to the free area
    n = disc.replace_stream("OPENXA.STR", raw)                    # same-layout STR video sectors (T-39509)
    disc.write(out_track1, cue_path=out_cue, track2_path=track2)  # returns the output SHA-1

Disc facts this relies on (wiki/disc-layout.md):
  - Raw MODE2/2352 sectors; files are Mode 2 Form 1 except the XA/STR streams (Form 2). A
    stream is never moved or resized; replace_stream rewrites only its Form 1 video sectors in
    place (same subheaders, new EDC/ECC), never an audio sector. Sectors that are not written
    are copied byte for byte.
  - LBA 14515 (first sector of the /CDROM directory) has no valid subheader: its records start
    at byte 16, where the subheader would be. Its EDC/ECC are valid over the raw Form 1 layout,
    so a record update patches the raw bytes and recomputes EDC/ECC at the Form 1 positions.
  - The ISO volume space size counts track 2 as well (TR02.DA points into it), so the end of
    track 1 is found from the extents, not from the PVD.
  - The track ends in a run of empty Form 1 sectors (submode 0, the postgap). Moved files are
    put at the first sector after the last extent in track 1; the postgap and track 2 move back
    by the number of added sectors; the PVD volume space size and every extent past track 1
    (TR02.DA) move with them. Directories never move, so the path tables never change.
"""
import argparse
import hashlib
import os
import struct
import sys

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from patch import sector as cs  # noqa: E402

PVD_LBA = 16
MAX_DEPTH = 8
CUE = (
    'FILE "{track1}" BINARY\n'
    "  TRACK 01 MODE2/2352\n"
    "    FLAGS DCP\n"
    "    INDEX 01 00:00:00\n"
    'FILE "{track2}" BINARY\n'
    "  TRACK 02 AUDIO\n"
    "    FLAGS DCP\n"
    "    INDEX 00 00:00:00\n"
    "    INDEX 01 00:02:00\n"
)
FILLER_SUBHEADER = b"\0\0\0\0"  # the empty sectors between files and in the postgap


class DiscError(Exception):
    pass


class Entry:
    """One ISO9660 directory record. `rec_lba`/`rec_off` locate it: sector and byte offset
    inside the parent directory's user data (None for the root, whose record is in the PVD)."""

    def __init__(self, path, lba, size, is_dir, rec_lba, rec_off):
        self.path = path
        self.lba = lba
        self.size = size
        self.is_dir = is_dir
        self.rec_lba = rec_lba
        self.rec_off = rec_off

    @property
    def sectors(self):
        return max(1, (self.size + cs.FORM1_DATA - 1) // cs.FORM1_DATA)

    def __repr__(self):
        return "Entry(%r, lba=%d, size=%d%s)" % (self.path, self.lba, self.size, ", dir" if self.is_dir else "")


class Track:
    """Read-only raw MODE2/2352 track."""

    def __init__(self, path):
        self.path = path
        self.f = open(path, "rb")
        size = os.fstat(self.f.fileno()).st_size
        if size % cs.SIZE:
            raise DiscError("%s: size %d is not a multiple of %d" % (path, size, cs.SIZE))
        self.count = size // cs.SIZE

    def raw(self, lba):
        if not 0 <= lba < self.count:
            raise DiscError("LBA %d outside the track (%d sectors)" % (lba, self.count))
        self.f.seek(lba * cs.SIZE)
        return self.f.read(cs.SIZE)

    def close(self):
        self.f.close()


def payload_offset(raw):
    """Where the 2048 user bytes of a Form 1 sector start: 24, or 16 when the subheader is
    missing (the damaged directory sector)."""
    return cs.DATA if cs.has_subheader(raw) else 16


def user_data(raw):
    """Form 1 user data (2048 bytes) of a raw sector; DiscError for Form 2."""
    if cs.has_subheader(raw) and cs.is_form2(raw):
        raise DiscError("Form 2 sector where Form 1 data was expected")
    off = payload_offset(raw)
    return raw[off:off + cs.FORM1_DATA]


def _both32(value):
    return struct.pack("<I", value) + struct.pack(">I", value)


class Iso:
    """The ISO9660 tree of a track: PVD and every directory record (paths without ';1')."""

    def __init__(self, track):
        self.track = track
        pvd = user_data(track.raw(PVD_LBA))
        if pvd[0:6] != b"\x01CD001":
            raise DiscError("no primary volume descriptor at LBA %d" % PVD_LBA)
        self.volume_space = struct.unpack_from("<I", pvd, 80)[0]
        self.path_table_size = struct.unpack_from("<I", pvd, 132)[0]
        self.path_tables = struct.unpack_from("<II", pvd, 140) + struct.unpack_from(">II", pvd, 148)
        root = pvd[156:190]
        self.root = Entry("", struct.unpack_from("<I", root, 2)[0], struct.unpack_from("<I", root, 10)[0],
                          True, PVD_LBA, 156)
        self.entries = {"": self.root}
        self._walk(self.root, 0)

    def _walk(self, d, depth):
        if depth > MAX_DEPTH:
            raise DiscError("directory nesting too deep at %r" % d.path)
        for i in range(d.sectors):
            lba = d.lba + i
            data = user_data(self.track.raw(lba))
            pos = 0
            while pos < cs.FORM1_DATA and data[pos]:
                ln = data[pos]
                rec = data[pos:pos + ln]
                name_len = rec[32]
                name = rec[33:33 + name_len]
                if name not in (b"\0", b"\1"):
                    name = name.decode("latin1").split(";")[0]
                    path = d.path + "/" + name if d.path else name
                    e = Entry(path, struct.unpack_from("<I", rec, 2)[0], struct.unpack_from("<I", rec, 10)[0],
                              bool(rec[25] & 2), lba, pos)
                    self.entries[path] = e
                    if e.is_dir:
                        self._walk(e, depth + 1)
                pos += ln

    def find(self, path):
        e = self.entries.get(path.strip("/").split(";")[0])
        if e is None:
            raise DiscError("no file %r on the disc" % path)
        return e

    def files(self):
        return [e for e in self.entries.values() if not e.is_dir]

    def read(self, path):
        """File bytes (Form 1 files only)."""
        e = self.find(path)
        if e.is_dir:
            raise DiscError("%r is a directory" % path)
        out = b"".join(user_data(self.track.raw(e.lba + i)) for i in range(e.sectors))
        return out[:e.size]

    def is_stream(self, e):
        """True if any sector of the extent is Form 2 (XA audio, STR video)."""
        for i in range(e.sectors):
            if e.lba + i >= self.track.count:
                return True  # CD-DA reference, no data track sectors
            raw = self.track.raw(e.lba + i)
            if cs.has_subheader(raw) and cs.is_form2(raw):
                return True
        return False

    def data_end(self):
        """First LBA after every extent inside the track."""
        end = 0
        for e in self.entries.values():
            if e.lba < self.track.count:
                end = max(end, e.lba + e.sectors)
        return end


class Disc:
    """A patched copy of track 1. Nothing is written until write()."""

    def __init__(self, track1_path):
        self.track = Track(track1_path)
        self.iso = Iso(self.track)
        self.count = self.track.count
        self.free_lba = self.iso.data_end()  # where moved files go
        self.sectors = {}  # lba -> raw sector (bytearray) that replaces the original
        self.added = []  # raw sectors inserted at self.free_lba, in order
        self.done = set()  # paths already replaced (each file once)
        self.written = False
        self.beyond = [e for e in self.iso.entries.values() if e.lba >= self.count]  # TR02.DA
        tail = range(self.free_lba, self.count)
        for lba in tail:
            raw = self.track.raw(lba)
            if raw[16:20] != FILLER_SUBHEADER or user_data(raw).strip(b"\0"):
                raise DiscError("LBA %d after the last extent is not an empty sector" % lba)

    def close(self):
        self.track.close()

    def __enter__(self):
        return self

    def __exit__(self, *exc):
        self.close()

    @property
    def inserted(self):
        return len(self.added)

    def _raw(self, lba):
        if lba in self.sectors:
            return self.sectors[lba]
        return bytearray(self.track.raw(lba))

    def _subheaders(self, e, n):
        """Subheaders (4 bytes) for n new sectors of entry e: the originals when the count is
        unchanged, else the first sector's for the body and the last one's (EOR/EOF) for the end."""
        orig = [self.track.raw(e.lba + i)[16:20] for i in range(e.sectors)]
        if n == len(orig):
            return orig
        body = bytes((orig[0][0], orig[0][1], orig[0][2] & ~(cs.SUB_EOR | cs.SUB_EOF) & 0xFF, orig[0][3]))
        return [body] * (n - 1) + [orig[-1]]

    def _update_record(self, e, lba, size):
        raw = self._raw(e.rec_lba)
        off = payload_offset(raw) + e.rec_off
        raw[off + 2:off + 10] = _both32(lba)
        raw[off + 10:off + 18] = _both32(size)
        cs.finish_form1(raw)
        self.sectors[e.rec_lba] = raw
        e.lba = lba
        e.size = size

    def replace(self, path, data, relocate=False):
        """Put `data` as the content of file `path`. Written in place when it needs no more
        sectors than the original (and relocate is false), else at the end of the data area
        (the old copy stays on the disc, unreferenced). Freed sectors in place become empty
        filler sectors. Each file can be replaced once. Returns the file's LBA."""
        e = self.iso.find(path)
        if e.is_dir:
            raise DiscError("%r is a directory" % path)
        if self.iso.is_stream(e):
            raise DiscError("%r is an XA/STR/CD-DA stream; streams are never rewritten" % path)
        if e.path in self.done:
            raise DiscError("%r is already replaced" % path)
        self.done.add(e.path)
        n = max(1, (len(data) + cs.FORM1_DATA - 1) // cs.FORM1_DATA)
        subs = self._subheaders(e, n)
        if relocate or n > e.sectors:
            lba = self.free_lba + len(self.added)
            for i in range(n):
                self.added.append(cs.make_form1(lba + i, subs[i], data[i * cs.FORM1_DATA:(i + 1) * cs.FORM1_DATA]))
        else:
            lba = e.lba
            for i in range(e.sectors):
                if i < n:
                    sub, chunk = subs[i], data[i * cs.FORM1_DATA:(i + 1) * cs.FORM1_DATA]
                else:
                    sub, chunk = FILLER_SUBHEADER, b""
                self.sectors[lba + i] = cs.make_form1(lba + i, sub, chunk)
        if (lba, len(data)) != (e.lba, e.size):
            self._update_record(e, lba, len(data))
        return lba

    def replace_stream(self, path, raw):
        """Put new raw sectors (2352 bytes each) over the stream `path` (an STR movie, T-39509),
        same size and same layout. Only Form 1 sectors with the video submode bit may differ, and
        each one must keep its sync, address and subheader and carry a valid EDC/ECC. Audio
        (Form 2) and every other sector must be byte-identical. Returns the number of sectors
        rewritten."""
        e = self.iso.find(path)
        if e.is_dir or not self.iso.is_stream(e):
            raise DiscError("%r is not an XA/STR stream" % path)
        if e.path in self.done:
            raise DiscError("%r is already replaced" % path)
        if e.lba + e.sectors > self.count or len(raw) != e.sectors * cs.SIZE:
            raise DiscError("%r: new stream is %d bytes, the extent is %d sectors" % (path, len(raw), e.sectors))
        self.done.add(e.path)
        changed = {}
        for i in range(e.sectors):
            new = raw[i * cs.SIZE:(i + 1) * cs.SIZE]
            old = self.track.raw(e.lba + i)
            if new == old:
                continue
            video = cs.has_subheader(old) and not cs.is_form2(old) and old[18] & cs.SUB_VIDEO
            if not video:
                raise DiscError("%r sector %d is not a video sector; only video sectors may change" % (path, i))
            if new[:cs.DATA] != old[:cs.DATA]:
                raise DiscError("%r sector %d: sync, address or subheader changed" % (path, i))
            if not cs.form1_valid(new):
                raise DiscError("%r sector %d: EDC/ECC do not match the data" % (path, i))
            changed[e.lba + i] = bytearray(new)
        self.sectors.update(changed)
        return len(changed)

    def _shift_streams(self):
        """Move the PVD volume space size and the extents past track 1 by the inserted count."""
        if not self.added:
            return
        n = len(self.added)
        pvd = self._raw(PVD_LBA)
        off = payload_offset(pvd)
        pvd[off + 80:off + 88] = _both32(self.iso.volume_space + n)
        cs.finish_form1(pvd)
        self.sectors[PVD_LBA] = pvd
        for e in self.beyond:
            self._update_record(e, e.lba + n, e.size)
        self.iso.volume_space += n

    def write(self, out_path, cue_path=None, track2_path=None):
        """Write the patched track (and a cue next to it if asked). Returns its SHA-1 hex.
        Call once: the stream shift is applied to this object."""
        if self.written:
            raise DiscError("write() was already called")
        self.written = True
        self._shift_streams()
        n = len(self.added)
        h = hashlib.sha1()
        with open(out_path, "wb") as out:
            def put(raw):
                h.update(raw)
                out.write(raw)
            for lba in range(self.free_lba):
                put(bytes(self._raw(lba)))
            for raw in self.added:
                put(bytes(raw))
            for lba in range(self.free_lba, self.count):
                raw = self.track.raw(lba)
                if n:  # the postgap moves back: new address, so new EDC/ECC
                    raw = cs.make_form1(lba + n, raw[16:20], b"")
                put(bytes(raw))
        if cue_path:
            write_cue(cue_path, out_path, track2_path)
        return h.hexdigest()


def write_cue(cue_path, track1_path, track2_path):
    """A cue for the patched track 1 and the unchanged track 2 (paths relative to the cue)."""
    if not track2_path:
        raise DiscError("a cue needs the track 2 path")
    here = os.path.dirname(os.path.abspath(cue_path))
    rel = lambda p: os.path.relpath(os.path.abspath(p), here)
    for p in (track1_path, track2_path):
        if '"' in rel(p):
            raise DiscError("track path %r cannot be written to a cue" % p)
    with open(cue_path, "w") as f:
        f.write(CUE.format(track1=rel(track1_path), track2=rel(track2_path)))


def _pairs(specs):
    out = []
    for spec in specs or []:
        if "=" not in spec:
            raise DiscError("expected DISC_PATH=FILE, got %r" % spec)
        out.append(spec.split("=", 1))
    return out


def main(argv):
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    sub = ap.add_subparsers(dest="cmd", required=True)
    ls = sub.add_parser("list", help="print LBA, size and path of every file")
    ls.add_argument("track1")
    ls.add_argument("prefix", nargs="?", default="")
    rb = sub.add_parser("rebuild", help="write a patched track 1")
    rb.add_argument("track1")
    rb.add_argument("out")
    rb.add_argument("--replace", action="append", metavar="DISC_PATH=FILE")
    rb.add_argument("--relocate", action="append", metavar="DISC_PATH=FILE")
    rb.add_argument("--cue", help="also write this cue (needs --track2)")
    rb.add_argument("--track2", help="the unchanged audio track the cue names")
    args = ap.parse_args(argv)
    try:
        if args.cmd == "list":
            iso = Iso(Track(args.track1))
            for e in sorted(iso.entries.values(), key=lambda e: e.lba):
                if e.path.startswith(args.prefix):
                    print("%6d %9d %s%s" % (e.lba, e.size, e.path, "/" if e.is_dir else ""))
            return 0
        with Disc(args.track1) as disc:
            for specs, relocate in ((args.replace, False), (args.relocate, True)):
                for path, src in _pairs(specs):
                    with open(src, "rb") as f:
                        print("%s -> LBA %d" % (path, disc.replace(path, f.read(), relocate)))
            sha = disc.write(args.out, args.cue, args.track2)
            print("sha1 %s (%d sectors added)" % (sha, disc.inserted))
    except (DiscError, OSError) as e:
        print("disc.py: %s" % e, file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
