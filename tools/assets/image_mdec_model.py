"""T-39008 reviewed fixed-point MDEC numerical model, independently expressed.

Matches pinned DuckStation accurate-model execution on four original BS inputs
and numerical edge vectors. This is not hardware certification or Redux parity.
The BS transport remains shared with the repository reader, not an independent
validation of game VLC output. No external implementation is copied here.
"""
import struct
from tools.assets import image_mdec as preview

MODEL_ID = "tokimemo-mdec-fixed-point-v1"
MODEL = {
    "id": MODEL_ID,
    "bs_versions": [1, 2, 3],
    "quantization": "SDK default table; positive scales1..63",
    "arithmetic": "four fractional dequant bits; signed15 clamp; two rounded18-bit matrix passes; signed9 wrap and signed8 saturation; rounded RGB555",
    "reference": {
        "project": "DuckStation",
        "revision": "4ac22708f3f07f112b1e11877ee2401d0b670d6f",
        "source_sha256": "84f2c4e08eee890213e29cf760c275dfff59468a094ea213a12a11d13eedc4be",
        "path": "src/core/mdec.cpp",
        "scope": "accurate numeric model comparison; no copied implementation",
    },
    "hardware_pixel_parity_verified": False,
    "redux_aan_pixel_parity_verified": False,
    "game_runlevel_transport_independently_verified": False,
}

MATRIX = tuple(tuple(preview._signed(int(word,16),16) for word in row.split())
               for row in preview._SCALE_WORDS)


def clamp(value, low, high):
    return min(high,max(low,value))


def dequantize(level, quantizer, scale, dc=False):
    # Four fractional bits retained; nonzero coefficients biased half a unit
    # toward zero before the bounded signed-15-bit storage stage.
    product = level * quantizer if dc else (level * quantizer * scale) >> 3
    fraction = 0 if level == 0 else (8 if level < 0 else -8)
    return clamp(product * 16 + fraction,-16384,16383)


def coefficient_blocks(data):
    checked, metadata = preview._coefficients(data)
    version, scale = metadata['bs_version'],metadata['quantization']
    stream = preview._Bits(data[8:])
    blocks=[]
    predictors=[0,0,0]
    for _ in checked:
        component=min(len(blocks)%6,2)
        level=(preview._v3_dc(stream,component,predictors) if version==3
               else preview._signed(stream.take(10)))
        if level is None:raise ValueError('candidate premature DC end marker')
        block=[0]*64;block[0]=dequantize(level,preview.QUANT[0],scale,True)
        index=0
        while True:
            prefix=''
            for _ in range(16):
                prefix += str(stream.take(1))
                if prefix=='10': packed=0xfe00;break
                if prefix=='000001': packed=stream.take(16);break
                if prefix in preview.AC_CODES:
                    packed=preview.AC_CODES[prefix]
                    if stream.take(1):packed=(packed&0xfc00)|((-preview._signed(packed&1023))&1023)
                    break
            else:raise ValueError('candidate AC code differs from validated stream')
            if packed==0xfe00:break
            index+=(packed>>10)+1
            if index>63:raise ValueError('candidate AC index overflow')
            block[preview.ZIGZAG[index]]=dequantize(preview._signed(packed&1023),preview.QUANT[index],scale)
        blocks.append(block)
    end=(preview._v3_dc(stream,0,predictors) if version==3 else stream.take(10))
    if end!=(None if version==3 else 511):raise ValueError('candidate end marker differs')
    return blocks,metadata


def inverse_transform(block):
    # Two separable matrix passes; each row is independently accumulated using
    # unbounded Python integers, rounded at the stated fixed-point boundary.
    rows=[block[y*8:y*8+8] for y in range(8)]
    for _ in range(2):
        rows=[[clamp((sum(rows[z][y]*MATRIX[z][x] for z in range(8))+131072)>>18,-32768,32767)
               for x in range(8)] for y in range(8)]
    return [clamp(preview._signed(value&511,9),-128,127) for row in rows for value in row]


def color_word(y, cb, cr, stp=True):
    offsets=((359*cr+128)>>8,
             (((-88*cb)&~31)+((-183*cr)&~7)+128)>>8,
             (454*cb+128)>>8)
    channels=[]
    for offset in offsets:
        signed=preview._signed((y+offset)&511,9)
        channel=clamp(signed,-128,127)+128
        channels.append(min((channel+4)>>3,31))
    return channels[0]|(channels[1]<<5)|(channels[2]<<10)|(0x8000 if stp else 0)


def decode_padded_words(data,width,height):
    blocks,meta=coefficient_blocks(data)
    width,height,padded=preview._context(meta,{'width':width,'height':height,'evidence':['explicit candidate caller dimensions; not inferred from stream']})
    output=bytearray(width*padded*2)
    for index in range(meta['macroblock_count']):
        cr,cb,*luma=[inverse_transform(b) for b in blocks[index*6:index*6+6]]
        mx,my=index//(padded//16)*16,index%(padded//16)*16
        for y in range(16):
            for x in range(16):
                chroma=y//2*8+x//2
                luminance=luma[y//8*2+x//8][y%8*8+x%8]
                struct.pack_into('<H',output,((my+y)*width+mx+x)*2,color_word(luminance,cb[chroma],cr[chroma]))
    return bytes(output)



def decode_words(data,width,height):
    """Return caller-active rows; padding is still fully decoded first."""
    return decode_padded_words(data,width,height)[:width*height*2]
