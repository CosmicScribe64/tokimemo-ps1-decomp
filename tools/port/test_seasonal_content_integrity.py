"""Original seasonal integrity regressions, run in the existing GUI test Docker image."""
import json
import os
from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest


@unittest.skipUnless(os.environ.get('TOKIMEMO_SEASONAL_PACK'), 'requires original generated seasonal pack and Godot')
class SeasonalIntegrityTests(unittest.TestCase):
    def test_payload_and_descriptor_adversaries(self):
        source=Path(os.environ['TOKIMEMO_SEASONAL_PACK'])
        for defect in ['hash','size','traversal','absolute','symlink','sequence_object','sequence_steps','frame_boolean','step_reference']:
            with self.subTest(defect=defect), tempfile.TemporaryDirectory() as tmp:
                target=Path(tmp)/'pack';shutil.copytree(source,target)
                doc=json.loads((target/'manifest.json').read_text())
                bg=next(iter(doc['backgrounds'].values()))
                spec=next(v for k,v in bg['files'].items() if k.endswith('.background.png'))
                sd=next(iter(doc['sd_sets'].values()))
                payload=target/'assets'/spec['path']
                if defect=='hash':
                    data=bytearray(payload.read_bytes());data[-1]^=1;payload.write_bytes(data)
                elif defect=='size':spec['size']+=1
                elif defect=='traversal':spec['path']='../'+spec['path']
                elif defect=='absolute':spec['path']=str(source/'assets'/spec['path'])
                elif defect=='symlink':
                    payload.unlink();payload.symlink_to(source/'assets'/spec['path'])
                elif defect=='sequence_object':sd['sequences'][0]=[]
                elif defect=='sequence_steps':sd['sequences'][0]['steps']='invalid'
                elif defect=='frame_boolean':sd['frames'][0]['frame_index']=False
                else:sd['sequences'][0]['steps'][0]['frame_index']=4095
                (target/'manifest.json').write_text(json.dumps(doc))
                env=dict(os.environ,SEASONAL_ORIGINAL=str(source),SEASONAL_REJECTED=str(target))
                result=subprocess.run(['godot','--headless','--path','port/godot','--script','tests/services/run_seasonal_integrity.gd'],env=env,text=True,capture_output=True,timeout=60)
                self.assertEqual(result.returncode,0,result.stdout+result.stderr)
                self.assertNotIn('SCRIPT ERROR',result.stdout+result.stderr)
                self.assertIn('Seasonal payload/descriptor rejection and clearing passed',result.stdout)


if __name__=='__main__':unittest.main()
