"""Invented native pointer tables, dispatchers and stream provenance (Docker)."""
from copy import deepcopy
import hashlib
import struct
import unittest

from assets.media_background_steps import _EXPECTED, _SIGNATURES, build_background_step_contexts


def fixture():
    base = 0x80132000
    data = bytearray(0x1000)
    struct.pack_into('<4I', data, 0x100, base + 0x40, 0x80050000, 0x80050004, 0x80050008)
    native = {'CDROM/EXEDIR/INVENTED.EXN': bytes(data)}
    inventory = {'schema': 'tokimemo.media.v1', 'assets': [
        {'id': 'picture', 'source': {'path': 'invented.BS', 'size': 4096,
                                    'sha256': 'a' * 64, 'location': {'lba': 100}}},
        {'id': 'overlay', 'source': {'path': next(iter(native)), 'size': len(data),
                                   'sha256': hashlib.sha256(data).hexdigest(), 'location': {'lba': 200}}}]}
    helpers = '\n'.join('%s {%s}' % (_SIGNATURES[name], body) for name, body in _EXPECTED.items())
    sources = {'src/main/protocol.c': helpers + '\nint dec_bg_decode_to_vram(int layer) {'
               'if(layer==0){bg_still_decode_to_vram(0,0,320,88);}else if(layer==1){bg_still_decode_to_vram(0,0,320,160);}}',
               'src/ovl/INVENTED/scene.c': 'void load(void){bg_read_sub2(101);mode_step_next();}\n'
               'void dispatch(void){s32 index; SceneSteps handlers; handlers=steps;'
               'index=game_state.mode_step;handlers.callbacks[index]();}'}
    headers = {'include/ovl/INVENTED.h': 'typedef struct {void (*callbacks[4])();} SceneSteps; extern SceneSteps steps;'}
    configs = {'config/overlays/INVENTED.yaml': 'sha1: ' + hashlib.sha1(data).hexdigest() + '\noptions:\n'
               '  target_path: disc/files/CDROM/EXEDIR/INVENTED.EXN\n  symbol_addrs_path:\n'
               '    - config/invented.txt\nsegments:\n  - vram: 0x80132000\n'}
    symbols = {'config/invented.txt': 'steps = 0x80132100;\nload = 0x80132040;\ndispatch = 0x80132080;\n'
               'scene_wait_cd_read = 0x80050000;\ndec_init = 0x80050004;\nchange_dec_bg = 0x80050008;\n'}
    return inventory, sources, headers, configs, symbols, native


def result(*values):
    return build_background_step_contexts(*values)


class BackgroundStepTests(unittest.TestCase):
    def test_original_native_sequence_composes_legacy_contract_with_exact_offset(self):
        report = result(*fixture())
        self.assertEqual(report['schema'], 'tokimemo.background-context.v1')
        self.assertEqual(report['counts']['native_step_calls'], 1)
        stream = next(row for row in report['streams'] if row['parent_asset_id'] == 'picture')
        self.assertEqual(stream['source_offset'], 2048)
        self.assertEqual(stream['shape_candidates'], [{'width': 320, 'height': 160}])
        self.assertEqual(stream['context_status'], 'proved_single_shape')
        context = stream['contexts'][0]
        self.assertEqual(context['physical_read_bytes'], 10 * 2048)
        self.assertTrue(context['physical_read_exceeds_logical_file'])
        self.assertTrue(context['decoder_evidence'])
        self.assertEqual(context['native_step_evidence']['table_index'], 0)
        self.assertEqual(context['native_step_evidence']['field'], 'callbacks')
        self.assertFalse(report['source_coverage_complete'])

    def test_header_field_and_simple_direct_mode_step_index(self):
        args = list(fixture())
        args[2] = {key: text.replace('callbacks', 'f') for key, text in args[2].items()}
        args[1]['src/ovl/INVENTED/scene.c'] = ('void load(void){bg_read_sub2(101);mode_step_next();}\n'
            'void dispatch(void){SceneSteps local;local=steps;local.f[game_state.mode_step](0x80);}')
        self.assertEqual(result(*args)['counts']['native_step_calls'], 1)
        args[1]['src/ovl/INVENTED/scene.c'] = args[1]['src/ovl/INVENTED/scene.c'].replace('local.f[', 'local.callbacks[')
        self.assertEqual(result(*args)['counts']['native_step_calls'], 0)
        for alteration in ('s32 unused; s32 index;', 's32 mode_step_next; s32 index;'):
            args = list(fixture())
            args[1]['src/ovl/INVENTED/scene.c'] = args[1]['src/ovl/INVENTED/scene.c'].replace('s32 index;', alteration)
            self.assertEqual(result(*args)['counts']['native_step_calls'], 0)
        args = list(fixture())
        args[1]['src/ovl/INVENTED/scene.c'] = args[1]['src/ovl/INVENTED/scene.c'].replace('index', 'steps')
        self.assertEqual(result(*args)['counts']['native_step_calls'], 0)
        for old, new in (('void load(void)', 'void load(int bg_read_sub2)'),
                         ('void dispatch(void)', 'void dispatch(int game_state)')):
            args = list(fixture())
            args[1]['src/ovl/INVENTED/scene.c'] = args[1]['src/ovl/INVENTED/scene.c'].replace(old, new)
            self.assertEqual(result(*args)['counts']['native_step_calls'], 0)

    def test_mutations_aliases_and_ambiguous_dispatch_control_flow_are_unresolved(self):
        for suffix in ('steps.callbacks[0]=load;', 'void *p=&steps;',
                       'void mutation(void){steps.callbacks[0]=load;}'):
            args = list(fixture())
            args[1]['src/ovl/INVENTED/scene.c'] += '\n' + suffix
            self.assertEqual(result(*args)['counts']['native_step_calls'], 0)
        for alteration in ('if(index){handlers.callbacks[index]();}',
                           'handlers.callbacks[index+1]();',
                           'handlers.callbacks[index]();mode_step_next();',
                           'handlers.callbacks[game_state.mode_stage]();'):
            args = list(fixture())
            args[1]['src/ovl/INVENTED/scene.c'] = args[1]['src/ovl/INVENTED/scene.c'].replace('handlers.callbacks[index]();', alteration)
            self.assertEqual(result(*args)['counts']['native_step_calls'], 0)
        args = list(fixture())
        args[1]['src/ovl/INVENTED/scene.c'] = args[1]['src/ovl/INVENTED/scene.c'].replace(
            's32 index;', 's32 index;GameState game_state;')
        self.assertEqual(result(*args)['counts']['native_step_calls'], 0)

    def test_native_hash_config_pointer_count_order_and_symbol_conflicts_reject(self):
        for change in ('native', 'sha1', 'base', 'path', 'count', 'alias', 'conflict', 'out_of_range'):
            args = list(fixture())
            if change == 'native':
                args[5] = {key: data[:-1] + b'X' for key, data in args[5].items()}
            elif change in {'sha1', 'base', 'path'}:
                key = next(iter(args[3]))
                old, new = {'sha1': ('sha1: ', 'sha1: 0'), 'base': ('0x80132000', '0x80133000'),
                            'path': ('EXEDIR/INVENTED', 'OTHER/INVENTED')}[change]
                args[3][key] = args[3][key].replace(old, new)
            elif change == 'count':
                args[2] = {key: text.replace('[4]', '[3]') for key, text in args[2].items()}
            elif change == 'alias':
                args[4]['config/invented.txt'] += 'other = 0x80050004;\n'
            elif change == 'conflict':
                args[4]['config/invented.txt'] += 'steps = 0x80132104;\n'
            else:
                args[4]['config/invented.txt'] = args[4]['config/invented.txt'].replace('0x80132100', '0x80132ffc')
            with self.subTest(change=change):
                self.assertEqual(result(*args)['counts']['native_step_calls'], 0)
        args = list(fixture())
        blob = bytearray(next(iter(args[5].values())))
        struct.pack_into('<I', blob, 0x108, 0x80050008)
        args[5] = {next(iter(args[5])): bytes(blob)}
        args[0]['assets'][1]['source']['sha256'] = hashlib.sha256(blob).hexdigest()
        key = next(iter(args[3]))
        args[3][key] = args[3][key].replace(hashlib.sha1(fixture()[5]['CDROM/EXEDIR/INVENTED.EXN']).hexdigest(), hashlib.sha1(blob).hexdigest())
        self.assertEqual(result(*args)['counts']['native_step_calls'], 0)

    def test_changed_protocol_blocks_new_proofs_but_preserves_legacy_unknowns(self):
        args = list(fixture())
        args[1]['src/main/protocol.c'] = args[1]['src/main/protocol.c'].replace('game_state.mode_step+=1', 'game_state.mode_step+=2')
        args[1]['src/ovl/INVENTED/scene.c'] += '\nvoid unknown(void){dec_bg_cd_read(100,1);}'
        report = result(*args)
        self.assertEqual(report['native_step_proof']['status'], 'unresolved')
        self.assertEqual(report['streams'][0]['context_status'], 'shape_unresolved')
        args = list(fixture())
        args[1]['src/ovl/INVENTED/scene.c'] = '#define mode_step_next() something_else()\n' + args[1]['src/ovl/INVENTED/scene.c']
        self.assertEqual(result(*args)['native_step_proof']['status'], 'unresolved')

    def test_conflicting_and_unknown_legacy_uses_are_not_erased(self):
        args = list(fixture())
        args[1]['src/main/other.c'] = 'void other(void){dec_bg_cd_read(101,0);dec_bg_decode_to_vram(0);}'
        report = result(*args)
        self.assertEqual(report['streams'][0]['context_status'], 'conflicting_shapes')
        args[1]['src/main/other.c'] = 'void other(void){dec_bg_cd_read(101,0);}'
        self.assertEqual(result(*args)['streams'][0]['context_status'], 'proved_single_shape_with_unresolved_uses')


if __name__ == '__main__':
    unittest.main()
