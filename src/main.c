#include <stdio.h>
#include "format.h"
#include <dirent.h>
#include <string.h>

#define BUF_SIZE 256

int main(int argc, char *argv[]) {
    char path[BUF_SIZE] = ".";

    char showHidden = 0;
    for (int i = 1; i < argc; i++) {
	if (strcmp(argv[i], "-a") == 0) {
	    showHidden = 1;
	    continue;
	}
	strcpy(path, argv[i]);
    }

    struct dirent *pDirent; // Directory ending.

    DIR *dirP = opendir(path);
    if (dirP == NULL) { // test if the directory exists
	printf("%sERR:%s Couldn't open directory %s\n", RED, RESET_FORMAT, path);
	return 1;
    }

    while ((pDirent = readdir(dirP)) != NULL) { // Print the files
	char *fname = strdup(pDirent->d_name);
	if (fname[0] != '.' || showHidden >= 1) {
		printf("%s\n", fname);
	}
    }

    return 0;
}
