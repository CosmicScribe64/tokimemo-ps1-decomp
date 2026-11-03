"""Source provenance and non-mutating dialogue review (T-39537)."""
import unittest
from tools.patch.text.break_review import review
from tools.patch.text.wrap import FontMetrics


class BreakReviewTests(unittest.TestCase):
    def setUp(self):
        self.metrics = FontMetrics({n: 1 for n in range(32, 127)}, 2, 16)

    def test_generated_phrase_split(self):
        findings = review(b'We went to the old hall.', b'We went to the\nold hall.', self.metrics, 20)
        self.assertEqual(findings[0]['origin'], 'wrapped')
        self.assertIn('phrase_link_at_row_end', findings[0]['reasons'])

    def test_retained_pause(self):
        text = b'Wait.\nPlease.'
        findings = review(text, text, self.metrics, 40)
        self.assertEqual(findings[0]['origin'], 'retained')
        self.assertIn('short_single_word_row', findings[0]['reasons'])

    def test_reply_boundaries_are_not_line_breaks(self):
        text = b'Yes.\x0bNo.'
        self.assertEqual(review(text, text, self.metrics, 40), [])

    def test_provenance_survives_added_rows(self):
        source = b'A long sentence ends here.\nWait.'
        result = b'A long sentence\nends here.\nWait.'
        findings = review(source, result, self.metrics, 30)
        pause = next(row for row in findings if row['after_row'] == 1)
        self.assertEqual(pause['origin'], 'retained')
        self.assertEqual(source, b'A long sentence ends here.\nWait.')
        self.assertEqual(result, b'A long sentence\nends here.\nWait.')

    def test_identical_replies_have_independent_provenance(self):
        source = b'Hello.\nWait.\x0bHello. Wait.'
        result = b'Hello.\nWait.\x0bHello.\nWait.'
        findings = review(source, result, self.metrics, 40)
        self.assertEqual([r['origin'] for r in findings], ['retained', 'wrapped'])

    def test_separated_punctuation(self):
        findings = review(b'Hello!', b'Hello\n!', self.metrics, 20)
        self.assertIn('punctuation_only_row', findings[0]['reasons'])

    def test_name_substitution_is_not_punctuation(self):
        findings = review(b'Hello, \x10.', b'Hello,\n\x10.', self.metrics, 40)
        self.assertFalse(any('punctuation_only_row' in row['reasons'] for row in findings))

    def test_numbered_title_stays_a_review_candidate(self):
        findings = review(b'We are No. 1.', b'We are No.\n1.', self.metrics, 40)
        self.assertIn('number_label_split', findings[0]['reasons'])

    def test_number_counts_as_a_word(self):
        findings = review(b'We took over 500?', b'We took\nover 500?', self.metrics, 40)
        self.assertFalse(any('short_single_word_row' in row['reasons'] for row in findings))


if __name__ == '__main__':
    unittest.main()
