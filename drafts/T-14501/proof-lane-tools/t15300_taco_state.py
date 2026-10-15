"""Use the canonical unsigned GameState field in a boolean mask test."""
from pathlib import Path
p=Path('src/ovl/TACO/801335A0.c');s=p.read_text().replace('extern s32 D_800E7378;\n','').replace('D_800E7378 & 0xFF','D_800E6280.unk_10F8 & 0xFF');p.write_text(s)
