"""Invented tiny PNG fixtures; no game data or actual review decisions."""
import base64
import http.client
import json
import os
from pathlib import Path
import tempfile
import threading
import unittest
from translation_gallery.server import Root, Decisions, Gallery, Invalid, ReviewServer, encoded, digest, object_json, read_image
PNG=base64.b64decode('iVBORw0KGgoAAAANSUhEUgAAAAEAAAABCAQAAAC1HAwCAAAAC0lEQVR42mP8/x8AAwMCAO+aPacAAAAASUVORK5CYII=')
class GalleryTests(unittest.TestCase):
 def setUp(self):
  self.temp=tempfile.TemporaryDirectory();self.path=Path(self.temp.name);self.repo=self.path/'repo';self.repo.mkdir();self.state=self.path/'state';self.state.mkdir()
  self.batch=self.repo/'.asset-cache/translation-images/batch-test';self.batch.mkdir(parents=True)
  (self.batch/'source.png').write_bytes(PNG);(self.batch/'output.png').write_bytes(PNG)
  self.manifest={'assets':[{'id':'fixture','source':'source.png','output':'output.png','source_sha256':digest(PNG),'output_sha256':digest(PNG),'dimensions':[1,1],'frame':1,'source_container':'CDROM/TEST.NBN','catalog_region_ids':['region-test']} ]}
  self.publish()
  self.inventory={'schema':'tokimemo.translation-review-inventory.v1','source_catalog_sha256':'a'*64,'counts':{'regions':1,'reviews':{'confirmed_japanese_text':1,'suspected':0},'unrepresented_sources':1,'probable_image_sources_unrepresented':1,'unreviewed_unresolved_graphics_sources':1},'coverage':{'corpus_complete':False},'records':[{'id':'region-test','kind':'region','source_container':'CDROM/TEST.NBN','frame':1,'palette':0,'variant':'base','classification':'confirmed_japanese_text','context':['fixture'],'original':{'path':'.asset-cache/translation-images/batch-test/source.png','sha256':digest(PNG),'dimensions':[1,1]},'dimensions':[1,1]},{'id':'unknown:test','kind':'unknown','source_container':'CDROM/UNKNOWN.BIN','classification':'unrepresented_source','original':None}]}
  (self.repo/'inventory.json').write_bytes(encoded(self.inventory));self.root=Root(self.repo);self.store=Decisions(self.state);self.gallery=Gallery(self.root,'inventory.json','.asset-cache/translation-images/index.jsonl',self.store)
 def publish(self):
  data=encoded(self.manifest);(self.batch/'manifest.json').write_bytes(data)
  (self.batch.parent/'index.jsonl').write_bytes(encoded({'batch_id':'batch-test','manifest':'batch-test/manifest.json','sha256':digest(data)})+b'\n')
 def tearDown(self):
  self.root.close();self.store.root.close();self.temp.cleanup()
 def row(self):return self.gallery.snapshot()['items'][0]
 def decision(self,**kw):
  row=self.row();return {'asset_id':row['id'],'revision':row['revision'],'decision':'approve','notes':'Local fixture',**kw}
 def test_inventory_contains_pending_and_unknown(self):
  view=self.gallery.snapshot();self.assertEqual(view['summary']['mapped_japanese'],1);self.assertEqual(view['summary']['unrepresented_sources'],1);self.assertEqual(len(view['items']),2)
  self.manifest['assets'][0]['catalog_region_ids']=[];self.publish();view=self.gallery.snapshot();self.assertEqual(view['summary']['mapped_japanese'],0);self.assertEqual(len(view['items']),3)
 def test_decisions_persist_and_bind_hashes(self):
  event=self.gallery.decide(self.decision());self.assertEqual(event['source_sha256'],digest(PNG));self.assertEqual(event['category'],'style')
  another=Decisions(self.state)
  try:self.assertEqual(another.history(),[event])
  finally:another.root.close()
  self.assertEqual(self.row()['style'],'approve');self.gallery.decide(self.decision(decision='request_changes',notes='Center this label'));self.assertEqual(self.row()['style'],'request_changes')
 def test_changed_output_invalidates_and_requires_new_revision(self):
  old=self.decision();self.gallery.decide(old);(self.batch/'output.png').write_bytes(PNG+b'version2')
  self.assertEqual(self.row()['style'],'stale')
  with self.assertRaises(Invalid):self.gallery.decide(old)
  self.manifest['assets'][0]['output_sha256']=digest(PNG+b'version2');self.publish();self.assertEqual(self.row()['style'],'stale')
  with self.assertRaises(Invalid):self.gallery.decide(old)
  self.gallery.decide(self.decision());self.assertEqual(self.row()['style'],'approve')
 def test_source_change_invalidates(self):
  self.gallery.decide(self.decision());(self.batch/'source.png').write_bytes(PNG+b'changed');self.assertEqual(self.row()['style'],'stale');self.assertFalse(self.row()['reviewable'])
 def test_no_asset_write_on_decision(self):
  before={p.name:p.read_bytes() for p in self.batch.iterdir()};self.gallery.decide(self.decision(notes='<script>alert(1)</script>'));self.assertEqual(before,{p.name:p.read_bytes() for p in self.batch.iterdir()})
 def test_bad_decisions(self):
  for patch in [{'asset_id':'unknown:test'},{'asset_id':'../../secret'},{'revision':'0'*64},{'decision':'runtime_approve'},{'notes':'x'*4001},{'notes':{}},{'path':'/etc/passwd'}]:
   with self.subTest(patch=patch),self.assertRaises(Invalid):self.gallery.decide(self.decision(**patch))
  self.assertEqual(self.store.history(),[])
 def test_bad_paths_and_symlinks(self):
  outside=self.path/'secret';outside.write_bytes(b'private');(self.batch/'link.png').symlink_to(outside)
  for name in ['../secret','/etc/passwd','.asset-cache/../secret','.asset-cache//x','foo\\bar','.asset-cache/translation-images/batch-test/link.png']:
   with self.subTest(name=name),self.assertRaises((Invalid,OSError)):self.root.read(name)
  (self.repo/'linked').symlink_to(self.batch,target_is_directory=True)
  with self.assertRaises(OSError):self.root.read('linked/source.png')
  with self.assertRaises(Invalid):Root(self.repo/'linked')
 def test_state_symlink_and_hardlink_rejected(self):
  p=self.state/'style-decisions.jsonl';p.unlink();outside=self.path/'secret';outside.write_bytes(b'private');p.symlink_to(outside)
  with self.assertRaises(OSError):Decisions(self.state)
  p.unlink();os.link(outside,p)
  with self.assertRaises(Invalid):Decisions(self.state)
  self.assertEqual(outside.read_bytes(),b'private')
 def test_malformed_history_and_json(self):
  (self.state/'style-decisions.jsonl').write_bytes(b'{broken\n')
  with self.assertRaises(Invalid):self.gallery.decide(self.decision())
  for data in [b'{"x":1,"x":2}',b'{"x":NaN}']:
   with self.assertRaises(Invalid):object_json(data)
 def test_manifest_traversal_and_pin(self):
  self.manifest['assets'][0]['output']='../../../../outside.png';self.publish()
  with self.assertRaises(Invalid):self.gallery.snapshot()
 def test_png_dimensions_and_media_bytes(self):
  row=self.row();self.assertEqual(read_image(self.root,row['original']),PNG);bad=dict(row['original'],dimensions=[2,2])
  with self.assertRaises(Invalid):read_image(self.root,bad)
 def test_duplicate_assets_and_batches_rejected(self):
  self.manifest['assets'].append(dict(self.manifest['assets'][0]));self.publish()
  with self.assertRaises(Invalid):self.gallery.snapshot()
  self.manifest['assets'].pop();self.publish();index=self.batch.parent/'index.jsonl';index.write_bytes(index.read_bytes()*2)
  with self.assertRaises(Invalid):self.gallery.snapshot()
 def test_explicit_superseding_batch_keeps_approval_stale(self):
  self.gallery.decide(self.decision());index=self.batch.parent/'index.jsonl';old=object_json(index.read_bytes())
  revised=dict(self.manifest);revised['assets']=[dict(self.manifest['assets'][0],output_sha256=digest(PNG+b'new'))]
  data=encoded(revised);(self.batch/'revision.json').write_bytes(data);(self.batch/'output.png').write_bytes(PNG+b'new')
  index.write_bytes(encoded(old)+b'\n'+encoded({'batch_id':'batch-test','manifest':'batch-test/revision.json','sha256':digest(data),'supersedes_sha256':old['sha256']})+b'\n')
  self.assertEqual(self.row()['style'],'stale');self.assertEqual(self.gallery.snapshot()['summary']['drafts'],1)
 def test_http_localhost_origin_token_and_write_scope(self):
  server=ReviewServer(('127.0.0.1',0),self.gallery,Path(__file__).parent/'translation_gallery/static',0);server.port=server.server_port
  thread=threading.Thread(target=server.serve_forever,daemon=True);thread.start()
  def request(method,path,body=None,headers=None):
   conn=http.client.HTTPConnection('127.0.0.1',server.port,timeout=3)
   try:conn.request(method,path,body=body,headers=headers or {});response=conn.getresponse();return response.status,response.read(),dict(response.getheaders())
   finally:conn.close()
  try:
   code,data,headers=request('GET','/api/catalog');self.assertEqual(code,200);token=json.loads(data)['review_token'];self.assertIn("frame-ancestors 'none'",headers['Content-Security-Policy'])
   self.assertNotIn('Access-Control-Allow-Origin',headers)
   body=encoded(self.decision());good={'Content-Type':'application/json','Origin':f'http://127.0.0.1:{server.port}','X-Review-Token':token}
   for patch in [{'Host':'evil.example'},{'Origin':'https://evil.example'},{'X-Review-Token':'wrong'},{'Content-Type':'text/plain'}]:
    self.assertEqual(request('POST','/api/reviews',body,{**good,**patch})[0],409)
   self.assertEqual(request('POST','/api/reviews',body,{'Content-Type':'application/json','X-Review-Token':token})[0],409)
   self.assertEqual(request('POST','/api/reviews',body,good)[0],200)
   self.assertEqual(request('POST','/assets/source.png',body,good)[0],404)
   self.assertEqual(request('GET','/../../etc/passwd')[0],404)
   self.assertEqual(request('GET','/api/catalog',headers={'Host':'evil.example'})[0],409)
   self.assertEqual(request('GET',self.row()['original_url'])[1],PNG)
   self.gallery.decide(self.decision(notes='=IMPORTXML("remote")'));self.assertIn(b"'=IMPORTXML",request('GET','/api/export.csv')[1])
  finally:server.shutdown();server.server_close();thread.join(2)
 def test_reviewed_queue_includes_comments_and_stale_returns_to_needs_review(self):
  server=ReviewServer(('127.0.0.1',0),self.gallery,Path(__file__).parent/'translation_gallery/static',0);server.port=server.server_port
  thread=threading.Thread(target=server.serve_forever,daemon=True);thread.start()
  def queue(status):
   conn=http.client.HTTPConnection('127.0.0.1',server.port,timeout=3)
   try:
    conn.request('GET','/api/catalog?kind=draft&status='+status);response=conn.getresponse();self.assertEqual(response.status,200);return object_json(response.read())
   finally:conn.close()
  try:
   self.assertEqual(queue('unreviewed')['total'],1);self.assertEqual(queue('reviewed')['total'],0)
   for decision in ['defer','request_changes','approve']:
    self.gallery.decide(self.decision(decision=decision,notes='Durable comment for '+decision))
    self.assertEqual(queue('unreviewed')['total'],0);reviewed=queue('reviewed');self.assertEqual(reviewed['total'],1);self.assertEqual(reviewed['items'][0]['notes'],'Durable comment for '+decision)
   (self.batch/'output.png').write_bytes(PNG+b'new');self.manifest['assets'][0]['output_sha256']=digest(PNG+b'new');self.publish()
   self.assertEqual(queue('reviewed')['total'],0);needs=queue('unreviewed');self.assertEqual(needs['total'],1);self.assertEqual(needs['items'][0]['style'],'stale')
  finally:server.shutdown();server.server_close();thread.join(2)
if __name__=='__main__':unittest.main(verbosity=2)
