Documentation d'utilisation :

Lien du github :
https://github.com/M4THIA5/Project_BQM.git


Le but de notre projet est de créer un éditeur de texte à l'aide de SDL.
On souhaite pouvoir créer un document à partir de ce qu'on écrit dans notre fenêtre et que notre texte soit ajouté dans notre fichier txt à 
chaque fois que nous tapons sur notre clavier. Cela nous permettra de pouvoir ouvrir notre fichier txt avec notre texte dedans dans un autre
éditeur de texte.

Pour lancer le code il faut dans un premier compiler : 

gcc src/main.c -o bin/main -I include -L lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf

Et enfin il faut éxécuter la commande suivante il faut se situer dans le dossier "bin":

./main.exe

Fonctionnalités :

- Menu avec un sous Menu :
    - Changement des couleurs des lettres avec plusieurs couleurs de possible
    - Quitter le programme
- Gestion de l'écriture :
        - Ecrire les lettres
        - Ecrire en majuscule
        - Ecrire des cractères spéciaux
        - Supprimer les lettres, chiffres, majuscules, caractères spéciaux
        - Pouvoir se déplacer dans notre texte avec flèche gauche et droite 
        - Remplacer la lettre une fois déplacé à l'endroit souhaité
- Sauvegarde de notre texte en continue
- Fichier de configuration

Le fichier de configuration permet de sauvegarder les options qu'un joueur souhaite avoir au démarrage du programme.


