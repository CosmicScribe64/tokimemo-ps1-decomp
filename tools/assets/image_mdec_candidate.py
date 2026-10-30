"""T-39008 diagnostic CLI for the reviewed fixed-point MDEC model.

Independent scalar expression of numeric stages reviewed in PSX-SPX and pinned
emulator references (see wiki/port-adversarial-review.md). This is not a hardware-accuracy claim; the shared exporter uses the same model.
Only validated BS1/2/3, positive quantization and source-proven geometry supported.
"""
import argparse
import hashlib
import json
from pathlib import Path
import struct

from tools.assets import image_mdec as preview


from tools.assets.image_mdec_model import (MATRIX, clamp, dequantize, coefficient_blocks,
                                           inverse_transform, color_word, decode_words)


def main(argv=None):
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--input',type=Path,required=True);p.add_argument('--sha256',required=True)
    p.add_argument('--width',type=int,required=True);p.add_argument('--height',type=int,required=True)
    p.add_argument('--out',type=Path,required=True)
    args=p.parse_args(argv);data=args.input.read_bytes()
    if hashlib.sha256(data).hexdigest()!=args.sha256:raise ValueError('candidate original identity differs')
    words=decode_words(data,args.width,args.height)
    with args.out.open('xb') as stream:stream.write(words)
    print(json.dumps({'scope':'reviewed numeric model; no hardware or Redux parity claim',
                      'input_sha256':args.sha256,'width':args.width,'height':args.height,
                      'output_sha256':hashlib.sha256(words).hexdigest(),'encoding':'little-endian RGB555 with STP=1'}))
    return 0


if __name__=='__main__':raise SystemExit(main())
