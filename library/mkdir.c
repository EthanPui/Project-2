#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "sifs-internal.h"

//  CITS2002 Project 2 2019
//  Name(s):             Ethan Pui (, Shuyan Zheng)
//  Student number(s):   22704879 (, 22429904)

// make a new directory within an existing volume

/*Directory-blocks have a fixed format. 
Each includes the name of the directory and its modification time; the number of entries in the directory (either subdirectories or files); 
and, for each entry, the block number (blockID) of each subdirectory or file.*/
int SIFS_mkdir(const char *volumename, const char *pathname)
{

    int number_block_entries = 0;
    int cnt; //count

    uint32_t new_dir_blk_id;

    SIFS_errno = SIFS_EOK;
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
    
    //int var1;

    SIFS_VOLUME_HEADER header;             // we need to initialise a new structure
    fread(&header, sizeof header, 1, vol); // read the contents of just the header
    printf("Header Blocksize:%zu\n", header.blocksize);
    printf("Header Nblocks %i\n", header.nblocks);

    SIFS_BIT bitmap[header.nblocks];
    fread(bitmap, sizeof bitmap, 1, vol); // reads vol into bitmap
    for (int i = 0; i < header.nblocks; i++)
        printf("%c ", bitmap[i]);
    printf("\n");


    //Root Directory 
    SIFS_DIRBLOCK rootdir_block;

    //fseek to parent directory
    fread(&rootdir_block, sizeof(SIFS_DIRBLOCK), 1, vol);
    

    for(int i = 0; i < rootdir_block.nentries; i++){
        printf("ROOT_Entry: %i", rootdir_block.entries[i].blockID); //entry not put in command? 
        number_block_entries ++; 
        
    }
    printf("ROOT_Entry2: %i\n", rootdir_block.entries[0].blockID);

    for (cnt = 0; cnt < header.nblocks; cnt++)
        if (bitmap[cnt] == SIFS_UNUSED) { //If block is u then create a new dir
            new_dir_blk_id = cnt; //assign new directory entries ID
            printf("Count2: %i\n", cnt);
            break;
        }
        printf("Count1: %i\n", cnt);

    if (cnt == header.nblocks) {
        //set SISF_ERRNO to indicate no unused blocks in volume
        SIFS_errno = SIFS_ENOSPC;
        return 1;
    }

    //Assign the block d for Directory
    bitmap[cnt] = SIFS_DIR;

    printf("RootDir_Nentries1: %i\n", rootdir_block.nentries);
    //rootdir_blk.entries[rootdir_blk.nentries] = new_dir_blk_id;
    rootdir_block.nentries = new_dir_blk_id; //nentries is the entries ID 
    
    rootdir_block.nentries++; //Increase count of nentries 
    printf("RootDir_Nentries2: %i\n", rootdir_block.nentries);

    char		oneblock[header.blocksize];

    memset(&rootdir_block, 0, sizeof rootdir_block);	// cleared to all zeroes
    
    rootdir_block.name[rootdir_block.nentries] = *volumename; 
    rootdir_block.modtime = time(NULL);
    //rootdir_blk.entries[] =  rootdir_blk.entries[number_block_entries];
    rootdir_block.nentries = 5;

    memset(oneblock, 0, sizeof oneblock); //set oneblock to 0's
    memcpy(oneblock, &rootdir_block, sizeof rootdir_block);
    

    printf("cnt: %i\n", cnt);
    
    //  WRITE ALL OF THE INITIALISED SECTIONS TO THE VOLUME
    fseek(vol, sizeof header, SEEK_SET); //move up position to start of bitmap
    fwrite(bitmap, sizeof bitmap, 1, vol); //write bitmap onto vol

    fwrite(oneblock, sizeof oneblock, 1, vol);	// write rootdir to the disk or vol
    memset(oneblock, 0, sizeof oneblock);	// reset to all zeroes 
    for(int b=1 ; b< header.nblocks ; ++b) {
        fwrite(oneblock, sizeof oneblock, 1, vol); //write to disk
    }
//  FINISHED, CLOSE THE VOLUME
    fclose(vol);
    
    //  AND RETURN INDICATING SUCCESS
    return 0;
}
