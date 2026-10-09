#!/usr/bin/env python3
"""Extract the game disc into disc/ (reproducible, no game data is committed).

Usage (inside Docker):
    tools/docker.sh python3 tools/extract_disc.py <game.zip> <out_dir>

Steps:
  1. Unzip the Redump-style zip: "(Track 1).bin" -> <out_dir>/track1.bin,
     "(Track 2).bin" -> <out_dir>/track2.bin, plus a rewritten track1/track2 cue.
  2. Walk the ISO9660 tree of track1.bin (raw MODE2/2352) and write every file
     to <out_dir>/files/<path> (version suffix ";1" stripped).

Why not dumpsxiso: it does not extract this disc's tree (non-standard CDROM
directory sector). Sector rules, all at byte offset LBA*2352:
  - sync(12)+header(4) = 16 bytes, then an 8-byte XA subheader (bytes 16..23);
  - Form1 sector: 2048 data bytes at +24;
  - Form2 sector (subheader submode bit 0x20): 2324 data bytes at +24;
  - XA/STR streams (Form2 sectors) are skipped, see extract();
  - a directory sector may lack a valid subheader (two identical 4-byte
    halves is the valid pattern); then data starts at +16.
Exit status is non-zero if the image is short or no SLPM_86.053 is found.
The tracks are always re-extracted from the zip (no stale-file reuse).
"""
import os
import struct
import sys
import zipfile

SECTOR = 2352
MAX_DEPTH = 8
CUE = (
    'FILE "track1.bin" BINARY\n'
    "  TRACK 01 MODE2/2352\n"
    "    FLAGS DCP\n"
    "    INDEX 01 00:00:00\n"
    'FILE "track2.bin" BINARY\n'
    "  TRACK 02 AUDIO\n"
    "    FLAGS DCP\n"
    "    INDEX 00 00:00:00\n"
    "    INDEX 01 00:02:00\n"
)


def unzip_tracks(zip_path, out_dir):
    """Write track1.bin, track2.bin and track.cue into out_dir."""
    names = {"(Track 1).bin": "track1.bin", "(Track 2).bin": "track2.bin"}
    with zipfile.ZipFile(zip_path) as z:
        for info in z.infolist():
            for suffix, dst in names.items():
                if info.filename.endswith(suffix):
                    dst_path = os.path.join(out_dir, dst)
                    with z.open(info) as src, open(dst_path, "wb") as out:
                        while True:
                            buf = src.read(1 << 20)
                            if not buf:
                                break
                            out.write(buf)
    for dst in names.values():
        if not os.path.exists(os.path.join(out_dir, dst)):
            sys.exit("missing %s in zip" % dst)
    with open(os.path.join(out_dir, "game.cue"), "w") as f:
        f.write(CUE)


class Image:
    def __init__(self, path):
        self.f = open(path, "rb")
        self.f.seek(0, 2)
        self.sectors = self.f.tell() // SECTOR

    def read_sector(self, lba):
        """Return (payload, is_form2) for one sector."""
        self.f.seek(lba * SECTOR + 16)
        sub = self.f.read(8)
        if len(sub) < 8:
            raise EOFError("LBA %d past end of image" % lba)
        if sub[:4] != sub[4:]:  # no subheader (directory sector quirk)
            self.f.seek(lba * SECTOR + 16)
            return self.f.read(2048), False
        if sub[2] & 0x20:
            return self.f.read(2324), True
        return self.f.read(2048), False

    def read_dir(self, lba, size):
        data = b"".join(self.read_sector(lba + i)[0] for i in range((size + 2047) // 2048))
        pos, out = 0, []
        while pos < size:
            ln = data[pos]
            if ln == 0:
                pos = (pos // 2048 + 1) * 2048
                continue
            rec = data[pos:pos + ln]
            r_lba = struct.unpack("<I", rec[2:6])[0]
            r_size = struct.unpack("<I", rec[10:14])[0]
            name = rec[33:33 + rec[32]].decode("latin1")
            out.append((name, r_lba, r_size, rec[25]))
            pos += ln
        return out

    def read_file(self, lba, size):
        """Return the file bytes, or None if any sector is Form2 (a stream)."""
        out = bytearray()
        i = 0
        while len(out) < size:
            payload, form2 = self.read_sector(lba + i)
            if form2:
                return None
            out += payload
            i += 1
        return bytes(out[:size])


def extract(img, lba, size, dest, depth, counts):
    if depth > MAX_DEPTH:
        sys.exit("directory nesting too deep at %s" % dest)
    os.makedirs(dest, exist_ok=True)
    for name, r_lba, r_size, flags in img.read_dir(lba, size):
        if name in ("\x00", "\x01"):
            continue
        name = name.split(";")[0]
        if "/" in name or name in ("", ".", ".."):
            sys.exit("unsafe name %r" % name)
        path = os.path.join(dest, name)
        if flags & 2:
            extract(img, r_lba, r_size, path, depth + 1, counts)
        elif r_lba >= img.sectors:
            counts["skipped"] += 1  # CD-DA reference, no data in the bin
        else:
            data = img.read_file(r_lba, r_size)
            if data is None:
                # XA/STR stream: the ISO size counts 2048-byte units, so a
                # Form2 payload cannot be cut to size. Not needed for the decomp.
                counts["skipped"] += 1
                continue
            with open(path, "wb") as f:
                f.write(data)
            counts["files"] += 1


def main(argv):
    if len(argv) != 3:
        sys.exit(__doc__)
    zip_path, out_dir = argv[1], argv[2]
    os.makedirs(out_dir, exist_ok=True)
    unzip_tracks(zip_path, out_dir)
    img = Image(os.path.join(out_dir, "track1.bin"))
    pvd, _ = img.read_sector(16)
    root = pvd[156:190]
    root_lba = struct.unpack("<I", root[2:6])[0]
    root_size = struct.unpack("<I", root[10:14])[0]
    counts = {"files": 0, "skipped": 0}
    try:
        extract(img, root_lba, root_size, os.path.join(out_dir, "files"), 0, counts)
    except EOFError as e:
        sys.exit("truncated image: %s" % e)
    if not os.path.exists(os.path.join(out_dir, "files", "SLPM_86.053")):
        sys.exit("SLPM_86.053 not found")
    print("extracted %d files, skipped %d (CD-DA/XA streams)" % (counts["files"], counts["skipped"]))


if __name__ == "__main__":
    main(sys.argv)
