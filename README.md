# Platformer-Raylib

Objectif : faire un platformer dans un style 8 bit médiéval avec une option création de niveau
+ trouver une mécanique de jeu unique

## TODO
- améliorer caméra -> plus fluide
- modifier structure map/world pour en avoir plusieurs mais de différentes tailles
- enlever rotation joueur et faire une fonction collision AABB
- finir handleCollision -> avec les bon décalage + utiliser while
- séparer en différents .c et .h

## Brouillon
Problème collision (marche pas si forme de pensement en croix)
-> Pas vraiment un problème vu que il faut d'abord traverser l'objet (et donc avoir une collision) pour être dans cette position
Vrai solution = algorithme SAT
Ma version des collisions devrait suffire et semble plus rapide
