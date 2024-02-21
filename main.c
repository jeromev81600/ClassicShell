#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "printf_center.h"

#define TAILLE_BUFFER 150

//Préprocesseur défini par le compilateur lorsque le programme est sur windows
#ifdef _WIN32
#include <windows.h>
#include "get_console_width.h"
#include "glob_windows.h"
#define OS "Windows"

//Préprocesseur défini par le compilateur lorsque le programme est sur Linux
#elif __linux__
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <glob.h>
#define OS "Linux"

//Préprocesseur défini par le compilateur lorsque le programme ne correspond à aucun des deux
#else
#define OS "Unknown"
#endif


int main()
{
    /* message to be appeared on the screen */
    char main_title_str[] = "+---------------------------------------------------+\n*  Welcome to my command interpreter  *\n created for my registration at holberton school \n+---------------------------------------------------+\n\n\n\n";

    int console_width = get_console_width();
    printf_center(main_title_str, console_width);

    printf("\nThis version of the program runs on : %s\n< Tape \"exit\" to exit > \n\n\n", OS);
    char    buffer[TAILLE_BUFFER];


    while(1)
    {
        printf("Prompt : ");
        fgets(buffer,TAILLE_BUFFER,stdin);              // Stock la saisie dans le tampon "buffer"
        buffer[strlen(buffer)-1]='\0';                  // Suppression du retour chariot de fin
        if (strcmp("exit",buffer)==0)                   // Condition gérant la sortie de l'interpréteur lorsque "exit" est tapé en ligne de commande
        {
            exit(0);
        }
        else if (strncmp("cd ", buffer, 3) == 0)            // Vérification de la commande "cd"
        {
            char *dir = buffer + 3;                         // Extrait le chemin du répertoire à changer
            if (chdir(dir) != 0)                            // Changer de répertoire
            {
                perror("chdir");
            }
        }else if (strcmp("cd ..", buffer) == 0) {
            chdir(".."); // Changer de répertoire de travail dans le processus parent
        } else {
            system(buffer); // Exécuter la commande dans le processus fils
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
#ifdef _WIN32
                glob(tmp, arg_list, &increment);
#else
                // Traitement spécifique à Linux
                glob_t g;
                int retour_glob=glob(tmp,0,NULL,&g);
                if (retour_glob==0)
                {
                    int boucle;
                    for (boucle=0; boucle<g.gl_pathc; ++boucle)
                    {
                        arg_list[increment]=strdup(g.gl_pathv[boucle]);
                        increment++;
                    }
                }
                globfree(&g);
#endif
            }
            tmp=strtok(NULL," ");
        }
        arg_list[increment]=NULL;

#ifdef _WIN32
        // Créer un processus cmd.exe
        PROCESS_INFORMATION piProcInfo;
        STARTUPINFO siStartInfo;
        BOOL bSuccess = FALSE;

        // Initialiser les structures STARTUPINFO et PROCESS_INFORMATION.
        ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));
        ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));
        siStartInfo.cb = sizeof(STARTUPINFO);

        // Créer une commande avec les arguments.
        char command[TAILLE_BUFFER] = "cmd.exe /C ";
        strcat(command, buffer);

        // Créer le processus
        bSuccess = CreateProcess(NULL,
                                 command,
                                 NULL,
                                 NULL,
                                 FALSE,
                                 0,
                                 NULL,
                                 NULL,
                                 &siStartInfo,
                                 &piProcInfo);

        // Vérifier si le processus a été créé
        if (!bSuccess)
        {
            fprintf(stderr, "CreateProcess failed (%lu).\n", GetLastError());
            return 1;
        }

        // Attendre que le processus se termine
        WaitForSingleObject(piProcInfo.hProcess, INFINITE);

        // Fermer les handles de processus et de thread.
        CloseHandle(piProcInfo.hProcess);
        CloseHandle(piProcInfo.hThread);
#else
        // Traitement spécifique à Linux
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
#endif
        increment=0;
        while (arg_list[increment]!=NULL)
        {
            free(arg_list[increment]);                     // Libération des chaînes contenues dans arg_list ;
            increment++;
        }
        free(p);
    }
}
