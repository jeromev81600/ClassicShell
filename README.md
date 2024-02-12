# Création d'un système shell classique.
Interpréteur de commande qui lit la commande entrée sur la console et l'exécute, ou qui exécute une liste d'instructions stockée dans un fichier (script).

## Technologies utilisées
CodeBlocks, linux (ubuntu).

## Fonctionnalités

Boucle sans fin effectuant les opérations suivantes :

printf   - Affichage.

fgets    - Lecture sur entrée standard  qui permet d'éviter les buffers overflow en limitant le nombre de caractères lus, elle gère également le retour charriot et les espaces.

strlen   - Calcule l'avant-dernier caractère en calculant la longueur de la chaîne.

strdup   - Permet de dupliquer une chaîne.

strtok   - Extrait des mots d'une chaîne.

### Prise en compte caractère joker *.

glob     - Permet de rechercher les occurrences de fichiers selon le motif lui ayant été passé.

strstr   - Permet de voir si le symbole « * » est contenu lors du parcours des sous-chaînes séparées par un espace (boucle while ligne 32).
Si le retour de strstr est NULL, il n'y a pas de caractère « * », j'exécute le code comme avant.
S'il y a le caractère « * », je passe la sous-chaîne en paramètre à glob, puis parcours la liste retournée et copie chaque occurrence dans la arg_list.
Ne pouvant plus connaître à l'avance le nombre d'arguments (celui-ci variant selon les retours éventuels de glob), je crée une arg_list de 32 éléments fixes à la place du calcul selon le nombre de sous-chaînes.

globfree - Libère la mémoire occupée par un appel précédent à glob, évite une fuite de mémoire.

### Rendre le programme père autonome en lui donnant la possibilité d''attendre ou non le comportement de son fils avec la fonction wait.

fork     - Créée un processus fils.

execvp   - Remplace le contenu du processus par l'application passée en paramètre. Cette fonction utilise le tableau de pointeurs arg_list[], pointant sur des chaînes de caractères terminées par des caractères nuls qui constituent les arguments disponibles pour le programme à exécuter.

strerror - Obtient le libellé du numéro d'erreur.

wait     - Fonction qui attend la fin du processus lui ayant été passé en paramètre (dans notre le cas le processus fils généré par fork).

Si le retour de fork est différent de 0, il s'agit du numéro de processus père (exécution de celui-ci), et me mets en attente de la fin du processus fils (wait);
si le retour de fork est égal à 0, je suis dans le processus fils, je lance execvp qui remplace le processus actuel par le contenu des paramètres passés et exécute donc le code de celui-ci.
Dans ce cas, execve sera l'équivalent d'une fonction infinie, car celle-ci modifie, comme expliqué, le code en cours d'exécution. Si execvp retourne (retour de -1 et code erreur dans errno), c'est qu'une erreur s'est produite et que le code ne s'est pas exécuté. Dans ce cas j'affiche l'erreur (strerror(errno)) ;
à ce stade, le processus fils est terminé et le père est réveillé.

### Libération de la mémoire.

free  - Libère la mémoire occupée par la chaîne retournée par strdup.
