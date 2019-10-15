#include "sifs-internal.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

// make a new directory within an existing volume

/*Directory-blocks have a fixed format. 
Each includes the name of the directory and its modification time; the number of entries in the directory (either subdirectories or files); 
and, for each entry, the block number (blockID) of each subdirectory or file.*/
int SIFS_mkdir(const char *volumename, const char *pathname)
{
    //ENSURE THAT RECEIVED PARAMETERS ARE VALID
    if (volumename == NULL || pathname == NULL)
    {
        SIFS_errno = SIFS_ENOTYET;
        return 1;
    }

    //  ATTEMPT TO CREATE THE NEW VOLUME - OPEN FOR WRITING
    FILE *vol = fopen(volumename, "r+");

    if (vol == NULL)
    {
        SIFS_errno = SIFS_ENOVOL;
        return 1;
        
    }


    SIFS_VOLUME_HEADER header;             // we need to initialise a new structure
        fread(&header, sizeof header, 1, vol); // read the contents of just the header
    
    SIFS_BIT bitmap[header.nblocks];
    fread(bitmap, sizeof bitmap, 1, vol); // reads vol into bitmap
    for(int i = 1; i < bitmap[header.nblocks]; i++){    //go through each block assigning if its a u/d etc.  
        bitmap[i] = i;
        printf("bitmap index %d\n", i);

        //use fseek() later on?

    }
    if (SIFS_UNUSED > 0 )
    {
        //Check if MD5 is the same if it is break out of this 
        /*if (MD5_format == )
        {
            
        }
        */
        SIFS_DIRBLOCK	rootdir_block;
        fread(&rootdir_block, sizeof rootdir_block, 1, vol);

        
        [.nentries];

        SIFS_DIRBLOCK newdir_block;
        

        newdir_block.name[0] = '\0';
        newdir_block.modtime = time(NULL);
        newdir_block.nentries = 0; // later need to change foer each entry the block n ID of each subdir or file

        //change root entry and put the new directory in there
        //rootdir_block.nentries ++1 ;
        rootdir_block.nentries ++ ;
        
    }
    

    //pathname , check if a file is in root dir then check if a file is in the 2nd file etc. 
    
    //go through all blocks and assign them the char
    printf("hello world\n");
     
    
    fseek(vol, sizeof header, SEEK_SET); //move up position to start of bitmap
    fwrite(bitmap, sizeof bitmap, 1, vol); 
    fclose(vol);
    //  AND RETURN INDICATING SUCCESS
    return 0;
}
