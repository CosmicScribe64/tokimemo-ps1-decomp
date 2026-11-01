"""Raw CD-ROM sectors (2352 bytes): Mode 2 XA subheaders, EDC and ECC (T-39501).

Layout of a Mode 2 sector (ECMA-130, CD-ROM XA):
  0..11   sync (00, ten FF, 00)
  12..15  header: minute, second, frame (BCD, absolute time incl. the 2 s pregap), mode (2)
  16..23  subheader: file, channel, submode, coding, repeated once
  Form 1 (submode bit 0x20 clear): 2048 data at 24, EDC at 2072, ECC P at 2076 (172), Q at 2248 (104)
  Form 2 (submode bit 0x20 set):   2324 data at 24, EDC at 2348 (zero means "not computed")

EDC is a CRC-32 (polynomial 0x8001801B, bit-reversed, init 0, no final xor). ECC is the
Reed-Solomon product code over GF(2^8) (polynomial 0x11D) of ECMA-130 annex A; for Mode 2 the
four header bytes count as zero. The ECC is linear, so each P column and Q diagonal is a sum of
its bytes times powers of 2; computed here a row at a time with bytes.translate tables.
"""
import operator

SIZE = 2352
SYNC = b"\x00" + b"\xff" * 10 + b"\x00"
DATA = 24
FORM1_DATA = 2048
FORM2_DATA = 2324
FORM1_EDC = 2072
FORM2_EDC = 2348
P_OFF = 2076
Q_OFF = 2248
PREGAP = 150  # 2 s of 75 frames before LBA 0

SUB_EOR = 0x01
SUB_VIDEO = 0x02
SUB_AUDIO = 0x04
SUB_DATA = 0x08
SUB_FORM2 = 0x20
SUB_EOF = 0x80


def _edc_table():
    table = []
    for i in range(256):
        v = i
        for _ in range(8):
            v = (v >> 1) ^ (0xD8018001 if v & 1 else 0)
        table.append(v)
    return table


EDC_TABLE = _edc_table()


def edc(data, crc=0):
    """CD-ROM EDC of `data` (int, store little-endian)."""
    table = EDC_TABLE
    for b in data:
        crc = (crc >> 8) ^ table[(crc ^ b) & 0xFF]
    return crc


def _gf_tables():
    """exp/mul tables of GF(2^8) mod x^8+x^4+x^3+x^2+1 (0x11D)."""
    mul2 = [((i << 1) ^ (0x11D if i & 0x80 else 0)) & 0xFF for i in range(256)]
    div3 = [0] * 256  # inverse of x -> 3x (x ^ 2x)
    for i in range(256):
        div3[i ^ mul2[i]] = i
    powers = [bytes(range(256))]  # powers[k][x] = x * 2^k
    for _ in range(64):
        powers.append(bytes(mul2[x] for x in powers[-1]))
    return bytes(mul2), bytes(div3), powers


MUL2, DIV3, POW2 = _gf_tables()


def _rows(major_count, minor_count, major_mult, minor_inc):
    """Byte indices (into sector[12:]) of each ECC row: row m holds byte m of every column."""
    size = major_count * minor_count
    cols = []
    for major in range(major_count):
        index = (major >> 1) * major_mult + (major & 1)
        col = []
        for _ in range(minor_count):
            col.append(index)
            index += minor_inc
            if index >= size:
                index -= size
        cols.append(col)
    return [operator.itemgetter(*[cols[j][m] for j in range(major_count)]) for m in range(minor_count)]


P_ROWS = _rows(86, 24, 2, 86)
Q_ROWS = _rows(52, 43, 86, 88)


def _xor(a, b):
    return bytes(x ^ y for x, y in zip(a, b))


def _parity(src, rows):
    """Two parity bytes per column (ecc_computeblock of the reference ECM code)."""
    n = len(rows)
    count = len(rows[0](src))
    acc_a = 0
    acc_b = 0
    for m, row in enumerate(rows):
        line = bytes(row(src))
        acc_a ^= int.from_bytes(line.translate(POW2[n - m]), "little")
        acc_b ^= int.from_bytes(line, "little")
    a = acc_a.to_bytes(count, "little")
    b = acc_b.to_bytes(count, "little")
    a = _xor(a.translate(MUL2), b).translate(DIV3)
    return a + _xor(a, b)


def ecc(sector, zero_address=True):
    """P and Q parity (276 bytes) of a Mode 2 Form 1 sector. Mode 2 counts the header as zero;
    zero_address=False keeps it (the Mode 1 rule, which the last sector of track 1 was made with)."""
    src = bytearray(sector[12:Q_OFF])
    if zero_address:
        src[0:4] = b"\0\0\0\0"
    p = _parity(src, P_ROWS)
    src[P_OFF - 12:Q_OFF - 12] = p
    q = _parity(src, Q_ROWS)
    return p + q


def is_form2(sector):
    return bool(sector[18] & SUB_FORM2)


def has_subheader(sector):
    """A valid XA subheader is two identical 4-byte halves."""
    return sector[16:20] == sector[20:24]


def lba_header(lba, mode=2):
    """The 4 header bytes of `lba`: BCD minute/second/frame of lba+150, then the mode."""
    a = lba + PREGAP
    m, rest = divmod(a, 75 * 60)
    s, f = divmod(rest, 75)
    bcd = lambda v: ((v // 10) << 4) | (v % 10)
    return bytes((bcd(m), bcd(s), bcd(f), mode))


def finish_form1(sector):
    """Recompute EDC and ECC of a Mode 2 Form 1 sector in place (bytearray of 2352)."""
    sector[FORM1_EDC:FORM1_EDC + 4] = edc(sector[16:FORM1_EDC]).to_bytes(4, "little")
    sector[P_OFF:SIZE] = ecc(sector)


def finish_form2(sector):
    """Recompute the EDC of a Mode 2 Form 2 sector in place, unless it is the "none" value 0."""
    if sector[FORM2_EDC:SIZE] != b"\0\0\0\0":
        sector[FORM2_EDC:SIZE] = edc(sector[16:FORM2_EDC]).to_bytes(4, "little")


def make_form1(lba, subheader, data):
    """A complete Mode 2 Form 1 sector: sync, header, subheader (4 bytes, doubled), data, EDC, ECC."""
    if len(data) > FORM1_DATA:
        raise ValueError("Form 1 payload is %d bytes" % len(data))
    sector = bytearray(SIZE)
    sector[0:12] = SYNC
    sector[12:16] = lba_header(lba)
    sector[16:24] = bytes(subheader) * 2
    sector[DATA:DATA + len(data)] = data
    finish_form1(sector)
    return sector


def form1_valid(sector):
    """True if the EDC and ECC of a Mode 2 Form 1 sector match its contents."""
    if edc(sector[16:FORM1_EDC]).to_bytes(4, "little") != sector[FORM1_EDC:FORM1_EDC + 4]:
        return False
    return ecc(sector) == sector[P_OFF:SIZE]
