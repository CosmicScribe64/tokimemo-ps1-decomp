"""T-39312: unit tests of the generator helpers (synthetic input only). Run in the engine-gen image:
docker run --rm -v "$PWD":/work -w /work tokimemo-engine-gen python3 -I -m unittest discover -s tools/port/engine_gen -p 'test_*.py'
"""
import os
import sys
import unittest

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from pycparser import c_parser  # noqa: E402
import cgen  # noqa: E402
import ctypes_  # noqa: E402
import uninit  # noqa: E402


class Helpers(unittest.TestCase):
    def test_char_escapes(self):
        self.assertEqual(ctypes_.parse_int("'\\x1b'"), 0x1B)
        self.assertEqual(ctypes_.parse_int("'\\033'"), 0o33)
        self.assertEqual(ctypes_.parse_int("'\\n'"), 10)
        self.assertEqual(ctypes_.parse_int("'A'"), 65)

    def test_wrap_keeps_short_lines_and_breaks_inside_brackets(self):
        short = '\tx = 1'
        self.assertEqual(cgen.wrap_line(short), [short])
        long_call = '\tengine.call_c("f", [' + ', '.join('0x%X' % (0x10000 + i) for i in range(30)) + '])'
        lines = cgen.wrap_line(long_call)
        self.assertGreater(len(lines), 1)
        self.assertTrue(all(len(line.replace('\t', '    ')) <= 120 for line in lines))
        self.assertEqual(''.join(l.strip() for l in lines).replace(' ', ''), long_call.strip().replace(' ', ''))

    def test_lowp_parenthesises_lower_precedence_operands(self):
        self.assertEqual(cgen.lowp('a << 2'), '(a << 2)')
        self.assertEqual(cgen.lowp('a * 2'), 'a * 2')
        self.assertEqual(cgen.lowp('f(a & b)'), 'f(a & b)')


class Uninit(unittest.TestCase):
    def check(self, source, name='f'):
        ast = c_parser.CParser().parse(source)
        fd = [e for e in ast.ext if getattr(e, 'decl', None) is not None and e.decl.name == name][0]
        return uninit.check(fd, [])

    def test_read_before_write(self):
        self.assertEqual(self.check('int f(int a) { int x; if (a) { x = 1; } return x; }'), [('x', 'read')])

    def test_assigned_on_every_path(self):
        self.assertEqual(self.check('int f(int a) { int x; if (a) { x = 1; } else { x = 2; } return x; }'), [])

    def test_label_after_return_is_reachable(self):
        self.assertEqual(self.check('int f(int a) { int x; if (a) goto L; x = 1; return x; L: return x; }'), [('x', 'read')])


if __name__ == '__main__':
    unittest.main()
