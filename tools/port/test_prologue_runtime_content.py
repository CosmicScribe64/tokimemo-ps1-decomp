"""Invented parser negatives and original-input invariant checks (Docker only)."""
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import tempfile
import unittest
from unittest.mock import patch

SPEC=importlib.util.spec_from_file_location('prologue_runtime_content',Path(__file__).with_name('prologue_runtime_content.py'))
p=importlib.util.module_from_spec(SPEC);SPEC.loader.exec_module(p)

class PrologueRuntimeTests(unittest.TestCase):
    def test_verified_numeric_and_symbol_pointer_directives(self):
        blocks=p._blocks('dlabel first\n/* 0 80100000 */ .word target\nenddlabel first\n')
        raw=(0x80100010).to_bytes(4,'little')
        extent=p._label('first',blocks,{'target':0x80100010},raw,0x80100000,0,'invented.bin')
        self.assertEqual(p._pointers(extent,1),[0x80100010])
        with self.assertRaises(p.PrologueContentError):p._pointers(extent,2)
        with self.assertRaises(p.PrologueContentError):p._label('first',blocks,{'target':0x80100014},raw,0x80100000,0,'invented.bin')

    def test_address_gap_and_missing_label_rejected(self):
        blocks=p._blocks('dlabel first\n/* 0 80100000 */ .byte 1\n/* 2 80100002 */ .byte 2\nenddlabel first\n')
        with self.assertRaises(p.PrologueContentError):p._label('first',blocks,{},b'\1\0\2',0x80100000,0,'invented.bin')
        with self.assertRaises(p.PrologueContentError):p._label('absent',blocks,{},b'',0,0,'invented.bin')
        with self.assertRaises(p.PrologueContentError):p._blocks('dlabel first\ndlabel first\n')

    def test_nul_padding_and_unknown_codec_are_preserved(self):
        extent={'raw_hex':'414200ff','address':0,'offset':0,'length':4}
        view=p._text(extent)
        self.assertEqual(view['text']['raw_hex'],'4142')
        self.assertEqual(view['terminator_offset'],2)
        self.assertEqual(view['padding_hex'],'ff')
        bad=p._text({'raw_hex':'8100'})
        self.assertIsNone(bad['text']['source_decoded_text'])
        self.assertEqual(bad['text']['raw_hex'],'81')
        with self.assertRaises(p.PrologueContentError):p._text({'raw_hex':'4142'})

    def test_source_pin_and_link_fail_closed(self):
        with tempfile.TemporaryDirectory() as temp:
            repo=Path(temp);file=repo/'proof.c';file.write_bytes(b'invented source')
            with patch.object(p,'SOURCE_PINS',{'proof.c':hashlib.sha256(file.read_bytes()).hexdigest()}):
                p._source_proof(repo)
                file.write_bytes(b'changed source')
                with self.assertRaises(p.PrologueContentError):p._source_proof(repo)
            (repo/'linked.c').symlink_to(file)
            with self.assertRaises(p.PrologueContentError):p._safe_file(repo,'linked.c')

    def test_immutable_source_pin_registry(self):
        with self.assertRaises(TypeError):p.SOURCE_PINS['invented']='0'*64

    def test_export_immutable_portable_payload(self):
        with tempfile.TemporaryDirectory() as temp:
            repo=Path(temp);cache=repo/'.asset-cache/port';cache.mkdir(parents=True)
            with patch.object(p,'build_bundle',return_value={'lines':[],'invented':True}):
                manifest=p.export('invented',cache/'bundle',repo)
                record=json.loads(manifest.read_bytes())
                raw=(manifest.parent/record['payload']).read_bytes()
                self.assertEqual(record['sha256'],hashlib.sha256(raw).hexdigest())
                self.assertFalse(Path(record['payload']).is_absolute())
                with self.assertRaises(ValueError):p.export('invented',cache/'bundle',repo)
                with self.assertRaises(ValueError):p.export('invented',repo/'outside',repo)

    def test_actual_original_storage_and_portability(self):
        root=Path(os.environ['TOKIMEMO_ROOT'])
        bundle=p.build_bundle(root/'disc/files',root)
        self.assertEqual([row['line_id'] for row in bundle['lines']],list(range(43)))
        self.assertEqual(len({row['id'] for row in bundle['lines']}),43)
        self.assertEqual(bundle['tables']['speakers']['length'],44)
        self.assertEqual(bundle['tables']['clip1']['length'],344)
        self.assertEqual(bundle['tables']['clip2']['length'],344)
        self.assertNotIn('raw_hex',bundle['tables']['clip2']['trailing_label_storage'])
        self.assertEqual(len(bundle['dynamic_pieces']['birthday_numeric_rows']),13)
        self.assertEqual(len(bundle['dynamic_pieces']['blood_rows']),4)
        self.assertEqual(len(bundle['speaker_labels']['girl_short_name_rows']),16)
        self.assertEqual(bundle['initial_mascot']['scalar_width'],2)
        self.assertEqual(set(bundle['renderer_pieces']),{'player_quote_open','parenthesis_open','girl_quote_open'})
        self.assertTrue(any(c['function']=='sndi' for c in bundle['proof']['code']))
        self.assertEqual(bundle['dynamic_recipes']['birthday']['native_month_domain'],[1,12])
        self.assertTrue(any(c['function']=='name_entry_profile_store' for c in bundle['proof']['code']))
        exe=(root/'disc/files/SLPM_86.053').read_bytes()
        scalar=int.from_bytes(exe[0x75408:0x7540a],'little',signed=True)
        self.assertTrue(bundle['initial_mascot']['value']==scalar)
        for row in bundle['lines']:
            storage=row['storage'];raw=bytes.fromhex(storage['raw_hex']);text=storage['text']
            self.assertTrue(raw[:storage['terminator_offset']].hex()==text['raw_hex'])
            self.assertTrue(all(isinstance(value,int) for value in row['clip1']+row['clip2']))
            self.assertFalse(row['runtime_execution_proven'])
            if text['source_decoded_text'] is not None:
                self.assertTrue(text['source_decoded_text'].encode('shift_jis').hex()==text['raw_hex'])
        self.assertFalse(bundle['limits']['sdk_string_copy_semantics_proven'])
        self.assertFalse(bundle['limits']['audio_playback_proven'])
        self.assertTrue(all(not Path(path).is_absolute() for path in bundle['proof']['source_pins']))

if __name__=='__main__':unittest.main()
