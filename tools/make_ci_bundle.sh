#!/usr/bin/env bash
# Build the encrypted bundle of original binaries that CI downloads (T-0902).
#
# Usage: tools/make_ci_bundle.sh        (run on your own machine, after extracting disc/)
#
# Writes to ci-bundle/ (gitignored, outside build/ so clean rebuilds never delete the key; override with BUNDLE_OUT):
#   data/game-bundle.tar.gz.enc   AES-256 encrypted tarball of SLPM_86.053 and the 26 overlays
#   game-bundle.key               random passphrase, becomes the GAME_BUNDLE_KEY secret
#   deploy_key, deploy_key.pub    read-only SSH key pair for the private data repository
# Nothing is uploaded and no secret is set; the script prints the gh commands for you to run.
# Re-running reuses an existing key pair and passphrase, so the secrets stay valid.
set -euo pipefail
cd "$(dirname "${BASH_SOURCE[0]}")/.."

DATA_REPO="${DATA_REPO:-CosmicScribe64/tokimemo-ci-data}"
MAIN_REPO="${MAIN_REPO:-CosmicScribe64/tokimemo-ps1-decomp}"
OUT="${BUNDLE_OUT:-ci-bundle}"
BUNDLE=$OUT/data/game-bundle.tar.gz.enc

files=(disc/files/SLPM_86.053)
while read -r name _rest; do
  case "$name" in ''|'#'*) continue ;; esac
  files+=("disc/files/CDROM/EXEDIR/$name.EXN")
done < config/overlays.txt

for f in "${files[@]}"; do
  [ -f "$f" ] || { echo "missing $f (run tools/extract_disc.py first)" >&2; exit 1; }
done
[ "${#files[@]}" -eq 27 ] || { echo "expected 27 files, found ${#files[@]}" >&2; exit 1; }

want=$(cut -d' ' -f1 config/SLPM_86.053.sha1)
have=$(openssl dgst -sha1 disc/files/SLPM_86.053 | awk '{print $NF}')
[ "$want" = "$have" ] || { echo "SLPM_86.053 sha1 differs from config/SLPM_86.053.sha1" >&2; exit 1; }

mkdir -p "$OUT/data"
[ -f "$OUT/game-bundle.key" ] || { umask 077; openssl rand -base64 32 > "$OUT/game-bundle.key"; }
[ -f "$OUT/deploy_key" ] || ssh-keygen -q -t ed25519 -N '' -C tokimemo-ci-data -f "$OUT/deploy_key"

export GAME_BUNDLE_KEY
GAME_BUNDLE_KEY=$(cat "$OUT/game-bundle.key")
COPYFILE_DISABLE=1 tar -czf - "${files[@]}" \
  | openssl enc -aes-256-cbc -pbkdf2 -iter 600000 -salt -pass env:GAME_BUNDLE_KEY -out "$BUNDLE"

# Round trip: decrypt and list, as CI will.
n=$(openssl enc -d -aes-256-cbc -pbkdf2 -iter 600000 -pass env:GAME_BUNDLE_KEY -in "$BUNDLE" | tar -tzf - | wc -l)
[ "$n" -eq 27 ] || { echo "round trip found $n files, expected 27" >&2; exit 1; }
printf 'Encrypted CI inputs for %s. No plaintext game data.\n' "$MAIN_REPO" > "$OUT/data/README.md"
echo "bundle: $BUNDLE ($(wc -c < "$BUNDLE" | tr -d ' ') bytes, 27 files, round trip OK)"

cat <<CMDS

Run these yourself (nothing has been uploaded or set):

  # 1. private data repository with the encrypted bundle (once)
  git -C $OUT/data init -q -b main
  git -C $OUT/data add .
  git -C $OUT/data commit -q -m "Encrypted CI bundle"
  gh repo create $DATA_REPO --private --source $OUT/data --remote origin --push

  # 2. read-only deploy key so CI can clone it
  gh repo deploy-key add $OUT/deploy_key.pub --repo $DATA_REPO --title "tokimemo-ps1-decomp CI (read-only)"

  # 3. secrets for the main repository
  gh secret set CI_DATA_DEPLOY_KEY --repo $MAIN_REPO < $OUT/deploy_key
  gh secret set GAME_BUNDLE_KEY    --repo $MAIN_REPO < $OUT/game-bundle.key

To refresh the bundle later, rerun this script, then commit and push in $OUT/data.
Keep $OUT/game-bundle.key somewhere safe: without it the bundle cannot be decrypted.
CMDS
