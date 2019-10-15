#include <stdio.h>
#include <stdlib.h>
#include "sifs.h"
void usage(char *progname)
{
    fprintf(stderr, "Usage: %s volumename pathname\n", progname);
    fprintf(stderr, "or     %s pathname\n", progname);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    char *volumename ;    // filename storing the SIFS volume
    char *pathname;

    if(argc == 3) {
	volumename	= argv[1];
	pathname		= argv[2];
    }
//  ... OR FROM A COMMAND-LINE PARAMETER
    else {
	usage(argv[0]);
	exit(EXIT_FAILURE);
    }

    if(SIFS_mkdir(volumename, pathname) != 0) {
	SIFS_perror(argv[0]);
	exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
