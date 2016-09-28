
/************************************************************************
 *
 * Purpose: Program to demonstrate the 'statfs' function.
 *          The program will show the number of free blocks
 *	    on the /tmp filesystem.
 *
 * Author:  M J Leslie
 *
 * Date:    09-Nov-97 MJL Inital Release
 *
 * Notes:   This function is only available on older unix systems.
 *          SVR4 systems replaced statfs with statvfs.
 *
 ************************************************************************/

#include <sys/types.h>
#include <sys/vfs.h>

#define UNDEF  -1
#define  OK     0
#define FAIL    1

void GetFileSystemStat(
    const char  *Path,                 /* I    Path to a file on the file system to be queried */
    long        *BSize,                /* O    Size of blocks on this file system */
    long        *Blocks,               /* O    Total no. of blocks on this file system */
    long        *BFree,                /* O    No. of blocks still available to an ordinary user */
    long        *Files,                /* O    Maximum no. of files possible on this file system */
    long        *FFree,                /* O    No. of free file nodes (ie max - currently allocated) */
    int         *Status);              /* O    Returned status of this function call:
                                          OK       File system parameters found
                                          FAIL     Unable to determine file system
                                                   parameters */

main()
{
    long        BSize;
    long        Blocks;
    long        BFree;
    long        Files;
    long        FFree;
    int         Status;

    GetFileSystemStat("/tmp",          /* I    Path to a file on the file system to be queried */
                      &BSize,          /* O    Size of blocks on this file system */
                      &Blocks,         /* O    Total no. of blocks on this file system */
                      &BFree,          /* O    No. of blocks still available to an ordinary user */
                      &Files,          /* O    Maximum no. of files possible on this file system */
                      &FFree,          /* O    No. of free file nodes (ie max - currently allocated) */
                      &Status);        /* O    REturned status of this function call:
                                                   OK     File system parameters found
                                                   FAIL */

    printf("Free Blocks = %d\n", BFree);

}


/* PUBLIC FUNCTION
 ********************************************************************************
 **
 **     mos_GetFileSystemStat
 **
 ** DESCRIPTION
 **
 **     Gets information on the file-system on which a particular file resides
 **
 ** IMPLEMENTATION NOTES
 **
 **     On some file systems (eg NFS mounted partitions) it may not be possible
 **     to determine all the required parameters.  In these cases the returned
 **     value will be set to MOS_UNDEF.
 **
 ********************************************************************************
*/

void GetFileSystemStat(
    const char  *Path,          /* I    Path to a file on the file system to be queried */
    long         *BSize,         /* O    Size of blocks on this file system */
    long         *Blocks,        /* O    Total no. of blocks on this file system */
    long         *BFree,         /* O    No. of blocks still available to an ordinary user */
    long         *Files,         /* O    Maximum no. of files possible on this file system */
    long         *FFree,         /* O    No. of free file nodes (ie max - currently allocated) */
    int         *Status)        /* O    returned status of this function call:
                                           OK       File system parameters found
                                           FAIL     Unable to determine file system
                                                    parameters */
 
{
    struct statfs FSBuf;        /* structure to contain file system information */

    int           locStatus;      /* local status value */
 
    locStatus = statfs(Path, &FSBuf);

    if (locStatus != 0)
    {
        *BSize  = UNDEF;
        *Blocks = UNDEF;
        *BFree  = UNDEF;
        *Files  = UNDEF;
        *FFree  = UNDEF;

        *Status = FAIL;
    }
    else
    {
        *BSize  = FSBuf.f_bsize;
        *Blocks = FSBuf.f_blocks;
        *BFree  = FSBuf.f_bavail;
        *Files  = FSBuf.f_files;
        *FFree  = FSBuf.f_ffree;

        *Status = OK;
    }
}
