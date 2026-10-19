# ORDRE STRICT : tu crées de la STRUCTURE, des COMMENTAIRES et de la DOCUMENTATION. Tu n'implémentes RIEN.

Projet : miniRTX, mini ray tracer C++17, 3 étudiants (A/B/C), ~10h chacun. Le dépôt contient déjà un squelette (src/, tests/, bench/, scenes/, CMakeLists.txt, README.md). Le but pédagogique est que les étudiants écrivent TOUT le code eux-mêmes. Ton travail : (1) corriger quelques points d'interface, (2) mettre des commentaires/TODO détaillés, (3) créer un deuxième README qui sépare le travail en 3.

## RÈGLES (à respecter partout)
- INTERDIT : remplir un corps de fonction, ajouter des boucles/validations/logique dans un .cpp, écrire de vrais tests.
- Dans les .cpp, chaque corps de fonction ne contient que `// TODO(A|B|C): étapes numérotées` + un `return` par défaut (`return false;`, `return Color{};`, `return Ray(origin_, Vec3{});`).
- Les comportements, validations, formules et conventions s'écrivent en COMMENTAIRES « TODO(X) » dans le fichier concerné, jamais en code.
- Les tests restent des placeholders (un `EXPECT_TRUE(true)` ou l'existant) avec, en commentaire, la liste des cas à écrire et leurs valeurs (voir docs ci-dessous). Le test `ImageTest.Placeholder` échoue tant que `setPixel` est vide : c'est voulu, ajoute le commentaire `// Doit échouer tant que la Personne C n'a pas implémenté setPixel/pixel`.
- Ne modifie pas les 6 signatures figées sauf si elles sont listées dans la partie 1 : Hittable::hit, Camera::getRay, Scene::hit, Renderer::trace, SceneParser::parseFile, Image::savePPM.
- Commentaires en français, identifiants en anglais.
- À la fin : `cmake -S . -B build && cmake --build build` doit compiler sans warning. Affiche `git diff --stat` et confirme qu'aucun corps de fonction n'a été implémenté en dehors de la partie 1.

==================================================
# PARTIE 1 : seul code autorisé (corrections minimales)
==================================================
1. **Vec3.hpp (bug bloquant)** : `normalized()` et `reflect()` utilisent des opérateurs libres (`/`, `-`, `*`) déclarés APRÈS la classe, donc invisibles dans le corps d'une méthode définie dans la classe. Réécris-les avec les composantes directement : `normalized()` : `inv = 1.0/len` puis `Vec3(x_*inv, y_*inv, z_*inv)` ; `reflect(v,n)` : `d = 2.0*dot(v,n)` puis `Vec3(v.x_-d*n.x_, v.y_-d*n.y_, v.z_-d*n.z_)`. Commente : « normalized() retourne le vecteur NUL si la longueur <= EPSILON : l'appelant doit tester ce cas » et « les méthodes de Vec3 n'utilisent que des membres, jamais les opérateurs libres définis plus bas ».
2. **Utils.hpp** : ajoute `inline constexpr double RAY_T_MIN = 1e-4;` avec un commentaire : « tMin de TOUS les rayons secondaires (ombre, réflexion) pour éviter l'acné d'ombre ; EPSILON (1e-6) sert aux tests de dégénérescence (longueur nulle, dénominateur ~ 0) ». Mets à jour les commentaires de Material.hpp/.cpp et Renderer.cpp qui parlent d'EPSILON pour les rayons secondaires.
3. **Caméra et aspect ratio** : le fichier de scène ne contient pas l'aspect ratio (il dépend de -w/-h) mais le constructeur de Camera en a besoin. Donc :
   - dans Camera.hpp, ajoute `struct CameraParams { Vec3 position{0,1,5}; Vec3 lookAt{0,0,0}; Vec3 up{0,1,0}; double verticalFovDegrees{45.0}; };`
   - dans Scene.hpp/.cpp, remplace `camera()` / `setCamera(const Camera&)` / le membre `Camera camera_` par `const CameraParams& cameraParams() const` / `void setCameraParams(const CameraParams&)` (accesseurs triviaux autorisés) ;
   - en commentaire TODO(C) dans Renderer::render : « construire la Camera ici avec aspect = double(width)/height » ; en commentaire TODO(C) dans SceneParser : « remplir CameraParams, up = (0,1,0) par défaut ».
4. **Constructeurs** de Sphere, Plane, Triangle, Camera : retire `noexcept` (signature uniquement) et écris en commentaire TODO(A) les cas où lancer `std::invalid_argument` : radius <= EPSILON ; normale de longueur <= EPSILON ; matériau nul ; triangle dégénéré ; Camera avec lookAt == position (et up parallèle à la visée : utiliser (0,0,1) à la place). Note en commentaire TODO(C) que SceneParser devra les rattraper et les relancer préfixées par `line N: `.
5. `SceneParser::parseFile` devient `static`.
6. **CMakeLists.txt** : `if(NOT CMAKE_BUILD_TYPE) set(CMAKE_BUILD_TYPE Release CACHE STRING "" FORCE) endif()` (plus de FORCE inconditionnel) ; retire les `-O2` en dur ; `-Wall -Wextra -Wpedantic` seulement si `CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang"` ; `option(RT_BUILD_TESTS "Compiler les tests" ON)` (FetchContent googletest + tests seulement si ON) ; `option(RT_SANITIZE "ASan+UBSan" OFF)` avec `-fsanitize=address,undefined -fno-omit-frame-pointer` (mentionne `-fsanitize=thread` en commentaire pour les data races) ; `target_compile_definitions(raytracer_lib PUBLIC RT_SOURCE_DIR="${CMAKE_SOURCE_DIR}")`.
7. Ajoute `.gitignore` (build/, *.ppm, .vscode/, .idea/, *.o) et `.clang-format` (indent 4, colonne 100).
8. **README.md** : précise que `-h` = hauteur et que l'aide = `--help` uniquement ; ajoute une section « Voir l'image » : un .ppm s'ouvre avec GIMP, IrfanView, l'extension VS Code « PBM/PPM viewer », ou `convert out.ppm out.png` (ImageMagick) ; ajoute en haut « Par où commencer : lis README_TACHES.md puis docs/PERSONNE_X.md ».

==================================================
# PARTIE 2 : comportements à documenter en COMMENTAIRES TODO (pas en code)
==================================================
Écris ces règles en commentaires détaillés dans les fichiers indiqués :
- Ray.hpp (TODO A) : toutes les directions de rayons sont NORMALISÉES à la création (caméra, ombre, réflexion). Les intersections restent correctes sans normalisation (a = d·d) mais t n'est une distance que si la direction est normalisée. `Camera::getRay` normalise.
- Color.hpp : le gamma est appliqué UNIQUEMENT dans Image::savePPM. Color ne fait que clamp + conversion 8 bits. Supprime le TODO « décider si le gamma doit rester ici ».
- Image.hpp/.cpp (TODO C) : constructeur : invalid_argument si width/height <= 0, taille calculée en `size_t` (pas de produit en int) ; setPixel hors image -> std::out_of_range ; savePPM : ordre EXACT par composante clampColor -> pow(c, 1/2.2) -> toRgb8, fichier binaire, en-tête `P6\n<w> <h>\n255\n`, vérifier ouverture ET état du flux après écriture (sinon runtime_error).
- ThreadPool (TODO C) : workerCount == 0 -> utiliser 1 ; capturer les exceptions dans chaque worker (std::exception_ptr) et les relancer dans run() après les join() (sinon std::terminate) ; Renderer.cpp inclura ThreadPool.hpp et passera une lambda qui boucle sur un `std::atomic<int> nextTile` (`fetch_add(1)`) jusqu'à épuisement.
- Renderer (TODO C) : RNG `std::mt19937` par TUILE seedé `baseSeed + tileIndex` (image identique quel que soit le nombre de threads) ; mapping `u = (x+jx)/width`, `v = 1 - (y+jy)/height` (y=0 en haut dans l'image, v=0 en bas pour la caméra) ; avec 1 sample jx = jy = 0.5 ; samples accumulés en linéaire puis divisés par samplesPerPixel ; PAS de clamp dans trace()/shading (clamp uniquement à l'export).
- Renderer (TODO B/C) : ajoute la déclaration privée `Color skyColor(const Ray&) const` : dégradé vertical `(1-t)*blanc + t*background` avec `t = 0.5*(direction.normalized().y()+1)`, utilisée quand le rayon ne touche rien ET à la profondeur max (l'actuel renvoie `scene.background()` plat, incohérent avec M1 « dégradé »). La directive `background` = couleur du HAUT du ciel. Corps : TODO + `return Color{};`.
- Renderer.cpp : B n'écrit que `trace()`, C que `render()` (jamais la même fonction).
- Scene::hit (TODO C) : closestSoFar = tMax ; pour chaque objet, si hit sur [tMin, closestSoFar], mettre à jour closestSoFar et rec ; vrai si au moins un hit.
- SceneParser (TODO C) : arité exacte, nombres finis (`std::isfinite`), matériau DÉCLARÉ AVANT son utilisation, nom de matériau dupliqué = erreur, fov dans ]0,180[, radius > 0, intensity >= 0, reflectivity dans [0,1], shininess >= 0, directive inconnue = erreur, une seule directive camera (obligatoire), messages `line N: ...`.
- bench/benchmark.cpp (TODO C) : échauffement puis médiane de 3 mesures ; speedup = t1/tN ; vérifier que les images sont identiques pour tous les N ; chemin de scène via RT_SOURCE_DIR ; noter `hardware_concurrency()`.
- Chaque fichier de test : en commentaire, la liste des cas à écrire avec leurs valeurs numériques (reprendre la section « Tests » de chaque docs/PERSONNE_X.md ci-dessous).

==================================================
# PARTIE 3 : nouveaux fichiers de documentation (Markdown, en français, ton clair)
==================================================
Crée, un fichier à la fois, sans tronquer : `README_TACHES.md`, `docs/PERSONNE_A.md`, `docs/PERSONNE_B.md`, `docs/PERSONNE_C.md`.
Ensuite, remplace le bandeau d'en-tête de CHAQUE fichier source par ce format, en reprenant MOT POUR MOT les explications « En clair » des docs :
  // Fichier / Responsable / Dépendances (comme avant)
  // EN CLAIR : 2 à 4 phrases sans jargon (avec analogie si utile)
  // ENTRÉES / SORTIES : ce qu'il reçoit, ce qu'il renvoie
  // APPELÉ PAR : qui l'utilise
  // PAS À FAIRE ICI : ce qui appartient à un autre fichier/personne
  // TODO(X) : étapes numérotées
Chaque docs/PERSONNE_X.md a cette structure : Mission en une phrase ; Fichiers (« En clair » + formules) ; Plan heure par heure (cases à cocher) ; Tests à écrire (valeurs chiffrées) ; Pièges ; Contrat avec les autres ; Définition de « fini ».

## README_TACHES.md : contenu

**L'idée en 5 lignes.** Une image est une grille de pixels. Pour chaque pixel, on envoie un rayon depuis l'« œil » (la caméra) à travers ce pixel, dans la scène 3D. On cherche le PREMIER objet touché. Rien touché : couleur du ciel. Sinon : on calcule la couleur du point (éclairage Phong), l'ombre (un objet entre le point et la lampe), et si la surface est un miroir on relance un rayon réfléchi (récursion limitée par une profondeur max). Les couleurs vont dans une image sauvegardée en PPM.

**Pipeline :**
```
main (options CLI) -> SceneParser (fichier .scene -> Scene)
-> Renderer::render : pour chaque pixel (par tuiles, multithread)
      Camera::getRay(u,v) -> Renderer::trace(rayon, depth)
            Scene::hit -> Hittable::hit (Sphere/Plane/Triangle) -> HitRecord
            computeLocalShading (Phong) <- isInShadow (rayon d'ombre)
            si reflectivity > 0 -> trace(rayon réfléchi, depth+1)
      -> Color -> Image::setPixel
-> Image::savePPM (clamp + gamma) -> out.ppm
```

**Glossaire** (1-2 phrases + exemple chacun) : rayon P(t)=O+t·D ; t ; HitRecord ; normale ; frontFace ; albedo ; Phong (ambiant/diffus/spéculaire) ; ombre et « acné d'ombre » ; profondeur de récursion ; anti-aliasing par jitter ; gamma ; tuile ; thread ; std::atomic ; RNG ; EPSILON vs RAY_T_MIN ; produit scalaire ; produit vectoriel.

**Conventions communes** : repère monde Y vers le haut, la caméra regarde vers -Z ; directions de rayons normalisées ; couleurs linéaires qui peuvent dépasser 1 (clamp + gamma 2.2 à l'export seulement) ; Scene possède objets (unique_ptr) et matériaux (shared_ptr), HitRecord a un pointeur non propriétaire ; image y=0 en haut, caméra v=0 en bas ; tMin des rayons secondaires = RAY_T_MIN ; les 6 signatures figées ne changent pas sans prévenir les 2 autres.

**Résultat attendu** : `./build/raytracer scenes/demo.scene -o out.ppm` : sol gris, sphère rouge mate à gauche, sphère miroir au centre qui reflète les autres et le ciel, sphère bleue brillante à droite, deux lumières donc deux ombres, ciel en dégradé.

**Technique de debug** : avant d'avoir l'éclairage, faire renvoyer à `trace` la couleur `0.5*(normale + (1,1,1))` : une sphère doit apparaître en dégradé lisse de couleurs (sphère, caméra et normale OK). Itérer en 200x100, 1 sample, 1 thread.

**Qui débloque qui** : A débloque tout le monde (Vec3 corrigé + Ray + Utils à pousser en premier), puis Camera + Sphere pour M2. C fait M1 seul (trace qui renvoie skyColor). B teste Phong avec un HitRecord fait main et une Scene vide.

**Milestones** : M1 dégradé (C, avec Camera de A) ; M2 première sphère (A+C) ; M3 Phong + ombres (B) ; M4 réflexions (B) ; M5 multithread + benchmark (C). Pour chacun : « comment savoir que c'est bon ».

**Git / intégration** : une branche par personne (`feature/a-geometry`, `feature/b-lighting`, `feature/c-infra`) ; chacun ne modifie QUE ses fichiers ; fichiers partagés (Scene.hpp, Renderer.cpp, CMakeLists.txt) : prévenir les autres ; rebase sur main chaque jour, petits commits, un tag par milestone. Ordre : 1) A pousse Vec3+Ray+Utils, tout le monde rebase ; 2) C : M1 ; 3) A : Camera+Sphere -> M2 ; 4) B : Phong+ombres -> M3, réflexions -> M4 ; 5) C : multithread -> M5 ; 6) tous : rendu final, tableau de perf, rapport. Checklist « j'ai fini » : compile sans warning (Release ET Debug), ctest vert, ASan/UBSan propre (`-DRT_SANITIZE=ON`), C : TSan propre, README à jour, PR relue par un autre. Intégration finale (2h tous ensemble) : rendu 800x450, 16 samples, profondeur 5 ; comparer avec le résultat attendu ; tableau de perf 1/2/4/8 threads ; extensions possibles (BVH, .obj, textures, depth of field, réfraction, export PNG).

## docs/PERSONNE_A.md : contenu (Cœur géométrique)

**Mission** : les maths : vecteurs, rayons, intersections, caméra. Tout le monde dépend de toi : pousse Vec3 + Ray + Utils dès la première heure.

**Fichiers : « En clair » + formules**
- Vec3.hpp : la brique de base, un triplet (x,y,z) pour positions, directions ET couleurs (Color = Vec3). `dot` mesure l'alignement (1 si même direction et normalisés, 0 si perpendiculaires, négatif si opposés). `cross` donne un vecteur perpendiculaire aux deux. `reflect(v,n) = v - 2(v·n)n` (rebond sur un miroir).
- Ray.hpp : demi-droite P(t) = origine + t·direction. Si direction normalisée, t = distance parcourue.
- Color.hpp : alias de Vec3 + clampColor + conversion 8 bits. Pas de gamma ici.
- Utils.hpp : constantes (EPSILON, RAY_T_MIN, PI), clamp, degrés -> radians.
- HitRecord.hpp : la « fiche rapport » d'une collision : point touché, normale (perpendiculaire à la surface, orientée CONTRE le rayon), distance t, frontFace (le rayon touche-t-il l'extérieur ?), matériau touché. `setFaceNormal` retourne la normale si le rayon vient de l'intérieur.
- Hittable.hpp : un contrat. Tout objet visible sait répondre « ce rayon te touche-t-il entre tMin et tMax ? Si oui remplis rec ». Grâce à `virtual`, Scene boucle sur des objets sans connaître leur type.
- Sphere : oc = O - C ; a = D·D ; half_b = oc·D ; c = oc·oc - r² ; disc = half_b² - a·c. disc < 0 : raté. Sinon racine proche (-half_b - sqrt(disc))/a ; si hors de ]tMin,tMax[, essayer la lointaine. Normale sortante = (P - C)/r, puis setFaceNormal. Tangente (disc = 0) = hit. Ne pas oublier rec.material = material_.get().
- Plane : denom = N·D ; si |denom| < EPSILON : parallèle, raté. t = ((point - O)·N)/denom ; rejeter si t hors de ]tMin,tMax[ ; normale via setFaceNormal.
- Triangle (OPTIONNEL) : Möller-Trumbore (e1 = B-A, e2 = C-A ; det = e1·(D×e2) ; rejeter |det| < EPSILON ; u, v >= 0 et u+v <= 1).
- Camera : l'œil + une fenêtre rectangulaire à distance 1 devant l'œil. Chaque pixel de l'image correspond à un point de la fenêtre. `getRay(u,v)` renvoie le rayon qui part de l'œil et passe par le point (u,v) de la fenêtre : (0,0) = coin bas-gauche, (1,1) = coin haut-droit, (0.5,0.5) = centre = pile vers lookAt. Le FOV règle la taille de la fenêtre (grand FOV = fenêtre large = grand angle). Maths : forward = normalize(lookAt - position) ; right = normalize(cross(forward, up)) ; trueUp = cross(right, forward) ; halfH = tan(fovRad/2) ; halfW = aspect·halfH ; horizontal = 2·halfW·right ; vertical = 2·halfH·trueUp ; lowerLeft = position + forward - horizontal/2 - vertical/2 ; getRay(u,v) = Ray(position, normalize(lowerLeft + u·horizontal + v·vertical - position)). Exemple : FOV 45° -> tan(22.5°) ≈ 0.4142, donc à distance 1 la fenêtre fait ≈ 0.83 de haut.

**Plan (10h)** : [ ] 1h fix Vec3 + tests + push sur main (débloque B et C) ; [ ] 0.5h Ray/Utils ; [ ] 2h Sphere + tests ; [ ] 1.5h Plane + tests ; [ ] 2.5h Camera + tests (+ cas dégénérés) ; [ ] 1h brancher caméra + sphère avec C (M2) ; [ ] 1.5h Triangle (bonus) OU aider C (main.cpp, benchmark).

**Tests (valeurs exactes)**
- Vec3 : dot((1,2,3),(4,5,6)) = 32 ; cross((1,0,0),(0,1,0)) = (0,0,1) ; length((3,4,0)) = 5 ; normalized((0,0,0)) = (0,0,0) ; reflect((1,-1,0),(0,1,0)) = (1,1,0).
- Sphere centre (0,0,-5) r=1 : O=(0,0,0) D=(0,0,-1) -> t=4, point (0,0,-4), normale (0,0,1), frontFace vrai. Rayon à y=2 : raté. Tangent (y=1) : hit, t=5. tMax=3 : raté. Sphère derrière le rayon : raté. Depuis l'intérieur (centre (0,0,0), r=1, O=(0,0,0), D=(0,0,1)) : t=1, frontFace faux, normale (0,0,-1).
- Plane point (0,0,0) normale (0,1,0) : O=(0,2,0) D=(0,-1,0) -> t=2, normale (0,1,0), frontFace vrai. D=(1,0,0) : raté. D=(0,1,0) : raté. O=(0,-1,0) D=(0,1,0) : t=1, frontFace faux, normale (0,-1,0).
- Camera position (0,0,0), lookAt (0,0,-1), up (0,1,0), fov 90, aspect 1 : getRay(0.5,0.5).direction = (0,0,-1) ; getRay(0,0) = normalize(-1,-1,-1) ; getRay(1,1) = normalize(1,1,-1). Aspect 2 : getRay(1,0.5) = normalize(2,0,-1). lookAt == position : invalid_argument. up parallèle à la visée : pas de NaN.

**Pièges** : division par zéro (a = 0, denom = 0) ; mauvaise racine ; oublier rec.material ou setFaceNormal ; NaN si normalized() d'un vecteur nul.

**Contrat** : B reçoit de toi un HitRecord complet (point, normale unitaire, t, frontFace, material). C reçoit un Ray de direction normalisée.

## docs/PERSONNE_B.md : contenu (Éclairage / matériaux)

**Mission** : donner leur couleur aux points touchés : lumière, ombres, miroirs.

**Fichiers : « En clair » + formules**
- Material.hpp : la fiche d'identité d'une surface. albedo = couleur de base. ambient = lumière minimale même à l'ombre (sans ça, noir pur). diffuse = matière mate, dépend de l'angle entre la surface et la lampe. specular + shininess = le petit reflet brillant ; shininess élevé = reflet petit et net (8 = mat, 96 = quasi miroir). reflectivity : 0 = pas de miroir, 1 = miroir parfait.
- Light.hpp : lampe ponctuelle : position, couleur, intensité (facteur multiplicatif).
- Material.cpp : computeLocalShading (couleur sans réflexions) et isInShadow.
- Renderer::trace (ta partie dans Renderer.cpp ; C ne touche que render()).

Formules. N = normale ; V = normalize(-viewRay.direction) ; par lumière : L = normalize(light.pos - P).
- ambiant (UNE seule fois, pas par lumière) : ambient · albedo
- si isInShadow : on saute cette lumière ; sinon diff = max(N·L, 0) ; R = reflect(-L, N) ; spec = (diff > 0) ? max(R·V, 0)^shininess : 0
- contribution = light.color · light.intensity · (diffuse·diff·albedo + specular·spec) (produit composante par composante)
- local = ambiant + somme des contributions
- ombre : rayon d'origine P, direction L, tMin = RAY_T_MIN, tMax = distance(P, lumière) - RAY_T_MIN. Si Scene::hit -> dans l'ombre (tMax évite qu'un objet DERRIÈRE la lampe fasse de l'ombre).
- trace(r, depth) : si depth >= maxDepth -> skyColor ; si Scene::hit raté -> skyColor ; local = computeLocalShading ; si reflectivity k > 0 : rayon réfléchi d'origine P, direction reflect(r.direction, N), tMin = RAY_T_MIN, profondeur +1 ; résultat = (1-k)·local + k·reflected.

**Plan (10h)** : [ ] 1.5h comprendre Phong (dessiner N, L, V, R sur papier) ; [ ] 1.5h isInShadow ; [ ] 2h ambiant + diffus ; [ ] 1h spéculaire ; [ ] 2h trace() avec réflexion récursive ; [ ] 1h régler les matériaux de demo.scene ; [ ] 1h intégration avec A et C.

**Tests (Scene vide, HitRecord construit à la main)**
- Point (0,0,0), normale (0,1,0), lumière (0,10,0) blanche intensité 1, albedo (1,0,0), ambient 0.1, diffuse 0.8, specular 0 : résultat (0.9, 0, 0).
- Lumière en (0,-10,0) (sous la surface) : seulement l'ambiant (0.1, 0, 0).
- Spéculaire : vue depuis (0,5,0), specular 0.5, albedo (1,0,0) : R·V = 1 -> résultat (1.4, 0.5, 0.5) (NON clampé, voulu).
- Ombre (avec la Sphere de A) : sphère centre (0,5,0) r=1 entre le point (0,0,0) et la lumière (0,10,0) -> true ; sphère en (0,20,0) (derrière la lampe) -> false ; sphère en (0,-5,0) -> false.
- trace : reflectivity = 0 -> pas de rayon récursif ; maxDepth = 0 -> skyColor.

**Pièges** : oublier de normaliser L ou V ; oublier RAY_T_MIN (acné d'ombre) ; spéculaire calculé quand diff <= 0 ; pow avec base négative (max(.,0) avant) ; clamper trop tôt ; récursion infinie si la condition de profondeur est fausse.

**Contrat** : tu reçois un HitRecord complet de A, tu renvoies une Color NON clampée à C.

## docs/PERSONNE_C.md : contenu (Infrastructure / performance)

**Mission** : tout ce qui entoure le moteur : lire la scène, parcourir les pixels en parallèle, stocker et sauver l'image, la ligne de commande, la mesure de performance, CMake.

**Fichiers : « En clair »**
- Image : tableau de pixels en mémoire (couleurs linéaires) + sauvegarde PPM, le format d'image le plus simple (petite en-tête texte puis octets RGB bruts).
- Scene : la boîte qui contient tout : objets (propriétaire), lumières, matériaux par nom, paramètres caméra, couleur du ciel. `hit()` répond à « quel est le PREMIER objet touché ? » en gardant le plus petit t.
- SceneParser : lit le fichier .scene ligne par ligne (une directive par ligne, `#` = commentaire), crée les objets et indique EXACTEMENT où est l'erreur (« line 12: unknown material 'foo' »).
- Renderer::render : découpe l'image en tuiles 32x32. Chaque thread prend la tuile suivante via un compteur atomique (pas de mutex). Pour chaque pixel : N rayons légèrement décalés au hasard (anti-aliasing : la moyenne lisse les bords). Un RNG par tuile.
- ThreadPool : petit wrapper autour de std::thread (ce n'est pas un vrai pool : il lance un lot de threads).
- main.cpp : lit les options (-o -w -h -s -d -t, --help), appelle parseur -> rendu -> sauvegarde, affiche le temps.
- bench/benchmark.cpp : rend la même scène avec 1, 2, 4, 8 threads et calcule le speedup.
- CMakeLists.txt : comment tout est compilé.

**Plan (10h)** : [ ] 1.5h Image + savePPM + test_image ; [ ] 1.5h Renderer::render en 1 thread avec ciel en dégradé (M1) ; [ ] 3h SceneParser + test_scene_parser (fichiers de test écrits dans un répertoire temporaire) ; [ ] 1h main.cpp ; [ ] 2h tuiles + threads + RNG par tuile (M5) ; [ ] 1h benchmark + tableau + court rapport de perf. Si A finit tôt, A prend main.cpp et le benchmark.

**Tests (valeurs exactes)**
- Image : dimensions ; setPixel/pixel aller-retour ; hors image -> out_of_range ; savePPM : le fichier commence par `P6\n2 2\n255\n` (11 octets) suivi de 12 octets ; pixel gris (0.5,0.5,0.5) -> 186 (0.5^(1/2.2) = 0.7297, x255.999) ; (1,0,0) -> 255,0,0 ; couleur > 1 clampée à 255 ; négative -> 0.
- Parser : scène valide (compter objets et lumières) ; commentaires et lignes vides ignorés ; directive inconnue -> `line 3: ...` ; matériau inconnu ; mauvaise arité ; `nan`/`inf` rejetés ; fichier introuvable.
- Rendu : image identique avec 1 thread et avec 4 threads (même scène, même seed) ; image 2x2 sans objets = couleurs du ciel.
- Benchmark : voir règles de la partie 2.

**Pièges** : data race (jamais deux threads sur la même tuile) ; `rand()` partagé ; y inversé dans l'image ; clamp avant pow ; chemin de scène relatif (utiliser RT_SOURCE_DIR) ; `-h` utilisé pour l'aide alors que c'est la hauteur. Tester le multithread avec `-fsanitize=thread`.

**Contrat** : de A : Camera::getRay (rayon normalisé), Scene::hit appelle Hittable::hit. De B : trace() renvoie une Color non clampée.

==================================================
# RAPPEL FINAL
==================================================
Aucune implémentation. Seulement : les corrections de la partie 1, des commentaires TODO détaillés (partie 2), les 4 fichiers Markdown et les nouveaux bandeaux (partie 3). Ensuite `cmake --build` sans warning et `git diff --stat`.