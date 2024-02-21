#include <windows.h>    // Inclure la bibliothèque Windows pour utiliser GetConsoleScreenBufferInfo

// Fonction pour obtenir la largeur de la console sous Windows
int get_console_width() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.dwSize.X;
}
