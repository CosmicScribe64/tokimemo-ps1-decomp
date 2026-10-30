"""Synthetic decoder checks; actual captures are checked separately (T-39008)."""
import copy
import json
from pathlib import Path
import tempfile
import unittest

from tools.port.native_memory_projection import main, project_sample, validate_map


class NativeProjectionTests(unittest.TestCase):
    def setUp(self):
        self.mapping = {"schema": "tokimemo.native-memory-map.v1", "fields": [
            {"name": "signed", "address": "0x80000000", "type": "s16", "length": 2},
            {"name": "unsigned", "address": "0x80000002", "type": "u32", "length": 4},
            {"name": "raw", "address": "0x80000000", "type": "hex", "length": 6}]}
        self.sample = {"frame": 23, "cycles": 456789, "pc": 0x80100000, "mode": 20,
                       "phase": "vsync_after_input_override_before_next_cpu_tick", "tag": "sample_23",
                       "dumps": [{"addr": 0x80000000, "length": 6, "status": "ok", "data": "feff78563412"}]}

    def test_decodes_little_endian_and_preserves_clock(self):
        result = project_sample(self.sample, self.mapping, "map-pin")
        self.assertEqual(result["fields"], {"signed": -2, "unsigned": 0x12345678, "raw": "feff78563412"})
        self.assertEqual(result["capture"], {k: v for k, v in self.sample.items() if k != "dumps"})
        self.assertEqual(result["map_sha256"], "map-pin")

    def test_all_integer_widths(self):
        for kind, width, expected in [("u8",1,255),("s8",1,-1),("u16",2,65535),("s16",2,-1),("u32",4,0xffffffff),("s32",4,-1)]:
            with self.subTest(kind=kind):
                mapping = {"schema": self.mapping["schema"], "fields": [{"name":"v","address":0x80000000,"type":kind,"length":width}]}
                sample = copy.deepcopy(self.sample)
                sample["dumps"] = [{"addr":0x80000000,"length":width,"status":"ok","data":"ff"*width}]
                self.assertEqual(project_sample(sample,mapping,"pin")["fields"]["v"],expected)

    def test_bad_dump_evidence_rejected(self):
        for change in [{"status":"unreadable"},{"data":"feff"},{"data":"gg"*6},{"length":5},{"length":0},{"addr":0xffffffff}]:
            with self.subTest(change=change):
                sample = copy.deepcopy(self.sample)
                sample["dumps"][0].update(change)
                with self.assertRaises(ValueError): project_sample(sample,self.mapping,"pin")
        for key in ("status","length","data"):
            sample = copy.deepcopy(self.sample)
            del sample["dumps"][0][key]
            with self.assertRaises((ValueError,KeyError)): project_sample(sample,self.mapping,"pin")

    def test_missing_overlap_and_split_ranges_fail(self):
        for dumps in [[],self.sample["dumps"]*2,[{"addr":0x80000000,"length":2,"status":"ok","data":"feff"},{"addr":0x80000002,"length":4,"status":"ok","data":"78563412"}]]:
            sample = dict(self.sample,dumps=dumps)
            with self.assertRaises(ValueError): project_sample(sample,self.mapping,"pin")

    def test_missing_clock_fails(self):
        for key in ("frame","cycles","phase","pc","mode"):
            sample = dict(self.sample)
            del sample[key]
            with self.assertRaises(ValueError): project_sample(sample,self.mapping,"pin")

    def test_invalid_map_fails(self):
        for update in [{"type":"float"},{"length":1},{"address":-1},{"name":""}]:
            mapping = copy.deepcopy(self.mapping)
            mapping["fields"][0].update(update)
            with self.assertRaises(ValueError): validate_map(mapping)
        mapping = copy.deepcopy(self.mapping)
        mapping["fields"].append(mapping["fields"][0])
        with self.assertRaises(ValueError): validate_map(mapping)

    def test_cli_atomic_validation_and_exclusive_output(self):
        with tempfile.TemporaryDirectory() as root:
            root = Path(root)
            map_path, samples, out = root/'map.json', root/'samples.jsonl', root/'out.jsonl'
            map_path.write_text(json.dumps(self.mapping))
            samples.write_text(json.dumps(self.sample)+'\n'+json.dumps(dict(self.sample,dumps=[]))+'\n')
            args = ['--map',str(map_path),'--samples',str(samples),'--out',str(out)]
            with self.assertRaises(ValueError): main(args)
            self.assertFalse(out.exists())
            samples.write_text(json.dumps(self.sample)+'\n')
            self.assertEqual(main(args),0)
            before = out.read_bytes()
            with self.assertRaises(FileExistsError): main(args)
            self.assertEqual(out.read_bytes(),before)

    def test_committed_map_covered_by_declared_ranges(self):
        mapping = json.loads(Path('config/port/name-entry-memory-projection.json').read_text())
        sample = dict(self.sample,dumps=[])
        for spec in mapping['required_dumps']:
            address,length = spec.split(':')
            sample['dumps'].append(dict(addr=int(address,16),length=int(length),status='ok',data='00'*int(length)))
        result = project_sample(sample,mapping,'fixture')
        self.assertEqual(len(result['fields']),len(mapping['fields']))
        self.assertEqual(result['fields']['date_frame_table'],0)


if __name__ == '__main__':
    unittest.main()
