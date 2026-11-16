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

Normaliser L et V, utiliser RAY_T_MIN, ignorer le spéculaire si le diffus est
nul, borner la base de pow à zéro, arrêter la récursion et ne pas clamper tôt.

## Contrat

A fournit un `HitRecord` fiable ; B renvoie une `Color` linéaire non clampée.

## Définition de « fini »

Les valeurs Phong, ombres et réflexions sont vérifiées sans clamp prématuré,
et C peut conserver exclusivement `Renderer::render`.

# Théorie

## Ordre de calcul dans le code

Le calcul d'un point suit toujours le même ordre :

1. récupérer `N`, `V`, le matériau et le point `P` depuis le `HitRecord` ;
2. initialiser la couleur avec `ambient * albedo` ;
3. pour chaque lumière, lancer le rayon d'ombre ;
4. si la lumière est visible, calculer `L`, puis le diffus ;
5. si le diffus est positif, calculer `R` et le spéculaire ;
6. multiplier la contribution de cette lumière par sa couleur et son intensité ;
7. retourner la couleur linéaire sans clamp.

![alt text](img/pipeline_shading.svg)


L'ombre ne supprime donc pas l'ambiant et ne supprime pas les autres lumières :
elle supprime seulement les composantes diffuse et spéculaire de la lumière
masquée.

## Comprendre le rayon d'ombre

![alt text](img/ombres.svg)


Pour un point `P` et une lumière `Q`, le vecteur vers la lumière est :

```text
toLight = Q - P
distance = length(toLight)
L = toLight / distance
```

Le rayon d'ombre est `Ray(P, L)`. Comme `L` est normalisé, le paramètre `t`
du rayon représente une distance. On ne teste que le segment vers la lumière :

```text
tMin = RAY_T_MIN
tMax = distance - RAY_T_MIN
```

`tMin` évite que la surface de départ se détecte elle-même (acné d'ombre).
`tMax` empêche un objet situé derrière la lumière de bloquer la lumière.

## Lire les résultats des tests

Avec `P=(0,0,0)`, `N=(0,1,0)` et une lumière blanche en `(0,10,0)`,
`L=N`, donc `N·L=1`. Pour un albedo rouge, `ambient=0.1` et `diffuse=0.8`,
le résultat est `(0.9,0,0)`.

Si la lumière est en `(0,-10,0)`, alors `N·L=-1`. Après le `max`, le diffus
vaut zéro. Le spéculaire est également ignoré : il reste uniquement l'ambiant.

Avec une vue alignée sur le reflet et `specular=0.5`, la composante spéculaire
peut dépasser 1 dans un canal. Le résultat `(1.4,0.5,0.5)` est correct avant
export : le clamp et le gamma ne doivent pas être appliqués dans le shading.

## Vecteurs de PHONG

Le modèle de Phong calcule la couleur d'un point de la surface à partir de quatre vecteurs, tous définis **au point touché** `P`. Ils sont **normalisés** (longueur 1) et tous orientés **en partant de la surface**. Pour deux vecteurs unitaires, le produit scalaire est égal au cosinus de l'angle qui les sépare : 1 s'ils sont alignés, 0 s'ils sont perpendiculaires, négatif s'ils sont opposés. C'est ce qui permet de mesurer l'éclairage avec de simples produits scalaires.

### Les quatre vecteurs

| Vecteur | Rôle | Formule |
|---|---|---|
| **N** | Normale à la surface | Fournie par le `HitRecord` |
| **L** | Direction vers la lumière | `L = normalize(light.position - P)` |
| **V** | Direction vers l'œil | `V = normalize(-viewRay.direction)` |
| **R** | Direction de réflexion de la lumière | `R = reflect(-L, N)` |

![alt text](img/phong_vectors.png)

**N : la normale.** Vecteur perpendiculaire à la surface en `P`. Elle est orientée contre le rayon incident, donc toujours tournée vers l'observateur. Elle est calculée lors de l'intersection (`HitRecord::setFaceNormal`) et fournie dans le `HitRecord`.

**L : direction vers la lumière.** Vecteur unitaire allant de `P` vers la lampe. La distance `‖light.position - P‖` est conservée pour le calcul des ombres.

**V : direction vers l'œil.** Vecteur unitaire allant de `P` vers la caméra. Le rayon de vue va de la caméra vers le point : on l'inverse pour que `V` parte de la surface.

**R : direction de réflexion.** Direction dans laquelle un miroir parfait renverrait la lumière issue de la lampe, avec `reflect(d, n) = d - 2·(d·n)·n`. On passe `-L` car la fonction attend un vecteur incident (qui arrive sur la surface), alors que la lumière arrive de la lampe vers le point.

### Les trois composantes de l'éclairage

| Composante | Formule | Interprétation |
|---|---|---|
| Ambiante | `ambient · albedo` | Lumière indirecte, indépendante des vecteurs |
| Diffuse | `diffuse · max(N·L, 0) · albedo` | La surface est-elle face à la lampe ? |
| Spéculaire | `specular · max(R·V, 0)^shininess` | L'œil est-il dans l'axe du reflet ? |

- **Ambiante** : ajoutée **une seule fois** par point, et non par lumière.
- **Diffuse** : maximale quand la lampe est face à la surface, nulle quand elle est rasante ou derrière.
- **Spéculaire** : plus `shininess` est grand, plus le reflet est petit et net. Elle n'est calculée que si le diffus est **strictement positif**, pour éviter un reflet fantôme provenant d'une lampe située derrière la surface.

Pour chaque lumière non masquée par une ombre, la contribution est :

```text
light.color · intensity · (diffuse · max(N·L, 0) · albedo + specular · spec)
```

Les couleurs restent **linéaires** et ne sont pas bornées avant l'export de l'image.

