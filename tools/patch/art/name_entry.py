"""Extra texture storage and ABC frames for English name entry (T-39506).

The original NBN is loaded at 0x80190000. Its existing background pages and
sequence table keep their addresses. Seven added 4bpp pages and a 31-entry frame
table follow the original file; include/english/name_art.h describes the
same layout to NAME_ENT. The CD remapper grows the original whole-file read.
"""
import struct

from tools.patch.art.indexed import ArtError, Part, View

LOAD = 0x80190000
SIZE = 0x1B000
MARKER = 0x1AFF8
MAGIC = 0x4D4E454E
PAGE = 0x1B000
PAGE_COUNT = 8
TPAGES = PAGE + (PAGE_COUNT - 1) * 0x2000
FRAMES = TPAGES + PAGE_COUNT * 4
RECORDS = FRAMES + 31 * 4


def extended(data: bytes) -> bool:
    return len(data) > SIZE and struct.unpack_from("<I", data, MARKER)[0] == MAGIC


def prepare(data: bytes) -> bytes:
    """Add empty actor pages and two independent frames for the ABC tab."""
    if extended(data):
        return data
    if len(data) != SIZE or any(data[MARKER:MARKER + 4]):
        raise ArtError("NAME_ENT: unexpected extension marker storage")
    size = (RECORDS + 48 + 4 + 2047) // 2048 * 2048
    out = bytearray(data + bytes(size - len(data)))
    struct.pack_into("<I", out, MARKER, MAGIC)
    struct.pack_into("<%dI" % PAGE_COUNT, out, TPAGES, LOAD + 0x200,
                     *(LOAD + PAGE + i * 0x2000 for i in range(PAGE_COUNT - 1)))
    pointers = list(struct.unpack_from("<29I", data, 0x1AF5C))
    for frame, at in ((15, RECORDS), (16, RECORDS + 24)):
        source = pointers[frame] - LOAD
        count = struct.unpack_from("<h", data, source)[0]
        if count != 1:
            raise ArtError("NAME_ENT: expected one part in the selector template")
        out[at:at + 22] = data[source:source + 22]
        pointers.append(LOAD + at)
    struct.pack_into("<31I", out, FRAMES, *pointers)
    out[-4:] = data[-4:]
    return bytes(out)


def views(data: bytes, palette: tuple) -> list[View]:
    """Decode the extended table, including grown records and all texture pages."""
    result = []
    for index, pointer in enumerate(struct.unpack_from("<31I", data, FRAMES)):
        offset = pointer - LOAD
        if offset < 0 or offset + 8 > len(data) - 4:
            raise ArtError("NAME_ENT: frame outside the extended file")
        count = struct.unpack_from("<h", data, offset)[0]
        if not 0 <= count <= 128 or offset + 8 + 14 * count > len(data) - 4:
            raise ArtError("NAME_ENT: invalid extended frame extent")
        parts = []
        for n in range(count):
            record = offset + 8 + 14 * n
            x, y, texture, info, flip, width, height = struct.unpack_from("<2h3H2h", data, record)
            page = texture >> 8
            u, v = (texture & 15) * 8, ((texture & 255) >> 4) * 8
            if page >= PAGE_COUNT or not 0 < width <= 128 or not 0 < height <= 128 or u + width > 128 or v + height > 128:
                raise ArtError("NAME_ENT: invalid extended texture geometry")
            column = info & 0xF0
            parts.append(Part(x, y, width, height, flip, PAGE + (page - 1) * 0x2000 if page else 0x200, 4, u, v,
                              palette[column:column + 16], record, page, info))
        if parts:
            result.append(View.of("frame-%03d.base" % index, parts, frame=index, record=offset))
    return result
