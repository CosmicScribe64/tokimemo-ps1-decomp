"""Reversible code mappings, not a display/pixel parity acceptance rule."""
import unittest
from PIL import Image
from tools.port.rgb555_diagnostic import compare_codes,decode_channel


class RGB555DiagnosticTests(unittest.TestCase):
    def test_all_codes_roundtrip_under_both_explicit_conventions(self):
        for code in range(32):
            self.assertEqual(decode_channel(code<<3,'zero_extended'),code)
            self.assertEqual(decode_channel((code<<3)|(code>>2),'bit_replicated'),code)
        for value in range(256):
            if value not in {c<<3 for c in range(32)}|{(c<<3)|(c>>2) for c in range(32)}:
                with self.assertRaises(ValueError):decode_channel(value,'source_proven_mixed')

    def test_display_difference_is_only_diagnostic(self):
        a=Image.new('RGB',(32,1));b=Image.new('RGB',(32,1))
        for c in range(32):a.putpixel((c,0),(c<<3,)*3);b.putpixel((c,0),((c<<3)|(c>>2),)*3)
        report=compare_codes(a,b,'zero_extended','bit_replicated')
        self.assertEqual(report['different_color_codes'],0)
        self.assertFalse(report['acceptance'])
        b.putpixel((0,0),(8,0,0))
        self.assertEqual(compare_codes(a,b,'zero_extended','bit_replicated')['different_color_codes'],1)

    def test_no_lossy_alpha_or_resize(self):
        a=Image.new('RGBA',(1,1),(0,0,0,255));b=Image.new('RGBA',(1,1),(0,0,0,0))
        with self.assertRaises(ValueError):compare_codes(a,b,'zero_extended','zero_extended')
        with self.assertRaises(ValueError):compare_codes(a,Image.new('RGB',(2,2)),'zero_extended','zero_extended')


if __name__=='__main__':unittest.main()
