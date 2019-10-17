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
    int cnt; 

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
    fread(&rootdir_block, sizeof rootdir_block, 1, vol);

    rootdir_block.nentries = rootdir_block.nentries + 1;
     
    printf("rootdir_block.entries: %i\n", rootdir_block.nentries);

    // first unassigned block ID sent to rootdir nentries
    for (cnt = 0; cnt < header.nblocks; cnt++)
        if (bitmap[cnt] == SIFS_UNUSED) { //If block is u then create a new dir
            rootdir_block.nentries = cnt; //assign new directory entries ID
            printf("Bitmap ID (Latest): %i\n", cnt);
            break;
        }
    //Check if volume is full
    if (cnt == header.nblocks) {
        //set SISF_ERRNO to indicate no unused blocks in volume
        SIFS_errno = SIFS_ENOSPC;
        return 1;
    }
    //Volumename set to rootdir_block.name
    strcpy(rootdir_block.name, volumename);
    printf("DIRNAME:%s\n", rootdir_block.name);

    //Assign the block d for Directory
    bitmap[cnt] = SIFS_DIR;

    char		oneblock[header.blocksize];

    memset(&rootdir_block, 0, sizeof rootdir_block);	// cleared to all zeroes
    
    printf("CNT:%i\n", cnt);
    //rootdir_block.name = ; 
    //rootdir_block.modtime = time(NULL);
    //rootdir_blk.entries[] =  rootdir_blk.entries[number_block_entries];
    rootdir_block.nentries = cnt;

    memset(oneblock, 0, sizeof oneblock); //set oneblock to 0's
    //memcpy(oneblock, &rootdir_block, sizeof rootdir_block);
    
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
    SIFS_errno = SIFS_EOK;
    return 0;
}
