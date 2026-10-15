#!/usr/bin/env python3
"""Normalize C hexadecimal byte escapes to IDO K&R-compatible fixed-width octal.

Only real string/character literals are changed. Comments, escaped backslashes,
non-ASCII source text, and out-of-byte-range hex escapes are preserved. The caller
must score the resulting full translation unit and retain the original candidate.
"""
import re


def octal_hex_escapes(text):
    """Return (rewritten C, changes); changes record original offsets and byte values."""
    out, changes = [], []
    i, quote = 0, None
    while i < len(text):
        if quote is None and text.startswith('/*', i):
            end = text.find('*/', i+2)
            end = len(text) if end < 0 else end+2
            out.append(text[i:end]); i=end
            continue
        if quote is None and text.startswith('//', i):
            end = text.find('\n',i+2)
            end = len(text) if end < 0 else end
            out.append(text[i:end]); i=end
            continue
        ch=text[i]
        if quote is None:
            if ch in '\"\'':
                quote=ch
            out.append(ch);i+=1
        elif ch=='\\':
            match=re.match(r'\\x([0-9a-fA-F]+)',text[i:])
            if match and int(match.group(1),16)<=255:
                value=int(match.group(1),16)
                replacement='\\%03o'%value
                changes.append(dict(offset=i,original=match.group(),replacement=replacement,value=value))
                out.append(replacement);i+=len(match.group())
            else:
                out.append(text[i:i+2]);i+=2
        else:
            out.append(ch);i+=1
            if ch==quote:
                quote=None
    return ''.join(out),changes
