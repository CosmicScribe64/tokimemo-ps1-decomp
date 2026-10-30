"""Candidate numerical edge vectors; full decoder accuracy needs external words."""
import unittest
from tools.assets.image_mdec_candidate import color_word,dequantize,inverse_transform,decode_words,coefficient_blocks
from tools.test_asset_image_mdec import bs


class CandidateTests(unittest.TestCase):
    def test_color_rounding_clipping_and_sign_wrap(self):
        self.assertEqual(color_word(0,0,0),0xc210)
        self.assertEqual(color_word(-128,0,0),0x8000)
        self.assertEqual(color_word(127,0,0),0xffff)
        self.assertEqual(color_word(3,0,0)&31,16)
        self.assertEqual(color_word(4,0,0)&31,17)
        self.assertEqual(color_word(256,0,0),0x8000)
        self.assertEqual(color_word(0,0,0,False),0x4210)

    def test_dequant_precision_signed_bias_and_limits(self):
        self.assertEqual(dequantize(1,2,4,True),24)
        self.assertEqual(dequantize(-1,2,4,True),-24)
        self.assertEqual(dequantize(0,83,63),0)
        self.assertEqual(dequantize(511,83,63),16383)
        self.assertEqual(dequantize(-512,83,63),-16384)

    def test_idct_zero_and_independent_dc_vectors(self):
        self.assertEqual(inverse_transform([0]*64),[0]*64)
        for dc,expected in [(100,25),(-100,-25)]:
            block=[0]*64;block[0]=dequantize(dc,2,4,True)
            self.assertEqual(inverse_transform(block),[expected]*64)

    def test_malformed_stream_rejected(self):
        with self.assertRaises(ValueError):decode_words(b'',320,160)

    def test_v3_differential_predictors_reset_and_signed_precision(self):
        codes=['011','1001','0111','100','000','0110']
        codes+=['00','00']+['100']*4
        data=bs([c+'10' for c in codes],version=3)
        blocks,meta=coefficient_blocks(data)
        expected=[4,-8,12,12,8,16,4,-8,16,16,16,16]
        self.assertEqual([b[0] for b in blocks],[dequantize(v,2,1,True) for v in expected])
        self.assertEqual(meta['bs_version'],3)
        self.assertEqual(coefficient_blocks(data)[0],blocks)
        overflow=['11111110'+'01111111','00']+['100']*4
        overflow+=['010','00']+['100']*4
        with self.assertRaises(ValueError):coefficient_blocks(bs([c+'10' for c in overflow],version=3))

    def test_v3_gray_complete_frame(self):
        codes=(['00','00']+['100']*4)*120
        data=bs([c+'10' for c in codes],version=3)
        self.assertEqual(decode_words(data,320,88),b'\x10\xc2'*(320*88))


if __name__=='__main__':unittest.main()
