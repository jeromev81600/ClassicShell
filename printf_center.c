#include <stdio.h>
#include <string.h>

void printf_center(const char* main_title_str,int console_width)
{
    // Découpez le texte en lignes
    const char* delimiter = "\n";
    char* main_title = strtok((char*)main_title_str, delimiter);
    int num_lines = 0;
    char* lines[10]; // Suppose un maximum de 10 lignes
    while (main_title != NULL)
    {
        lines[num_lines++] = main_title;
        main_title = strtok(NULL, delimiter);
    }

    // Affiche chaque ligne centrée
    for (int i = 0; i < num_lines; i++)
    {
        int padding = (console_width - strlen(lines[i])) / 2;
        printf("%*s%s\n", padding, "", lines[i]); // Affiche la ligne centrée
    }
}
