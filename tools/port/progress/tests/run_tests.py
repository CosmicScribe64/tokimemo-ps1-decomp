"""Run protocol tests with operator authority confined to a temporary copy."""
import os
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile

with tempfile.TemporaryDirectory(prefix='progress-admission-tests-') as temporary:
    bundle = Path(temporary) / 'progress'
    shutil.copytree(Path(__file__).resolve().parents[1], bundle,
                    ignore=shutil.ignore_patterns('__pycache__'))
    env = dict(os.environ, PORT_PROGRESS_TEST_COPY=str(bundle))
    result = subprocess.run([sys.executable, '-m', 'unittest', 'discover', '-s', str(bundle / 'tests'), '-v'], env=env)
    sys.exit(result.returncode)
