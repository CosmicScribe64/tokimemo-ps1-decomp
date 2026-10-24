"""Write the T-27003 rename tables from drafts/T-27003/uses.json (scan.py)."""
import json
rows = json.load(open("drafts/T-27003/uses.json"))
obin, sdk = [], []
for ph, a, n, files, names in rows:
    if a == 0x80084C98:
        continue                      # deliberate two-name address (T-9540 call view)
    name, path = names[0][0], names[0][1]
    kind = "func" if ph.startswith("func_") else "data"
    if path == "symbol_addrs_obin.txt":
        note = "T-27003 O.BIN name (T-0600 symbol table, wiki/obin.md) carried into the overlays"
        obin.append((a, "%s main %s %s %s" % (kind, ph, name, note)))
    else:
        note = "T-27003 SDK name (T-0010 signature match, wiki/psyq-sdk.md) carried into the overlays"
        sdk.append((a, "%s main %s %s %s" % (kind, ph, name, note)))
for fn, lst in (("obin", obin), ("sdk", sdk)):
    with open("drafts/T-27003/%s.tsv" % fn, "w") as f:
        f.write("# kind scope old new note\n")
        for a, line in sorted(lst):
            f.write(line + "\n")
    print(fn, len(lst))
