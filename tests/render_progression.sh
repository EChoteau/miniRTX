#!/usr/bin/env bash
# Rend l'enchaînement scenes/progression/ (une fonctionnalité ajoutée par étape)
# dans out/progression/, puis assemble une planche avec toutes les étapes côte à côte.
# Usage : ./tests/render_progression.sh [largeur] [hauteur] [threads]
# À lancer depuis la racine du projet après avoir compilé (cmake --build build).
set -uo pipefail

W=${1:-640}
H=${2:-360}
T=${3:-$(nproc 2>/dev/null || echo 4)}
BIN=./build/raytracer
OUT=out/progression

if [[ ! -x "$BIN" ]]; then
    echo "Binaire $BIN introuvable : compile d'abord (cmake --build build)." >&2
    exit 1
fi
mkdir -p "$OUT"

CONV=""
if command -v magick >/dev/null 2>&1; then CONV="magick"
elif command -v convert >/dev/null 2>&1; then CONV="convert"; fi

LABELED=()
render() {  # render <scene> <nom> <légende> [options raytracer]
    local scene=$1 name=$2 label=$3; shift 3
    if ! "$BIN" "$scene" -o "$OUT/$name.ppm" -w "$W" -h "$H" -t "$T" "$@"; then
        echo "  FAIL $name"
        return 0
    fi
    echo "  OK   $name"
    if [[ -n "$CONV" ]]; then
        # PNG avec la légende écrite au-dessus de l'image.
        "$CONV" "$OUT/$name.ppm" -background white -gravity north -splice 0x30 \
            -pointsize 20 -annotate +0+4 "$label" "$OUT/$name.png" || true
        LABELED+=("$OUT/$name.png")
    fi
}

echo "Progression ${W}x${H}, ${T} threads -> $OUT/"
P=scenes/progression
render $P/01_ciel.scene          01_ciel           "1. Ciel (camera + fond)"      -s 1 -d 5
render $P/02_sol.scene           02_sol            "2. + Sol (plan)"              -s 1 -d 5
render $P/03_sphere.scene        03_sphere         "3. + Sphere (sans lumiere)"   -s 1 -d 5
render $P/04_lumiere.scene       04_lumiere        "4. + Lumiere : diffus + ombre" -s 1 -d 5
render $P/05_speculaire.scene    05_speculaire     "5. + Speculaire (Phong)"      -s 1 -d 5
render $P/06_deux_lumieres.scene 06_deux_lumieres  "6. + 2e lumiere"              -s 1 -d 5
render $P/07_triangles.scene     07_triangles      "7. + Triangles"               -s 1 -d 5
render $P/08_miroir.scene        08_miroir         "8. + Miroir (reflexions)"     -s 1 -d 5
# Même scène que l'étape 8 : seule l'option de rendu change.
render $P/08_miroir.scene        09_antialiasing   "9. + Anti-aliasing (16 spp)"  -s 16 -d 5

if [[ -n "$CONV" ]] && command -v montage >/dev/null 2>&1; then
    montage "${LABELED[@]}" -tile 3x -geometry +6+6 -background white "$OUT/planche.png" \
        && echo "Planche : $OUT/planche.png"
elif [[ -z "$CONV" ]]; then
    echo "(ImageMagick absent : seuls les .ppm sont générés, pas de légende ni de planche)"
fi
