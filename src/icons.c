#include <string.h>
#include <strings.h>

char* specialFileNames(char *fName, char isExec) {
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
}

char* fTypeIcon(char *fName, char isExec) {
    size_t len = strlen(fName);

	
    if (fName[len -1] == '.') { // why?
    } else if (fName[len -2] == '.') { // single character file extension
	switch (fName[len -1]) {
	    case 'c': return " "; break;
	    case 'h': return " ";
	}

    } else if (fName[len -3] == '.') { // duble character file extension
	switch (fName[len -2]) {
	    case 'j': {
		switch (fName[len -1]) {
		    case 's': return " "; break;
		}
	    } break;
	    
	    case 'r': {
		switch (fName[len -1]) {
		    case 's': return " "; break;
		} 

	    } break;
	    case 's': {
		switch (fName[len -1]) {
		    case 'h': return " "; break;
		}
	    }
	    case 'p': {
		switch (fName[len -1]) {
		    case 'y': {
			return " ";
		    }
		}
	    }
	} 
    } else if (fName[len -4] == '.') { // Triple character file extension
    
    } else if (fName[len -5] == '.') { // Quadriple character file extension
	switch (fName[len -4]) {
	    case 'j': {
		switch (fName[len -3]) {
		    case 's': {
			switch (fName[len -2]) {
			    case 'o': {
				switch (fName[len -1]) {
				    case 'n': return "{}"; break;
				} 
			    } break; // jso
			}
		    } break; // js
		}
	    } break; // j
	}
    }

    return specialFileNames(fName, isExec);
}
