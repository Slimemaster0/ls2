#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "format.h"

char fType(const char *path) {
    struct stat path_stat;
    stat(path, &path_stat);
    
    if (S_ISREG(path_stat.st_mode) != 0) {
	return 'r';
    } else if (S_ISDIR(path_stat.st_mode) != 0) {
	return 'd';
    } else if (S_ISBLK(path_stat.st_mode) != 0) {
	return 'b';
    } else if (S_ISCHR(path_stat.st_mode) != 0) {
	return 'c';
    } else if (S_ISFIFO(path_stat.st_mode) != 0) {
	return 'p';
    } else if (S_ISLNK(path_stat.st_mode) != 0) {
	return 'l';
    } else if (S_ISSOCK(path_stat.st_mode) != 0) {
	return 's';
    } else {
	return '-';
    }

    printf("%sERR:%s Could not identify filetype\n", RED, RESET_FORMAT);
    exit(3);
}
