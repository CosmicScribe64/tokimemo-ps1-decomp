"""T-39533: regenerate the rental sign and the paired roster lettering."""
import hashlib

import translation_bs_signs as signs
import translation_bs_stills as roster
from tools.assets import mdec_splice
from tools.patch.art import ArtBuildError
from tools.patch.art.items import Image as ArtImage

SOURCES = (
    ('I_28_02', 'bs-i-28-02-counter-sign',
     '56f448b9d7045f452c718e9b0799a1821abeab2a3bfeb3b715f3ac2efcc50d40'),
    ('P_01_03', 'bs-p-01-03-roster-a-rotated',
     'aa2e82335720f7091a54ccc26e7e0a94be3a75fa1513d6bc1d5e1f3e36e5b230'),
    ('P_01_03B', 'bs-p-01-03b-name-a-rotated',
     '090ad2700ac094805298eabeb90560308e89ad7cca098c27146a24b8181b5c92'),
)


def replace(items, root, files_root):
    """Replace registered crops; retain the roster pair's shared name position."""
    for name, image_id, source_hash in SOURCES:
        container = 'CDROM/BS/' + name + '.BS'
        data = (files_root / container).read_bytes()
        if hashlib.sha256(data).hexdigest() != source_hash:
            raise ArtBuildError('Sign lettering source changed: ' + name)
        source = mdec_splice.decode_rgb5(data, 320, 160)
        module, plans = (signs, signs.SIGNS) if name == 'I_28_02' else (roster, roster.PLANS)
        plan = next(p for p in plans if p['id'] == image_id)
        goal, _facts = module.draw_plan(source, plan)
        crop, _changed = roster.png_rgba(source, goal, plan['rect'])
        path = root / '.asset-cache/art/generated' / (image_id + '.png')
        path.parent.mkdir(parents=True, exist_ok=True)
        crop.save(path)
        digest = hashlib.sha256(path.read_bytes()).hexdigest()
        meta = {'geometry': {'width': 320, 'height': 160},
                'crop': {'rect': list(plan['rect'])}, 'source_container': container}
        count = 0
        for item in items:
            for index, image in enumerate(item.images):
                if image.id == image_id and image.meta.get('source_container', item.container) == container:
                    item.images[index] = ArtImage(image_id, path, digest, 'generated-spaced-v1', meta)
                    count += 1
        if count != 1:
            raise ArtBuildError('Expected one registered lettering crop: ' + image_id)
