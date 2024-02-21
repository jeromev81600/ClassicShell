  #ifdef _WIN32
  #include <windows.h>

  void glob(const char *pattern, char *arg_list[], int *increment) {
        WIN32_FIND_DATA findFileData;
        HANDLE hFind = FindFirstFile(pattern, &findFileData);

        if (hFind == INVALID_HANDLE_VALUE) {
            printf("Aucun fichier correspondant au motif '%s' n'a été trouvé.\n", pattern);
            return;
        }

        do {
            if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                arg_list[*increment] = strdup(findFileData.cFileName);
                (*increment)++;
            }
        } while (FindNextFile(hFind, &findFileData) != 0);

        FindClose(hFind);
    }
#endif
