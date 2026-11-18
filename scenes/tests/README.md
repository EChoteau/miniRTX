# Scènes de test

Chaque scène teste une seule fonctionnalité du ray tracer. Pour chacune, ce
README dit ce qu'elle teste et ce qu'on doit voir sur l'image. Si le rendu ne
ressemble pas à la description, la partie indiquée est la première à vérifier.

## Générer les images

Depuis la racine du projet, après `cmake --build build -j` :

```bash
./tests/render_scenes.sh 800 450 16      # toutes les scènes -> out/
```

Une seule scène :

```bash
./build/raytracer scenes/tests/02_sphere.scene -o out/sphere.ppm -w 800 -h 450 -s 16 -d 5 -t 8
xdg-open out/sphere.ppm
```

Les vérifications automatiques de ces rendus sont dans `tests/test_render.cpp` :

```bash
./build/raytracer_tests --gtest_filter='RenderTest.*'
```

## Les scènes

### 01_ciel — dégradé du ciel (M1)

Aucun objet, seulement une caméra et un fond.

**Attendu :** un dégradé vertical, blanc en bas et bleu en haut.

**Si c'est faux :** une image à l'envers (bleu en bas) vient de la convention
`v` dans `Renderer::render` ou de l'ordre des lignes dans `Image::savePPM`. Des
bandes ou des pixels noirs viennent du découpage en tuiles.

**Partie concernée :** C (Renderer, Image), A (Camera).

### 02_sphere — première sphère (M2)

Une sphère rouge centrée, éclairée par une lampe en haut à gauche.

**Attendu :** un disque rouge bien rond au centre, plus clair en haut à gauche
et plus sombre en bas à droite, avec un petit reflet blanc côté lumière. Le
ciel est autour.

**Si c'est faux :** un disque ovale indique un problème de rapport largeur/hauteur
(`aspect` dans la caméra). Une sphère absente ou décentrée vient de
`Sphere::hit` ou de `Camera::getRay`. Une sphère uniformément rouge, sans
volume, vient de la normale dans le `HitRecord`.

**Partie concernée :** A (Sphere, Camera), B (diffus).

### 03_ombre — ombre portée (M3)

Une sphère verte posée au-dessus d'un sol gris, avec la lampe juste au-dessus.

**Attendu :** une ombre ronde et nette sur le sol, juste sous la sphère.

**Si c'est faux :** l'absence d'ombre vient de `isInShadow`. Des petits points
noirs partout sur le sol (« acné d'ombre ») viennent d'un `tMin` trop petit
pour le rayon d'ombre.

**Partie concernée :** B (isInShadow), A (Plane).

### 04_miroir — réflexions (M4)

Une sphère miroir au centre, une sphère rouge à gauche et une bleue à droite,
sur un sol gris.

**Attendu :** dans la sphère centrale, on voit le reflet de la rouge et de la
bleue (petites et déformées), ainsi que le sol et le ciel.

**Si c'est faux :** un miroir qui ne montre que du gris vient de la réflexion
dans `Renderer::trace` ou de `Vec3::reflect`. Avec `-d 1`, c'est normal de ne
voir aucun reflet (voir la scène 08).

**Point blanc sur le miroir :** une lampe ponctuelle n'a pas de taille, donc un
rayon réfléchi ne la touche jamais. Son reflet est simulé par le spéculaire de
Phong (`specular 1.00`, `shininess 256` pour un miroir). `Renderer::trace`
garde ce spéculaire entier et n'atténue que l'ambiant et le diffus par
`1 - reflectivity` : on doit voir un petit point blanc net sur la sphère miroir,
côté lampe.

**Partie concernée :** B (réflexion), A (reflect).

### 05_triangles — triangles

Une pyramide jaune à quatre faces, posée sur un sol et vue de biais, éclairée
par une lampe placée à gauche et assez bas.

**Attendu :** la face gauche est claire et la face droite sombre, parce
qu'elles ne sont pas orientées pareil par rapport à la lampe. La pyramide
projette une longue ombre vers la droite sur le sol.

**Si c'est faux :** des faces absentes ou trouées viennent de
`Triangle::hit`. Des faces noires viennent d'une normale orientée du mauvais
côté.

**Partie concernée :** A (Triangle).

### 06_lumieres_rgb — lumières colorées

Une sphère blanche éclairée par trois lampes : rouge à gauche, verte en haut,
bleue à droite.

**Attendu :** la sphère est rouge à gauche, verte en haut et bleue à droite,
avec des mélanges (jaune, cyan, magenta) là où deux lampes éclairent ensemble.
Sur le sol, trois ombres colorées : chaque ombre a la couleur des deux lampes
qui l'éclairent encore.

**Si c'est faux :** une sphère blanche partout signifie que la couleur des
lampes est ignorée. Des ombres noires signifient qu'une seule ombre coupe
toutes les lampes, au lieu de traiter chaque lampe séparément.

**Partie concernée :** B (Phong multi-lumières).

**Remarque :** le fond est gris clair, alors que `background` vaut presque
noir. C'est normal : le ciel mélange toujours du blanc avec cette couleur
(`skyColor`).

### 07_brillance — exposant spéculaire

Quatre sphères grises identiques, avec une brillance (`shininess`) de 4, 16,
64 et 256 de gauche à droite.

**Attendu :** le reflet blanc est grand et flou à gauche, puis de plus en plus
petit et net vers la droite.

**Si c'est faux :** quatre reflets identiques signifient que `shininess` n'est
pas utilisé. L'absence de reflet vient du calcul spéculaire.

**Partie concernée :** B (spéculaire).

### 08_couloir_miroirs — profondeur de récursion

Deux miroirs plans face à face, avec une sphère rouge entre les deux. Le
script la rend trois fois : avec la profondeur par défaut (5), avec `-d 1` et
avec `-d 10`.

**Attendu :**
- `08_couloir_d1` : aucun reflet, les miroirs montrent juste le ciel. La
  profondeur compte aussi le premier rayon, donc `-d 1` ne laisse aucun rebond.
- `08_couloir_d10` : une enfilade de sphères rouges qui se répètent et
  s'assombrissent au loin, comme dans un couloir de miroirs.

**Si c'est faux :** des images identiques en `-d 1` et `-d 10` signifient que
`maxDepth` n'est pas respecté. Un programme qui plante ou qui tourne sans fin
signifie que la récursion ne s'arrête pas.

**Partie concernée :** B (récursion), C (option `-d`).

## L'enchaînement pour la démo (`scenes/progression/`)

La même scène, construite étape par étape : chaque fichier ajoute une seule
fonctionnalité par rapport au précédent. C'est fait pour montrer au prof ce
que chaque partie du code apporte.

```bash
./tests/render_progression.sh 800 450
```

Les images vont dans `out/progression/`. Si ImageMagick est installé, chaque
PNG a sa légende, et `out/progression/planche.png` met les 9 étapes côte à côte.

| Étape | Fichier | Ce qui est ajouté | Ce qu'on voit |
|---|---|---|---|
| 1 | `01_ciel` | caméra + fond | le dégradé du ciel |
| 2 | `02_sol` | plan | un sol uni, sans lumière (ambiant seul) |
| 3 | `03_sphere` | sphère | un disque rouge plat : l'intersection marche, mais sans lumière il n'y a pas de volume |
| 4 | `04_lumiere` | une lampe | la sphère prend du volume (diffus) et projette une ombre |
| 5 | `05_speculaire` | spéculaire sur le matériau rouge | un reflet blanc côté lampe |
| 6 | `06_deux_lumieres` | 2e lampe bleutée | une 2e ombre, éclairage combiné |
| 7 | `07_triangles` | pyramide | intersection rayon/triangle, avec son ombre |
| 8 | `08_miroir` | sphère miroir | réflexions de la sphère rouge et de la pyramide, petit point blanc de la lampe |
| 9 | `08_miroir` avec `-s 16` | anti-aliasing | bords lisses (horizon, contours) ; à comparer à l'étape 8 en zoomant |

Les étapes 1 à 8 sont rendues avec `-s 1` pour que l'anti-aliasing n'apparaisse
qu'à l'étape 9.

## Autre scène

`scenes/demo.scene` regroupe tout : trois sphères (mate, miroir, brillante),
un sol et deux lumières. C'est la scène à montrer pour la démo finale.
