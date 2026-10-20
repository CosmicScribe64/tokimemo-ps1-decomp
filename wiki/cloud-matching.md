---
type: howto
updated: 2026-10-20
---

# Matching one C file in a cloud session (match packets)

How a claude.ai cloud container (Ubuntu, root, Docker client installed, no daemon running,
HTTPS through an intercepting agent proxy) compiles one C file with the project toolchain and
compares one function against the original, without the disc. First run: [[tickets/T-24200-cloud-packet-taco-8013b460]].
See [[toolchain]] for the image itself.

## The packet

The repository is public and holds no game data, so the local session puts the original
disassembly of one C file (the splat `.s` files of `asm/<unit>/{matchings,nonmatchings}/<file>/`)
into the prompt as a shell script. The cloud session saves it **outside the repo** (e.g. `/tmp/packet.sh`)
and runs it in the repo root; it refuses to run unless `asm/` is gitignored. Nothing from it is
ever committed, pushed or quoted in issues/PRs. Before every commit: `git status` shows only
`src/`, `include/`, `wiki/` (and `tools/` for a tool fix).

## Toolchain setup (about 10 minutes, worked first time)

1. Start the Docker daemon (the container has `dockerd` but nothing runs it):

       (dockerd > /tmp/dockerd.log 2>&1 &); until docker info >/dev/null 2>&1; do sleep 1; done

2. `tools/docker.sh` alone fails at the `pip3 install` step: the proxy re-signs TLS and the
   Ubuntu image does not trust its CA (`CERTIFICATE_VERIFY_FAILED`; apt over http works).
   Build the image from a copy of the Dockerfile with the proxy CA injected after `FROM`,
   in a scratch context outside the repo, and label it with the hash of the *unchanged*
   `tools/Dockerfile`, so `tools/docker.sh` takes it as current and does not rebuild:

       S=<scratch>/dctx; mkdir -p $S; cp /root/.ccr/ca-bundle.crt $S/ca.crt
       C=/usr/local/share/ca-certificates/proxy-ca.crt
       awk -v C=$C '{print} /^FROM /{print "COPY ca.crt " C; print "ENV SSL_CERT_FILE=" C " PIP_CERT=" C " GIT_SSL_CAINFO=" C " CURL_CA_BUNDLE=" C " REQUESTS_CA_BUNDLE=" C}' tools/Dockerfile > $S/Dockerfile
       docker build --platform linux/amd64 --label dockerfile.sha256=$(sha256sum tools/Dockerfile | cut -d' ' -f1) -t tokimemo-decomp $S

   The full build (old-gcc, IDO 5.3/7.1 release binaries, asm-processor, permuter, objdiff) took
   about 8 minutes. The CA is only in the cloud image; never commit this Dockerfile variant.
   If the build stops at `ubuntu:22.04` with `429 Too Many Requests` (Docker Hub rate limit),
   retry `docker pull --platform linux/amd64 ubuntu:22.04` with a short backoff, then build again (T-24207).

3. Each `tools/docker.sh` call is a fresh container: only the repo (`/work`) persists, so
   write objects under the gitignored `build/` (e.g. `build/t24200/`), not `/tmp`.

## Proving the setup

    tools/docker.sh sh -c 'python3 tools/cc.py src/ovl/TACO/8013B170.c build/x/t.o ido 5.3 &&
        python3 tools/object_layout.py src/ovl/TACO/8013B170.c build/x/t.o && sha1sum build/x/t.o'

For T-24200 the unchanged file gave the sha1 the local session measured
(`40a801c5a66bec258366d8069fe8011b70b2302c`): the cloud compile is byte-identical.

## Comparing without the disc

`tools/funcdiff.py --built OBJ UNIT:func` builds the reference object from the packet's `.s` files
(`expected/build/ovl/<UNIT>/src/.../<file>.o`), but both its literal-byte MATCH proof and its row diff
(`resolved_verdict`) read `disc/files/.../<UNIT>.EXN`, so without the disc it ends in
`FileNotFoundError` or `DIFF (literal-byte proof unavailable ...)`. The last form means the
symbolic instruction keys are already equal. For a usable row diff a scratch script (kept
in `build/`, not committed) disassembled both objects with
`mips-linux-gnu-objdump -drz -M no-aliases`, mapped `D_XXXXXXXX`/`func_XXXXXXXX` relocations plus the
`%lo` immediate to absolute addresses (so `D_800E6280+0xF88` equals `D_800E7208`), took any `R_MIPS_26`
target as equal, and diffed the function's rows. The four matched functions of the T-24200 file all came out equal. String
literals are section-relative on the C side and need the rodata base to compare.
A one-variant loop (splice a body file into the C file, `cc.py`, diff) takes about 3 seconds.

`tools/autotweak.py` runs (stripped fastscore compile) but its score is not usable here: it
reported 160 for a draft the scratch diff put at 4 lines, so do not trust its ranking without the disc.
`regdiff` needs the traced uopt (not built here).

`tools/permute.py setup UNIT:func --src <scratch NON_MATCHING copy>` then `run --no-verify` works
disc-free: its target is the packet `.s`. Score 0 is unreachable when the original names a
split symbol (`D_800E7200` for `D_800E6280+0xF80`); read the candidate's `diff.txt` and
confirm with `funcdiff --expected` (T-24207, found both FAKEs in about 10 minutes).

Single-function packets (the file is too big to send whole, [[tickets/T-24203-cloud-packet-geko-80136c20]]):
compile a scratch copy under `build/scratch/` with every other `INCLUDE_ASM`/`INCLUDE_RODATA` line
removed, and a reference `.c` holding only `#include "common.h"` and the one `INCLUDE_ASM`. `cc.py`
takes no `-D`, so the `NON_MATCHING` copy gets `#define NON_MATCHING` as its first line. The
asm-fallback copy must equal the reference first. ugen's listing (`/opt/ido/5.3/cc -S` with the
`IDO_CFLAGS` minus `-c` and `cc.ido_frame_env`) lands in the current directory whatever `-o` says;
it shows which pass placed an instruction, but assembling it does not reproduce the build.

The local session still has to prove every result with the full linked build (`ninja`, funcdiff MATCH, SHA1).
