"""Synthetic native DATE initializer and source-identity regressions, T-28007."""
import unittest
from tools.assets.text_date_context import DateContextError, replay_initializer, source_assignments


class DateInitializerTests(unittest.TestCase):
    def example(self):
        # LUI/ORI t0 = synthetic value, LUI at = synthetic global base,
        # JR ra with the one global store in its delay slot.
        return [0x3C081234, 0x35085678, 0x3C012000, 0x03E00008, 0xAC280010]

    def test_return_delay_store_is_applied(self):
        result = replay_initializer(self.example())
        self.assertEqual(result, {0x20000010: {"value": 0x12345678, "instruction_index": 4}})

    def test_signed_store_offset(self):
        words = self.example()
        words[-1] = 0xAC28FFF0
        self.assertEqual(set(replay_initializer(words)), {0x1FFFFFF0})

    def test_unknown_value_register_rejected(self):
        words = self.example()
        words[-1] = 0xAC290010
        with self.assertRaises(DateContextError):
            replay_initializer(words)

    def test_unknown_base_register_rejected(self):
        words = self.example()
        words[-1] = 0xACA80010
        with self.assertRaises(DateContextError):
            replay_initializer(words)

    def test_nonreturn_control_rejected(self):
        for word in (0x0C000010, 0x10000001, 0x08000010, 0x03200008):
            with self.subTest(word=word):
                words = self.example()
                words.insert(3, word)
                with self.assertRaises(DateContextError):
                    replay_initializer(words)

    def test_early_return_rejected(self):
        words = self.example()
        words.insert(3, 0x03E00008)
        with self.assertRaises(DateContextError):
            replay_initializer(words)

    def test_duplicate_store_rejected(self):
        words = self.example()
        words.insert(3, words[-1])
        with self.assertRaises(DateContextError):
            replay_initializer(words)

    def test_unaligned_store_rejected(self):
        words = self.example()
        words[-1] += 1
        with self.assertRaises(DateContextError):
            replay_initializer(words)

    def test_missing_delay_rejected(self):
        with self.assertRaises(DateContextError):
            replay_initializer(self.example()[:-1])

    def test_unknown_ori_source_rejected(self):
        words = self.example()
        words[1] = 0x35285678
        with self.assertRaises(DateContextError):
            replay_initializer(words)

    def test_source_assignments_bind_named_globals(self):
        result = source_assignments("void demo(void) {\n alpha = 0x12345678;\n}", "demo", {"alpha": 0x20000010})
        self.assertEqual(result, {0x20000010: {"symbol": "alpha", "value": 0x12345678}})

    def test_source_call_or_expression_rejected(self):
        for body in ("alpha = 0x10; helper();", "alpha = 0x10 + 4;", "if (flag) alpha = 0x10;", "alpha = beta;"):
            with self.subTest(body=body):
                with self.assertRaises(DateContextError):
                    source_assignments("void demo(void) {" + body + "}", "demo", {"alpha": 0x1000})

    def test_source_duplicate_store_or_symbol_rejected(self):
        for body, symbols in (("alpha = 0x10; alpha = 0x20;", {"alpha": 0x1000}),
                              ("alpha = 0x10;", {}),
                              ("alpha = 0x10; beta = 0x20;", {"alpha": 0x1000, "beta": 0x1000})):
            with self.assertRaises(DateContextError):
                source_assignments("void demo(void) {" + body + "}", "demo", symbols)

    def test_duplicate_function_rejected(self):
        source = "void demo(void) {alpha = 0x10;}\nvoid demo(void) {alpha = 0x10;}"
        with self.assertRaises(DateContextError):
            source_assignments(source, "demo", {"alpha": 0x1000})


if __name__ == "__main__":
    unittest.main()
