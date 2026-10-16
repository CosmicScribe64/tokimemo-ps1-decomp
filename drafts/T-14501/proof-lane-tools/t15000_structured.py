"""Replace m2c's shared-store goto with the two source-level branches."""
from pathlib import Path
p=Path('src/ovl/KANGEI/801355F0.c');s=p.read_text();s=s.replace('        goto block_4;\n    }\n    if (D_800E6280.unk_F5F == 3) {','        D_80120657 = 0;\n    } else if (D_800E6280.unk_F5F == 3) {').replace('block_4:\n','');p.write_text(s)
