"""Failure-contract checks for the archive's subprocess-result reporting."""
from contextlib import redirect_stdout
from io import StringIO
from pathlib import Path
import subprocess
import tempfile
import unittest

from diagnostic import report

NAME = 'func_801449D4'
DIFF = (NAME + ': compared explicit object\n' + NAME
        + ': DIFF (- expected, + built)\n'
        '  -@row 3 27a4002c  ; addiu a0,sp,44\n'
        '  +@row 3 27a40030  ; addiu a0,sp,48\n'
        '  resolved: REAL DIFFERENCE, not naming.\n')
MATCH = NAME + ': MATCH (linked function bytes equal target (1100 bytes))\n'


class DiagnosticTests(unittest.TestCase):
    def check_result(self, code, stdout, stderr='', expected=None):
        result = subprocess.CompletedProcess(['funcdiff.py'], code, stdout, stderr)
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / 'result.diff'
            captured = StringIO()
            with redirect_stdout(captured):
                if expected is None:
                    with self.assertRaises(ValueError):
                        report(result, NAME, path)
                else:
                    self.assertEqual(report(result, NAME, path), expected)
            self.assertEqual(path.read_text(), stdout + stderr)
            if expected is None:
                self.assertEqual(captured.getvalue(), '')

    def test_expected_difference_is_successful_diagnostic(self):
        self.check_result(1, DIFF, expected=2)

    def test_match_requires_zero_exit(self):
        self.check_result(0, MATCH, expected=0)
        self.check_result(1, MATCH)

    def test_tool_failure_without_error_keyword_reports_no_score(self):
        self.check_result(1, 'objdump failed: unreadable object\n')
        self.check_result(1, 'Traceback (most recent call last):\nValueError: bad ELF\n')

    def test_wrong_status_or_incomplete_verdict_reports_no_score(self):
        self.check_result(2, DIFF)
        self.check_result(0, DIFF)
        self.check_result(0, '')
        self.check_result(1, DIFF.replace(NAME, 'func_80144E20'))
        self.check_result(1, NAME + ': DIFF (- expected, + built)\n')

    def test_exception_text_invalidates_apparent_verdict(self):
        self.check_result(1, DIFF + 'Traceback (most recent call last):\n')
        self.check_result(0, MATCH, 'RuntimeError: bad object\n')


if __name__ == '__main__':
    unittest.main()
