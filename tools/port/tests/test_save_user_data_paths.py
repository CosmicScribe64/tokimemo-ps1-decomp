#!/usr/bin/env python3
"""Check launcher save-root containment with invented inputs and a fake Docker.

Run in the Python Docker image; no game input or real daemon call is used.
"""
import argparse
import json
import os
import pathlib
import shutil
import subprocess
import tempfile


def run_cases(launcher):
    with tempfile.TemporaryDirectory(prefix='save-ui-paths-') as directory:
        work = pathlib.Path(directory)
        repo = work / 'repository'
        project = repo / 'port/godot'
        script = repo / 'tools/port/run_playable.sh'
        project.mkdir(parents=True)
        script.parent.mkdir(parents=True)
        shutil.copyfile(launcher, script)
        (project / 'app').mkdir()
        (project / 'app/playable.tscn').write_text('invented scene sentinel\n')
        (repo / '.asset-cache').mkdir()
        tracked = repo / 'src'
        tracked.mkdir()
        media = work / 'media'
        media.mkdir()
        (media / 'manifest.json').write_text('{}\n')
        boundary, tables = work / 'boundary.json', work / 'tables.json'
        boundary.write_text('{}\n')
        tables.write_text('{}\n')
        fake_bin = work / 'bin'
        fake_bin.mkdir()
        fake_docker = fake_bin / 'docker'
        fake_docker.write_text('#!/bin/sh\nprintf \'%s\\n\' "$@" > "$SAVE_UI_CAPTURE"\n')
        fake_docker.chmod(0o755)
        capture = work / 'arguments.txt'
        alias = work / 'repository-alias'
        alias.symlink_to(repo, target_is_directory=True)
        outside = work / 'outside user data'
        outside.mkdir()
        linked = repo / '.asset-cache/linked-to-tracked'
        linked.symlink_to(tracked, target_is_directory=True)
        results = []

        def case(name, user_data=None, alias_root=False, allowed=False):
            capture.unlink(missing_ok=True)
            env = os.environ.copy()
            for key in list(env):
                if key.startswith('TOKIMEMO_') or key == 'GODOT_PORT_IMAGE':
                    env.pop(key)
            env.update(PATH=str(fake_bin) + os.pathsep + env.get('PATH', ''), SAVE_UI_CAPTURE=str(capture))
            if user_data is not None:
                env['TOKIMEMO_PLAYABLE_USER_DATA'] = str(user_data)
            invoked = (alias if alias_root else repo) / 'tools/port/run_playable.sh'
            command = ['sh', str(invoked), str(boundary), '0' * 64, str(tables), '1' * 64,
                       str(media), str(work / 'captures')]
            completed = subprocess.run(command, env=env, capture_output=True, text=True, timeout=10)
            called = capture.exists()
            ok = completed.returncode == 0 and called if allowed else completed.returncode != 0 and not called
            arguments = capture.read_text().splitlines() if called else []
            if allowed:
                expected = (outside if user_data is not None else repo / '.asset-cache/port-user-data').resolve()
                ok = ok and str(expected) + ':/userdata' in arguments and 'XDG_DATA_HOME=/userdata' in arguments
            result = {'case': name, 'pass': ok, 'docker_called': called}
            results.append(result)
            return arguments

        first = case('default persistent root', allowed=True)
        persistent = repo / '.asset-cache/port-user-data'
        (persistent / 'sentinel').write_text('persists across launches\n')
        second = case('second launch uses same root', allowed=True)
        results[-1]['pass'] = results[-1]['pass'] and first == second and (persistent / 'sentinel').is_file()
        case('explicit outside user directory with spaces', outside, allowed=True)
        case('project directory rejected', project)
        case('tracked repository directory rejected', tracked)
        case('linked ignored path to tracked directory rejected', linked)
        case('symlink repository root cannot bypass containment', tracked, alias_root=True)
        return results


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--launcher', required=True, type=pathlib.Path)
    parser.add_argument('--output', type=pathlib.Path)
    args = parser.parse_args()
    cases = run_cases(args.launcher.resolve(strict=True))
    report = {'status': 'PASS' if all(row['pass'] for row in cases) else 'FAIL', 'cases': cases,
              'scope': 'invented shell fixtures and captured Docker arguments; no original data'}
    if args.output:
        args.output.write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps({'status': report['status'], 'cases': len(cases)}))
    raise SystemExit(0 if report['status'] == 'PASS' else 1)


if __name__ == '__main__':
    main()
