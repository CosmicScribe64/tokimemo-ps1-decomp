"""T-39511: source anomaly overrides must fail closed when their evidence changes."""
import hashlib
import unittest
from tools.patch.text.anomaly_repairs import repair_target


class RepairGuardTest(unittest.TestCase):
    def test_both_source_and_previous_translation_are_bound(self):
        source, previous = b'A\tB', 'partial [[T001:unknown]]'
        repairs = {'test': {'source_sha256': hashlib.sha256(source).hexdigest(),
                            'previous_target_sha256': hashlib.sha256(previous.encode()).hexdigest(),
                            'target_text': 'Complete sentence.'}}
        self.assertEqual(repair_target('test', source, previous, repairs), 'Complete sentence.')
        with self.assertRaisesRegex(ValueError, 'Source changed'):
            repair_target('test', b'new source', previous, repairs)
        with self.assertRaisesRegex(ValueError, 'Translation changed'):
            repair_target('test', source, 'new translation', repairs)
        self.assertIsNone(repair_target('another record', source, previous, repairs))


if __name__ == '__main__':
    unittest.main()
