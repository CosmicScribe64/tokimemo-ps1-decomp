"""Synthetic identity/error fixtures, not evidence of game parity (T-39008)."""
import copy
import hashlib
import json
from pathlib import Path
import tempfile
import unittest

from PIL import Image
from tools.port.compare_capture_pair import compare, main


class CapturePairTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name)
        Image.new('RGBA',(320,240),(20,30,40,255)).save(self.root/'shot_final.png')
        Image.new('RGBA',(320,240),(20,30,40,255)).save(self.root/'port.png')
        self.native = dict(frame=3,cycles=500,pc=0x80001000,mode=21,phase='vsync',state_load_phase='restored',path='shot_final.raw',width=320,height=240,
                           image_status='ok',png='shot_final.png',png_sha256=self.pin('shot_final.png')['sha256'],
                           dumps=[dict(addr=0x800e7388,length=1,status='ok',data='15')])
        self.port = dict(mode=21,frame=42,phase='post_draw',sha256=self.pin('port.png')['sha256'])
        self.write('native.json',dict(shots=[self.native])); self.write('port.json',self.port)
        self.write('sync.json',dict(scope='synthetic unit fixture only'))
        self.pair = {'schema':'tokimemo.capture-pair.v1','scope':'synthetic unit fixture only',
            'native':{'record':self.pin('native.json'),'selector':['shots',0],'image':self.pin('shot_final.png')},
            'port':{'record':self.pin('port.json'),'selector':[],'image':self.pin('port.png'),'image_sha_path':['sha256'],'clock_paths':{'frame':['frame'],'phase':['phase']}},
            'native_map':{'schema':'tokimemo.native-memory-map.v1','fields':[{'name':'mode','address':'0x800E7388','type':'u8','length':1}]},
            'mappings':[{'name':'mode','native_field':'mode','port_path':['mode'],'meaning':'dispatcher game mode byte','compatibility':'equivalent'}],
            'checkpoint':{'name':'fixture','expected':{'mode':21}},
            'synchronization':{'status':'verified','basis':'synthetic matched fixture','evidence':self.pin('sync.json')}}

    def write(self,name,value): (self.root/name).write_text(json.dumps(value))
    def pin(self,name): return {'path':name,'sha256':hashlib.sha256((self.root/name).read_bytes()).hexdigest()}

    def test_identity_preserves_different_clock_domains(self):
        report,code=compare(self.pair,self.root)
        self.assertEqual(code,0)
        self.assertEqual(report['pixels']['differing_pixels'],0)
        self.assertEqual(report['clocks']['native']['cycles'],500)
        self.assertEqual(report['clocks']['port']['frame'],42)

    def test_edge_pixel_and_alpha_changes_fail(self):
        for position,color in [((0,0),(20,30,41,255)),((0,239),(20,30,40,254)),((319,0),(21,30,40,255))]:
            with self.subTest(position=position):
                image=Image.new('RGBA',(320,240),(20,30,40,255)); image.putpixel(position,color);image.save(self.root/'port.png')
                self.port['sha256']=self.pin('port.png')['sha256'];self.write('port.json',self.port)
                self.pair['port']['record']=self.pin('port.json');self.pair['port']['image']=self.pin('port.png')
                report,code=compare(self.pair,self.root)
                self.assertEqual(code,1);self.assertEqual(report['pixels']['differing_pixels'],1)
                self.assertEqual(report['pixels']['max_channel_difference'],1)

    def test_unverified_sync_and_incompatible_domains_fail(self):
        self.pair['synchronization']={'status':'unverified','basis':'independent clocks'}
        self.assertEqual(compare(self.pair,self.root)[1],1)
        incompatible=dict(self.pair['mappings'][0],name='different_domain',compatibility='incompatible',reason='fixture domain disagreement')
        self.pair['mappings'].append(incompatible)
        report,code=compare(self.pair,self.root)
        self.assertEqual(code,1)
        self.assertEqual(report['semantics'][1]['status'],'incompatible')
        self.pair['mappings'][0]['compatibility']='incompatible'
        self.pair['mappings'][0]['reason']='fixture'
        with self.assertRaises(ValueError): compare(self.pair,self.root)

    def test_dimension_mismatch_is_not_normalized(self):
        Image.new('RGBA',(640,480),(20,30,40,255)).save(self.root/'port.png')
        self.port['sha256']=self.pin('port.png')['sha256'];self.write('port.json',self.port)
        self.pair['port']['record']=self.pin('port.json');self.pair['port']['image']=self.pin('port.png')
        report,code=compare(self.pair,self.root)
        self.assertEqual(code,1);self.assertEqual(report['pixels']['status'],'dimension_mismatch')

    def test_missing_changed_and_unbound_evidence_exit_two(self):
        for defect in ('hash','field','binding','dump'):
            with self.subTest(defect=defect):
                pair=copy.deepcopy(self.pair)
                if defect=='hash': pair['native']['image']['sha256']='0'*64
                elif defect=='field': pair['mappings'][0]['port_path']=['absent']
                elif defect=='binding': pair['port']['image_sha_path']=['phase']
                else: pair['native_map']['fields'][0]['address']='0x800E7389'
                self.write('pair.json',pair)
                self.assertEqual(main([str(self.root/'pair.json'),'--out',str(self.root/(defect+'.json'))]),2)

    def test_equivalent_numeric_types_are_strict(self):
        for value in (True,21.0,'21',None):
            with self.subTest(value=value):
                self.port['mode']=value;self.write('port.json',self.port)
                self.pair['port']['record']=self.pin('port.json')
                with self.assertRaises(ValueError): compare(self.pair,self.root)

    def test_nonobject_manifest_and_selected_record_exit_two(self):
        self.write('bad-top.json',[])
        self.assertEqual(main([str(self.root/'bad-top.json'),'--out',str(self.root/'bad-top-report.json')]),2)
        self.write('port.json',[]);self.pair['port']['record']=self.pin('port.json')
        self.write('pair.json',self.pair)
        self.assertEqual(main([str(self.root/'pair.json'),'--out',str(self.root/'bad-record-report.json')]),2)


if __name__=='__main__': unittest.main()
