"""Validate funcdiff process results before reporting a diagnostic score."""
import re


def report(result, name, path):
    text = result.stdout + result.stderr
    path.write_text(text)
    failed = (result.stderr.strip() or re.search(
        r'Traceback|\bERROR\b|\b(?:\w*Error|Exception):', result.stdout))
    match = re.search(r'^' + re.escape(name) + r': MATCH \([^\n]+\)$',
                      result.stdout, re.M)
    diff = re.search(r'^' + re.escape(name) + r': DIFF \(- expected, \+ built\)$',
                     result.stdout, re.M)
    rows = sum(bool(re.match(r'^\s+[+-](?:@row \d+ )?[0-9a-f]{8}\b', line))
               for line in result.stdout.splitlines())
    valid_match = result.returncode == 0 and match and not diff and rows == 0
    valid_diff = (result.returncode == 1 and diff and not match and rows > 0
                  and 'resolved: REAL DIFFERENCE' in result.stdout)
    if failed or not (valid_match or valid_diff):
        raise ValueError(name + ': invalid funcdiff result (exit '
                         + str(result.returncode) + '); see ' + str(path))
    print(name + ': diagnostic resolved +/- rows = ' + str(rows), flush=True)
    return rows
