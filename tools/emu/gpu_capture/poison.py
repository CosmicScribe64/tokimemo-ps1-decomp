"""T-39302: write deterministic noise into rectangles of the VRAM of a save state (same length, so
the protobuf framing is untouched). Used for the noise-background run: the state taken when
DrawOTag n-1 returns gets noise in the given rectangles (capture_frame.sh derives them); the emulator then resumes, the game builds
and draws the next ordering table over the noise, and the capture of that DrawOTag is compared
with the renderer. Pixels the frame does not cover stay noise on both sides; covered and blended
pixels must agree exactly.

usage: python3 -I poison.py <in.sstate> <out.sstate> x0,y0,x1,y1 [x0,y0,x1,y1 ...]   (inclusive)
"""
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import extract  # noqa: E402


def main(argv):
    data = bytearray(Path(argv[1]).read_bytes())
    gpu = [v for n, w, v in extract.fields(data, 0, len(data)) if n == 5 and w == 2]
    if len(gpu) != 1:
        raise ValueError('expected one GPU block')
    spots = [v for n, w, v in extract.fields(data, *gpu[0]) if n == 3 and w == 2 and v[1] - v[0] == 0x100000]
    if len(spots) != 1:
        raise ValueError('expected one VRAM field')
    base = spots[0][0]
    seed = 0x2545F491
    for spec in argv[3:]:
        x0, y0, x1, y1 = (int(v) for v in spec.split(','))
        for y in range(y0, y1 + 1):
            for x in range(x0, x1 + 1):
                seed = (seed * 1664525 + 1013904223) & 0xFFFFFFFF
                at = base + (y * 1024 + x) * 2
                data[at] = (seed >> 16) & 0xFF
                data[at + 1] = (seed >> 24) & 0xFF
    Path(argv[2]).write_bytes(bytes(data))


if __name__ == '__main__':
    main(sys.argv)
