"""Raw sectors of a PS1 STR movie: MDEC video frames interleaved with XA audio (T-39509).

The stream is read and written as raw 2352-byte sectors (tools/patch/sector.py). Facts this
relies on, checked on read (wiki/movie-patch.md):

* Video sectors are Mode 2 Form 1 with the video submode bit (0x02). Their 2048 data bytes:

  | offset | size | field |
  |---|---|---|
  | 0x00 | 2 | 0x0160 |
  | 0x02 | 2 | 0x8001 (MDEC frame data) |
  | 0x04 | 2 | chunk index in the frame |
  | 0x06 | 2 | chunk count of the frame |
  | 0x08 | 4 | frame number (from 1) |
  | 0x0C | 4 | frame data bytes (BS stream, a multiple of 4) |
  | 0x10 | 2+2 | width, height |
  | 0x14 | 8 | copy of the BS header (MDEC words, 0x3800, quantiser, version) |
  | 0x1C | 4 | zero |
  | 0x20 | 2016 | frame data, zero after its end |

* Every other sector (XA audio, Form 2, and the empty sectors at the end) is never rewritten.
* A frame keeps its sectors: a new frame's data must fit chunk count x 2016 bytes, and its
  sectors keep their position, sync, address and subheader. Only the data bytes and EDC/ECC
  change, so the interleave and with it the timing of the stream stay the same.
"""
import struct
from collections import OrderedDict

from tools.patch import sector as cs

CHUNK = cs.FORM1_DATA - 0x20
HEADER = struct.Struct('<HHHHIIHH8s4s')
MAGIC = (0x0160, 0x8001)


class StrError(ValueError):
    """The stream does not have the layout this module rewrites, or a new frame does not fit."""


class Frame:
    """One video frame: its sector indices in the extent and its header fields."""

    def __init__(self, number, chunks, size, width, height, bs_header):
        self.number = number
        self.sectors = []
        self.chunks = chunks
        self.size = size
        self.width = width
        self.height = height
        self.bs_header = bs_header

    @property
    def capacity(self):
        return len(self.sectors) * CHUNK


class Stream:
    """An STR extent (raw sectors). `raw` is bytes, `lba` the first sector's address."""

    def __init__(self, raw, lba):
        if len(raw) % cs.SIZE:
            raise StrError('extent is not whole raw sectors')
        self.raw = bytes(raw)
        self.lba = lba
        self.count = len(raw) // cs.SIZE
        self.frames = OrderedDict()
        self.kinds = []  # per sector: 'video', 'audio' or 'other'
        for i in range(self.count):
            s = self.sector(i)
            if s[:12] != cs.SYNC or s[12:16] != cs.lba_header(lba + i):
                raise StrError('sector %d: sync or address differs' % i)
            if cs.has_subheader(s) and not cs.is_form2(s) and s[18] & cs.SUB_VIDEO:
                self.kinds.append('video')
                self._add_chunk(i, s)
            elif cs.has_subheader(s) and s[18] & cs.SUB_AUDIO:
                self.kinds.append('audio')
            else:
                self.kinds.append('other')
        for f in self.frames.values():
            if len(f.sectors) != f.chunks:
                raise StrError('frame %d has %d of %d chunks' % (f.number, len(f.sectors), f.chunks))
            data = self._payload(f)
            if data[f.size:].strip(b'\0'):
                raise StrError('frame %d: bytes after the frame data are not zero' % f.number)

    def sector(self, i):
        return self.raw[i * cs.SIZE:(i + 1) * cs.SIZE]

    def _add_chunk(self, i, s):
        status, kind, index, chunks, number, size, width, height, bs_header, zero = HEADER.unpack_from(s, cs.DATA)
        if (status, kind) != MAGIC or zero != b'\0\0\0\0':
            raise StrError('sector %d: not an MDEC frame chunk' % i)
        f = self.frames.get(number)
        if f is None:
            f = self.frames[number] = Frame(number, chunks, size, width, height, bs_header)
        if index != len(f.sectors) or (chunks, size, width, height, bs_header) != (f.chunks, f.size, f.width, f.height, f.bs_header):
            raise StrError('sector %d: chunk %d of frame %d out of order or inconsistent' % (i, index, number))
        if not cs.form1_valid(s):
            raise StrError('sector %d: EDC/ECC differ' % i)
        f.sectors.append(i)

    def _payload(self, f):
        return b''.join(self.sector(i)[cs.DATA + 0x20:cs.DATA + cs.FORM1_DATA] for i in f.sectors)

    def frame_data(self, number):
        """The BS bytes of frame `number` (size from the header)."""
        f = self.frames[number]
        return self._payload(f)[:f.size]

    def frame_sectors(self, number, data):
        """{sector index: new raw sector} for frame `number` holding `data`."""
        f = self.frames[number]
        if len(data) % 4 or len(data) < 8:
            raise StrError('frame %d: data must be a multiple of 4 bytes' % number)
        if len(data) > f.capacity:
            raise StrError('frame %d: %d bytes do not fit %d sectors (%d bytes)' % (
                number, len(data), len(f.sectors), f.capacity))
        padded = bytes(data) + bytes(f.capacity - len(data))
        out = {}
        for index, i in enumerate(f.sectors):
            s = bytearray(self.sector(i))
            HEADER.pack_into(s, cs.DATA, MAGIC[0], MAGIC[1], index, len(f.sectors), number, len(data),
                             f.width, f.height, bytes(data[:8]), b'\0\0\0\0')
            s[cs.DATA + 0x20:cs.DATA + cs.FORM1_DATA] = padded[index * CHUNK:(index + 1) * CHUNK]
            cs.finish_form1(s)
            out[i] = bytes(s)
        return out

    def rewrite(self, frames):
        """New raw extent with frames {number: data} replaced. Returns (bytes, sector indices that
        changed). Every other sector is copied unchanged."""
        sectors = {}
        for number, data in frames.items():
            if number not in self.frames:
                raise StrError('no frame %d' % number)
            sectors.update(self.frame_sectors(number, data))
        out = bytearray(self.raw)
        changed = []
        for i in sorted(sectors):
            if self.kinds[i] != 'video':
                raise StrError('sector %d is not a video sector' % i)
            if sectors[i] != self.sector(i):
                out[i * cs.SIZE:(i + 1) * cs.SIZE] = sectors[i]
                changed.append(i)
        return bytes(out), changed
