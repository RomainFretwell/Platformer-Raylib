# Platformer-Raylib

Objectif : faire un platformer dédié au speedrun dans un univers cyberpunk. Il y aura une mécanique de perte d'énergie au cours du temps ce qui encourage le joueur à rapidement finir le niveau. Il y aura aussi un éditeur de niveau.

## BUGS
- mettre une protection ou vérifier qu'on lit bien à des endroit valide de la mémoire pour les tableaux map.data et map.tiled sinon segmentation fault ou crash
- joueur tremble quand float ET quand la caméra bouge (joueur et map ne tremblent pas si caméra immobile, comme dans un coin)
- on multiplie position par screenRatio donc on bouge de 3 pixels à chaque fois en grand écran (même si pas le vrai problème car on peut aussi voir sacadé en petit écran)

## TODO
- Faire idle du perso
- Finir de dessiner background de ville futuriste
- Exporter BG en plusieurs couches + coder parallax
- Faire tableau d'animation avec comme index : AnimationState animState
- Retrouver de bonnes valeurs pour avoir un mouvement sympa 
