"""Regenerate the opening plaque from its lettering plan (T-39523).

The registered brush-lettered crop is replaced at build time. Generated pixels
stay in the ignored cache; the existing still encoder verifies the disc image.
"""
import hashlib

import translation_bs_stills as lettering
from tools.assets import mdec_splice
from tools.patch.art import ArtBuildError
from tools.patch.art.items import Image as ArtImage

PATH = 'CDROM/BS/P_01_01.BS'
IMAGE_ID = 'bs-p-01-01-gate-plate'
SOURCE_SHA256 = '280b440f4f005d53340afd5389cd755a5bbb574fa30c36013df484d039fd7aab'


def replace(items, root, files_root):
    """Replace exactly the registered plaque crop at its original geometry."""
    data = (files_root / PATH).read_bytes()
    if hashlib.sha256(data).hexdigest() != SOURCE_SHA256:
        raise ArtBuildError('School gate source differs from the verified original')
    plan = next(plan for plan in lettering.PLANS if plan['id'] == IMAGE_ID)
    source = mdec_splice.decode_rgb5(data, 320, 160)
    goal, _facts = lettering.draw_plan(source, plan)
    crop, _changed = lettering.png_rgba(source, goal, plan['rect'])
    path = root / '.asset-cache/art/generated/school-gate.png'
    path.parent.mkdir(parents=True, exist_ok=True)
    crop.save(path)
    digest = hashlib.sha256(path.read_bytes()).hexdigest()
    meta = {'geometry': {'width': 320, 'height': 160},
            'crop': {'rect': list(plan['rect'])}, 'source_container': PATH}
    count = 0
    for item in items:
        if item.container != PATH:
            continue
        for index, image in enumerate(item.images):
            if image.id == IMAGE_ID:
                item.images[index] = ArtImage(IMAGE_ID, path, digest, 'generated-clean-v1',
                                              meta)
                count += 1
    if count != 1:
        raise ArtBuildError('Expected exactly one registered school gate plaque')
