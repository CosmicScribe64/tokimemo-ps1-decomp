"""Flag dialogue boundaries for review without changing text (T-39537)."""

import re


LINK_WORDS = frozenset((b"a", b"an", b"the", b"my", b"your", b"our", b"their",
                        b"of", b"to", b"and", b"or"))


def boundaries(payload):
    """Locate row ends by non-whitespace byte position within each reply."""
    result = set()
    for page, content in enumerate(payload.split(b"\x0b")):
        offset = 0
        for row in content.split(b"\n")[:-1]:
            offset += len(row.replace(b" ", b""))
            result.add((page, offset))
    return result


def review(before_wrap, wrapped, metrics, width):
    """Return candidates with provenance; retained breaks are never auto-fixes.

    before_wrap is the source-aware reflow result, including reviewed overrides.
    Page boundaries remain separate, even when two replies have identical text.
    """
    retained = boundaries(before_wrap)
    findings = []
    for page, content in enumerate(wrapped.split(b"\x0b")):
        rows = content.split(b"\n")
        offset = 0
        for index, (left, right) in enumerate(zip(rows, rows[1:])):
            offset += len(left.replace(b" ", b""))
            words = re.findall(rb"[A-Za-z0-9]+(?:'[A-Za-z]+)?", left)
            right_words = re.findall(rb"[A-Za-z0-9]+(?:'[A-Za-z]+)?", right)
            reasons = []
            if left.rstrip().endswith(b"No.") and right.lstrip()[:1].isdigit():
                reasons.append("number_label_split")
            if words and words[-1].lower() in LINK_WORDS:
                reasons.append("phrase_link_at_row_end")
            if right.strip() and not right_words and all(32 <= c < 128 for c in right):
                reasons.append("punctuation_only_row")
            if len(right_words) == 1 and all(c >= 32 for c in right) and metrics.width(right) < width / 3:
                reasons.append("short_single_word_row")
            if metrics.width(left) < width / 2 and right_words \
                    and metrics.width(left + b" " + right.strip().split(b" ")[0]) <= width:
                reasons.append("early_break")
            if reasons:
                findings.append({"page": page, "after_row": index,
                                 "origin": "retained" if (page, offset) in retained else "wrapped",
                                 "reasons": reasons,
                                 "widths_px": [metrics.width(left), metrics.width(right)]})
    return findings
