#include <stddef.h>
#include <stdio.h>
#include "format.h"
#include <dirent.h>
#include <string.h>
#include "fAttrib.h"
#include <errno.h>
#include <stdlib.h>
#include "icons.h"

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
		if (strcmp(fname, ".") != 0 && strcmp(fname, "..") != 0 || showHidden >= 2) {
		    char *fPath = malloc(BUFSIZE + 128);
		    fPath = strdup(path);
		    strcat(fPath, "/");
		    strcat(fPath, fname);

		    char icon[9] = {' ',0,0,0,0,0,0,0,0};

		    char type = fType(fPath);

		    // Set the name color of the file.
		    
		    char fileColor[18] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

		    switch (type) {
			case '.': {} break;
			case 'd': {
				    strcpy(icon, " ");
				    strcpy(fileColor, BOLD);
				    strcat(fileColor, BLUE);
				  } break;
			case 'b': {
				    strcpy(fileColor, YELLOW);
				    strcat(fileColor, BOLD);
				    strcat(fileColor, BLACK_BG);
				  } break;
			case 'l': {
				    strcpy(icon, " ");
				    strcpy(fileColor, CYAN );
				  } break;
		    }
		    printf("%s%c%s", fileColor, type, RESET_FORMAT);
		    char isExec = fPermissions(fPath);

		    char sizePrefix[2] = "-";
		    char filler[9] = "\x1b[90m   ";

		    size_t len; // used later
		    if (type == '.') {
			strcpy(icon, fTypeIcon(fname, isExec));
			
			// Get the size of the file
			FILE *fptr = fopen(fPath, "r"); // Open the current file
			fseek(fptr, 0l, SEEK_END); // move the "cursor" to the end of the file
			len = ftell(fptr); // Get the "cursor" position and by that the file size
			fclose(fptr); // free(fptr);
			
			// format size text
			if (len < 10) {
			    strcpy(filler, "   ");
			    strcpy(sizePrefix, "");
			} else if (len < 100) { // 10
			    strcpy(filler, "  ");
			    strcpy(sizePrefix, "");
			} else if (len < 1000) { // 100
			    strcpy(sizePrefix, "");
			    strcpy(filler, " ");
			    strcat(filler, GREEN);
			} else if (len < 2000) { // [1k - 2k[
			    strcpy(sizePrefix, "");
			    strcpy(filler, GREEN);
			} else if (len < 10000) { // 1000
			    len /= 1000;
			    strcpy(sizePrefix, "k");
			    strcpy(filler, "  ");
			} else if (len < 100000) { // 10k
			    len /= 1000;
			    strcpy(sizePrefix, "k");
			    strcpy(filler, " ");
			    strcat(filler, GREEN);
			} else if (len < 1000000) { // 100k
			    len /= 1000;
			    strcpy(sizePrefix, "k");
			    strcpy(filler, "");
			    strcat(filler, GREEN);
			} else if (len < 10000000) { // 1m
			    len /= 1000000;
			    strcpy(sizePrefix, "m");
			    strcpy(filler, "  ");
			} else if (len < 100000000) { // 10m
			    len /= 1000000;
			    strcpy(sizePrefix, "m");
			    strcpy(filler, " ");
			} else if (len < 1000000000) { // 10m
			    len /= 1000000;
			    strcpy(sizePrefix, "m");
			    strcpy(filler, "");
			}


		    }

		    if (isExec > 0 && type == '.') { strcpy(fileColor, GREEN); }
		    if (type == '.') {
			printf("%s%ld%s%s %s%s %s%s%s\n", filler, len, sizePrefix, RESET_FORMAT, fileColor, icon, fileColor, fname, RESET_FORMAT);
		    } else {
			printf("%s%s%s %s%s %s%s%s\n", filler, sizePrefix, RESET_FORMAT, fileColor, icon, fileColor, fname, RESET_FORMAT);
		    }
		    free(fPath);
		}
	    }
	    free(fname);
	}
    }

    return 0;
}
