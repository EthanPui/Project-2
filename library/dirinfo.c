#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#include "sifs-internal.h"

// get information about a requested directory
int SIFS_dirinfo(const char *volumename, const char *pathname,char ***entrynames, uint32_t *nentries, time_t *modtime)
{
    char **entries = NULL;
    if(volumename != NULL || pathname != NULL || nentries != 0) {
        SIFS_errno  = SIFS_ENOTYET;
        return 1;
    }
    for(int i = 0 ; i < *nentries; ++i) {
        entries[i] = malloc(SIFS_MAX_ENTRIES * sizeof(char));
        printf("%s\n", entries[i]);
    }
    *entrynames = entries;
    printf("modified %s\n", ctime(modtime));
    return 0;
}

