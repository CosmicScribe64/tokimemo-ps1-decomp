#!/usr/bin/env python3
"""T-39008: reversible color-code diagnosis only; never a parity acceptance gate."""
import argparse
import hashlib
import json
from pathlib import Path

from PIL import Image


def decode_channel(value, encoding):
    code = value >> 3
    forms = {"zero_extended": {code << 3}, "bit_replicated": {(code << 3) | (code >> 2)},
             "source_proven_mixed": {code << 3, (code << 3) | (code >> 2)}}
    if encoding not in forms or value not in forms[encoding]:
        raise ValueError("channel is outside declared reversible RGB555 encoding")
    return code


def compare_codes(native, port, native_encoding, port_encoding):
    if native.size != port.size:
        raise ValueError("RGB555 diagnostic requires identical dimensions; no rescaling")
    changed = 0
    deltas = {}
    for a,b in zip(native.convert('RGBA').getdata(),port.convert('RGBA').getdata()):
        if a[3] != 255 or b[3] != 255:
            raise ValueError("opaque final frames required; STP/alpha cannot be inferred")
        left = tuple(decode_channel(c,native_encoding) for c in a[:3])
        right = tuple(decode_channel(c,port_encoding) for c in b[:3])
        delta = tuple(y-x for x,y in zip(left,right))
        changed += left != right
        deltas[delta] = deltas.get(delta,0)+1
    return {"acceptance": False, "scope":"RGB555 color-code diagnostic only; original exact RGBA gate unchanged; bit15/STP not reconstructed",
            "native_encoding":native_encoding,"port_encoding":port_encoding,
            "pixels":native.width*native.height,"different_color_codes":changed,
            "deltas":[{"rgb":list(delta),"pixels":count} for delta,count in sorted(deltas.items())]}


def main(argv=None):
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('native',type=Path);parser.add_argument('port',type=Path)
    for side in ('native','port'):
        parser.add_argument('--'+side+'-encoding',required=True,choices=['zero_extended','bit_replicated','source_proven_mixed'])
    parser.add_argument('--basis',required=True,help='Source proof for the declared encodings; numeric membership alone is not provenance')
    args=parser.parse_args(argv)
    with Image.open(args.native) as native,Image.open(args.port) as port:
        result=compare_codes(native,port,args.native_encoding,args.port_encoding)
    result['basis']=args.basis
    result['raw_images']={side:{'path':str(path),'sha256':hashlib.sha256(path.read_bytes()).hexdigest()} for side,path in [('native',args.native),('port',args.port)]}
    print(json.dumps(result,sort_keys=True,indent=2))
    return 0


if __name__=='__main__':raise SystemExit(main())
