# Tâches miniRTX

## L'idée en 5 lignes

Une image est une grille de pixels. Pour chaque pixel, un rayon part de l'œil
vers la scène 3D. On garde le premier objet touché ; sinon on voit le ciel.
Au point touché, Phong calcule lumière et ombre, puis un miroir peut relancer un
rayon avec une profondeur limitée. Les couleurs sont enfin sauvegardées en PPM.

## Pipeline

`main` lit les options, `SceneParser` construit la scène, `Renderer` lance les
rayons, `Scene::hit` trouve l'intersection, le shading calcule la couleur et
`Image::savePPM` exporte le résultat. L'image utilise `y=0` en haut, tandis que
la caméra utilise `v=0` en bas.

## Lire le projet de bas en haut puis de haut en bas

Commencer par les briques mathématiques : `Utils` définit les seuils, `Vec3`
décrit les nombres 3D, `Ray` décrit un trajet et `Color` réutilise `Vec3`.
Ensuite, `HitRecord` décrit une collision, `Hittable` fixe le contrat, puis
Sphere/Plane/Triangle remplissent ce contrat. `Camera` transforme un pixel en
rayon.

Remonter ensuite le pipeline : `Scene` possède les objets et laisse chaque
primitive essayer un rayon ; `Material` transforme le `HitRecord` en éclairage ;
`Renderer::trace` choisit ciel, éclairage ou réflexion ; `Renderer::render`
répète cela pour chaque pixel ; `Image` stocke puis exporte. Enfin `main` et
`SceneParser` sont les entrées humaines : le premier orchestre, le second
traduit le fichier texte.

Chaque fonction du squelette doit commencer par un TODO qui explique :
1. ses entrées et sa sortie ;
2. les étapes de calcul dans l'ordre ;
3. les validations et cas limites ;
4. la fonction suivante qui consommera son résultat.

Conventions : repère monde Y vers le haut, caméra vers -Z, directions normalisées,
couleurs linéaires non clampées jusqu'à l'export, gamma 2.2 uniquement dans PPM,
`RAY_T_MIN` pour les rayons secondaires et `EPSILON` pour les dégénérescences.

## Glossaire

- **Rayon** : `P(t)=O+tD`, une demi-droite partant de `O`.
- **t** : paramètre d'intersection ; c'est une distance si `D` est normalisée.
- **HitRecord** : fiche contenant point, normale, `t`, face et matériau.
- **Normale** : perpendiculaire à la surface ; elle est orientée contre le rayon.
- **frontFace** : vrai si le rayon arrive depuis l'extérieur.
- **Albedo** : couleur intrinsèque réfléchie par une surface.
- **Phong** : somme ambiante, diffuse et spéculaire.
- **Ombre** : un objet bloque le rayon entre le point et la lampe ; l'acné est
  le faux auto-hit évité par `RAY_T_MIN`.
- **Profondeur** : nombre maximal de rebonds récursifs.
- **Jitter** : léger déplacement aléatoire des rayons pour lisser les bords.
- **Gamma** : transformation d'affichage appliquée seulement à l'export.
- **Tuile** : bloc de pixels, ici 32x32, attribué à un worker.
- **Thread** : fil d'exécution qui traite des tuiles.
- **std::atomic** : compteur partagé sans course pour choisir la prochaine tuile.
- **RNG** : générateur pseudo-aléatoire local à une tuile.
- **EPSILON/RAY_T_MIN** : dégénérescence numérique / borne des rayons secondaires.
- **Produit scalaire** : mesure l'alignement, `dot((1,0,0),(0,1,0))=0`.
- **Produit vectoriel** : donne une perpendiculaire, `cross((1,0,0),(0,1,0))`.

## Répartition

- [Personne A](docs/PERSONNE_A.md) : Vec3, rayons, intersections et caméra.
- [Personne B](docs/PERSONNE_B.md) : matériaux, Phong, ombres et réflexions.
- [Personne C](docs/PERSONNE_C.md) : image, parsing, rendu parallèle, CLI, CMake
  et benchmark.

Branches recommandées : `feature/a-geometry`, `feature/b-lighting` et
`feature/c-infra`. Les fichiers partagés (`Scene.hpp`, `Renderer.cpp`,
`CMakeLists.txt`) doivent être annoncés aux deux autres personnes.

## Plan global étape par étape

### Étape 0 — Préparer le dépôt (tous, 30 min)

- [ ] Lire ce README et les trois fiches de personne.
- [ ] Vérifier la configuration CMake, puis compiler en Release et Debug.
- [ ] Lancer `ctest` et identifier les tests placeholders.
- [ ] Créer les branches `feature/a-geometry`, `feature/b-lighting` et
  `feature/c-infra`.
- [ ] Confirmer les signatures figées et les conventions communes.

**Sortie attendue :** chacun connaît ses fichiers et le squelette compile avant
toute implémentation.

### Étape 1 — Socle mathématique (A, 1 h 30)

- [ ] Implémenter les composantes et accesseurs de `Vec3`.
- [ ] Implémenter les opérations, `length`, `normalized`, `dot`, `cross` et
  `reflect`.
- [ ] Implémenter `clamp` et `degreesToRadians` dans `Utils`.
- [ ] Implémenter `clampColor` et `toRgb8`, sans gamma.
- [ ] Implémenter `Ray::origin`, `direction` et `at`.
- [ ] Compléter `test_vec3`.

**Validation :** dot = 32, cross = `(0,0,1)`, longueur = 5, normalisation du
vecteur nul sans NaN et réflexion `(1,-1,0)` → `(1,1,0)`.

### Étape 2 — Intersections (A, 3 h 30)

- [ ] Implémenter `HitRecord::setFaceNormal`.
- [ ] Implémenter Sphere : validation, discriminant, racines, intervalle,
  matériau et normale.
- [ ] Implémenter Plane : normale, dénominateur, intervalle et orientation.
- [ ] Implémenter Triangle si le temps le permet.
- [ ] Compléter les tests de sphère et de plan.

**Validation :** la sphère `(0,0,-5)`, rayon 1, est touchée à `t=4`; la
tangente est à `t=5`; un rayon parallèle au plan échoue; un rayon intérieur
donne `frontFace=false`.

### Étape 3 — Caméra (A, 2 h 30)

- [ ] Valider `lookAt != position`.
- [ ] Construire `forward`, `right` et `trueUp`.
- [ ] Remplacer un `up` parallèle par `(0,0,1)`.
- [ ] Calculer la fenêtre avec FOV vertical et aspect ratio.
- [ ] Implémenter `getRay(u,v)` et normaliser sa direction.
- [ ] Tester centre, coins, aspect 2 et cas dégénérés.

**Validation :** au centre, le rayon regarde vers `-Z`; les coins et le bord
droit changent correctement avec l'aspect.

### Étape 4 — Image et PPM (C, 1 h 30)

- [ ] Valider largeur et hauteur, puis calculer la taille en `size_t`.
- [ ] Implémenter `setPixel` et `pixel` avec contrôle des coordonnées.
- [ ] Implémenter PPM binaire `P6`.
- [ ] Appliquer exactement `clampColor -> pow(1/2.2) -> toRgb8`.
- [ ] Vérifier ouverture et état du flux.
- [ ] Tester dimensions, limites, en-tête, gamma et saturation.

**Validation :** une image 2x2 contient 11 octets d'en-tête et 12 octets RGB;
`0.5` donne environ 186, une valeur négative donne 0.

### Étape 5 — Scene et parser (C, 3 h)

- [ ] Implémenter le stockage des objets, matériaux et lumières.
- [ ] Implémenter `Scene::hit` avec `closestSoFar`.
- [ ] Lire le fichier ligne par ligne, ignorer vides et commentaires.
- [ ] Parser background, material, light, camera, sphere, plane et triangle.
- [ ] Vérifier arité, nombres finis et bornes.
- [ ] Refuser matériau inconnu, doublon et caméra absente ou répétée.
- [ ] Préfixer chaque erreur par `line N:`.

### Étape 6 — Premier rendu, M1 (C avec A, 1 h 30)

- [ ] Construire Camera avec `aspect = double(width)/height`.
- [ ] Implémenter `skyColor`.
- [ ] Commencer avec un sample au centre `(0.5,0.5)`.
- [ ] Convertir `y` image en `v` caméra avec `1-(y+jy)/height`.
- [ ] Appeler Camera, `trace`, puis `Image::setPixel`.
- [ ] Vérifier le dégradé sur une scène vide.

**Validation M1 :** en 200x100, le haut et le bas n'ont pas la même couleur.

### Étape 7 — Première sphère, M2 (A+C, 1 h)

- [ ] Utiliser une scène avec une seule sphère.
- [ ] Vérifier le trajet fichier → parser → caméra → sphère → image.
- [ ] En cas de bug, afficher temporairement `0.5*(normale+(1,1,1))`.
- [ ] Vérifier une silhouette ronde, des normales continues et aucun NaN.

### Étape 8 — Phong et ombres, M3 (B, 4 h)

- [ ] Ajouter l'ambiante une seule fois.
- [ ] Calculer diffus et spéculaire pour chaque lumière.
- [ ] Implémenter `isInShadow` avec `RAY_T_MIN` et
  `distance - RAY_T_MIN`.
- [ ] Ne jamais clamper avant l'export.
- [ ] Tester lumières devant, derrière et obstacle entre point et lampe.

**Validation M3 :** les deux lumières de `demo.scene` produisent deux ombres.

### Étape 9 — Réflexions, M4 (B, 2 h)

- [ ] Arrêter à `depth >= maxDepth`.
- [ ] Retourner `skyColor` pour un rayon sans intersection.
- [ ] Calculer le rayon réfléchi avec `RAY_T_MIN`.
- [ ] Mélanger `(1-k)*local + k*reflected`.
- [ ] Tester reflectivity 0, 1 et profondeur 0.

**Validation M4 :** le miroir reflète objets et ciel, sans récursion infinie.

### Étape 10 — Samples et anti-aliasing (C, 1 h 30)

- [ ] Ajouter les samples par pixel et le jitter.
- [ ] Utiliser un `mt19937` par tuile, seedé par `baseSeed + tileIndex`.
- [ ] Accumuler en linéaire puis diviser par le nombre de samples.
- [ ] Vérifier la reproductibilité avec une seed fixe.

### Étape 11 — Tuiles et threads, M5 (C, 2 h)

- [ ] Découper l'image en tuiles 32x32.
- [ ] Distribuer les tuiles avec `std::atomic<int>::fetch_add`.
- [ ] Faire traiter chaque tuile par un seul worker.
- [ ] Capturer les exceptions et les relancer après `join`.
- [ ] Transformer zéro worker en un worker.
- [ ] Vérifier avec TSan.

**Validation M5 :** les images 1, 2, 4 et 8 threads sont identiques octet par
octet pour une même seed.

### Étape 12 — CLI et benchmark (C, 2 h)

- [ ] Implémenter `--help`, `-o`, `-w`, `-h`, `-s`, `-d` et `-t`.
- [ ] Rappeler que `-h` signifie hauteur.
- [ ] Utiliser `RT_SOURCE_DIR` pour les ressources.
- [ ] Faire un échauffement puis trois mesures par configuration.
- [ ] Afficher médiane, `speedup=t1/tN` et `hardware_concurrency()`.
- [ ] Comparer les images des configurations 1/2/4/8 threads.

## Milestones

1. **M1 dégradé** : C avec la caméra de A ; une scène vide affiche le ciel.
   Bon signe : les pixels du haut et du bas ont des couleurs différentes.
2. **M2 première sphère** : A+C ; la sphère apparaît avec une normale correcte.
3. **M3 Phong et ombres** : B ; les deux lumières produisent deux ombres.
4. **M4 réflexions** : B ; le miroir reflète les objets et le ciel.
5. **M5 multithread** : C ; un, deux, quatre et huit threads donnent la même
   image et le benchmark mesure le speedup.

Rendu attendu : `./build/raytracer scenes/demo.scene -o out.ppm`, avec sol gris,
sphère rouge mate à gauche, miroir au centre, sphère bleue brillante à droite,
deux ombres et ciel dégradé. Débogage conseillé : 200x100, un sample et un thread.

## Validation et intégration

Compiler en Release et Debug sans warning, exécuter `ctest`, puis ASan/UBSan avec
`-DRT_SANITIZE=ON`. C doit aussi vérifier les data races avec TSan. Chaque
milestone reçoit un petit commit et un tag ; une PR est relue par un autre
étudiant. L'intégration finale utilise 800x450, 16 samples, profondeur 5 et un
tableau de performances 1/2/4/8 threads.

Le benchmark chauffe chaque configuration, prend la médiane de trois mesures,
calcule `speedup=t1/tN`, compare les images produites et note
`hardware_concurrency()`. Le chemin de scène passe par `RT_SOURCE_DIR`.

### À chaque commit

- [ ] Le code compile sans warning.
- [ ] Les tests concernés passent.
- [ ] Aucun fichier d'une autre personne n'est modifié sans accord.
- [ ] Le commit correspond à une seule intention.
- [ ] Les TODO et la documentation restent à jour.

### Ordre d'intégration

1. A pousse `Vec3`, `Ray`, `Utils` et leurs tests.
2. C branche Image et le parser minimal.
3. A pousse Camera et Sphere ; A+C valident M2.
4. B branche Phong et les ombres ; tous valident M3.
5. B branche les réflexions ; tous valident M4.
6. C ajoute samples, tuiles et threads ; tous valident M5.
7. C termine CLI et benchmark.
8. Tous font le rendu final et la relecture croisée.

### Validation finale

- [ ] Release et Debug compilent sans warning.
- [ ] `ctest --test-dir build --output-on-failure` est vert.
- [ ] ASan/UBSan sont propres avec `RT_SANITIZE=ON`.
- [ ] TSan est propre sur le rendu parallèle.
- [ ] Le rendu final utilise 800x450, 16 samples et profondeur 5.
- [ ] Le PPM s'ouvre dans un viewer.
- [ ] Les images 1/2/4/8 threads sont identiques.
- [ ] Le tableau de performances est ajouté au compte rendu.
- [ ] Chaque personne a relu le travail d'une autre personne.

## Définition de « projet terminé »

Une commande lit une scène, construit les objets, lance les rayons, calcule le
ciel, l'éclairage, les ombres et les réflexions, utilise plusieurs samples,
produit la même image quel que soit le nombre de threads et sauvegarde un PPM
visualisable. Le code, les tests, les sanitizers, le benchmark et les fiches
de tâches sont également à jour.