# Platformer-Raylib

Objectif : faire un platformer dans un style 8 bit médiéval avec une option création de niveau
(trouver une mécanique de jeu unique)

## BUGS
- vérifier qu'on lit bien à des endroit valide dela mémoire pour les tableaux map.data et map.tiled sinon segmentation fault ou crash
- joueur tremble quand float ET quand la caméra bouge (joueur et map ne tremblent pas si caméra immobile, comme dans un coin)
- on multiplie position par screenRatio donc on bouge de 3 pixels à chaque fois en grand écran (même si pas le vrai problème car on peut aussi voir sacadé en petit écran)

## TODO
- optimisation fonctions : completeAutoTile et autoTile (que pour les 9 blocks concerné)
- Faire tableau d'animation avec comme index : AnimationState animState
- Retrouver de bonnes valeurs pour avoir un mouvement sympa 
