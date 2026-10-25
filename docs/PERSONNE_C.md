# Personne C — Infrastructure et performance

## Mission

Implémenter Image/PPM, SceneParser, `Renderer::render`, ThreadPool, CLI, CMake
et le benchmark. `Scene::hit` conserve l'intersection la plus proche :
`closestSoFar=tMax`, mise à jour après chaque hit dans `[tMin,closestSoFar]`.

`Image` rejette les dimensions non positives, calcule la taille en `size_t`,
rejette les coordonnées hors image et exporte `P6\n<w> <h>\n255\n`. Pour chaque
composante l'ordre est clamp, `pow(c,1/2.2)`, conversion 8 bits ; ouverture et
état final du flux doivent être vérifiés.

Le parseur exige l'arité exacte, des nombres finis, les matériaux déclarés avant
usage, aucun doublon, `fov` dans ]0,180[, rayon strictement positif, intensité
positive ou nulle, réflectivité dans [0,1], shininess positive ou nulle, une
seule caméra obligatoire et des erreurs `line N: ...`.

Le rendu découpe en tuiles 32x32. Chaque worker prend `nextTile.fetch_add(1)`,
avec un `mt19937` seedé par `baseSeed + tileIndex`; ainsi une image est
identique quel que soit le nombre de threads. Le mapping est
`u=(x+jx)/width`, `v=1-(y+jy)/height`, avec `jx=jy=0.5` pour un sample.
Les samples sont moyennés en linéaire et jamais clampés dans `trace`.
ThreadPool remplace zéro worker par un, capture les `exception_ptr` et relance
après les `join`.

## Plan heure par heure

- [x] 1,5 h : Image et PPM.
- [ ] 1,5 h : rendu mono-thread et M1.
- [ ] 3 h : SceneParser.
- [ ] 1 h : CLI.
- [ ] 2 h : tuiles, threads et RNG.
- [ ] 1 h : benchmark.

## Tests à écrire

Tester dimensions, aller-retour pixel, limites,
en-tête de 11 octets, gris 0.5 vers 186, saturation et négatifs. Tester scènes
valides, commentaires, directive inconnue, matériau absent, mauvaise arité,
`nan`/`inf`, fichier absent, image 2x2 vide et égalité 1/4 threads.

## Contrat

A fournit `Camera::getRay` et `Scene::hit`; B fournit `trace` non clampé.
Tester `-fsanitize=thread` pour les courses et utiliser `RT_SOURCE_DIR` pour les
chemins de ressources.

## Définition de « fini »

Les images sont identiques entre 1 et 4 threads, les erreurs de parsing sont
localisées, le benchmark fournit médianes et speedup, et les sanitizers sont
propres.
