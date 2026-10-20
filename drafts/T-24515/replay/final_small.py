"""Narrow controls for remaining name-copy byte/endpoint scheduling differences."""
from pathlib import Path
from probe import probe
name='func_80138C1C';base=Path('build/T-24515/func_80138C1C/input-byte-local/body.c').read_text()
probe(name,'word-input-local',base.replace('    u8 c;', '    s32 c;'))
probe(name,'word-space-locals',base.replace('    u8 sp0;', '    s32 sp0;').replace('    u8 sp1;', '    s32 sp1;'))
probe(name,'ordered-third-unknown-end',base.replace('p != &D_8014CC84[10]', 'p < &D_8014CC8E'))
# Original third loop has no entry check and a direct separately labelled endpoint.
old='''    for (p = D_8014CC84; p != &D_8014CC84[10]; p += 2) {'''
new='''    p = D_8014CC84;
    do {'''
third=base.replace(old,new)
a=third.index('    p = D_8014CC84;');b=third.index('    func_800AE090',a)
part=third[a:b]
assert part.endswith('    }\n')
part=part[:-6]+'        p += 2;\n    } while (p != &D_8014CC8E);\n'
probe(name,'third-do-pointer-end',third[:a]+part+third[b:])
