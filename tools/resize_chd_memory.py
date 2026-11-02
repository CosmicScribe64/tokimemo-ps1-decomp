#!/usr/bin/env python3
"""T-39527: give the pinned CHD WebAssembly tool a 128 MiB fixed heap."""
import argparse
import hashlib
from pathlib import Path


def resize(source: bytes) -> bytes:
    expected = 'da1d4a57af54c47e3069bb0af84144e465d98bfc2ea2c10f9e0fead3be081eda'
    if hashlib.sha256(source).hexdigest() != expected:
        raise ValueError('Expected the pinned, unmodified chdman.wasm')
    position = 8
    while position < len(source):
        start = position
        kind = source[position]
        position += 1
        size = shift = 0
        while True:
            value = source[position]
            position += 1
            size |= (value & 127) << shift
            if not value & 128:
                break
            shift += 7
        end = position + size
        if kind == 5:
            if source[position:end] != bytes([1, 1, 131, 2, 131, 2]):
                raise ValueError('Unexpected memory limits')
            return source[:start] + bytes([5, 6, 1, 1, 128, 16, 128, 16]) + source[end:]
        position = end
    raise ValueError('Missing memory section')


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('source', type=Path)
    parser.add_argument('output', type=Path)
    args = parser.parse_args()
    args.output.write_bytes(resize(args.source.read_bytes()))
