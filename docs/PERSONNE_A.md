# Personne A — Cœur géométrique

## Mission

Implémenter les mathématiques : vecteurs, rayons, intersections et caméra.
A pousse d'abord `Vec3`, `Ray` et `Utils`, car B et C en dépendent.

## Formules et conventions

- `Vec3` fournit `dot`, `cross` et `reflect(v,n)=v-2(v·n)n`. Une normalisation
  nulle retourne le vecteur nul ; l'appelant doit traiter ce cas.
- Un rayon est `P(t)=O+tD`. Avec `D` normalisée, `t` est une distance.
- Sphère : `oc=O-C`, `a=D·D`, `half_b=oc·D`, `c=oc·oc-r²`,
  `disc=half_b²-a*c`, puis essayer la racine proche avant la lointaine.
- Plan : rejeter `|N·D| < EPSILON`, puis calculer
  `t=((point-O)·N)/(N·D)`.
- Triangle optionnel : Möller-Trumbore, avec `|det| < EPSILON` et contrôles de
  `u`, `v` et `u+v`.
- Caméra : `forward=normalize(lookAt-position)`, `right=cross(forward,up)`,
  `trueUp=cross(right,forward)`, fenêtre à distance 1 et rayon normalisé.
  Si `lookAt == position`, lever `invalid_argument`; si `up` est parallèle,
  utiliser `(0,0,1)`.

Les constructeurs doivent signaler `radius <= EPSILON`, normale quasi nulle,
matériau nul, triangle dégénéré et cas caméra invalides. C préfixera leurs
erreurs par `line N:`.

## Plan heure par heure

- [ ] 1 h : Vec3, tests et push.
- [ ] 0,5 h : Ray et Utils.
- [ ] 2 h : Sphere et tests.
- [ ] 1,5 h : Plane et tests.
- [ ] 2,5 h : Camera et cas dégénérés.
- [ ] 1 h : intégration M2 avec C.
- [ ] 1,5 h : Triangle optionnel ou aide à C.

## Tests à écrire

Valeurs minimales : `dot((1,2,3),(4,5,6))=32`, `cross((1,0,0),(0,1,0))=(0,0,1)`,
`length((3,4,0))=5`, normalisation nulle nulle, réflexion `(1,-1,0)` sur
`(0,1,0)` égale `(1,1,0)`. Tester la sphère `(0,0,-5), r=1` avec `t=4`,
la tangente `t=5`, `tMax=3`, l'arrière et l'intérieur. Tester le plan horizontal,
le parallélisme et l'intérieur. Pour la caméra FOV 90/aspect 1, centre
`(0,0,-1)`, coin bas-gauche `normalize(-1,-1,-1)` ; aspect 2, bord droit
`normalize(2,0,-1)`.

## Pièges

Éviter les divisions par zéro (`a=0`, dénominateur nul), choisir la bonne
racine, remplir `rec.material`, appeler `setFaceNormal` et ne jamais produire
NaN en normalisant un vecteur nul.

## Contrat

B reçoit un `HitRecord` complet (point, normale unitaire, `t`, `frontFace`,
matériau). C reçoit un rayon de direction normalisée.

## Définition de « fini »

Les cas numériques et dégénérés ci-dessus passent, et Vec3/Ray/Utils sont
poussés avant l'intégration de B et C.
