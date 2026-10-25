"""Optional real holiday media integration; original fixtures stay external and ignored."""
import copy
import hashlib
import json
import os
import shutil
import struct
import tempfile
import unittest
from pathlib import Path
from unittest import mock
from tools.port.validate_school_day_media import validate_pack
from tools.port.test_media_cli import clean_cli
from tools.assets.visual_text_equivalence import _decode_png

ROOT=Path(__file__).resolve().parents[2]


@unittest.skipUnless(os.environ.get('TOKIMEMO_HOLIDAY_PACK'), 'set original holiday input paths for real checks')
class HolidayMediaTests(unittest.TestCase):
    def setUp(self):
        self.pack=Path(os.environ['TOKIMEMO_HOLIDAY_PACK'])
        self.doc=validate_pack(str(self.pack))
        self.temp=Path(tempfile.mkdtemp(prefix='holiday-media-',dir=ROOT/'.asset-cache/port'))

    def tearDown(self):shutil.rmtree(self.temp)

    def args(self,target):
        return (os.environ['TOKIMEMO_IMPORT_INPUT'],os.environ['TOKIMEMO_IMPORT_MEDIA'],
                os.environ['TOKIMEMO_IMPORT_NATIVE'],str(target),os.environ['TOKIMEMO_HOLIDAY_PROFILE'])

    def reject(self,change):
        target=self.temp/'bad';shutil.rmtree(target,ignore_errors=True);shutil.copytree(self.pack,target)
        doc=copy.deepcopy(self.doc);change(doc,target);(target/'manifest.json').write_text(json.dumps(doc))
        with self.assertRaises(ValueError):validate_pack(str(target))

    def test_all_previous_components_and_pngs_remain_identical(self):
        old=Path(os.environ['TOKIMEMO_MEDIA_V2']);prev=validate_pack(str(old))
        for key,component in prev['components'].items():self.assertEqual(self.doc['components'][key],component)
        for png in (old/'assets').glob('*.png'):self.assertEqual(png.read_bytes(),(self.pack/'assets'/png.name).read_bytes())
        self.assertEqual(self.doc['schema'],'tokimemo.school-day-media.v3')
        self.assertEqual(len(self.doc['components']),7)
        self.assertEqual(len(list((self.pack/'assets').glob('*.png'))),64)

    def test_actual_background_geometry_masks_and_distinct_room_dependencies(self):
        for key in ('holiday_room_background','holiday_command_0_background'):
            bg=self.doc['components'][key]
            active=_decode_png((self.pack/'assets'/bg['image']['path']).read_bytes())
            padded=_decode_png((self.pack/'assets'/bg['padded_image']['path']).read_bytes())
            self.assertEqual(active[:2],(320,88));self.assertEqual(padded[:2],(320,96))
            self.assertEqual(active[2],padded[2][:320*88*4])
            self.assertEqual(bg['native_read_extent']['requested_sector_count'],10)
            self.assertEqual(bg['native_read_extent']['sectors_after_iso_extent'],2)
            self.assertFalse(bg['native_read_extent']['overread_payload_used_for_mdec'])
        table=self.doc['route_tables']['values']
        from tools.assets.input import open_input
        with open_input(os.environ['TOKIMEMO_IMPORT_INPUT']) as source:
            exe=source.read(next(e for e in source.entries() if e['path']=='SLPM_86.053'))
        for name in ('weekday_room_background_lbas','seasonal_room_background_lbas'):
            proof=self.doc['route_tables']['provenance'][name]
            self.assertEqual(table[name],list(struct.unpack('<4i',exe[proof['exe_offset']:proof['exe_offset']+proof['size']])))
        sd=self.doc['components']['holiday_command_0_sd']
        self.assertEqual(sd['caller_profile']['family'],'holiday');self.assertEqual(sd['profile_index'],1)
        self.assertEqual(len(sd['frames']),8)
        self.assertEqual([s['frame_index'] for s in sd['sequences'][0]['steps']],[0,1,3,2,1,4,5])
        for frame in sd['frames']:
            self.assertEqual(len(frame['source_palette_words']),256)
            for part in frame['parts']:self.assertEqual(part['clut_vram'],[0,496])

    def test_source_native_read_and_route_table_forgery_rejected(self):
        changes=[lambda d:d['route_tables']['values']['weekday_room_background_lbas'].__setitem__(0,16318),
            lambda d:d['provenance'].update(holiday_profile_sha256='0'*64),
            lambda d:d['components']['holiday_room_background']['native_read_extent'].update(overread_payload_used_for_mdec=True),
            lambda d:d['components']['holiday_command_0_background']['geometry_context'].update(height=96),
            lambda d:d['components']['holiday_command_0_sd']['source'].update(lba=41446),
            lambda d:d['components']['holiday_command_0_sd']['caller_profile'].update(index=0),
            lambda d:d['components']['holiday_command_0_sd']['native_route'].update(event=[0,0,0,0]),
            lambda d:d['components']['holiday_command_0_sd']['frames'][0]['parts'][0].update(clut_vram=[16,496]),
            lambda d:d['components']['holiday_command_0_sd']['palette'].update(mode=1),
            lambda d:d['components'].pop('holiday_room_background')]
        for change in changes:
            with self.subTest(change=change):self.reject(lambda d,t:change(d))

    def test_coherent_sd_timing_frame_and_palette_changes_rejected(self):
        def palette(d):
            sd=d['components']['holiday_command_0_sd']
            sd['palette']['source_words'][1]^=1
            for frame in sd['frames']:frame['source_palette_words'][1]^=1
        changes=[lambda d:d['components']['holiday_command_0_sd']['sequences'][0]['steps'][0].update(duration=5),
                 lambda d:d['components']['holiday_command_0_sd']['sequences'][0]['steps'][0].update(frame_index=1),
                 lambda d:d['components']['holiday_command_0_sd']['frames'][0]['origin'].__setitem__(0,1),palette]
        for change in changes:
            with self.subTest(change=change):self.reject(lambda d,t:change(d))

    def test_missing_and_corrupt_holiday_payloads_rejected(self):
        def missing(d,t):
            spec=d['components']['holiday_command_0_sd']['frames'][0]['png'];(t/'assets'/spec['path']).unlink()
        self.reject(missing)
        def corrupt(d,t):
            spec=d['components']['holiday_room_background']['image'];p=t/'assets'/spec['path'];p.write_bytes(p.read_bytes()[:-1])
        self.reject(corrupt)

    def test_native_profile_mutation_rejected_before_output(self):
        from tools.port.school_day_media import import_pack
        native=json.loads(Path(os.environ['TOKIMEMO_HOLIDAY_PROFILE']).read_text());native['sd_profile']=0
        altered=self.temp/'native.json';altered.write_text(json.dumps(native));target=self.temp/'out'
        args=list(self.args(target));args[4]=str(altered)
        with self.assertRaisesRegex(ValueError,'native profile'):import_pack(*args)
        self.assertFalse(target.exists())

    def test_new_source_pin_change_fails_before_original_reads(self):
        from tools.assets.holiday_content_profile import SOURCE_FILES,build_holiday_profile
        from tools.assets.bs_cohort_profile import SOURCE_FILES as SHARED
        root=self.temp/'sources'
        for relative in dict(SHARED,**SOURCE_FILES):
            p=root/relative;p.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(ROOT/relative,p)
        changed=root/'src/main/holiday.c';changed.write_bytes(changed.read_bytes()+b'\n/* invented stale witness */\n')
        with self.assertRaisesRegex(ValueError,'source evidence hash'):build_holiday_profile(None,[],root)

    def test_clean_cli_and_file_bounded_reimport_are_deterministic(self):
        from tools.port.school_day_media import import_pack
        from tools.assets import image_mdec
        observed=[];original=image_mdec.export_mdec
        def spy(data,*args,**kw):observed.append((kw['source_path'],len(data)));return original(data,*args,**kw)
        target=self.temp/'observed'
        with mock.patch.object(image_mdec,'export_mdec',side_effect=spy):import_pack(*self.args(target))
        self.assertIn(('CDROM/BS/C_02_01H.BS',16048),observed);self.assertIn(('CDROM/BS/C_02_03.BS',14840),observed)
        self.assertEqual((target/'manifest.json').read_bytes(),(self.pack/'manifest.json').read_bytes())
        cli=self.temp/'cli'
        result=clean_cli(str(ROOT/'tools/port/school_day_media.py'),'--input',self.args(cli)[0],
            '--media-manifest',self.args(cli)[1],'--native-profile',self.args(cli)[2],
            '--output',str(cli),'--holiday-profile',self.args(cli)[4])
        self.assertEqual(result.returncode,0,result.stderr);self.assertEqual(json.loads(result.stdout)['schema'],'tokimemo.school-day-media.v3')
        self.assertEqual((cli/'manifest.json').read_bytes(),(self.pack/'manifest.json').read_bytes())

    def test_incomplete_holiday_native_extent_rolls_back(self):
        from tools.assets import media_mdec_export
        from tools.port.school_day_media import import_pack
        original=media_mdec_export._native_cd_read_extent
        def incomplete(source,entry,*args):
            if entry['path']=='CDROM/BS/C_02_03.BS':raise ValueError('invented short holiday extent')
            return original(source,entry,*args)
        before=set(self.temp.glob('.school-day-media-*'));target=self.temp/'rollback'
        with mock.patch.object(media_mdec_export,'_native_cd_read_extent',side_effect=incomplete):
            with self.assertRaisesRegex(ValueError,'short holiday extent'):import_pack(*self.args(target))
        self.assertFalse(target.exists());self.assertEqual(set(self.temp.glob('.school-day-media-*')),before)

    def test_clean_public_mdec_cli_uses_exact_two_holiday_profiles(self):
        cache=ROOT/'.asset-cache/media';cache.mkdir(parents=True,exist_ok=True)
        target=Path(tempfile.mkdtemp(prefix='test-holiday-cli-',dir=cache))/'export'
        try:
            result=clean_cli(str(ROOT/'tools/assets/media_mdec_export.py'),os.environ['TOKIMEMO_IMPORT_INPUT'],
                '--holiday-command0','--baseline-media',os.environ['TOKIMEMO_IMPORT_MEDIA'],
                '--repository-root',str(ROOT),'--output',str(target))
            self.assertEqual(result.returncode,0,result.stderr)
            doc=json.loads((target/'manifest.json').read_text())
            for key in ('holiday_room_background','holiday_command_0_background'):
                bg=self.doc['components'][key];a=next(x for x in doc['assets'] if x['source']['path']==bg['source']['path'])
                self.assertEqual({x['sha256'] for x in a['outputs']},{bg[k]['sha256'] for k in ('image','stp_mask','padded_image','padded_stp_mask')})
                self.assertEqual(a['native_cd_read_extent'],bg['native_read_extent'])
        finally:shutil.rmtree(target.parent)


if __name__=='__main__':unittest.main()
