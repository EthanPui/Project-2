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
    SIFS_DIRBLOCK rootdir_blk;
    //fseek to parent directory
    fread(&rootdir_blk, sizeof(SIFS_DIRBLOCK), 1, vol);
    

    printf("ROOT_NEntry: %i\n", rootdir_blk.nentries);

    for(int i = 0; i < rootdir_blk.nentries; i++){
        printf("ROOT_Entry: %i", rootdir_blk.entries[i].blockID); //entry not put in command? 
        
    }
    printf("ROOT_Entry2: %i\n", rootdir_blk.entries[0].blockID);

    uint32_t new_dir_blk_id;
    int cnt; //count
    
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

    //fwrite(new_dir_blk_id, sizeof cnt, 1, vol); 

    //Assign the block d for Directory
    bitmap[cnt] = SIFS_DIR;

    printf("RootDir_Nentries1: %i\n", rootdir_blk.nentries);
    //rootdir_blk.entries[rootdir_blk.nentries] = new_dir_blk_id;
    rootdir_blk.nentries = new_dir_blk_id; //nentries is the entries ID 
    
    rootdir_blk.nentries++; //Increase count of nentries 
    printf("RootDir_Nentries2: %i\n", rootdir_blk.nentries);

    //output the whole name
    SIFS_DIRBLOCK new_dir_blk;
    //for(int i =0; i <)

    new_dir_blk.name[cnt] = *volumename; 
    new_dir_blk.modtime = time(NULL);
    //new_dir_blk.nentries = rootdir_blk.nentries + 1 ; 

    

    
    printf("New_DIR_Volumename: %c\n", new_dir_blk.name[cnt]);
    printf("DIR_MODTIME: %s\n", ctime(&new_dir_blk.modtime));
    //printf("New_DIR_modtime: %c\n", new_dir_blk);
    //printf("New_DIR_Nentries: %c\n", new_dir_blk);

    // Need to fwrite nentries, entries
    //then name, modtime and blockID

    /*for(int i = 1; i < header.nblocks; i++){    //go through each block assigning if its a u/d etc.  
        bitmap[i] = i;
        //printf("bitmap index %d\n", i);
        if (bitmap[i] > 0) 
        {
            var1 =i;
        }  
    } */
            /*SIFS_DIRBLOCK newdir_block;
            SIFS_DIRBLOCK rootdir_info;
            fread(&rootdir_info, sizeof rootdir_info, 1, vol); // read the contents of just the root_dir

            int rootdir_entries = rootdir_info.nentries; //number of rootdir_entries = entries in rootdir 
            rootdir_info.entries[rootdir_entries].blockID = var1; // var1 is the assigned BlockID

 
            newdir_block.name[i] = *volumename;
            newdir_block.modtime = time(NULL);
            newdir_block.nentries = newdir_block.nentries + 1 ; // later need to change foer each entry the block n ID of each subdir or file
            */
    char		oneblock[header.blocksize];
    //  WRITE ALL OF THE INITIALISED SECTIONS TO THE VOLUME
    fwrite(&header, sizeof header, 1, vol);
     
    fseek(vol, sizeof header, SEEK_SET); //move up position to start of bitmap
    fwrite(bitmap, sizeof bitmap, 1, vol); 

    fwrite(oneblock, sizeof oneblock, 1, vol);	// write rootdir
    memset(oneblock, 0, sizeof oneblock);	// reset to all zeroes
    for(int b=1 ; b< header.nblocks ; ++b) {
        fwrite(oneblock, sizeof oneblock, 1, vol);
    }

    fclose(vol);
    
    //  AND RETURN INDICATING SUCCESS
    return 0;
}
