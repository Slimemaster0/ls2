#include <stdio.h>
#include "format.h"
#include <dirent.h>
#include <string.h>
#include "fAttrib.h"
#include <errno.h>
#include <stdlib.h>

#define BUFSIZE 256

int main(int argc, char *argv[]) {
    char *path= ".";
    char showHidden = 0;

    for (int i = 1; i < argc; i++) {
	size_t argLen = strlen(argv[i]);
	if (argv[i][0] == '-' && argv[i][1] != '-') {

	    for (int j = 1; j <= argLen; j++) {
		switch(argv[i][j]) {
		    case 'a': showHidden = 2; break;
		    case 'A': showHidden = 1; break;

		    case 0: break;
		    default: {
			printf("%sERR:%s unknown argument '%s%c%s'\n", RED, RESET_FORMAT, BOLD, argv[i][j], RESET_FORMAT);
			return 1;
		    }
		}
	    }
	    continue;
	}
	path = malloc(sizeof(argv[i]));
	if(path != NULL)
	strcpy(path, argv[i]);
    }

    struct dirent *pDirent; // Directory ending.

    DIR *dirP = opendir(path);
    if (dirP == NULL) { // test if the directory exists
	printf("%sERR:%s Couldn't open directory %s\n", RED, RESET_FORMAT, path);
	return 2;
    }

    while ((pDirent = readdir(dirP)) != NULL) { // Print the files
	char *fname = malloc(BUFSIZE);
	if (fname != NULL) {
	    fname = strdup(pDirent->d_name);
	    if (fname[0] != '.' || showHidden >= 1) {
		    char *fPath = malloc(BUFSIZE + 64);
		    fPath = strdup(path);
		    strcat(fPath, "/");
		    strcat(fPath, fname);

		    char type = fType(fPath);

		    // Set the name color of the file.
		    char fileColor[18] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
		    switch (type) {
			case '.': {} break;
			case 'd': {
				    strcpy(fileColor, BOLD);
				    strcat(fileColor, BLUE);
				  } break;
			case 'b': {
				    strcpy(fileColor, YELLOW);
				    strcat(fileColor, BOLD);
				    strcat(fileColor, BLACK_BG);
				  } break;
			case 'l': { strcpy(fileColor, CYAN ); } break;
		    }

		    printf("%s%c%s", fileColor, type, RESET_FORMAT);
		    char isExec = fPermissions(fPath);

		    if (isExec > 0 && type == '.') { strcpy(fileColor, GREEN); }
		    
		    printf("%s%s%s\n", fileColor, fname, RESET_FORMAT);
		    free(fPath);
	    }
	    free(fname);
	}
    }

    return 0;
}
