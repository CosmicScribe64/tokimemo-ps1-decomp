"""Reproduce the final narrow call-ABI removal controls in Docker."""
from pathlib import Path
base = Path('drafts/T-25055/girl-profit-1.c').read_text()
base = base.replace('else if ((hp == 3) && (D_80140558 != g->unk_03F)',
                    'else if (((hp * 1) == 3) && (D_80140558 != g->unk_03F)')
out = Path('build/T-25055/controls')
out.mkdir(parents=True, exist_ok=True)
needle = 'strcat(sp78, D_800B3288[D_80140558], D_80140558);'
assert base.count(needle) == 2
for label, start, end in [('month-22', '    case 0x22:', '    case 0x23:'),
                          ('month-2f', '    case 0x2F:', '    case 0x30:')]:
    a, b = base.index(start), base.index(end)
    case = base[a:b]
    assert case.count(needle) == 1
    (out/(label+'.c')).write_text(base[:a]+case.replace(needle, 'strcat(sp78, D_800B3288[D_80140558]);')+base[b:])
(out/'month-both.c').write_text(base.replace(needle,'strcat(sp78, D_800B3288[D_80140558]);'))
old = '(s32) "デートする場所を選んで下さい"'
assert base.count(old) == 2
(out/'typed-text.c').write_text(base.replace(old,'"デートする場所を選んで下さい"'))
