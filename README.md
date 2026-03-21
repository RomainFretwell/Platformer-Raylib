# Platformer-Raylib

Objectif : faire un platformer dédié au speedrun dans un univers cyberpunk. Il y aura une mécanique de perte d'énergie au cours du temps ce qui encourage le joueur à rapidement finir le niveau. Il y aura aussi un éditeur de niveau.

## BUGS
- régler problème de vitesse du jeu selon FPS
- mettre une protection ou vérifier qu'on lit bien à des endroit valide de la mémoire pour les tableaux map.data et map.tiled sinon segmentation fault ou crash
- joueur tremble quand float ET quand la caméra bouge (joueur et map ne tremblent pas si caméra immobile, comme dans un coin)
- on multiplie position par screenRatio donc on bouge de 3 pixels à chaque fois en grand écran (même si pas le vrai problème car on peut aussi voir sacadé en petit écran)

## TODO
- Finir de faire le ménage (changer Hitbox, Entity, ...)
- Simplifier bcp struct Entity
- Faire struct player avec entity dedans
- Mettre Entity dans struct Saw ?
- Ajouter des curseurs pour régler des variables sans avoir à recompiler à chaque fois
- Retrouver un mouvement sympa comme avec 200 FPS
- Dessiner batiments de devant et exporter en plusieurs couches
- Coder parallax
