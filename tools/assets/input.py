#!/usr/bin/env python3
"""Read-only inventory and bounded reads of original asset inputs.

Supports extracted directories, MODE2/2352 data tracks, and ZIP files containing
such a track. The inventory never writes to the input.
"""
import hashlib
import os
import re
import struct
import zipfile

SECTOR = 2352


class InputError(ValueError):
    """Input is malformed, unsafe, or unsupported."""


class _Raw:
    def __init__(self, f, size):
        self.f, self.size = f, size
        if size % SECTOR:
            raise InputError("MODE2 image length is not a whole number of sectors")
        self.sectors = size // SECTOR

    def sector(self, lba, directory=False):
        if lba < 0 or lba >= self.sectors:
            raise EOFError("LBA %d outside data track" % lba)
        self.f.seek(lba * SECTOR)
        header = self.f.read(16)
        if len(header) != 16 or header[:12] != b"\x00" + b"\xff" * 10 + b"\x00":
            raise InputError("invalid raw sector header at LBA %d" % lba)
        if header[15] != 2:
            raise InputError("sector at LBA %d is not MODE2" % lba)
        self.f.seek(lba * SECTOR + 16)
        sub = self.f.read(8)
        if len(sub) != 8:
            raise EOFError("short sector at LBA %d" % lba)
        if sub[:4] != sub[4:]:
            if not directory:
                raise InputError("invalid duplicated XA subheader at file LBA %d" % lba)
            self.f.seek(lba * SECTOR + 16)
            return self.f.read(2048), False, True
        form2 = bool(sub[2] & 0x20)
        return self.f.read(2324 if form2 else 2048), form2, False

    def extent(self, lba, size, directory=False):
        buf = bytearray()
        records = []
        pos = 0
        while pos < size:
            payload, form2, fallback = self.sector(lba + len(records), directory=directory)
            records.append({"lba": lba + len(records), "payload_bytes": len(payload), "form2": form2,
                            "directory_subheader_fallback": fallback})
            if form2:
                return None, records
            buf.extend(payload)
            pos += len(payload)
        return bytes(buf[:size]), records


class InputSource:
    def __init__(self, path, kind, f=None, owner=None, base=None):
        self.path, self.kind, self._f, self._owner, self._base = path, kind, f, owner, base
        self._entries = None
        self._description = {"format": kind, "source": os.path.abspath(path), "source_sha256": _path_hash(path), "read_only": True}

    def describe(self):
        return dict(self._description)

    def entries(self):
        if self._entries is None:
            self._entries = self._directory_entries() if self.kind == "directory" else self._iso_entries()
        return [dict(e) for e in self._entries]

    def read(self, entry):
        if entry.get("kind") != "file":
            raise InputError("entry is not a readable file: %s" % entry.get("unsupported", "unsupported extent"))
        if self.kind == "directory":
            full = os.path.realpath(os.path.join(self.path, *entry["path"].split("/")))
            if os.path.commonpath((os.path.realpath(self.path), full)) != os.path.realpath(self.path):
                raise InputError("path escapes input directory")
            with open(full, "rb") as f:
                data = f.read()
        else:
            data, _ = self._raw.extent(entry["location"]["lba"], entry["size"])
            if data is None:
                raise InputError("Form2 stream cannot be read as an ISO file")
        if len(data) != entry["size"]:
            raise EOFError("short file: %s" % entry["path"])
        return data

    def _directory_entries(self):
        root = os.path.realpath(self.path)
        out = []
        for current, dirs, files in os.walk(root, followlinks=False):
            linked_dirs = [d for d in dirs if os.path.islink(os.path.join(current, d))]
            dirs[:] = sorted(d for d in dirs if d not in linked_dirs)
            for name in sorted(files + linked_dirs):
                full = os.path.join(current, name)
                rel = os.path.relpath(full, root).replace(os.sep, "/")
                if os.path.islink(full):
                    out.append({"path": rel, "size": 0, "kind": "unsupported", "unsupported": "symlink"})
                    continue
                st = os.stat(full, follow_symlinks=False)
                if not os.path.isfile(full):
                    out.append({"path": rel, "size": st.st_size, "kind": "unsupported", "unsupported": "non-regular file"})
                else:
                    out.append({"path": rel, "size": st.st_size, "kind": "file", "location": {"path": rel}})
        for e in out:
            if e["kind"] == "file":
                e["sha256"] = hashlib.sha256(self.read(e)).hexdigest()
        return out

    def _iso_entries(self):
        self._raw = _Raw(self._f, _size(self._f))
        pvd, form2, _fallback = self._raw.sector(16)
        if form2 or pvd[0] != 1 or pvd[1:6] != b"CD001":
            raise InputError("LBA 16 is not an ISO9660 primary volume descriptor")
        volume_le, volume_be = struct.unpack_from("<I", pvd, 80)[0], struct.unpack_from(">I", pvd, 84)[0]
        if not volume_le or volume_le != volume_be:
            raise InputError("invalid ISO volume-space bounds")
        volume_bound = min(volume_le, self._raw.sectors)
        self._description["declared_volume_sectors"] = volume_le
        self._description["available_track_sectors"] = self._raw.sectors
        root = pvd[156:190]
        if len(root) != 34 or root[0] < 34 or root[32] != 1 or root[33] != 0:
            raise InputError("invalid ISO root directory record")
        root_lba, root_size = struct.unpack_from("<I", root, 2)[0], struct.unpack_from("<I", root, 10)[0]
        if not root_size or root_lba >= volume_bound or (root_size + 2047) // 2048 > volume_bound - root_lba:
            raise InputError("ISO root directory extent is outside volume bounds")
        todo = [(root_lba, root_size, "", 0)]
        out, seen = [], set()
        while todo:
            lba, size, prefix, depth = todo.pop(0)
            if depth > 16 or (lba, size, prefix) in seen:
                raise InputError("directory recursion or cycle at %s" % (prefix or "/"))
            if lba >= volume_bound or (size + 2047) // 2048 > volume_bound - lba:
                raise InputError("directory extent is outside ISO volume bounds: %s" % (prefix or "/"))
            seen.add((lba, size, prefix))
            raw, ext = self._raw.extent(lba, size, directory=True)
            if raw is None:
                raise InputError("directory uses Form2 sectors: %s" % (prefix or "/"))
            pos = 0
            while pos < len(raw):
                n = raw[pos]
                if not n:
                    pos = ((pos // 2048) + 1) * 2048
                    continue
                rec = raw[pos:pos+n]
                if len(rec) < 34 or rec[0] != n or pos + n > len(raw):
                    raise InputError("malformed directory record at LBA %d" % lba)
                rlba, rsize = struct.unpack_from("<I", rec, 2)[0], struct.unpack_from("<I", rec, 10)[0]
                flags, file_unit, interleave_gap, namelen = rec[25], rec[26], rec[27], rec[32]
                rawname = rec[33:33+namelen]
                if 33 + namelen > n:
                    raise InputError("directory record name exceeds record length at LBA %d" % lba)
                pos += n
                if rawname in (b"\x00", b"\x01"):
                    continue
                name = rawname.decode("ascii", "replace").split(";", 1)[0]
                if not name or name in (".", "..") or "/" in name or "\\" in name:
                    raise InputError("unsafe ISO path component")
                path = (prefix + "/" if prefix else "") + name
                loc = {"lba": rlba, "record_lba": lba, "record_offset": pos-n, "extent_bytes": rsize}
                if flags & 0x80:
                    # ISO multi-extent files are represented by consecutive records.
                    segments = [loc]
                    total_size = rsize
                    while flags & 0x80:
                        if pos >= len(raw) or raw[pos] < 34:
                            raise InputError("unterminated multi-extent entry %s" % path)
                        next_rec = raw[pos:pos + raw[pos]]
                        next_name_len = next_rec[32] if len(next_rec) >= 34 else 0
                        next_name = next_rec[33:33 + next_name_len].decode("ascii", "replace").split(";", 1)[0]
                        if next_name != name or len(next_rec) < 34 or next_rec[0] != len(next_rec):
                            raise InputError("malformed multi-extent sequence for %s" % path)
                        next_lba, next_size = struct.unpack_from("<I", next_rec, 2)[0], struct.unpack_from("<I", next_rec, 10)[0]
                        flags = next_rec[25]
                        segments.append({"lba": next_lba, "record_lba": lba, "record_offset": pos,
                                         "extent_bytes": next_size})
                        total_size += next_size
                        pos += len(next_rec)
                    out.append({"path": path, "size": total_size, "kind": "unsupported",
                                "unsupported": "ISO multi-extent file is not joined by this reader",
                                "location": {"extents": segments}})
                elif file_unit or interleave_gap:
                    out.append({"path": path, "size": rsize, "kind": "unsupported",
                                "unsupported": "ISO interleaved extent is not supported", "location": loc})
                elif flags & 2:
                    todo.append((rlba, rsize, path, depth+1))
                elif rlba >= self._raw.sectors:
                    out.append({"path": path, "size": rsize, "kind": "unsupported", "unsupported": "extent begins beyond data-track image (CD-DA reference)", "location": loc})
                elif (rsize + 2047) // 2048 > self._raw.sectors - rlba:
                    available = self._raw.sectors - rlba
                    out.append({"path": path, "size": rsize, "kind": "unsupported",
                                "unsupported": "extent crosses the end of the data-track image",
                                "location": dict(loc, available_sectors=available,
                                                 sectors_outside_track=(rsize + 2047) // 2048 - available)})
                else:
                    data, records = self._raw.extent(rlba, rsize)
                    if data is None:
                        stream_sectors = (rsize + 2047) // 2048
                        stream_records = [{"lba": rlba+i, "byte_offset": (rlba+i)*SECTOR, "sector_bytes": SECTOR}
                                          for i in range(min(stream_sectors, self._raw.sectors - rlba))]
                        out.append({"path": path, "size": rsize, "kind": "unsupported", "unsupported": "Form2 XA/STR stream; ISO byte size does not describe payload", "location": dict(loc, sectors=stream_records)})
                        if len(stream_records) != stream_sectors:
                            out[-1]["location"]["sectors_outside_track"] = stream_sectors - len(stream_records)
                    else:
                        out.append({"path": path, "size": rsize, "kind": "file", "location": dict(loc, sectors=records), "sha256": hashlib.sha256(data).hexdigest()})
        return sorted(out, key=lambda e: e["path"])

    def close(self):
        if self._f is not None:
            self._f.close()
        if self._owner is not None:
            self._owner.close()


def _size(f):
    f.seek(0, 2)
    return f.tell()


def _path_hash(path):
    if os.path.isdir(path):
        h = hashlib.sha256()
        root = os.path.realpath(path)
        for current, dirs, files in os.walk(root, followlinks=False):
            linked_dirs = [d for d in dirs if os.path.islink(os.path.join(current, d))]
            dirs[:] = sorted(d for d in dirs if d not in linked_dirs)
            for name in sorted(files + linked_dirs):
                full = os.path.join(current, name)
                rel = os.path.relpath(full, root).replace(os.sep, "/")
                encoded = rel.encode("utf-8")
                h.update(struct.pack(">I", len(encoded)) + encoded)
                if os.path.islink(full):
                    target = os.readlink(full).encode("utf-8", "surrogateescape")
                    h.update(b"L" + struct.pack(">I", len(target)) + target)
                elif os.path.isfile(full):
                    size = os.path.getsize(full)
                    h.update(b"F" + struct.pack(">Q", size))
                    with open(full, "rb") as f:
                        for block in iter(lambda: f.read(1 << 20), b""):
                            h.update(block)
                else:
                    h.update(b"O")
        return h.hexdigest()
    return _file_hash(path)

def _file_hash(path):
    h = hashlib.sha256()
    with open(path, "rb") as f:
        for block in iter(lambda: f.read(1 << 20), b""):
            h.update(block)
    return h.hexdigest()


class _Context:
    def __init__(self, source): self.source = source
    def __enter__(self): return self.source
    def __exit__(self, *_): self.source.close()


def open_input(path):
    """Open directory, raw MODE2/2352 image, or ZIP data track read-only."""
    path = os.path.abspath(path)
    if os.path.isdir(path):
        return _Context(InputSource(path, "directory"))
    if path.lower().endswith(".zip"):
        z = zipfile.ZipFile(path)
        members = [i for i in z.infolist() if not i.is_dir()]
        candidates = [i for i in members if i.filename.lower().endswith((".bin", ".img"))]
        if not candidates:
            z.close()
            raise InputError("ZIP has no .bin/.img data track")
        cue_names = sorted((i for i in members if i.filename.lower().endswith(".cue")),
                           key=lambda item: item.filename)
        member = None
        if len(cue_names) > 1:
            z.close()
            raise InputError("ZIP has multiple CUE files; track selection is ambiguous")
        if cue_names:
            cue_text = z.read(cue_names[0]).decode("latin1")
            match = re.search(r'^\s*FILE\s+"([^"]+)"', cue_text, re.IGNORECASE | re.MULTILINE)
            if not match:
                z.close()
                raise InputError("ZIP CUE has no quoted first FILE track")
            cue_name = match.group(1).replace("\\", "/")
            if cue_name.startswith("/") or any(part in ("", ".", "..") for part in cue_name.split("/")):
                z.close()
                raise InputError("unsafe CUE track path")
            matches = [i for i in candidates if i.filename.casefold() == cue_name.casefold()]
            if not matches:
                matches = [i for i in candidates if i.filename.rsplit("/", 1)[-1].casefold() == cue_name.rsplit("/", 1)[-1].casefold()]
            if len(matches) != 1:
                z.close()
                raise InputError("CUE first FILE track is missing or ambiguous in ZIP")
            member = matches[0]
        else:
            raw_candidates = []
            for candidate in candidates:
                track = z.open(candidate)
                head = track.read(12)
                track.close()
                if head == b"\x00" + b"\xff" * 10 + b"\x00":
                    raw_candidates.append(candidate)
            marked = [i for i in raw_candidates if "(track 1)" in i.filename.lower()]
            if len(marked) == 1:
                member = marked[0]
            elif len(raw_candidates) == 1:
                member = raw_candidates[0]
            else:
                z.close()
                raise InputError("ZIP has ambiguous raw data tracks and no CUE selector")
        f = z.open(member)
        size = member.file_size
        head = f.read(12); f.seek(0)
        if head != b"\x00" + b"\xff"*10 + b"\x00":
            f.close(); z.close()
            raise InputError("ZIP data track is not raw MODE2/2352")
        source = InputSource(path, "zip-mode2-2352", f, z)
        source._description.update({"track_member": member.filename, "track_size": size, "other_members": sorted(i.filename for i in z.infolist() if not i.is_dir() and i.filename != member.filename)})
        return _Context(source)
    f = open(path, "rb")
    size = _size(f)
    f.seek(0)
    head = f.read(12); f.seek(0)
    if head != b"\x00" + b"\xff"*10 + b"\x00":
        f.close()
        raise InputError("unsupported image: expected raw MODE2/2352 sync header")
    return _Context(InputSource(path, "mode2-2352", f))
