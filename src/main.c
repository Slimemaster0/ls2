#include <stdio.h>
#include "format.h"
#include <dirent.h>
#include <string.h>
#include "fAttrib.h"
#include <errno.h>

#define BUF_SIZE 256

int main(int argc, char *argv[]) {
    char path[BUF_SIZE] = ".";
    char showHidden = 0;

    for (int i = 1; i < argc; i++) {
	if (argv[i][0] == '-' && argv[i][1] != '-') {
	    size_t argLen = strlen(argv[i]);

	    for (int j = 1; j <= argLen; j++) {
		switch(argv[i][j]) {
		    case 'a': showHidden = 2;
		    case 'A': showHidden = 1;

		    default: {
			printf("%sERR:%s unknown argument '%s%c%s'", RED, RESET_FORMAT, BOLD, argv[i][j], RESET_FORMAT);
			return errno;
		    }
		}
	    }
	    continue;
	}
	strcpy(path, argv[i]);
    }

    struct dirent *pDirent; // Directory ending.

    DIR *dirP = opendir(path);
    if (dirP == NULL) { // test if the directory exists
	printf("%sERR:%s Couldn't open directory %s\n", RED, RESET_FORMAT, path);
	return errno;
    }

    while ((pDirent = readdir(dirP)) != NULL) { // Print the files
	char *fname = strdup(pDirent->d_name);
	if (fname[0] != '.' || showHidden >= 1) {
		char *fPath = strdup(path);
		strcat(fPath, "/");
		strcat(fPath, fname);

		char type = fType(fPath);
		printf("%c", type);
		char isExec = fPermissions(fPath);
		printf("%s\n", fname);
	}
    }

    return 0;
}
