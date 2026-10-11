#!/usr/bin/env python3
"""Reproduce IDO aggregate home-slot measurements from T-9610 in Docker.

These synthetic cases check the compiler's measured behavior, including dead
local homes and frame_pass's separate relocation of locals and register saves.
No game data or expected game assembly is used.
"""
import os
from pathlib import Path
import subprocess
import tempfile
import unittest

import cc

ROOT = Path(__file__).resolve().parent.parent
IDO = Path(os.environ.get('IDO_DIR', '/opt/ido')) / '5.3/cc'
PREFIX = '''#include "common.h"
typedef struct { s16 value[3]; } Half3;
typedef struct { u8 value[7]; } Byte7;
typedef struct { s32 value[3]; } Word3;
extern Half3 halves;
extern Byte7 bytes;
extern Word3 words;
extern s32 consume(void *);
'''


class LocalFrameTests(unittest.TestCase):
    def compile(self, body, transformed):
        with tempfile.TemporaryDirectory(prefix='aggregate-frame-') as directory:
            source = Path(directory) / 'probe.c'
            obj = Path(directory) / 'probe.o'
            source.write_text(PREFIX + 's32 probe(s32 arg) { ' + body + ' }\n')
            if transformed:
                cmd = ['python3', 'tools/cc.py', str(source), str(obj), 'ido', '5.3']
            else:
                cmd = [str(IDO)] + cc.IDO_CFLAGS + [str(source), '-o', str(obj)]
            result = subprocess.run(cmd, cwd=ROOT, capture_output=True, text=True)
            self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
            return subprocess.check_output(['mips-linux-gnu-objdump', '-dr', str(obj)],
                                           text=True)

    def test_nonleaf_declaration_order(self):
        cases = [
            ('Half3 local;', 'halves', 32, 24),
            ('s32 unused; Half3 local;', 'halves', 40, 28),
            ('Half3 local; s32 unused;', 'halves', 40, 32),
            ('Byte7 local;', 'bytes', 32, 24),
            ('Word3 local;', 'words', 40, 28),
        ]
        for declarations, symbol, frame, home in cases:
            for transformed in (False, True):
                with self.subTest(declarations=declarations, transformed=transformed):
                    asm = self.compile(declarations + f' local={symbol}; '
                                       'return consume(&local);', transformed)
                    shift = 16 if transformed else 0
                    self.assertRegex(asm, rf'addiu\s+sp,sp,-{frame + shift}\b')
                    self.assertRegex(asm, rf'addiu\s+a0,sp,{home + shift}\b')
                    self.assertRegex(asm, r'sw\s+ra,20\(sp\)')
                    self.assertRegex(asm, rf'sw\s+a0,{frame + shift}\(sp\)')

    def test_leaf_and_frameless(self):
        for transformed in (False, True):
            with self.subTest(transformed=transformed):
                asm = self.compile('Word3 local; local=words; '
                                   'return local.value[arg];', transformed)
                shift = 16 if transformed else 0
                self.assertRegex(asm, rf'addiu\s+sp,sp,-{16 + shift}\b')
                self.assertRegex(asm, rf'addiu\s+v1,sp,{4 + shift}\b')
                asm = self.compile('s32 n; n=arg+1; return n;', transformed)
                self.assertNotRegex(asm, r'addiu\s+sp,sp,')


if __name__ == '__main__':
    unittest.main()
