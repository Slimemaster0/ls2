// vim:fileencoding=utf-8:foldmethod=marker
// Include libraries {{{
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// }}}

char* filler(size_t amount) {
    char* filler = malloc(sizeof(char) * amount + 1);
    for (int i = 0; i < amount; i++) {
	strcat(filler, " ");
    }
    return filler;
}
