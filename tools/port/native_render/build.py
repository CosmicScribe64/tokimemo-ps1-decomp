"""T-39302: reproducible build of the PS1 renderer GDExtension, and its C++ unit tests.

Invoke build.sh (Docker, Linux arm64 compiler image, no host installs). Uses the same pinned
godot-cpp commit and SCons wheel as tools/port/native_audio (its dependency-provenance.json is the
single pin). Never copies a candidate into the production Godot project.

  build.sh --output .asset-cache/m2-renderer/build-v1     build the library into a new directory
  build.sh --test                                         build and run test_gpu.cpp (UBSan + ASan)
"""
import argparse
import importlib.util
import json
import os
from pathlib import Path
import platform
import shutil
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[3]
SOURCE = Path(__file__).resolve().parent
AUDIO = ROOT / 'tools/port/native_audio'
SOURCE_FILES = ('gpu.hpp', 'extension.cpp', 'SConstruct', 'build-profile.json')
LIBRARY = 'project/bin/liboriginal_render.linux.template_release.arm64.so'


def load_audio_build():
    """The audio builder owns the verified download/extract/output-path helpers."""
    spec = importlib.util.spec_from_file_location('native_audio_build', AUDIO / 'build.py')
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


def run_tests():
    out = Path('/tmp/native-render-test')
    command = ['g++', '-std=c++17', '-O1', '-g', '-fsanitize=undefined,address', '-fno-omit-frame-pointer',
               '-Wall', '-Wextra', '-Werror', '-I', str(SOURCE), str(SOURCE / 'test_gpu.cpp'), '-o', str(out)]
    subprocess.run(command, check=True)
    result = subprocess.run([str(out)], text=True, capture_output=True)
    sys.stdout.write(result.stdout)
    sys.stderr.write(result.stderr)
    return result.returncode


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument('--output', default='.asset-cache/m2-renderer/build')
    parser.add_argument('--jobs', type=int, default=4)
    parser.add_argument('--test', action='store_true', help='run the C++ unit tests instead of building the library')
    args = parser.parse_args(argv)
    if platform.system() != 'Linux' or platform.machine() not in ('aarch64', 'arm64'):
        raise ValueError('use build.sh with a Linux arm64 Docker compiler image')
    if args.test:
        return run_tests()
    audio = load_audio_build()
    if not 1 <= args.jobs <= 16:
        raise ValueError('jobs must be between 1 and 16')
    output = audio.output_path(args.output)
    compiler = subprocess.check_output(['g++', '--version'], text=True).splitlines()[0]
    metadata_path = AUDIO / 'dependency-provenance.json'
    metadata = json.loads(metadata_path.read_text())
    output.mkdir(parents=True)
    deps = output / 'deps'
    deps.mkdir()
    for index, filename, directory, kind in ((0, 'godot-cpp.tar.gz', 'godot-cpp', 'tar'), (2, 'scons.whl', 'scons', 'zip')):
        spec = metadata['downloads'][index]
        archive = deps / filename
        audio.download(spec, archive)
        audio.extract(archive, deps / directory, kind)
        print('Verified and extracted ' + directory, flush=True)
    for name in SOURCE_FILES:
        shutil.copy2(SOURCE / name, output / name)
    receipt = {'scope': 'Linux arm64 candidate build of the PS1 renderer; no game data embedded',
               'compiler': compiler, 'python': platform.python_version(), 'tls_verified': True,
               'dependency_manifest_sha256': audio.digest(metadata_path),
               'downloads': [metadata['downloads'][i] for i in (0, 2)],
               'source_sha256': {name: audio.digest(output / name) for name in SOURCE_FILES}}
    (output / 'build-inputs.json').write_text(json.dumps(receipt, indent=2) + '\n')
    command = [sys.executable, '-m', 'SCons', 'platform=linux', 'arch=arm64', 'target=template_release',
               'build_profile=build-profile.json', 'disable_exceptions=no', '-j' + str(args.jobs)]
    with (output / 'build.log').open('w') as log:
        result = subprocess.run(command, cwd=output, env=dict(os.environ, PYTHONPATH=str(deps / 'scons')),
                                stdout=log, stderr=subprocess.STDOUT)
    library = output / LIBRARY
    receipt['exit_code'] = result.returncode
    if result.returncode == 0:
        receipt['library_sha256'] = audio.digest(library)
    (output / 'build-result.json').write_text(json.dumps(receipt, indent=2) + '\n')
    if result.returncode:
        raise ValueError('native build failed; inspect ' + str(output / 'build.log'))
    print('Candidate library: ' + str(library.relative_to(ROOT)))
    print('library_sha256 ' + receipt['library_sha256'])
    print('Not installed. Review and test the candidate before publication.')
    return 0


if __name__ == '__main__':
    try:
        raise SystemExit(main())
    except (OSError, ValueError, subprocess.SubprocessError) as error:
        print('Native render build failed: ' + str(error), file=sys.stderr)
        raise SystemExit(2)
