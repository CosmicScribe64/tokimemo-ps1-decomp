"""Invented-byte table-contract tests; original endpoint tests are separate."""
import copy
import hashlib
import importlib.util
import os
import sys
import unittest
from pathlib import Path
from unittest.mock import patch
ROOT=next(p for p in Path(__file__).resolve().parents if (p/'tools/assets/input.py').exists())
MODULE=Path(os.environ.get('NEW_GAME_ADAPTER_PATH',str(ROOT/'tools/port/new_game_tables.py')))
spec=importlib.util.spec_from_file_location('new_game_tables',MODULE)
tables=importlib.util.module_from_spec(spec);spec.loader.exec_module(tables)

class ContractTests(unittest.TestCase):
    def setUp(self):
        self.values={'compatibility':[1]*16,'initial_scores':list(range(20)),'space':[250,251], 'cheat_names':[[i+1,240] for i in range(9)],'girl_birthdays':[1,2]*14,'girl_bloods':[0]*12,'girl_fixed_clubs':list(range(7))}
        self.pins={}
        for key,value in self.values.items():
            raw=b''.join(bytes(row)+b'\0' for row in value) if key=='cheat_names' else bytes(value)
            self.pins[key]=hashlib.sha256(raw).hexdigest()
        self.bundle={'schema':'tokimemo.new-game-tables.v1','tables':self.values,'proof':{'source_pins':tables.SOURCE_PINS,'overlay_sha256':tables.OVERLAY_SHA256}}
    def validate(self):
        with patch.object(tables,'PAYLOAD_PINS',self.pins):return tables.validate(self.bundle)
    def test_invented_valid_bundle(self):self.assertEqual(self.validate(),self.bundle)
    def test_mutated_payload(self):
        self.values['initial_scores'][0]=99
        with self.assertRaises(ValueError):self.validate()
    def test_boolean_is_not_byte(self):
        self.values['space'][0]=True
        with self.assertRaises(ValueError):self.validate()
    def test_fraction_is_not_byte(self):
        self.values['space'][0]=1.5
        with self.assertRaises(ValueError):self.validate()
    def test_missing_table(self):
        del self.values['compatibility']
        with self.assertRaises(ValueError):self.validate()
    def test_changed_release(self):
        self.bundle['proof']['overlay_sha256']='0'*64
        with self.assertRaises(ValueError):self.validate()
    def test_bad_cheat_count(self):
        self.values['cheat_names'].pop()
        with self.assertRaises(ValueError):self.validate()
    def test_bad_source_pin(self):
        self.bundle['proof']['source_pins']={}
        with self.assertRaises(ValueError):self.validate()
    def test_exact_source_pins(self):self.assertEqual(tables.source_proof(),tables.SOURCE_PINS)

class DecoderTests(unittest.TestCase):
    def test_invented_byte_field_assignment(self):
        # lui t0,0x8010; lbu t1,4(t0); andi t2,t1,0xf9;
        # ori t3,t2,4; sb t3,4(t0). No original instruction fixture.
        words=[0x3c088010,0x91090004,0x312a00f9,0x354b0004,0xa10b0004]
        blob=b''.join(w.to_bytes(4,'little')for w in words)
        stores=tables._field_stores(blob,0x1000,0x1000,0x1014)
        self.assertEqual([(r['address'],r['mask'],r['value'])for r in stores],[(0x80100004,0xf9,4)])
    def test_wrong_store_address_has_no_witness(self):
        words=[0x3c088010,0x91090004,0x312a00f9,0x354b0004,0xa10b0005]
        blob=b''.join(w.to_bytes(4,'little')for w in words)
        self.assertEqual(tables._field_stores(blob,0x1000,0x1000,0x1014),[])
    def test_forced_bits_canonicalized(self):
        words=[0x3c088010,0x91090004,0x352a0006,0xa10a0004]
        blob=b''.join(w.to_bytes(4,'little')for w in words)
        self.assertEqual(tables._field_stores(blob,0x1000,0x1000,0x1010)[0]['mask'],0xf9)
    def test_unknown_value_cannot_prove_store(self):
        words=[0x3c088010,0x91090004,0x012a4821,0xa1090004]
        blob=b''.join(w.to_bytes(4,'little')for w in words)
        self.assertEqual(tables._field_stores(blob,0x1000,0x1000,0x1010),[])

if __name__=='__main__':unittest.main()
