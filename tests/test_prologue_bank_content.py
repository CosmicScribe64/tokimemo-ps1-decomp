"""Invented C-call fixtures for strict native bank-request source parsing."""
from pathlib import Path
import importlib.util,os,sys,unittest
ROOT=next(p for p in Path(__file__).resolve().parents if (p/'tools/assets/input.py').exists());sys.path.insert(0,str(ROOT))
path=Path(os.environ.get('PROLOGUE_BANK_ADAPTER_PATH',ROOT/'tools/port/prologue_bank_content.py'))
s=importlib.util.spec_from_file_location('banks',path);banks=importlib.util.module_from_spec(s);s.loader.exec_module(banks)
class RequestTests(unittest.TestCase):
    def source(self,args):return 'void invented_bank(void) {\n audio_bank_load_start('+args+');\n}\n'
    def test_valid_invented_arguments(self):
        self.assertEqual(banks._arguments(self.source('0,1,2,3,4,0xA'),'invented_bank'),{'bank':0,'se_header':1,'se_body':2,'music_header':3,'music_body':4,'sequence':10})
    def test_missing_function(self):
        with self.assertRaises(ValueError):banks._arguments(self.source('0,1,2,3,4,5'),'other')
    def test_not_bank_zero(self):
        with self.assertRaises(ValueError):banks._arguments(self.source('1,1,2,3,4,5'),'invented_bank')
    def test_expression_not_executed(self):
        with self.assertRaises(ValueError):banks._arguments(self.source('0,1+2,2,3,4,5'),'invented_bank')
    def test_wrong_argument_count(self):
        with self.assertRaises(ValueError):banks._arguments(self.source('0,1,2,3,4'),'invented_bank')
    def test_ambiguous_calls(self):
        source=self.source('0,1,2,3,4,5').replace('\n}', '\n audio_bank_load_start(0,2,3,4,5,6);\n}')
        with self.assertRaises(ValueError):banks._arguments(source,'invented_bank')
if __name__=='__main__':unittest.main()
