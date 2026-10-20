"""Restore the input-byte scalar documented in the earlier name-copy draft."""
from pathlib import Path
from probe import probe
name='func_80138C1C';base=Path('build/T-24515/func_80138C1C/typed-date-stores/body.c').read_text()
char=base.replace('    u8 buf[0x18];', '    u8 buf[0x18];\n    u8 c;')
char=char.replace('        if ((p[0] != sp0 || p[1] != sp1) && (p[0] != 0 || p[1] != 0))', '        c = p[0];\n        if ((c != sp0 || p[1] != sp1) && (c != 0 || p[1] != 0))')
for field in ['unk_0D4','unk_0DC','unk_0E4']:
    char=char.replace(field+'[n * 2] = p[0];',field+'[n * 2] = c;')
probe(name,'input-byte-local',char)
# Keep the original known-global endpoint, now avoiding the unsuitable direct-read lifetime.
probe(name,'input-byte-original-end',char.replace('p != &D_8014CC84[10]', 'p != &D_8014CC8E'))
