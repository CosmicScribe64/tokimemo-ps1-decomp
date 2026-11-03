"""Checks for source-bound translation corrections (T-39538)."""
import hashlib
import unittest
from tools.patch.text.reviewed_targets import reviewed_target


class ReviewedTargetsTests(unittest.TestCase):
    def setUp(self):
        self.source = b'original bytes'
        self.old = 'Bad text[[T001:row]]here[[T002:reply_variant]]Next.'
        self.row = {'target_text': self.old, 'protected_tokens': [{'marker': 'unchanged'}]}
        self.repairs = {'record': {
            'source_sha256': hashlib.sha256(self.source).hexdigest(),
            'previous_target_sha256': hashlib.sha256(self.old.encode()).hexdigest(),
            'target_text': 'Good text[[T001:row]]here[[T002:reply_variant]]Next.',
        }}

    def test_changes_only_reviewed_target(self):
        result = reviewed_target('record', self.source, self.row, self.repairs)
        self.assertTrue(result['target_text'].startswith('Good text'))
        self.assertEqual(result['protected_tokens'], self.row['protected_tokens'])
        self.assertEqual(self.row['target_text'], self.old)

    def test_unlisted_record_is_unchanged(self):
        self.assertIs(reviewed_target('other', self.source, self.row, self.repairs), self.row)

    def test_source_drift_fails(self):
        with self.assertRaisesRegex(ValueError, 'Source changed'):
            reviewed_target('record', b'new source', self.row, self.repairs)

    def test_target_drift_fails(self):
        with self.assertRaisesRegex(ValueError, 'Target changed'):
            reviewed_target('record', self.source, {'target_text': 'new target'}, self.repairs)

    def test_missing_or_reordered_markers_fail(self):
        for target in ('Good text', 'Good[[T002:reply_variant]]text[[T001:row]]'):
            with self.subTest(target=target):
                self.repairs['record']['target_text'] = target
                with self.assertRaisesRegex(ValueError, 'Control markers changed'):
                    reviewed_target('record', self.source, self.row, self.repairs)


if __name__ == '__main__':
    unittest.main()
