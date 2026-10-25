"""Optional real-input media regressions; original fixtures remain external and ignored."""
import copy
import hashlib
import json
import os
import shutil
import tempfile
import unittest
from unittest import mock
from pathlib import Path

from tools.port.validate_school_day_media import validate_pack, _metadata_equal


class MetadataTests(unittest.TestCase):
    def test_boolean_cannot_replace_numeric_proof(self):
        self.assertFalse(_metadata_equal({"n": False}, {"n": 0}))
        self.assertFalse(_metadata_equal({"n": 0.0}, {"n": 0}))
        self.assertFalse(_metadata_equal({"n": 0, "extra": True}, {"n": 0}))
        self.assertTrue(_metadata_equal({"n": [0, -1, True]}, {"n": [0, -1, True]}))


@unittest.skipUnless(os.environ.get('TOKIMEMO_MEDIA_V2'), 'set TOKIMEMO_MEDIA_V2 for ignored real-pack checks')
class OriginalMediaTests(unittest.TestCase):
    def setUp(self):
        self.pack = Path(os.environ['TOKIMEMO_MEDIA_V2'])
        self.old = Path(os.environ['TOKIMEMO_MEDIA_V1'])
        self.doc = validate_pack(str(self.pack))
        self.temp = Path(tempfile.mkdtemp(prefix='command7-media-'))

    def tearDown(self): shutil.rmtree(self.temp)

    def mutate(self, change):
        target=self.temp/'pack'; shutil.copytree(self.pack,target)
        doc=copy.deepcopy(self.doc); change(doc,target)
        (target/'manifest.json').write_text(json.dumps(doc))
        with self.assertRaises(ValueError): validate_pack(str(target))

    def test_v1_accepted_and_all_existing_components_and_payloads_identical(self):
        old=validate_pack(str(self.old))
        for key, component in old['components'].items(): self.assertEqual(component,self.doc['components'][key])
        for path in (self.old/'assets').glob('*.png'):
            self.assertEqual(path.read_bytes(),(self.pack/'assets'/path.name).read_bytes())
        self.assertEqual(set(self.doc['components']),set(old['components'])|{'command_7_background'})

    def test_missing_command7_component_rejected(self):
        self.mutate(lambda d,t:d['components'].pop('command_7_background'))

    def test_old_schema_cannot_claim_command7_background(self):
        self.mutate(lambda d,t:d.update(schema='tokimemo.school-day-media.v1',version=1))

    def test_wrong_source_and_route_rejected(self):
        changes=[lambda b:b['source'].update(lba=16284),lambda b:b['source'].update(path='invented.BS'),
                 lambda b:b['source'].update(sha256='0'*64),lambda b:b['native_route'].update(command=0)]
        for change in changes:
            with self.subTest(change=change):
                target=self.temp/'pack'; shutil.rmtree(target,ignore_errors=True)
                self.mutate(lambda d,t:change(d['components']['command_7_background']))

    def test_forged_geometry_native_extent_or_selector_rejected(self):
        changes=[lambda b:b.update(active_crop=[0,0,320,96]),
                 lambda b:b['native_read_extent'].update(overread_payload_used_for_mdec=True),
                 lambda b:b['native_read_extent'].update(sectors_after_iso_extent=0),
                 lambda b:b['native_read_extent'].update(overread_lbas=[]),
                 lambda b:b['geometry_context']['source_profile']['executable']['table_words'].__setitem__(7,16284),
                 lambda b:b['geometry_context']['source_profile']['assets'][0]['iso_directory_record'].update(record_payload_offset=1117),
                 lambda b:b['geometry_context'].update(width=True)]
        for change in changes:
            with self.subTest(change=change):
                target=self.temp/'pack'; shutil.rmtree(target,ignore_errors=True)
                self.mutate(lambda d,t:change(d['components']['command_7_background']))

    def test_missing_and_wrong_dimension_payload_rejected_even_with_new_hash(self):
        self.mutate(lambda d,t:(t/'assets'/d['components']['command_7_background']['image']['path']).unlink())
        shutil.rmtree(self.temp/'pack')
        def substitute(d,t):
            bg=d['components']['command_7_background']; src=t/'assets'/bg['padded_image']['path']; dst=t/'assets'/bg['image']['path']
            dst.write_bytes(src.read_bytes()); bg['image']['size']=dst.stat().st_size; bg['image']['sha256']=hashlib.sha256(dst.read_bytes()).hexdigest()
        self.mutate(substitute)

    def test_output_escape_and_symlink_rejected_before_input_or_staging(self):
        from tools.port import school_day_media as importer
        root = importer._root()
        link = root/'.asset-cache/port/test-command7-output-link'
        link.symlink_to(self.temp, target_is_directory=True)
        escaped = root/'.asset-cache/port/../../test-command7-outside'
        before = set((root/'.asset-cache/port').glob('.school-day-media-*'))
        try:
            for output in (escaped, link/'pack'):
                with self.subTest(output=output):
                    with mock.patch('tools.assets.input.open_input', side_effect=AssertionError('input opened before output admission')):
                        with self.assertRaisesRegex(ValueError, 'parent-directory|symlink'):
                            importer.import_pack(os.environ['TOKIMEMO_IMPORT_INPUT'],os.environ['TOKIMEMO_IMPORT_MEDIA'],
                                os.environ['TOKIMEMO_IMPORT_NATIVE'],str(output))
            self.assertFalse((root/'test-command7-outside').exists())
            self.assertFalse((self.temp/'pack').exists())
            self.assertEqual(set((root/'.asset-cache/port').glob('.school-day-media-*')),before)
        finally: link.unlink()

    def test_fresh_import_is_deterministic_file_bounded_and_immutable(self):
        from tools.port import school_day_media as importer
        from tools.assets.image_mdec import export_mdec
        output = importer._root()/'.asset-cache/port/test-command7-repeat'
        shutil.rmtree(output, ignore_errors=True)
        calls = []
        def observe(data, *args, **kwargs):
            calls.append((kwargs['source_path'],len(data),hashlib.sha256(data).hexdigest()))
            return export_mdec(data,*args,**kwargs)
        args = (os.environ['TOKIMEMO_IMPORT_INPUT'],os.environ['TOKIMEMO_IMPORT_MEDIA'],
                os.environ['TOKIMEMO_IMPORT_NATIVE'],str(output))
        try:
            with mock.patch('tools.assets.image_mdec.export_mdec',side_effect=observe):
                importer.import_pack(*args)
            self.assertIn(('CDROM/BS/C_01_44.BS',17396,'951ec47906f6a19748d2f3b803cb086c0e5af3a467f3cb6819515225bb6365e0'),calls)
            self.assertEqual((output/'manifest.json').read_bytes(),(self.pack/'manifest.json').read_bytes())
            before=(output/'manifest.json').read_bytes()
            with self.assertRaisesRegex(ValueError,'new immutable directory'): importer.import_pack(*args)
            self.assertEqual((output/'manifest.json').read_bytes(),before)
        finally: shutil.rmtree(output,ignore_errors=True)

    def test_failed_native_extent_check_rolls_back_before_publication(self):
        from tools.port import school_day_media as importer
        output=importer._root()/'.asset-cache/port/test-command7-rollback'
        shutil.rmtree(output,ignore_errors=True)
        before=set(output.parent.glob('.school-day-media-*'))
        with mock.patch('tools.assets.media_mdec_export._native_cd_read_extent',side_effect=ValueError('invented incomplete native extent')):
            with self.assertRaisesRegex(ValueError,'incomplete native extent'):
                importer.import_pack(os.environ['TOKIMEMO_IMPORT_INPUT'],os.environ['TOKIMEMO_IMPORT_MEDIA'],
                    os.environ['TOKIMEMO_IMPORT_NATIVE'],str(output))
        self.assertFalse(output.exists())
        self.assertEqual(set(output.parent.glob('.school-day-media-*')),before)



if __name__=='__main__': unittest.main()
