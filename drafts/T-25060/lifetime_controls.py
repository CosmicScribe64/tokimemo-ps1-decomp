"""Generate bounded packed-word/coordinate lifetime controls (Docker only)."""
from pathlib import Path

root = Path('drafts/T-25060')
base = Path('drafts/T-24511/menu-reconstruction.c').read_text()
# arg0 is dead after the page-selection condition. Reuse its existing home to
# keep the three packed-word captures separate from the coordinate range.
arg = base.replace('x = *(u32 *)D_800E6280.unk_F64;',
                   'arg0 = *(u32 *)D_800E6280.unk_F64;')
arg = arg.replace('(x << 0x11)', '(arg0 << 0x11)')
arg = arg.replace('(x << 0x12)', '(arg0 << 0x12)')
(root / 'word-arg0.c').write_text(arg)
(root / 'word-arg0-short-x.c').write_text(arg.replace('    s32 x;', '    s16 x;'))
# The target retains a negative quotient followed by a negative multiply
# macro. A negative literal distinguishes that macro from unary C negation.
for name, source in [('base', base), ('arg0', arg)]:
    source = source.replace('(- -((s32)i / 5) * 0x88)',
                            '(-((s32)i / 5) * -0x88)')
    (root / ('negative-scale-' + name + '.c')).write_text(source)
# Isolated priority controls: reads are always false for these unsigned casts.
source = arg.replace('            *xs = x;',
                     '            if ((u32) xs < 0) { }\n            *xs = x;')
(root / 'arg0-xs-read.c').write_text(source)
source = arg.replace('            i += 1;',
                     '            if ((u32) i < 0) { }\n            i += 1;')
(root / 'arg0-index-read.c').write_text(source)
# The first three page-one loads were signed aliases, unlike the remaining
# packed-word reads. Test a single unsigned alias identity throughout.
for name, source in [('base', base), ('arg0', arg),
                     ('negative', (root / 'negative-scale-arg0.c').read_text())]:
    source = source.replace('*(s32 *)D_800E6280.unk_F64',
                            '*(u32 *)D_800E6280.unk_F64')
    (root / ('unsigned-loads-' + name + '.c')).write_text(source)
# Name only the page-one invariant base; page two retains direct addresses.
source = (root / 'unsigned-loads-negative.c').read_text()
head, tail = source.split('    } else {\n        set_kanji_string', 1)
head = head.replace('    GameState *flags;', '    GameState *flags;\n    GameState *state;')
head = head.replace('        flags = &D_800E6280;',
                    '        state = &D_800E6280;\n        flags = &D_800E6280;')
head = head.replace('D_800E6280.unk_F64', 'state->unk_F64')
(root / 'named-page1-base.c').write_text(head + '    } else {\n        set_kanji_string' + tail)
# Combine the independently tested priority reads only after the target
# negative-multiply macro is present, to inspect their interactions.
source = (root / 'unsigned-loads-negative.c').read_text()
source = source.replace('            *xs = x;',
                        '            if ((u32) xs < 0) { }\n            *xs = x;')
source = source.replace('            i += 1;',
                        '            if ((u32) i < 0) { }\n            i += 1;')
(root / 'negative-priorities.c').write_text(source)
# Reads between coordinate computations split their expression ranges. Put
# the same diagnostic uses after induction updates, outside those live values.
source = (root / 'unsigned-loads-negative.c').read_text()
source = source.replace('        } while (',
                        '            if ((u32) xs < 0) { }\n'
                        '            if ((u32) i < 0) { }\n        } while (')
(root / 'late-priorities.c').write_text(source)
# Test x-array alias provenance alone: let uopt derive its induction pointer
# from the array instead of an independently declared mutable source pointer.
source = (root / 'unsigned-loads-negative.c').read_text()
source = source.replace('    s16 *xs;\n', '')
source = source.replace('        xs = spC0;\n', '')
source = source.replace('        xs = &spC0[6];\n', '')
source = source.replace('            xs += 1;\n', '')
source = source.replace('*xs = x;', 'spC0[i] = x;')
(root / 'indexed-xs.c').write_text(source)
# Isolate whether the late loop alone supplies the saved pointer/index uses,
# without splitting the page-one caller-register pointer ranges.
source = (root / 'unsigned-loads-negative.c').read_text()
source = source.replace('            byteOffset += 2;\n            xs += 1;',
                        '            byteOffset += 2;\n            xs += 1;\n'
                        '            if ((u32) xs < 0) { }\n'
                        '            if ((u32) i < 0) { }')
(root / 'third-loop-priorities.c').write_text(source)
# Explicitly joining disjoint year/late-offset lifetimes tests the target's
# s4 reuse, now without the old captured-coordinate interference.
source = (root / 'unsigned-loads-negative.c').read_text()
source = source.replace('    s32 byteOffset;\n', '').replace('byteOffset', 'arg1')
(root / 'joined-year-offset.c').write_text(source)

# One predicate exposes both source uses without two successive condition
# blocks. This distinguishes range profitability from CFG splitting costs.
source = (root / 'unsigned-loads-negative.c').read_text()
source = source.replace('        } while (',
                        '            if (((u32) xs | (u32) i) < 0) { }\n        } while (')
(root / 'combined-latch-read.c').write_text(source)

# Keep every generated diagnostic defined for packed-word shifts and mark
# matching-specific identities. Source loop indices stay in 0..9, so their
# coordinate arithmetic cannot overflow. arg3 is the game's day-of-month.
for path in root.glob('*.c'):
    source = path.read_text()
    source = source.replace('*(s32 *)D_800E6280.unk_F64',
                            '*(u32 *)D_800E6280.unk_F64')
    for value in ('x', 'arg0'):
        source = source.replace('(' + value + ' <<', '((u32) ' + value + ' <<')
    source = source.replace('            if ((u32)',
                            '            /* FAKE: defined false read adjusts allocation. T-25060. */\n'
                            '            if ((u32)')
    source = source.replace('            if (((u32)',
                            '            /* FAKE: defined false read adjusts allocation. T-25060. */\n'
                            '            if (((u32)')
    source = source.replace(' * FAKE: x also captures the packed flag word before month comparisons to\n'
                            ' * recover its load placement without a new frame slot. T-24511.',
                            ' * FAKE: the packed-word captures preserve their observed early load\n'
                            ' * using an existing scalar lifetime. T-24511/T-25060.')
    path.write_text(source)
