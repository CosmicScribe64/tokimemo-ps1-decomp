"""Fresh school source-pack integrity; original fixtures stay ignored."""
import copy
import json
import os
from pathlib import Path
import shutil
import tempfile
import unittest
from tools.port.validate_school_day_media import validate_pack, _valid_decoder_model
from tools.assets.image_mdec_model import MODEL


class ModelValidationTests(unittest.TestCase):
    def test_unbound_or_false_model_is_rejected(self):
        self.assertFalse(_valid_decoder_model(None))
        self.assertFalse(_valid_decoder_model(MODEL))
        self.assertFalse(_valid_decoder_model({'id': MODEL['id']}))


@unittest.skipUnless(os.environ.get('TOKIMEMO_FRESH_SCHOOL_PACK'), 'requires original generated school pack')
class FreshOriginalTests(unittest.TestCase):
    def setUp(self):
        self.source = Path(os.environ['TOKIMEMO_FRESH_SCHOOL_PACK'])
        self.temp = tempfile.TemporaryDirectory()
        self.pack = Path(self.temp.name)/'pack'
        shutil.copytree(self.source,self.pack)
        self.doc = json.loads((self.pack/'manifest.json').read_text())

    def tearDown(self):
        self.temp.cleanup()

    def test_whole_original_pack(self):
        doc=validate_pack(self.pack)
        self.assertEqual(doc['schema'],'tokimemo.school-day-media.v4')
        self.assertEqual(len(doc['components']),4)
        self.assertTrue(doc['components']['command_7_sd']['frames'])
        self.assertTrue(_valid_decoder_model(doc['components']['command_0_background']['decoder_model']))

    def test_changed_proof_route_sd_and_model_rejected(self):
        for kind in ['proof','route','sd','model']:
            with self.subTest(kind=kind):
                doc=copy.deepcopy(self.doc)
                if kind=='proof':doc['provenance']['selector_table_proof']['weekday_sd_lbas']['raw_sha256']='0'*64
                elif kind=='route':doc['components']['command_0_sd']['native_route']['actors']=[97]
                elif kind=='sd':doc['components']['command_7_sd']['frames'][0]['bounds'][2]+=1
                else:doc['components']['command_0_background']['decoder_model']['implementation_sha256']['image_mdec_model.py']='0'*64
                (self.pack/'manifest.json').write_text(json.dumps(doc))
                with self.assertRaises(ValueError):validate_pack(self.pack)

    def test_original_manifest_changed_sd_payload_rejected(self):
        spec=self.doc['components']['command_0_sd']['frames'][0]['png']
        path=self.pack/'assets'/spec['path'];data=bytearray(path.read_bytes());data[-1]^=1;path.write_bytes(data)
        with self.assertRaises(ValueError):validate_pack(self.pack)


@unittest.skipUnless(os.environ.get('TOKIMEMO_FRESH_HOLIDAY_PACK'), 'requires original generated v5 school pack')
class FreshHolidayTests(unittest.TestCase):
    """T-39002: school_day_media_fresh.py --holiday (schema v5)."""
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.pack = Path(self.temp.name)/'pack'
        shutil.copytree(Path(os.environ['TOKIMEMO_FRESH_HOLIDAY_PACK']),self.pack)
        self.doc = json.loads((self.pack/'manifest.json').read_text())

    def tearDown(self):
        self.temp.cleanup()

    def test_whole_original_pack(self):
        doc=validate_pack(self.pack)
        self.assertEqual(doc['schema'],'tokimemo.school-day-media.v5')
        self.assertEqual(sorted(doc['components']),['command_0_background','command_0_sd','command_7_background','command_7_sd',
            'holiday_command_0_background','holiday_command_0_sd','holiday_room_background'])
        self.assertEqual(doc['route_tables']['values']['seasonal_room_background_lbas'][0],16318)
        self.assertEqual(doc['route_tables']['values']['weekday_room_background_lbas'][0],16361)

    def test_changed_tables_component_or_schema_rejected(self):
        for kind in ['table','holiday','v4']:
            with self.subTest(kind=kind):
                doc=copy.deepcopy(self.doc)
                if kind=='table':doc['route_tables']['values']['holiday_sd_lbas'][1]=41446
                elif kind=='holiday':doc['components']['holiday_command_0_sd']['source']['lba']=41446
                else:doc['schema'],doc['version']='tokimemo.school-day-media.v4',4
                (self.pack/'manifest.json').write_text(json.dumps(doc))
                with self.assertRaises(ValueError):validate_pack(self.pack)


if __name__=='__main__':unittest.main()
