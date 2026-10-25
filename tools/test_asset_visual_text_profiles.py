"""Invented caller metadata; profile context never asserts visible text."""
import copy
import unittest

from tools.assets.visual_text_profiles import get_profile
from tools.assets.visual_text import build_catalog
from tools.test_asset_visual_text import media_manifest


def sd_metadata(path):
    return {'source_path_profile': path, 'source_lba': 123,
            'paired_exe_sha256': 'a' * 64,
            'caller_profile': {'family': 'weekly', 'index': 0, 'source_lba': 123,
                               'equivalent_caller_profiles': [{'family': 'weekly', 'source_lba': 123}]},
            'evidence': ['src/main/school_day.c:school_day_sd_actors_init']}


class VisualProfileTests(unittest.TestCase):
    def test_photo_context_requires_source_native_and_separate_palette_hashes(self):
        path = 'CDROM/LASTPHOT/LASTP_HU.NBN'
        overlay = {'path': 'CDROM/EXEDIR/ETC.EXN', 'sha256': 'a' * 64}
        metadata = {'source_path_profile': path, 'source_sha256': 'b' * 64,
                    'caller_profile': {'kind': 'last_photo', 'source_lba': 123, 'overlay_source': overlay,
                                       'canonical_overlay_profile': {'sha1': 'c' * 40, 'load_base': 0x80132000, 'size': 0x30000}},
                    'palette_source': {'path': path, 'sha256': 'b' * 64},
                    'source_dependencies': [overlay], 'source_evidence': ['invented numeric caller proof']}
        self.assertEqual(get_profile(path)['source_role'], 'graphic_role_unresolved')
        options = {'media_format': 'tokimemo-photo-pages', 'metadata': metadata}
        self.assertEqual(get_profile(path, **options)['source_role'], 'last_photo_source_pages')
        for key, value in (('source_path_profile', 'other'), ('source_sha256', 'bad'), ('source_dependencies', [])):
            changed = dict(metadata, **{key: value})
            self.assertEqual(get_profile(path, media_format='tokimemo-photo-pages', metadata=changed)['source_role'],
                             'graphic_role_unresolved')
        staff = copy.deepcopy(metadata)
        staff_path = 'CDROM/STAFF2/STAF2HU.NBN'
        staff['source_path_profile'] = staff_path
        staff['caller_profile']['kind'] = 'staff2_photo'
        staff['palette_source'] = {'path': 'CDROM/STAFF2/STAF2COL.NBN', 'sha256': 'd' * 64}
        staff['source_dependencies'].append(copy.deepcopy(staff['palette_source']))
        self.assertEqual(get_profile(staff_path, media_format='tokimemo-photo-pages', metadata=staff)['source_role'],
                         'staff_roll_photo_pages')
        staff['palette_source']['sha256'] = 'e' * 64
        self.assertEqual(get_profile(staff_path, media_format='tokimemo-photo-pages', metadata=staff)['source_role'],
                         'graphic_role_unresolved')
    def test_sd_requires_decoder_bound_path_lba_and_paired_metadata(self):
        path = 'invented/animation.NBN'
        self.assertEqual(get_profile(path)['source_role'], 'graphic_role_unresolved')
        metadata = sd_metadata(path)
        profile = get_profile(path, media_format='tokimemo-sd-animation', metadata=metadata)
        self.assertEqual(profile['source_role'], 'school_day_sd_animation_pack')
        self.assertEqual(profile['caller_profile'], metadata['caller_profile'])
        profile['caller_profile']['family'] = 'changed'
        self.assertEqual(metadata['caller_profile']['family'], 'weekly')
        for changed in (dict(metadata, source_lba=124), dict(metadata, source_path_profile='other'),
                        dict(metadata, paired_exe_sha256=None), dict(metadata, paired_exe_sha256='z' * 64)):
            self.assertEqual(get_profile(path, media_format='tokimemo-sd-animation', metadata=changed)['source_role'],
                             'graphic_role_unresolved')

    def test_sports_and_battle_contexts_are_exact_source_profiles(self):
        self.assertEqual(get_profile('CDROM/TAIIKU/SPSETU.BIN')['source_role'], 'sports_instruction_viewport_pack')
        self.assertEqual(get_profile('CDROM/BATTLE/VS_HABU.BIN')['source_role'], 'rpg_encounter_source_texture_page')
        self.assertEqual(get_profile('invented/SPSETU.BIN')['source_role'], 'graphic_role_unresolved')
        self.assertEqual(get_profile('CDROM/TAIIKU/OTHER/SPSETU.BIN')['source_role'], 'graphic_role_unresolved')
        self.assertEqual(get_profile('CDROM/BATTLE/UNKNOWN.BIN')['source_role'], 'graphic_role_unresolved')

    def test_catalog_passes_sd_metadata_without_text_promotion(self):
        manifest = media_manifest()
        asset = manifest['assets'][0]
        path = 'invented/animation.NBN'
        asset['source']['path'] = path
        asset['format'] = 'tokimemo-sd-animation'
        metadata = sd_metadata(path)
        metadata.update(width=8, height=4)
        asset['export']['metadata'] = metadata
        catalog = build_catalog(manifest)
        for region in catalog['regions']:
            self.assertEqual(region['source_profile']['source_role'], 'school_day_sd_animation_pack')
            self.assertEqual(region['state'], 'unreviewed')
            self.assertIsNone(region['confidence'])
            self.assertEqual(region['original_metadata']['caller_profile'], metadata['caller_profile'])


if __name__ == '__main__':
    unittest.main()
