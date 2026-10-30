"""T-39004: read unscaled Latin glyph masks from the authenticated game font.

No original glyph bitmap is committed. Artwork layout may trim empty horizontal
bearings; all occupied texels and twelve source rows remain unchanged.
"""
import sys

from translation_assets import ROOT,digest
sys.path.insert(0,str(ROOT))
from tools.assets.text_font import font_metadata,font_reference


class NativeLatin:
    def __init__(self,executable):
        self.data=executable.read_bytes()
        self.metadata=font_metadata(self.data)
        self.glyphs={}
        self.proof={}

    def glyph(self,character):
        if character==' ': return {'points':[],'width':4}
        if character in self.glyphs: return self.glyphs[character]
        punctuation={"'":'’','-':'－','"':'”'}
        full=punctuation.get(character,chr(ord(character)+0xfee0) if 0x21<=ord(character)<=0x7e else character)
        code=full.encode('cp932')
        reference=font_reference(code,self.metadata)
        if reference['status']!='glyph': raise ValueError('character missing from native font')
        raw=self.data[reference['offset']:reference['offset']+18]
        points=[(i%12,i//12) for i in range(144) if raw[i//8]>>(7-i%8)&1]
        if not points: raise ValueError('native character is blank')
        left=min(x for x,y in points);right=max(x for x,y in points)
        self.glyphs[character]={'points':[(x-left,y) for x,y in points],'width':right-left+1}
        self.proof[character]={'code':code.hex(),'offset':reference['offset'],'sha256':digest(raw),'trim_left':left,'trim_right':11-right}
        return self.glyphs[character]

    def width(self,text):
        return sum(self.glyph(c)['width']+1 for c in text)-1 if text else 0

    def provenance(self):
        return {'source_sha256':self.metadata['source_sha256'],'glyph_bank_sha256':self.metadata['glyph_bank_sha256'],
            'source_height':12,'rescaled':False,'glyphs':dict(self.proof)}
