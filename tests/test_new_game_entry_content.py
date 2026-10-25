"""Invented original-input shape tests for the bounded kana/preset adapter."""
import hashlib
import importlib.util
import os
from pathlib import Path
import struct
import sys
import unittest
from unittest.mock import patch
ROOT=next(p for p in Path(__file__).resolve().parents if (p/'tools/assets/input.py').exists())
sys.path.insert(0,str(ROOT))
TARGET=Path(os.environ.get('NEW_GAME_ENTRY_ADAPTER_PATH',ROOT/'tools/port/new_game_entry_content.py'))
spec=importlib.util.spec_from_file_location('entry_content',TARGET);entry=importlib.util.module_from_spec(spec);spec.loader.exec_module(entry)
class Source:
    def __init__(self,data):self.data=data
    def __enter__(self):return self
    def __exit__(self,*args):pass
    def entries(self):return [{'path':entry.original.OVERLAY}]
    def read(self,e):return bytes(self.data)
class EntryTests(unittest.TestCase):
    def setUp(self):
        self.data=bytearray(0x20000)
        for key in ['hiragana','katakana']:
            address,_=entry.FIELDS[key]
            for i in range(17):
                pointer=entry.BASE+0x1000+i*16+(0x400 if key=='katakana'else 0)
                struct.pack_into('<I',self.data,address-entry.BASE+i*4,pointer)
                self.data[pointer-entry.BASE:pointer-entry.BASE+10]=bytes([1,2,3,4,5,6,7,8,9,10])
    def build(self):
        with patch.object(entry.original,'build_bundle',return_value={'tables':{'space':[250,251]},'proof':{'source_pins':{}}}),patch.object(entry,'open_input',return_value=Source(self.data)),patch.object(entry.original,'OVERLAY_SHA256',hashlib.sha256(self.data).hexdigest()),patch.object(entry,'PAYLOAD_SHA256',''):
            return entry.build_bundle('invented')
    def test_bounded_invented_rows(self):
        b=self.build();self.assertEqual(len(b['content']['hiragana']),17);self.assertEqual(len(b['content']['preset_nickname']),15)
    def test_odd_native_extent_preserved(self):
        self.data[0x100a]=99
        self.assertEqual(len(self.build()['content']['hiragana'][0]),11)
    def test_pointer_outside_overlay_rejected(self):
        struct.pack_into('<I',self.data,entry.FIELDS['hiragana'][0]-entry.BASE,entry.BASE-1)
        with self.assertRaises(ValueError):self.build()
    def test_missing_terminator_rejected(self):
        self.data[0x1000:0x1021]=b'X'*33
        with self.assertRaises(ValueError):self.build()
    def test_preset_tail_rejected(self):
        self.data[entry.FIELDS['preset_family'][0]-entry.BASE+7]=1
        with self.assertRaises(ValueError):self.build()
if __name__=='__main__':unittest.main()
