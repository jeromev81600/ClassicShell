#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <glob.h>

#define TAILLE_BUFFER 150

int main()
{
      char    buffer[TAILLE_BUFFER];

    printf("\n Welcome to my command interpreter \n Tape \"exit\" to exit \n \n");

    while(1)
    {
        printf("Prompt : ");
        fgets(buffer,TAILLE_BUFFER,stdin);              // Stock la saisie dans le tampon "buffer"
        buffer[strlen(buffer)-1]='\0';                  // Suppression du retour chariot de fin
        if (strcmp("exit",buffer)==0)                   // Condition gérant la sortie de l'interpréteur lorsque "exit" est tapé en ligne de commande
        {
            exit(0);
        }
        char *arg_list[32];                             // Création d'un tableau contenant les bouts de ligne entre caractères espace ;
        char *p=strdup(buffer);
        char *tmp=strtok(p," ");
        int increment=0;
        while (tmp!=NULL)                               // Boucle qui itère sur chaque mot obtenu par strtok()
        {
            char *p=strstr(tmp,"*");
            if (p==NULL)                                // Si le mot ne contient pas '*', il est ajouté à arg_list.
            {
                arg_list[increment]=strdup(tmp);
                increment++;
            }
            else                                        //Si le mot contient '*', la fonction glob() est utilisée pour rechercher les fichiers correspondant au motif. Les fichiers trouvés sont ensuite ajoutés à arg_list.
            {
                glob_t     g;
                int retour_glob=glob(tmp,0,NULL,&g);
                if (retour_glob==0)
                {
                    int boucle;
                    for (boucle=0;boucle<g.gl_pathc;++boucle)
                    {
                        arg_list[increment]=strdup(g.gl_pathv[boucle]);
                        increment++;
                    }
                }
                globfree(&g);
            }
            tmp=strtok(NULL," ");
        }
        arg_list[increment]=NULL;
        pid_t process=fork();                              // Création d'un nouveau processus (fork);
        if (process==0)
        {
            int retour=execvp(arg_list[0],arg_list);
            if (retour==-1) printf("%s\n",strerror(errno));
            exit(0);
        }
        else
        {
            wait(&process);
        }
        increment=0;
        while (arg_list[increment]!=NULL)
        {
            free(arg_list[increment]);                     // Libération des chaînes contenues dans arg_list ;
            increment++;
        }
        free(p);
    }
}
