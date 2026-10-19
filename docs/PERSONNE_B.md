# Personne B — Éclairage et matériaux

## Mission

Implémenter `computeLocalShading`, `isInShadow` et `Renderer::trace` uniquement.
C conserve `Renderer::render`.

Pour chaque lumière, `N` est la normale, `V=normalize(-viewRay.direction)` et
`L=normalize(light.position-point)`. L'ambiant est ajouté une seule fois :
`ambient*albedo`. Hors ombre, ajouter
`light.color*intensity*(diffuse*max(N·L,0)*albedo + specular*spec)`, avec
`R=reflect(-L,N)` et `spec=max(R·V,0)^shininess` seulement si le diffus est
positif. Ne jamais clamper avant l'export.

L'ombre utilise un rayon de direction `L`, `tMin=RAY_T_MIN` et
`tMax=distance(point,light)-RAY_T_MIN`; cela évite l'acné et les objets derrière
la lampe. Une réflexion utilise aussi `RAY_T_MIN`. `trace` retourne `skyColor`
à profondeur maximale ou sans intersection, sinon combine local et réflexion :
`(1-k)*local+k*reflected`.

## Plan heure par heure

- [ ] 1,5 h : comprendre N, L, V et R.
- [ ] 1,5 h : ombres.
- [ ] 2 h : ambiant et diffus.
- [ ] 1 h : spéculaire.
- [ ] 2 h : trace et réflexion.
- [ ] 1 h : régler demo.scene.
- [ ] 1 h : intégration.

## Tests à écrire
Avec un point `(0,0,0)`, normale `(0,1,0)`, lumière blanche en `(0,10,0)`,
albedo rouge et ambient `0.1`, diffuse `0.8`, le résultat diffus est `(0.9,0,0)`.
Une lumière sous la surface ne donne que l'ambiant ; le test spéculaire attendu
est `(1.4,0.5,0.5)` et reste volontairement non clampé. Vérifier les ombres
devant, derrière et sous la lampe, puis `reflectivity=0` et `maxDepth=0`.

## Pièges

Normaliser L et V, utiliser RAY_T_MIN, ignorer le spéculaire si le diffus est
nul, borner la base de pow à zéro, arrêter la récursion et ne pas clamper tôt.

## Contrat

A fournit un `HitRecord` fiable ; B renvoie une `Color` linéaire non clampée.

## Définition de « fini »

Les valeurs Phong, ombres et réflexions sont vérifiées sans clamp prématuré,
et C peut conserver exclusivement `Renderer::render`.
