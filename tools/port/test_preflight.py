"""Invented planner fixtures; no game-content/native-execution acceptance."""
import copy
import json
from pathlib import Path
import tempfile
import unittest
from . import preflight
from .readiness import Invalid, canonical_sha, file_identity, runtime_identity

class PreflightTests(unittest.TestCase):
 def setUp(self):
  self.temp=tempfile.TemporaryDirectory();self.root=Path(self.temp.name)
  (self.root/'src').mkdir();(self.root/'src/native.c').write_text('original source month % 3')
  (self.root/'registry.gd').write_text('native mode0x38 exists')
  self.runtime=self.root/'runtime';self.runtime.mkdir();(self.runtime/'core.gd').write_text('invented runtime')
  self.policy={'schema':'tokimemo.readiness-policy.v1','coverage_status':'provisional','scenarios':[{'id':'calendar-events','checkpoints':[{'id':'month-notice'}]}]};(self.root/'policy.json').write_text(json.dumps(self.policy))
  (self.root/'review.txt').write_text('invented isolated test review, not actual accepted game evidence')
  case={'id':'fixture','native_mode':56,'policy_checkpoint':['calendar-events','month-notice'],'predicate':'invented month branch','implementation':'implemented_subset','owner':['T-fixture'],'dependencies':['original data needed'],'source_locators':[{'path':'src/native.c','symbol':'fixture'}],'evidence':{'status':'untested'}}
  self.data={'schema':'tokimemo.month-boundary-preflight.v1','source_pins':{p:{'sha256':file_identity(self.root/p)['sha256'],'anchors':[anchor]} for p,anchor in [('src/native.c','month % 3'),('registry.gd','mode0x38')]},'policy':{'path':'policy.json','sha256':file_identity(self.root/'policy.json')['sha256'],'coverage_status':'provisional'},'registry':{'path':'registry.gd','native_mode':56},'cases':[case]}
 def tearDown(self):self.temp.cleanup()
 def run_report(self):
  path=self.root/'inventory.json';path.write_text(json.dumps(self.data));return preflight.report(self.root,path,file_identity(path)['sha256'],self.runtime,['fixture'],{'operation':'month-start','year':95,'month':5,'day':1})
 def test_untested_does_not_clear(self):
  r=self.run_report();self.assertEqual(r['cases'][0]['status'],'untested');self.assertFalse(r['preflight_clear_for_selected_cases']);self.assertFalse(r['complete_port_ready'])
 def verified(self):
  self.data['cases'][0]['evidence']={'status':'verified','scope':'invented bounded fixture only','selection':{'operation':'month-start','year':95,'month':5,'day':1},'runtime_identity_sha256':runtime_identity(self.runtime)['sha256'],'references':[{'path':'review.txt','sha256':file_identity(self.root/'review.txt')['sha256']}]}
 def test_verified_is_exact_runtime_and_never_whole_port(self):
  self.verified();r=self.run_report();self.assertTrue(r['preflight_clear_for_selected_cases']);self.assertFalse(r['route_reachability_proven']);self.assertFalse(r['complete_port_ready'])
  (self.runtime/'new-handler.gd').write_text('new file invalidates identity');r=self.run_report();self.assertEqual(r['cases'][0]['status'],'untested');self.assertFalse(r['preflight_clear_for_selected_cases'])
 def test_unsupported_cannot_be_laundered_by_verified_evidence(self):
  self.verified();self.data['cases'][0]['implementation']='known_unsupported';r=self.run_report();self.assertEqual(r['known_expected_stops'],['fixture']);self.assertFalse(r['preflight_clear_for_selected_cases'])
 def test_explicit_regression_never_clears(self):
  self.verified();self.data['cases'][0]['evidence']['status']='regression';r=self.run_report();self.assertEqual(r['regressions'],['fixture']);self.assertFalse(r['preflight_clear_for_selected_cases'])
 def test_same_runtime_different_month_is_not_verified(self):
  self.verified();self.assertTrue(self.run_report()['preflight_clear_for_selected_cases']);path=self.root/'inventory.json'
  wrong=preflight.report(self.root,path,file_identity(path)['sha256'],self.runtime,['fixture'],{'operation':'month-start','year':97,'month':2,'day':1})
  self.assertEqual(wrong['cases'][0]['status'],'untested');self.assertFalse(wrong['preflight_clear_for_selected_cases'])
 def test_explicit_case_without_recipe_cannot_bypass_scope(self):
  self.verified();self.run_report();path=self.root/'inventory.json'
  for selection in [None,{}, {'operation':'month-start','year':95,'month':5,'day':2}, {'operation':'day-bank','year':95,'month':5,'day':1}]:
   r=preflight.report(self.root,path,file_identity(path)['sha256'],self.runtime,['fixture'],selection)
   self.assertFalse(r['preflight_clear_for_selected_cases']);self.assertFalse(r['cases'][0]['evidence_applies_to_requested_selection'])
 def test_month_start_selector_requires_first_day(self):
  with self.assertRaises(Invalid):preflight.selected_ids('month-start',95,5,day=2)
  self.assertEqual(preflight.selected_ids('month-start',97,2),['ordinary-month'])
 def test_outdated_source_and_review_reject(self):
  (self.root/'src/native.c').write_text('changed source')
  with self.assertRaisesRegex(Invalid,'outdated source'):self.run_report()
  (self.root/'src/native.c').write_text('original source month % 3');self.verified();(self.root/'review.txt').write_text('changed')
  with self.assertRaisesRegex(Invalid,'outdated evidence'):self.run_report()
 def test_policy_not_shrunk_or_reclassified(self):
  self.data['cases'][0]['policy_checkpoint']=['calendar-events','invented-pass']
  with self.assertRaisesRegex(Invalid,'existing readiness'):self.run_report()
  self.data['cases'][0]['policy_checkpoint']=['calendar-events','month-notice'];self.data['policy']['coverage_status']='audited'
  with self.assertRaisesRegex(Invalid,'policy identity/status'):self.run_report()
 def test_inventory_pin_and_unknown_case_reject(self):
  self.run_report();path=self.root/'inventory.json'
  with self.assertRaises(Invalid):preflight.report(self.root,path,'0'*64,self.runtime,['fixture'])
  with self.assertRaises(Invalid):preflight.report(self.root,path,file_identity(path)['sha256'],self.runtime,['missing'])
 def test_source_selectors_separate_all_quarter_branches(self):
  for month,cid in [(3,'quarter-march'),(6,'quarter-june'),(12,'quarter-december')]:self.assertEqual(preflight.selected_ids('month-start',96,month),[cid])
  self.assertEqual(preflight.selected_ids('month-start',95,5),['ordinary-may95'])
  self.assertEqual(preflight.selected_ids('month-start',96,9),['september96-skip'])
  for year in [95,97]:self.assertEqual(preflight.selected_ids('month-start',year,9),['september-notice'])
 def test_day_bank_override_requires_exact_day_and_caller(self):
  for vacation in range(4):self.assertEqual(preflight.selected_ids('day-bank',96,9,vacation,1),['bank-september96-override'])
  self.assertEqual(preflight.selected_ids('day-bank',96,9,0,2),['bank-season-2'])
  self.assertEqual(preflight.selected_ids('day-bank',96,9,2,2),['bank-vacation-2'])
  self.assertEqual(preflight.selected_ids('month-start',96,9),['september96-skip'])
  with self.assertRaises(Invalid):preflight.selected_ids('day-bank',96,9,0)
  with self.assertRaises(Invalid):preflight.selected_ids('day-bank',96,9,None,1)
 def test_seasons_year_roll_and_profile_bounds(self):
  for month,season in [(4,0),(7,1),(10,2),(1,3)]:self.assertEqual(preflight.selected_ids('day-bank',96,month,0,1),['bank-season-'+str(season)])
  self.assertEqual(preflight.selected_ids('year-roll',95,12,day=31),['calendar-year-roll','ordinary-month'])
  for operation,year,month,day in [('year-roll',95,12,1),('month-start',95,3,None),('month-start',98,4,None)]:
   with self.assertRaises(Invalid):preflight.selected_ids(operation,year,month,day=day)
if __name__=='__main__':unittest.main()
