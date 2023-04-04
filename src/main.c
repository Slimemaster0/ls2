// vim:fileencoding=utf-8:foldmethod=marker
// Include libraries {{{
#include <stddef.h>
#include <stdio.h>
#include "format.h"
#include <dirent.h>
#include <string.h>
#include "fAttrib.h"
#include <errno.h>
#include <stdlib.h>
#include "icons.h"
#include "extras.h"
// }}}

#define BUFSIZE 256

int main(int argc, char *argv[]) {
    char *path= ".";
    char showHidden = 0;

    for (int i = 1; i < argc; i++) { // {{{ Argument parser
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
    } // }}}

    struct dirent *pDirent; // Directory ending.

    DIR *dirP = opendir(path);
    if (dirP == NULL) { // test if the directory exists
	printf("%sERR:%s Couldn't open directory %s\n", RED, RESET_FORMAT, path);
	return 2;
    }

    while ((pDirent = readdir(dirP)) != NULL) { // {{{ Print the files
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

		    // {{{ File types
		    switch (type) {
			case '.': break;
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
		    } // }}}

		    // Print the type char
		    printf("%s%c%s", fileColor, type, RESET_FORMAT);

		    // Print the file permissions
		    char isExec = fPermissions(fPath);

		    char sizePrefix[2] = "-";
		    char sizeFiller[9] = "\x1b[90m   ";

		    size_t len; // used later
		    if (type == '.') {
			strcpy(icon, fTypeIcon(fname, isExec));
			
			// Get the size of the file
			FILE *fptr = fopen(fPath, "r"); // Open the current file
			fseek(fptr, 0l, SEEK_END); // move the "cursor" to the end of the file
			len = ftell(fptr); // Get the "cursor" position and by that the file size
			fclose(fptr); // free(fptr);

			// format size text #: {{{
			char spc3[4] = "   ";
			char spc2[3] = "  ";
			char spc1[2] = " ";
			char spc0[1] = "";
			
			if (len < 10) {
			    strcpy(sizeFiller, spc3);
			    strcpy(sizePrefix, spc0);
			} else if (len < 100) { // 10
			    strcpy(sizeFiller, spc2);
			    strcpy(sizePrefix, spc0);
			} else if (len < 1000) { // 100
			    strcpy(sizePrefix, spc0);
			    strcpy(sizeFiller, spc1);
			    strcat(sizeFiller, GREEN);
			} else if (len < 2000) { // [1k - 2k[
			    strcpy(sizePrefix, spc0);
			    strcpy(sizeFiller, GREEN);
			} else if (len < 10000) { // 1000
			    len /= 1000;
			    strcpy(sizePrefix, "K");
			    strcpy(sizeFiller, spc2);
			} else if (len < 100000) { // 10k
			    len /= 1000;
			    strcpy(sizePrefix, "K");
			    strcpy(sizeFiller, spc1);
			    strcat(sizeFiller, GREEN);
			} else if (len < 1000000) { // 100k
			    len /= 1000;
			    strcpy(sizePrefix, "K");
			    strcpy(sizeFiller, spc0);
			    strcat(sizeFiller, YELLOW);
			} else if (len < 10000000) { // 1m
			    len /= 1000000;
			    strcpy(sizePrefix, "M");
			    strcpy(sizeFiller, spc2);
			    strcat(sizeFiller, YELLOW);
			} else if (len < 100000000) { // 10m
			    len /= 1000000;
			    strcpy(sizePrefix, "M");
			    strcpy(sizeFiller, spc1);
			    strcat(sizeFiller, RED);
			} else if (len < 1000000000) { // 100m
			    len /= 1000000;
			    strcpy(sizePrefix, "M");
			    strcpy(sizeFiller, spc0);
			    strcat(sizeFiller, RED);
			} else if (len < 1000000000) { // 1b
			    len /= 1000000000;
			    strcpy(sizePrefix, "G");
			    strcpy(sizeFiller, spc2);
			    strcat(sizeFiller, RED);
			} else if (len < 10000000000) { // 10b
			    len /= 10000000000;
			    strcpy(sizePrefix, "G");
			    strcpy(sizeFiller, spc1);
			    strcat(sizeFiller, RED);
			} else if (len < 100000000000) { // 100b
			    len /= 1000000000;
			    strcpy(sizePrefix, "G");
			    strcpy(sizeFiller, spc0);
			    strcat(sizeFiller, RED);
			} // #: }}}
		    }

		    char* userName = getFileOwnerU(fname);
		    char fillerUN[28];
		    if (userName[0] == 27) {
			strcpy(fillerUN, filler(27 - strlen(userName)));
		    } else {
			strcpy(fillerUN, filler(18 - strlen(userName)));
		    }


		    // Print out the file size, -icon, -name
		    if (isExec > 0 && type == '.') { strcpy(fileColor, GREEN); }
		    if (type == '.') {
			printf("%s%ld%s%s %s%s %s%s%s%s%s\n", sizeFiller, len, sizePrefix, RESET_FORMAT, fillerUN, userName, fileColor, icon, fileColor, fname, RESET_FORMAT);
		    } else {
			printf("%s%s%s %s%s %s%s%s%s%s\n", sizeFiller, sizePrefix, RESET_FORMAT, fillerUN, userName, fileColor, icon, fileColor, fname, RESET_FORMAT);
		    }
		    // Free memory
		    free(fPath);
		}
	    }
	    free(fname);
	}
    } // }}}

    return 0;
}
