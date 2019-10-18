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
    SIFS_BLOCKID blockID;
    

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
    for (int i = 0; i < header.nblocks; i++)
        //printf("%c ", bitmap[i]);
    //printf("\n");

    fread(&blockID, sizeof blockID,1,vol);
    
    //Root Directory 
    
    SIFS_DIRBLOCK rootdir_block;
    SIFS_DIRBLOCK parentdir_block;
    fread(&rootdir_block, sizeof rootdir_block, 1, vol);
    fread(&parentdir_block, sizeof parentdir_block, 1, vol);
    
        //printf("BlockID after : %i\n", blockID);

    //Check if volume is full
    if (blockID == header.nblocks) {
        //set SISF_ERRNO to indicate no unused blocks in volume
        SIFS_errno = SIFS_ENOSPC;
        return 1;
    }
    //Assign the block d for Directory

    //Pathname 
    //char path[sizeof(pathname)];
    //strcpy(path,pathname);
    //const char s[2] = "/";
    //char *token;

    /* get the first token */
    //token = strtok(path, s);

    /* walk through other tokens */
    /*while( token != NULL ) {
        printf( " %s\n", token );

        token = strtok(NULL, s);
    }*/
    //printf("The name of the second dir: %s\n", token);
    
    for (int i = 0; i < blockID; i++)
    {
        if(pathname[i] == '/'){
            fread(&parentdir_block, sizeof parentdir_block, 1, vol);
            strcpy(parentdir_block.name, pathname);
            printf("DIRNAME:%s\n", parentdir_block.name);
            parentdir_block.modtime = time(NULL);
        }
    }

    
    for (blockID = 0; blockID < header.nblocks; blockID++)
         if (bitmap[blockID] == SIFS_UNUSED) { //If block is u then create a new dir
             rootdir_block.nentries = blockID; //assign new directory entries ID
             bitmap[blockID] = SIFS_DIR;
             break;
         }

        printf("BlockID1:%i\n", blockID);
        if (blockID == 1)
        { 
            printf("blockID2 %i\n", blockID);
            rootdir_block.modtime = time(NULL);
            rootdir_block.entries[0].blockID = blockID;
            rootdir_block.nentries = blockID;

            fread(&parentdir_block, sizeof parentdir_block, 1, vol);
            parentdir_block.modtime = time(NULL);
            strcpy(parentdir_block.name, pathname);

        }else if(blockID > 1){
            for (int i = 0; i < header.nblocks; i++){
                if (bitmap[i+2] == SIFS_UNUSED) { 
                    printf("Parent block entries:%i\n", rootdir_block.entries[i].blockID);
                    printf("i1:%i\n", i);
                    printf("Parent block nentries:%i\n", rootdir_block.nentries);


                    rootdir_block.entries[parentdir_block.nentries].blockID = i;
                    //parentdir_block.entries[rootdir_block.nentries].blockID = i;
                    strcpy(parentdir_block.name, pathname);
                    printf("DIRNAME:%s\n", parentdir_block.name);
                    parentdir_block.modtime = time(NULL);
                    printf("i2:%i\n", i);
                    
                    rootdir_block.entries[i].blockID = (i + 1);
                    printf("Parent block entries2:%i\n", rootdir_block.entries[i].blockID);
                    //bitmap[i] = SIFS_DIR;
                    break;
                } 
            }
        }
            
        
    char		oneblock[header.blocksize];
    //memset(&rootdir_block, 0, sizeof rootdir_block);	// cleared to all zeroes
    memset(oneblock, 0, sizeof oneblock); //set oneblock to 0's
    memcpy(oneblock, &rootdir_block, sizeof rootdir_block);
    
    
    //  WRITE ALL OF THE INITIALISED SECTIONS TO THE VOLUME
    fseek(vol, sizeof header, SEEK_SET); //move up position to start of bitmap
    fwrite(bitmap, sizeof bitmap, 1, vol); //write bitmap onto vol

    fwrite(oneblock, sizeof oneblock, 1, vol);	// write rootdir to the disk or vol

    //write in the new_dirblock info
    //fseek(vol, sizeof (header.blocksize * blockID) ,SEEK_CUR);
    memcpy(oneblock, &parentdir_block, sizeof rootdir_block);
    fwrite(oneblock, sizeof oneblock, 1, vol);
    //memset(oneblock, 0, sizeof oneblock);	// reset to all zeroes 

//  FINISHED, CLOSE THE VOLUME
    fclose(vol);
    //  AND RETURN INDICATING SUCCESS
    SIFS_errno = SIFS_EOK;
    return 0;
}
