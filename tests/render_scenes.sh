#!/usr/bin/env bash
# Rend toutes les scènes de scenes/ et scenes/tests/ dans out/ (PPM + PNG si possible).
# Usage : ./tests/render_scenes.sh [largeur] [hauteur] [samples] [threads]
# À lancer depuis la racine du projet après avoir compilé (cmake --build build).
set -euo pipefail

W=${1:-640}
H=${2:-360}
S=${3:-8}
T=${4:-$(nproc 2>/dev/null || echo 4)}
BIN=./build/raytracer
OUT=out

if [[ ! -x "$BIN" ]]; then
    echo "Binaire $BIN introuvable : compile d'abord (cmake --build build)." >&2
    exit 1
fi
mkdir -p "$OUT"

# Convertisseur PNG optionnel (ImageMagick 7 : magick, 6 : convert).
CONV=""
if command -v magick >/dev/null 2>&1; then CONV="magick"
elif command -v convert >/dev/null 2>&1; then CONV="convert"; fi

render() {  # render <scene> <nom_sortie> [options en plus]
    local scene=$1 name=$2; shift 2
    local start end
    start=$(date +%s%N)
    if "$BIN" "$scene" -o "$OUT/$name.ppm" -w "$W" -h "$H" -s "$S" -t "$T" "$@"; then
        end=$(date +%s%N)
        printf '  OK   %-28s %5d ms\n' "$name" $(( (end - start) / 1000000 ))
        if [[ -n "$CONV" ]]; then
            "$CONV" "$OUT/$name.ppm" "$OUT/$name.png" || true
        fi
    else
        printf '  FAIL %s\n' "$name"
        FAILS=$((FAILS + 1))
        return 0
    fi
}

FAILS=0
echo "Rendu ${W}x${H}, ${S} spp, ${T} threads -> $OUT/"
render scenes/demo.scene demo
for f in scenes/tests/*.scene; do
    render "$f" "$(basename "$f" .scene)"
done
# Le couloir de miroirs est rendu à deux profondeurs pour comparer.
render scenes/tests/08_couloir_miroirs.scene 08_couloir_d1 -d 1
render scenes/tests/08_couloir_miroirs.scene 08_couloir_d10 -d 10

if [[ -z "$CONV" ]]; then
    echo "(ImageMagick absent : seuls les .ppm sont générés)"
fi
echo "Échecs : $FAILS"
exit "$FAILS"
