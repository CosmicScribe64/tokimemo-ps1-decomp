#!/usr/bin/env python3
"""Replay the T-24047 full-TU source checkpoint in Docker without editing src/."""
import argparse
import json
import sys
from pathlib import Path

# permute imports the standard queue before restoring the tools search path.
sys.path.insert(0, str(Path(__file__).resolve().parents[2] / "tools"))
import funcdiff
import funcdiff_verify
import permute


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--case12", action="store_true",
                        help="diagnostic only: adds a table entry and changes the bound")
    parser.add_argument("--out", default="build/t24047-replay")
    args = parser.parse_args()
    directory = Path(args.out)
    directory.mkdir(parents=True, exist_ok=True)
    name = "func_80135F4C"
    context = funcdiff.locate(name, "ENDING")
    body = Path(__file__).with_name("body.c").read_text()
    if args.case12:
        assert body.count("    default:") == 1
        body = body.replace("    default:", "    case 12:\n    default:")
    source = permute.replace_function(Path(context.src).read_text(), name, body)
    if source is None:
        raise ValueError("target definition or fallback not found")
    candidate = directory / "candidate.c"
    candidate.write_text(source)
    built, _ = funcdiff_verify.compile_unit(context, directory, source=candidate,
                                            non_matching=False)
    expected, problem = funcdiff.expected_object(context)
    if problem:
        raise ValueError(problem)
    want = funcdiff.resolved_functions(expected, [name], context)[name]
    got = funcdiff.resolved_functions(built, [name])[name]
    if want is None or got is None:
        raise ValueError("function missing from comparison")
    result = {
        "function": "ENDING:" + name,
        "expected_instructions": len(want),
        "candidate_instructions": len(got),
        "aligned_cost": funcdiff.diff_score(want, got),
        "printed_rows": len(funcdiff.diff_lines(want, got)),
        "case12_diagnostic": args.case12,
        "matching_claim": False,
    }
    (directory / "result.json").write_text(json.dumps(result, indent=2) + "\n")
    (directory / "diff.txt").write_text(
        "\n".join(funcdiff.diff_lines(want, got)) + "\n")
    print(json.dumps(result))
    # A successful diagnostic is not a literal linked-byte match or C progress.
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except (OSError, ValueError) as error:
        print("replay.py: " + str(error), file=sys.stderr)
        raise SystemExit(2)
