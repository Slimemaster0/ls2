// vim:fileencoding=utf-8:foldmethod=marker
// {{{ // Include libraries
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "format.h"
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
// }}}

char fType(const char *path) { // {{{
    struct stat path_stat;
    stat(path, &path_stat);
    
    if (S_ISREG(path_stat.st_mode)) {
	return '.';
    } else if (S_ISDIR(path_stat.st_mode)) {
	return 'd';
    } else if (S_ISBLK(path_stat.st_mode)) {
	return 'b';
    } else if (S_ISCHR(path_stat.st_mode)) {
	return 'c';
    } else if (S_ISFIFO(path_stat.st_mode)) {
	return 'p';
    } else if (S_ISLNK(path_stat.st_mode)) {
	return 'l';
    } else if (S_ISSOCK(path_stat.st_mode)) {
	return 's';
    } else {
	return '.';
    }
} // }}}

char fPermissions(const char *path) { // {{{
    struct stat fileattrib;
    int fileMode;

    char isExec = 0;

    /* print a leading dash as start of file/directory permissions */
    printf("-");
    fileMode = fileattrib.st_mode;
    /* Check owner permissions */
    if ((fileMode & S_IRUSR) && (fileMode & S_IREAD))
      printf("%s%sr%s", BOLD, YELLOW, RESET_FORMAT);
    else
      printf("-");
    if ((fileMode & S_IWUSR) && (fileMode & S_IWRITE)) 
      printf("%s%sw%s", BOLD, RED, RESET_FORMAT);
    else
      printf("-");
    if ((fileMode & S_IXUSR) && (fileMode & S_IEXEC)) {
      printf("%s%sx%s", BOLD, GREEN, RESET_FORMAT);
      isExec = 1;
    } else
      printf("-");
    /* Check group permissions */
    if ((fileMode & S_IRGRP) && (fileMode & S_IREAD))
      printf("%sr%s", YELLOW, RESET_FORMAT);
    else
      printf("-");
    if ((fileMode & S_IWGRP) && (fileMode & S_IWRITE))
      printf("%sw%s", RED, RESET_FORMAT);
    else
      printf("-");
    if ((fileMode & S_IXGRP) && (fileMode & S_IEXEC)) {
      printf("%sx%s", GREEN, RESET_FORMAT);
      isExec = 2;
    } else
      printf("-");
    /* check other user permissions */
    if ((fileMode & S_IROTH) && (fileMode & S_IREAD))
      printf("%sr%s", YELLOW, RESET_FORMAT);
    else
      printf("-");
    if ((fileMode & S_IWOTH) && (fileMode & S_IWRITE))
      printf("%sw%s",  RED, RESET_FORMAT);
    else
      printf("-");
    if ((fileMode & S_IXOTH) && (fileMode & S_IEXEC)) {
      /* because this is the last permission, leave 3 blank spaces after print */
      printf("%sx%s   ", GREEN, RESET_FORMAT);
      isExec = 3;
    } else {
      printf("-   ");
    }

    return isExec;
} // }}}

char* getFileOwnerU(const char *path) { // {{{
    struct stat info;   
    stat(path, &info);

    struct passwd *pw = getpwuid(info.st_uid);
    
    if (pw != 0) {
	char* userName = pw->pw_name;
	if (strcmp(userName, getenv("USER")) == 0) {
	    char* retUserName = strdup(GREEN);
	    strcat(retUserName, userName);
	    strcat(retUserName, RESET_FORMAT);
	    return retUserName;
	}
	if (strcmp(userName, "root") == 0) {
	    char* retUserName = strdup(RED);
	    strcat(retUserName, userName);
	    strcat(retUserName, RESET_FORMAT);
	    return retUserName;
	}
	return userName;
    }

    return "\x1b[31mERROR\x1b[0m";
} // }}}

char* getFileOwnerG(const char *path) { // {{{
    struct stat info;   
    stat(path, &info);

    struct group *gr = getgrgid(info.st_gid);
    
    if (gr != 0) { return gr->gr_name; }

    return "\x1b[31mERROR\x1b[0m";
} // }}}
