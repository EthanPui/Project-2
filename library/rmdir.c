#include "sifs-internal.h"

// remove an existing directory from an existing volume
int SIFS_rmdir(const char *volumename, const char *pathname)
{

    
    //SIFS_errno = SIFS_ENOTEMPTY;
    

    /*SIFS_DIRBLOCK **rootdir_block;
    fread(&rootdir_block, sizeof rootdir_block, 1, vol);
    

    rootdir_block.name[0]       = '\0';
    rootdir_block.modtime	= time(NULL);
    rootdir_block.nentries	= 0;
    rootdir_block.entries[0].blockID = 0; */

    SIFS_errno	= SIFS_ENOTYET;
    return 1;
}
