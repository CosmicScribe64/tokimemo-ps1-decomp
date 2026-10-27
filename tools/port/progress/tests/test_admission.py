"""Synthetic public publisher/watcher contract tests; no gameplay acceptance."""
import base64
import json
import shutil
import subprocess
import sys
import tempfile
import unittest
import hashlib
from pathlib import Path

from evidence_fixture import approve, repin, store

PNG = base64.b64decode('iVBORw0KGgoAAAANSUhEUgAAAAEAAAABCAQAAAC1HAwCAAAAC0lEQVR42mP8/x8AAwMCAO+/pE8AAAAASUVORK5CYII=')


class AdmissionTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.base = Path(self.temp.name)
        self.bundle = self.base / 'trusted-tools'
        shutil.copytree(Path(__file__).resolve().parents[1], self.bundle,
                        ignore=shutil.ignore_patterns('tests', '__pycache__'))
        self.media = self.base / 'gallery'
        self.source = self.base / 'incoming.png'
        self.source.write_bytes(PNG)
        self.evidence = self.base / 'evidence'

    def tearDown(self):
        self.temp.cleanup()

    def publish(self, *sources):
        return subprocess.run([sys.executable, str(self.bundle / 'publish.py'),
                               '--media-root', str(self.media), '--evidence-root', str(self.evidence),
                               *map(str, sources or [self.source])], capture_output=True, text=True)

    def reject(self):
        result = self.publish()
        self.assertNotEqual(result.returncode, 0, result.stdout)
        self.assertFalse((self.media / 'manifest.json').exists())

    def change(self, relative, mutate):
        path = self.evidence / 'runs/fixture' / relative
        data = json.loads(path.read_text())
        mutate(data)
        store(path, data)

    def test_direct_publish_rejects_an_image_without_reviewed_capture_evidence(self):
        result = subprocess.run([sys.executable, str(self.bundle / 'publish.py'),
                                 '--media-root', str(self.media), str(self.source)],
                                capture_output=True, text=True)
        self.assertNotEqual(result.returncode, 0, 'An ordinary PNG alone must not enter actual gameplay progress')
        self.assertFalse((self.media / 'manifest.json').exists())

    def test_coordinator_approved_exact_capture_can_be_published(self):
        policy = approve(self.bundle, self.evidence, [PNG])
        result = subprocess.run([sys.executable, str(self.bundle / 'publish.py'),
                                 '--media-root', str(self.media), '--evidence-root', str(self.evidence),
                                 str(self.source)], capture_output=True, text=True)
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertTrue((self.media / 'manifest.json').is_file())
        entry = json.loads((self.media / 'manifest.json').read_text())['items'][0]
        self.assertEqual(entry['admission']['route_sha256'], policy['routes'][0]['route_sha256'])

    def test_rejected_batch_does_not_leave_partial_gallery_files(self):
        approve(self.bundle, self.evidence, [PNG])
        unreviewed = self.base / 'unreviewed.png'
        unreviewed.write_bytes(PNG[:-1] + bytes([PNG[-1] ^ 1]))
        result = subprocess.run([sys.executable, str(self.bundle / 'publish.py'),
                                 '--media-root', str(self.media), '--evidence-root', str(self.evidence),
                                 str(self.source), str(unreviewed)], capture_output=True, text=True)
        self.assertNotEqual(result.returncode, 0)
        self.assertFalse((self.media / 'manifest.json').exists())
        self.assertEqual(list(self.media.glob('*.png')), [])

    def test_watcher_publishes_a_reviewed_capture_after_stability(self):
        approve(self.bundle, self.evidence, [PNG])
        script = """
import json,sys
from pathlib import Path
from watcher import CaptureWatcher
w=CaptureWatcher([Path(sys.argv[1])],Path(sys.argv[2]),evidence_root=Path(sys.argv[3]))
print(json.dumps([w.poll_once(),w.poll_once(),w.poll_once()]))
"""
        watched = self.base / 'watched'
        watched.mkdir()
        shutil.copyfile(self.source, watched / 'capture.png')
        result = subprocess.run([sys.executable, '-c', script, str(watched), str(self.media), str(self.evidence)],
                                cwd=self.bundle, capture_output=True, text=True)
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertEqual(json.loads(result.stdout), [0, 1, 0])

    def test_self_declared_review_and_adjacent_registry_do_not_grant_admission(self):
        store(Path(str(self.source) + '.json'), {'reviewed': True, 'actual_input': True, 'passed': True})
        approve(self.base, self.evidence, [PNG])  # Authority beside incoming media is ignored.
        self.reject()

    def test_unknown_reencoded_or_renamed_fixture_is_denied(self):
        approve(self.bundle, self.evidence, [PNG])
        self.source.write_bytes(PNG[:-1] + bytes([PNG[-1] ^ 1]))
        self.reject()

    def test_pinned_capture_source_png_must_still_match(self):
        approve(self.bundle, self.evidence, [PNG])
        (self.evidence / 'runs/fixture/proof/images/test-0.png').write_bytes(PNG + b'changed')
        self.reject()

    def test_unpinned_changed_sidecar_is_denied(self):
        approve(self.bundle, self.evidence, [PNG])
        self.change('proof/images/test-0.png.json', lambda d: d.update(saved=False))
        self.reject()

    def test_rehashed_failed_or_fixture_routes_remain_ineligible(self):
        cases = [({'passed': False}, None), ({'failures': [{'outcome': 'fail'}]}, None),
                 ({'unattempted': [{'id': 'missing'}]}, None), ({'completed_steps': 0}, None),
                 ({'test_kind': 'synthetic_renderer_fixture'}, None), ({'test_kind': 'original_reference'}, None),
                 ({'test_kind': 'translated_draft'}, None), (None, {'outcome': 'fail'}),
                 (None, {'runtime_exit_code': 1}), (None, {'import_exit_code': 1})]
        for route_change, artifact_change in cases:
            with self.subTest(route=route_change, artifact=artifact_change):
                approve(self.bundle, self.evidence, [PNG])
                if route_change:
                    self.change('proof/route.json', lambda d: d.update(route_change))
                if artifact_change:
                    self.change('proof/artifacts.json', lambda d: d.update(artifact_change))
                repin(self.bundle, self.evidence)
                self.reject()

    def test_rehashed_smoke_or_gameplay_writes_are_rejected(self):
        for change in [{'environment': {'TOKIMEMO_PLAYABLE_SMOKE': '1'}},
                       {'harness_gameplay_writes': ['date']},
                       {'harness_service_completion_injection': True},
                       {'production_script_changes': ['fixture.gd']}]:
            with self.subTest(change=change):
                approve(self.bundle, self.evidence, [PNG])
                self.change('producer.json', lambda d: d.update(change))
                repin(self.bundle, self.evidence)
                self.reject()

    def test_coherently_pinned_but_mismatched_capture_links_are_rejected(self):
        for change in [{'png_sha256': '0' * 64}, {'image': 'images/other.png'},
                       {'resized': True}, {'display': 'headless'}, {'saved': False},
                       {'image_producer': 'presenter.render()'}, {'dimensions': [2, 1]}]:
            with self.subTest(change=change):
                approve(self.bundle, self.evidence, [PNG])
                self.change('proof/route.json', lambda d: d['captures'][0].update(change))
                repin(self.bundle, self.evidence)
                self.reject()

    def test_capture_sidecar_cannot_disagree_with_reviewed_route_record(self):
        approve(self.bundle, self.evidence, [PNG])
        self.change('proof/images/test-0.png.json', lambda d: d.update(saved=False))
        repin(self.bundle, self.evidence, bind_links=False)
        self.reject()

    def test_review_anchor_missing_or_mutated_is_rejected(self):
        approve(self.bundle, self.evidence, [PNG])
        (self.evidence / 'reviews/root.json').write_text('{"reviewed":true}')
        self.reject()

    def test_symlinked_evidence_and_traversal_are_rejected(self):
        approve(self.bundle, self.evidence, [PNG])
        path = self.evidence / 'runs/fixture/proof/images/test-0.png.json'
        target = self.base / 'real-sidecar.json'
        path.rename(target)
        path.symlink_to(target)
        self.reject()
        policy = approve(self.bundle, self.evidence, [PNG])
        policy['routes'][0]['directory'] = '../outside'
        store(self.bundle / 'reviewed_routes.json', policy)
        self.reject()

    def test_cli_cannot_override_the_trusted_registry(self):
        approve(self.base, self.evidence, [PNG])
        result = subprocess.run([sys.executable, str(self.bundle / 'publish.py'),
                                 '--registry', str(self.base / 'reviewed_routes.json'),
                                 str(self.source)], capture_output=True)
        self.assertNotEqual(result.returncode, 0)

    def test_watcher_restart_and_retained_archive_cannot_readd_denied_fixture(self):
        policy = approve(self.bundle, self.evidence, [PNG])
        policy['routes'] = []
        policy['denied'] = [{'sha256': hashlib.sha256(PNG).hexdigest(), 'reason': 'syntheticrendererfixture'}]
        store(self.bundle / 'reviewed_routes.json', policy)
        watched = self.base / 'watched'
        watched.mkdir()
        shutil.copyfile(self.source, watched / 'fixture.png')
        self.media.mkdir()
        shutil.copyfile(self.source, self.media / (hashlib.sha256(PNG).hexdigest()[:16] + '.png'))
        store(self.media / 'manifest.json', {'version': 1, 'items': []})
        script = """
import json,sys
from pathlib import Path
from watcher import CaptureWatcher
results=[]
for restart in range(2):
 w=CaptureWatcher([Path(sys.argv[1])],Path(sys.argv[2]),evidence_root=Path(sys.argv[3]))
 results.extend([w.poll_once(),w.poll_once()])
print(json.dumps(results))
"""
        result = subprocess.run([sys.executable, '-c', script, str(watched), str(self.media), str(self.evidence)],
                                cwd=self.bundle, capture_output=True, text=True)
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertEqual(json.loads(result.stdout), [0, 0, 0, 0])
        self.assertEqual(json.loads((self.media / 'manifest.json').read_text())['items'], [])
        self.assertNotEqual(self.publish().returncode, 0)

    def test_watcher_retries_unchanged_source_after_operator_approval(self):
        approve(self.bundle, self.evidence, [PNG])
        approved = (self.bundle / 'reviewed_routes.json').read_bytes()
        policy = json.loads(approved)
        policy['routes'] = []
        store(self.bundle / 'reviewed_routes.json', policy)
        watched = self.base / 'watched'
        watched.mkdir()
        shutil.copyfile(self.source, watched / 'capture.png')
        approved_path = self.base / 'operator-approved.json'
        approved_path.write_bytes(approved)
        script = """
import json,sys
from pathlib import Path
from watcher import CaptureWatcher
w=CaptureWatcher([Path(sys.argv[1])],Path(sys.argv[2]),evidence_root=Path(sys.argv[3]))
results=[w.poll_once(),w.poll_once()]
Path('reviewed_routes.json').write_bytes(Path(sys.argv[4]).read_bytes())
results.append(w.poll_once())
print(json.dumps(results))
"""
        result = subprocess.run([sys.executable, '-c', script, str(watched), str(self.media), str(self.evidence), str(approved_path)],
                                cwd=self.bundle, capture_output=True, text=True)
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertEqual(json.loads(result.stdout), [0, 0, 1])

    def test_boolean_counts_and_dimensions_are_not_native_integers(self):
        for change in [{'total_steps': True}, {'dimensions': [True, 1]}]:
            with self.subTest(change=change):
                approve(self.bundle, self.evidence, [PNG])
                if 'dimensions' in change:
                    self.change('proof/route.json', lambda d: d['captures'][0].update(change))
                else:
                    self.change('proof/route.json', lambda d: d.update(change))
                repin(self.bundle, self.evidence)
                self.reject()

    def test_incomplete_runtime_identity_and_malformed_capture_list_fail_cleanly(self):
        for case in ['runtime_identity', 'capture_list']:
            with self.subTest(case=case):
                approve(self.bundle, self.evidence, [PNG])
                if case == 'runtime_identity':
                    self.change('producer.json', lambda d: d.update(runtime_tree_sha256=None))
                    self.change('proof/artifacts.json', lambda d: d.update(runtime_tree_sha256=None))
                    repin(self.bundle, self.evidence)
                else:
                    self.change('proof/route.json', lambda d: d.update(captures=None))
                    repin(self.bundle, self.evidence, bind_links=False)
                result = self.publish()
                self.assertNotEqual(result.returncode, 0)
                self.assertIn('capture admission:', result.stderr)
                self.assertNotIn('Traceback', result.stderr)

    def test_denied_hash_cannot_be_reapproved_by_an_inconsistent_registry(self):
        policy = approve(self.bundle, self.evidence, [PNG])
        policy['denied'] = [{'sha256': hashlib.sha256(PNG).hexdigest(), 'reason': 'syntheticrendererfixture'}]
        store(self.bundle / 'reviewed_routes.json', policy)
        self.reject()


if __name__ == '__main__':
    unittest.main()
