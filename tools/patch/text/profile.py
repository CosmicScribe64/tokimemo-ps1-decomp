"""Widen JOHO's English profile labels inside its existing load (T-39506).

The 13 club labels move to the zero tail at 0x12F00, with stride 18. The
12 zodiac labels reuse the old 156-byte club table at 0x12200, with stride 12.
Blood types, measurements, notes, image data and the file length stay intact.
The English TEL reader uses include/english/profile.h for these two layouts.
"""
from .image import ImageError, has_cd_trailer, write_cd_trailer

PATH = "CDROM/FUKUTEST/JOHO.NBN"
CLUB_OFFSET, CLUB_STRIDE, CLUB_COUNT = 0x12F00, 18, 13
ZODIAC_OFFSET, ZODIAC_STRIDE, ZODIAC_COUNT = 0x12200, 12, 12


def write_labels(original: bytes, result, payloads: dict[str, bytes]) -> None:
    """Update a WriteResult after ordinary in-place fields have been written."""
    if len(original) != 0x13000 or any(original[CLUB_OFFSET:-4]):
        raise ImageError("JOHO: English club storage is not the expected zero tail")
    data = bytearray(result.data)
    moved = set()
    for table, offset, stride, count in (("club_labels", CLUB_OFFSET, CLUB_STRIDE, CLUB_COUNT),
                                         ("zodiac_labels", ZODIAC_OFFSET, ZODIAC_STRIDE, ZODIAC_COUNT)):
        for index in range(count):
            record = f"dictionary:{PATH}:{table}:{index:02d}"
            if record not in payloads:
                raise ImageError(f"JOHO: no English payload for {record}")
            body = payloads[record] + b"\0"
            if len(body) > stride:
                raise ImageError(f"JOHO: {record} exceeds the English stride")
            start = offset + index * stride
            data[start:start + stride] = body.ljust(stride, b"\0")
            moved.add(record)
    if has_cd_trailer(original):
        write_cd_trailer(data)
    result.data = bytes(data)
    result.overflows = [o for o in result.overflows if o.get("id") not in moved]

