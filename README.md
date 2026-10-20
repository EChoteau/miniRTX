# miniRTX

**Par où commencer : lis [README_TACHES.md](README_TACHES.md) puis
`docs/PERSONNE_X.md`.**

Mini ray tracer pédagogique en C++17 : rayons primaires, intersections, modèle de
Phong, ombres, réflexions récursives, anti-aliasing et rendu par tuiles.

## Construction

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
ctest --test-dir build --output-on-failure
```

Le moteur ne dépend que de la STL. GoogleTest est téléchargé par CMake uniquement
pour les tests.

## Utilisation

```bash
./build/raytracer scenes/demo.scene -o out.ppm -w 800 -h 450 -s 16 -d 5 -t 8
```

Options : `-o` sortie PPM, `-w` largeur, `-h` hauteur (et non l'aide), `-s`
échantillons par pixel, `-d` profondeur maximale, `-t` nombre de threads.
L'aide est demandée uniquement avec `--help`.

## Voir l'image

Un fichier `.ppm` peut être ouvert avec GIMP, IrfanView, l'extension VS Code
« PBM/PPM viewer », ou converti avec ImageMagick : 

```bash
convert out.ppm out.png
```

## Format de scène

Une directive par ligne ; `#` introduit un commentaire et les lignes vides sont ignorées :

```text
camera <px> <py> <pz> <lx> <ly> <lz> <fovDeg>
background <r> <g> <b>
material <name> <r> <g> <b> <ambient> <diffuse> <specular> <shininess> <reflectivity>
light <x> <y> <z> <r> <g> <b> <intensity>
sphere <cx> <cy> <cz> <radius> <materialName>
plane <px> <py> <pz> <nx> <ny> <nz> <materialName>
triangle <x1> <y1> <z1> <x2> <y2> <z2> <x3> <y3> <z3> <materialName>
```

## Répartition

- **Personne A Ewenn** : algèbre vectorielle, rayons, intersections et caméra.
- **Personne B Emilien** : matériaux, éclairage Phong, ombres et réflexions.
- **Personne C Eliot** : parsing, image, rendu multithreadé, CLI et benchmark.

## Milestones

- [ ] M1 : image de fond en dégradé
- [ ] M2 : première sphère visible
- [ ] M3 : Phong + ombres
- [ ] M4 : réflexions
- [ ] M5 : multithread + benchmark

Extensions possibles (non codées ici) : BVH, fichiers `.obj`, textures, depth of
field, réfraction/verre et export PNG.
