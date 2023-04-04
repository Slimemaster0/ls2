// vim:fileencoding=utf-8:foldmethod=marker
// {{{ Include libraries
#include <string.h>
#include <strings.h>
#include "format.h"
// }}}

char* specialFileNames(char *fName, char isExec) { // {{{ char* specialFileNames
    if (strcasecmp(fName, "LICENSE*") == 0) {
	return " ";
    }



    if (
	    strcasecmp(fName, "cmakelists.txt") == 0 ||
	    strcasecmp(fName, "Makefile") == 0 ||
	    isExec != 0) {

	return " ";
    }

    if (fName[0] == '.') {
	return "﬒ ";
    } else
    return " ";
} // }}}

char* fTypeIcon(char *fName, char isExec) { // char* fTypeIcon {{{
    size_t len = strlen(fName);

	
    if (fName[len -1] == '.') { // {{{ why?
    } else if (fName[len -2] == '.') { // single character file extension
	switch (fName[len -1]) {
	    case 'c': return " "; break;
	    case 'h': return " ";
	} // }}}
    } else if (fName[len -3] == '.') { // {{{ duble character file extension
	switch (fName[len -2]) {
	    case 'j': {
		switch (fName[len -1]) {
		    case 's': {
				  char* javaScript = strdup(BOLD);
				  strcat(javaScript, "JS");
				  strcat(javaScript, RESET_FORMAT);
				  return javaScript;
			      }; break;
		}
	    } break;

	    case 'p': {
		switch (fName[len -1]) {
		    case 'y': {
			return " ";
		    }
		}
	    }
	    
	    case 'r': {
		switch (fName[len -1]) {
		    case 's': return " "; break;
		} 

	    } break;
	    case 's': {
		switch (fName[len -1]) {
		    case 'h': return " "; break;
		}
	    } // s

	    case 't': {
		switch (fName[len -1]) {
		    case 's': {
				  char* typeScript = strdup(BOLD);
				  strcat(typeScript, "TS");
				  strcat(typeScript, RESET_FORMAT);
				  return typeScript;
			      }; break;
		}
	    } // t
	} // }}}
    } else if (fName[len -4] == '.') { // {{{ Triple character file extension
	switch (fName[len -3]) {
	    case 'r': {
		switch (fName[len -2]) {
		    case 'a': {
			switch (fName[len -1]) {
			    case 'r': {
				return " ";
			    } break;
			}
		    } break;
		}
	    } break;
	    case 'v': {
		switch (fName[len -2]) {
		    case 'p': {
			switch (fName[len -2]) {
			    case 'k': {
				return " ";
			    } break;
			}
		    } break;
		}
	    } break;
	    case 'z': {
		switch (fName[len -2]) {
		    case 's': {
			switch (fName[len -1]) {
			    case 'h': {
				return " ";
			    } break;
			}
		    } break;
		    case 'i': {
			switch (fName[len -1]) {
			    case 'p': {
				return " ";
			    } break;
			}
		    } break;
		}
	    } break;
	} // }}} 
    } else if (fName[len -5] == '.') { // {{{ Quadriple character file extension
	switch (fName[len -4]) {
	    case 'j': {
		switch (fName[len -3]) {
		    case 's': {
			switch (fName[len -2]) {
			    case 'o': {
				switch (fName[len -1]) {
				    case 'n': {
						  char* json = strdup(BOLD);
						  strcat(json, "{}");
						  strcat(json, RESET_FORMAT);
						  return json;
					      } break;
				} 
			    } break; // jso
			}
		    } break; // js
		}
	    } break; // j
	}
    } // }}}

    return specialFileNames(fName, isExec);
} // }}}
