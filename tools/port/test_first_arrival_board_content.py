"""Finite path/source negatives; real original pack checks are explicitly separate."""
import importlib.util,json,os,tempfile,unittest
from pathlib import Path
spec=importlib.util.spec_from_file_location('board',Path(__file__).with_name('first_arrival_board_content.py'));board=importlib.util.module_from_spec(spec);spec.loader.exec_module(board)
class BoardTests(unittest.TestCase):
 def setUp(self):
  self.temp=tempfile.TemporaryDirectory();self.root=Path(self.temp.name);self.repo=self.root/'repo';self.repo.mkdir();self.source=self.root/'original';self.source.mkdir();(self.source/'marker').write_bytes(b'untouched')
 def tearDown(self):self.temp.cleanup()
 def test_fresh_external_output(self):
  _,_,p=board.output_path(self.repo,self.source,self.root/'out');self.assertEqual(p,self.root/'out');self.assertFalse(p.exists())
 def test_input_child_and_parent_reject_unchanged(self):
  for output in [self.source/'bundle',self.source,self.root]:
   with self.assertRaises(board.Invalid):board.output_path(self.repo,self.source,output)
  self.assertEqual(list(self.source.iterdir()),[self.source/'marker']);self.assertEqual((self.source/'marker').read_bytes(),b'untouched')
 def test_existing_output_rejected(self):
  p=self.root/'out';p.mkdir();(p/'marker').write_bytes(b'keep')
  with self.assertRaises(board.Invalid):board.output_path(self.repo,self.source,p)
  self.assertEqual((p/'marker').read_bytes(),b'keep')
 def test_linked_ancestor_rejects(self):
  (self.root/'alias').symlink_to(self.root,target_is_directory=True)
  with self.assertRaises(board.Invalid):board.output_path(self.repo,self.source,self.root/'alias/new')
  self.assertFalse((self.root/'new').exists())
 def test_traversal_rejected(self):
  with self.assertRaises(board.Invalid):board.output_path(self.repo,self.source,self.root/'foo/../out')
 def test_protected_source_roots_reject(self):
  for name in ['disc','raw','.git','port','tools','src']:
   with self.assertRaises(board.Invalid):board.output_path(self.repo,self.source,self.repo/name/'new')
  for name in ['.claude','.asset-cache']:
   self.assertEqual(board.output_path(self.repo,self.source,self.repo/name/'new')[2],self.repo/name/'new')
 def test_wrong_original_rejects_before_output_creation(self):
  source=self.root/'bad.bin';source.write_bytes(b'not original')
  with self.assertRaises((ValueError,OSError)):board.build(Path('/repo'),source,self.root/'out')
  self.assertFalse((self.root/'out').exists());self.assertEqual(source.read_bytes(),b'not original')
 def test_source_context_does_not_guess_dimensions_from_bad_stream(self):
  with self.assertRaises((ValueError,OSError)):board.source_context(Path('/repo'),b'not a valid complete BS')
if __name__=='__main__':unittest.main()
