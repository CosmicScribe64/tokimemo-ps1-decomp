"""Bounded agent reviews select exact artifacts without becoming human approval."""
import json
from pathlib import Path
import tempfile
import unittest

import translation_draft_pack as subject
from translation_assets import ROOT


class ReviewedDraftTests(unittest.TestCase):
    def test_review_selects_nine_and_rejects_changed_identity(self):
        source=ROOT/'.asset-cache/translation-assets/name-native-font-v1'
        if not source.exists(): self.skipTest('generate source-font NAME references first')
        review=ROOT/'translations/en/graphics/reviews/T39004-name-native-font-v1.agent.json'
        with tempfile.TemporaryDirectory(dir=ROOT/'.asset-cache') as temporary:
            folder=Path(temporary)
            result=subject.build(source,folder/'pack','name-entry-actor-clut-10',review)
            self.assertEqual(result['assets'],9)
            manifest=json.loads((folder/'pack/manifest.json').read_text())
            self.assertTrue(manifest['development_drafts'])
            self.assertFalse(manifest['agent_review']['human_approval'])
            self.assertTrue(all(row['style']=='unreviewed' for row in manifest['assets']))
            changed=json.loads(review.read_text())
            changed['assets'][0]['output_sha256']='0'*64
            bad=folder/'changed-review.json';bad.write_text(json.dumps(changed))
            with self.assertRaisesRegex(ValueError,'identity differs'):
                subject.build(source,folder/'rejected','name-entry-actor-clut-10',bad)
            self.assertFalse((folder/'rejected').exists())


if __name__=='__main__': unittest.main()
