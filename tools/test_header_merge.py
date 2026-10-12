"""Synthetic text and disposable-git tests for T-10100 (Docker only)."""
from pathlib import Path
import os
import tempfile
import unittest
from unittest.mock import patch

import header_merge as hm


BASE = '#ifndef H\n#define H\nextern int old;\n#endif /* H */\n'


class HeaderMergeTests(unittest.TestCase):
    def test_ours_and_whole_other_insert_before_guard(self):
        ours = BASE.replace('extern int old;', 'extern short old;\nextern int ours;')
        block = '#ifndef OVERRIDE_new\nextern int new;\n#endif\n'
        theirs = BASE.replace('extern int old;\n', 'extern int old;\n' + block)
        expected = ours.replace('#endif /* H */', block + '#endif /* H */')
        self.assertEqual(hm.merge_header(BASE, ours, theirs), expected)

    def test_shared_closing_lines_do_not_split_struct_or_conditional(self):
        base = BASE.replace('extern int old;', '#if OLD\nextern int old;\n#endif')
        block = '#if NEW\ntypedef struct New {\n    int x;\n} New;\n#endif\n'
        theirs = base.replace('#endif /* H */', block + '#endif /* H */')
        out = hm.merge_header(base, base.replace('old;', 'ours;'), theirs)
        self.assertIn(block, out)
        self.assertEqual(out.count('#if NEW'), 1)
        self.assertEqual(out.count('#endif'), 3)

    def test_duplicate_block_is_skipped_but_scattered_lines_are_not(self):
        block = '#if NEW\nextern int new;\n#endif\n'
        theirs = BASE.replace('extern int old;\n', 'extern int old;\n' + block)
        self.assertEqual(hm.merge_header(BASE, theirs, theirs), theirs)
        ours = BASE.replace('extern int old;', '#if NEW\nextern int old;\n#endif\nextern int new;')
        self.assertIn(block, hm.merge_header(BASE, ours, theirs))

    def test_deletions_ignored_replacements_appended_no_guard(self):
        self.assertEqual(hm.merge_header('int old;\n', 'int ours;\n', ''), 'int ours;\n')
        self.assertEqual(hm.merge_header('int old;\n', 'int ours;', 'int theirs;\n'),
                         'int ours;\nint theirs;\n')

    def test_arbitrary_trailing_conditional_is_not_an_include_guard(self):
        ours = '#if FEATURE\nint ours;\n#endif\n'
        self.assertEqual(hm.merge_header('', ours, 'int new;\n'), ours + 'int new;\n')

    def test_guard_with_comments_and_blank_tail(self):
        ours = '/* Header\n * comment */\n' + BASE + '\n/* trailing\n * comment */\n'
        out = hm.merge_header(BASE, ours, BASE.replace('extern int old;\n', 'int new;\nextern int old;\n'))
        self.assertEqual(out, ours.replace('#endif /* H */', 'int new;\n#endif /* H */'))

    def test_multiline_typedef_comments_and_braces_in_strings(self):
        block = 'typedef struct New {\n    /* } */ int x;\n}\nNew;\n'
        out = hm.merge_header(BASE, BASE, BASE.replace('#endif', block + '#endif'))
        self.assertIn(block, out)

    def test_duplicate_detection_requires_whole_lines(self):
        self.assertEqual(hm.merge_header('', 'extern int x;\n', 'int x;\n'),
                         'extern int x;\nint x;\n')

    def test_if_not_defined_guard(self):
        for opening in ('#if !defined(H)', '#if !defined H', '#ifndef H'):
            with self.subTest(opening=opening):
                ours = BASE.replace('#ifndef H', opening).replace('#define H', '#define H 1')
                self.assertEqual(hm.merge_header('', ours, 'int new;\n'),
                                 ours.replace('#endif /* H */', 'int new;\n#endif /* H */'))


class GitMergeTests(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp.cleanup)
        self.root = Path(self.tmp.name)
        # Docker Desktop can remap a bind-mounted temp directory's ownership.
        # Trust only this test-created repo, without changing any git config file.
        config = patch.dict(os.environ, GIT_CONFIG_COUNT='1', GIT_CONFIG_KEY_0='safe.directory',
                            GIT_CONFIG_VALUE_0=str(self.root))
        config.start()
        self.addCleanup(config.stop)
        hm.git(self.root, 'init', '-q')
        hm.git(self.root, 'config', 'user.email', 'test@example.invalid')
        hm.git(self.root, 'config', 'user.name', 'Test')

    def commit(self, title):
        hm.git(self.root, 'add', '.')
        hm.git(self.root, 'commit', '-qm', title)
        return hm.git(self.root, 'rev-parse', 'HEAD').strip()

    def test_only_both_changed_headers_check_errors_and_idempotence(self):
        header = self.root / 'include/shared header.h'
        header.parent.mkdir()
        header.write_text(BASE)
        single = self.root / 'include/single.h'
        single.write_text(BASE)
        base = self.commit('base')
        header.write_text(BASE.replace('old;', 'ours;'))
        single.write_text('int single;\n')
        ours = self.commit('ours')
        hm.git(self.root, 'checkout', '-q', '-b', 'other', base)
        block = '#if NEW\nextern int new;\n#endif\n'
        header.write_text(BASE.replace('#endif /* H */', block + '#endif /* H */'))
        theirs = self.commit('theirs')
        # Build an intentionally bad merge result without relying on conflict resolution.
        header.write_text('bad union\n')
        tree = hm.git(self.root, 'write-tree').strip()
        merge = hm.git(self.root, 'commit-tree', tree, '-p', ours, '-p', theirs, '-m', 'merge').strip()
        before = header.read_text()
        self.assertEqual(hm.main(['--root', str(self.root), '--merge', merge, '--check']), 1)
        self.assertEqual(header.read_text(), before)
        self.assertEqual(hm.main(['--root', str(self.root), '--merge', merge]), 0)
        self.assertIn('extern int ours;', header.read_text())
        self.assertIn(block, header.read_text())
        self.assertEqual(single.read_text(), BASE)
        self.assertEqual(hm.repair(self.root, merge), [])
        self.assertEqual(hm.main(['--root', str(self.root), '--merge', base]), 2)
        self.assertEqual(hm.main(['--root', str(self.root), '--merge', 'absent']), 2)

    def test_other_side_deletion_preserves_ours_as_reference_rule_requires(self):
        header = self.root / 'include/shared.h'
        header.parent.mkdir()
        header.write_text(BASE)
        base = self.commit('base')
        ours_text = BASE.replace('old;', 'ours;')
        header.write_text(ours_text)
        ours = self.commit('ours')
        hm.git(self.root, 'checkout', '-q', '-b', 'other', base)
        header.unlink()
        header.parent.rmdir()
        theirs = self.commit('theirs deletes header')
        tree = hm.git(self.root, 'write-tree').strip()
        merge = hm.git(self.root, 'commit-tree', tree, '-p', ours, '-p', theirs, '-m', 'merge').strip()
        self.assertEqual(hm.repair(self.root, merge), ['include/shared.h'])
        self.assertEqual(header.read_text(), ours_text)
        header.unlink()
        header.parent.rmdir()
        hm.git(self.root, 'checkout', '-q', '-b', 'ours-delete', base)
        header.unlink()
        header.parent.rmdir()
        deleted_ours = self.commit('ours also deletes header')
        empty_merge = hm.git(self.root, 'commit-tree', tree, '-p', deleted_ours,
                             '-p', theirs, '-m', 'both delete').strip()
        self.assertEqual(hm.repair(self.root, empty_merge), [])
        self.assertFalse(header.exists())


if __name__ == '__main__':
    unittest.main()
