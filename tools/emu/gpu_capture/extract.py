"""T-39302: pull the GPU block (status word, GP1 control words, 1 MiB VRAM) out of PCSX-Redux v4
save states written by capture.lua, and bundle a capture for the renderer comparison.

usage: python3 -I extract.py <capture-dir> [<tag> ...]

For each tag (default: every <tag>.before.sstate) writes next to the states
  <tag>.vram_before.bin  <tag>.vram_after.bin   raw 1024x512 little-endian 16-bit words
  <tag>.gpu.json         ot address, GPUSTAT before/after, GP1 control words, display registers
  <tag>.cmp.args         the --ot/--status arguments for compare_capture
The RAM image <tag>.ram.bin is written by capture.lua itself. All of it is game data: it stays
under .asset-cache and is never committed.
"""
import json
import struct
import sys
from pathlib import Path


def varint(buf, i):
    value = shift = 0
    while True:
        byte = buf[i]
        i += 1
        value |= (byte & 0x7F) << shift
        shift += 7
        if not byte & 0x80:
            return value, i


def fields(buf, lo, hi):
    """Yield (field number, wire type, value or (start, end)) for one protobuf message level."""
    i = lo
    while i < hi:
        tag, i = varint(buf, i)
        number, wire = tag >> 3, tag & 7
        if wire == 0:
            value, i = varint(buf, i)
            yield number, wire, value
        elif wire == 2:
            length, i = varint(buf, i)
            yield number, wire, (i, i + length)
            i += length
        elif wire == 5:
            yield number, wire, struct.unpack_from('<I', buf, i)[0]
            i += 4
        elif wire == 1:
            yield number, wire, struct.unpack_from('<Q', buf, i)[0]
            i += 8
        else:
            raise ValueError('unsupported wire type %d' % wire)


def gpu_block(path):
    data = Path(path).read_bytes()
    gpu = [v for n, w, v in fields(data, 0, len(data)) if n == 5 and w == 2]
    if len(gpu) != 1:
        raise ValueError('%s: expected one GPU block, found %d' % (path, len(gpu)))
    status = control = vram = None
    for n, w, v in fields(data, *gpu[0]):
        if n == 1 and w == 0:
            status = v
        elif n == 2 and w == 2:
            control = struct.unpack_from('<256I', data, v[0]) if v[1] - v[0] == 1024 else None
        elif n == 3 and w == 2 and v[1] - v[0] == 0x100000:
            vram = data[v[0]:v[1]]
    if status is None or control is None or vram is None:
        raise ValueError('%s: GPU block lacks status, control or VRAM' % path)
    return status, control, vram


def main(argv):
    root = Path(argv[1])
    tags = argv[2:] or sorted(p.name[:-len('.before.sstate')] for p in root.glob('*.before.sstate'))
    infos = {}
    for line in (root / 'captures.jsonl').read_text().splitlines():
        record = json.loads(line)
        infos[record['tag']] = record
    for tag in tags:
        before = gpu_block(root / (tag + '.before.sstate'))
        after = gpu_block(root / (tag + '.after.sstate'))
        (root / (tag + '.vram_before.bin')).write_bytes(before[2])
        (root / (tag + '.vram_after.bin')).write_bytes(after[2])
        record = dict(infos.get(tag, {}))
        record.update({
            'status_before': before[0], 'status_after': after[0],
            'control_before': {str(i): v for i, v in enumerate(before[1]) if v},
            'control_after': {str(i): v for i, v in enumerate(after[1]) if v},
        })
        (root / (tag + '.gpu.json')).write_text(json.dumps(record, indent=1) + '\n')
        (root / (tag + '.cmp.args')).write_text('--ot %s --status %08x\n' % (record.get('ot', '0'), before[0]))
        diff = sum(1 for a, b in zip(before[2], after[2]) if a != b)
        print('%s: ot=%s status %08x -> %08x, %d VRAM bytes differ' % (
            tag, record.get('ot'), before[0], after[0], diff))


if __name__ == '__main__':
    main(sys.argv)
