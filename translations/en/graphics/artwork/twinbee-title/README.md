# TwinBee title lettering

`wordmark.png` replaces the Japanese title with **TWINBEE**. It was generated for this patch from the original title's color and lettering style, then fitted to the native 192×64 sprite canvas.

The build maps its colors to the original 16-color palette. The two sprite pieces keep their original positions and animation. The English "TIME ATTACK" subtitle, palette, and all texels outside those pieces stay unchanged.

`tools/patch/art/twinbee.py` places and verifies the replacement. The original game artwork is not included here. See T-39548 for native screenshots and release verification.
