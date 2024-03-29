#include <stdio.h>
#include <stdlib.h>
#include "fat32.h"


int main(int argc, char* argv[])
{
    printf("process: %s\nargc: %d\n", argv[0], argc);

    for (int i = 1; i < argc; i++)
    {
        printf("\n================================================================================\n");
        printf("Volume: %s\n", argv[i]);
        printf("================================================================================\n\n");

        FILE* volume;
        fat32BS bootSector;
        FSInfo fsInfoSector;

        char *nextFAT;
        char first2FAT[2];
        int freeSectors;

        int validBootSector = 1;
        int fsiError = 0;

        // open .img file
        if ((volume = fopen(argv[i], "r")) == NULL)
        {
            printf("Error: unable to open volume file");
            exit(1);
        }

        // read Boot Sector
        fread(&bootSector, sizeof(fat32BS), 1, volume);

        // print bootSector info
        printf("BS_jmpBoot:   0x%X 0x%X 0x%X\n", bootSector.BS_jmpBoot[0] & 0xff, bootSector.BS_jmpBoot[1] & 0xff, bootSector.BS_jmpBoot[2] & 0xff);
        printf("BPB_FATSz32:  %d\n", bootSector.BPB_FATSz32);
        printf("BPB_RootClus: %d\n", bootSector.BPB_RootClus);
        printf("BPB_TotSec32: %d\n", bootSector.BPB_TotSec32);
        printf("BPB_reserved: %X %X %X %X %X %X %X %X %X %X %X %X\n\n", bootSector.BPB_reserved[0], bootSector.BPB_reserved[1], bootSector.BPB_reserved[2], bootSector.BPB_reserved[3], bootSector.BPB_reserved[4], bootSector.BPB_reserved[5], bootSector.BPB_reserved[6], bootSector.BPB_reserved[7], bootSector.BPB_reserved[8], bootSector.BPB_reserved[9], bootSector.BPB_reserved[10], bootSector.BPB_reserved[11]);

        // validate BS_jmpBoot
        if ((int)(bootSector.BS_jmpBoot[0] & 0xff) != 0xEB && (int)(bootSector.BS_jmpBoot[0] & 0xff) != 0xE9)
        {
            printf("Inconsistent file system: BS_jmpBoot[0] must be 0xEB or 0xE9, but is 0x%X.\n", bootSector.BS_jmpBoot[0]);
            validBootSector = 0;
        }

        // validate BPB_FATSz32
        if (bootSector.BPB_FATSz32 == 0)
        {
            printf("Inconsistent file system: BPB_FATSz32 should be non-zero, but is %d.\n", bootSector.BPB_FATSz32);
            validBootSector = 0;
        }

        // validate BPB_RootClus
        if (bootSector.BPB_RootClus < 2)
        {
            printf("Inconsistent file system: BPB_RootClus should greater than 2, but is %d.\n", bootSector.BPB_RootClus);
            validBootSector = 0;
        }

        // validate BPB_TotSec32
        if (bootSector.BPB_TotSec32 < 65525)
        {
            printf("Inconsistent file system: BPB_TotSec32 should be greater than 65525, but is %d.\n", bootSector.BPB_TotSec32);
            validBootSector = 0;
        }

        // validate BPB_reserved
        if (bootSector.BPB_reserved[0] != 0 || bootSector.BPB_reserved[1] != 0 || bootSector.BPB_reserved[2] != 0 || bootSector.BPB_reserved[3] != 0 || bootSector.BPB_reserved[4] != 0 || bootSector.BPB_reserved[5] != 0 || bootSector.BPB_reserved[6] != 0 || bootSector.BPB_reserved[7] != 0 || bootSector.BPB_reserved[8] != 0 || bootSector.BPB_reserved[9] != 0 || bootSector.BPB_reserved[10] != 0 || bootSector.BPB_reserved[11] != 0)
        {
            printf("Inconsistent file system: BPB_reserved should all be 0, but is %X %X %X %X %X %X %X %X %X %X %X %X\n", bootSector.BPB_reserved[0], bootSector.BPB_reserved[1], bootSector.BPB_reserved[2], bootSector.BPB_reserved[3], bootSector.BPB_reserved[4], bootSector.BPB_reserved[5], bootSector.BPB_reserved[6], bootSector.BPB_reserved[7], bootSector.BPB_reserved[8], bootSector.BPB_reserved[9], bootSector.BPB_reserved[10], bootSector.BPB_reserved[11]);
            validBootSector = 0;
        }

        // if all validations passed, then print success
        if (validBootSector)
        {
            printf("MBR appears to be consistent.\n");
        }

        printf("\n");

        // read File System Info Sector
        fseek(volume, (bootSector.BPB_FSInfo * bootSector.BPB_BytesPerSec), SEEK_SET);
        fread(&fsInfoSector, sizeof(FSInfo), 1, volume);

        // validate FSI_LeadSig
        if (fsInfoSector.lead_sig != FSI_LEAD_SIG)
        {
            printf("FSI Sector Error: FSI lead signature should be 0x%X, but is 0x%X\n", FSI_LEAD_SIG, fsInfoSector.lead_sig);
            fsiError = 1;
        }

        // validate FSI_StrucSig
        if (fsInfoSector.signature != FSI_STRUC_SIG)
        {
            printf("FSI Sector Error: FSI struc signature should be 0x%X, but is 0x%X\n", FSI_STRUC_SIG, fsInfoSector.signature);
            fsiError = 1;
        }

        if (fsiError)
            printf("\n");

        // read File Allocation Table
        fseek(volume, (bootSector.BPB_RsvdSecCnt * bootSector.BPB_BytesPerSec), SEEK_SET);
        fread(&first2FAT, sizeof(first2FAT), 1, volume);

        // print FAT info
        printf("FAT[0]: 0x%X\n", first2FAT[0]);
        printf("FAT[1]: 0x%X\n\n", first2FAT[1]);

        // validate FAT[0]
        if ((first2FAT[0] & EOC) != BPB_MEDIA)
        {
            printf("Inconsistent file system: FAT[0] should be 0x%08X, but is 0x%08X\n", BPB_MEDIA, (first2FAT[0] & EOC));
        }

        // validate FAT[1]
        if ((first2FAT[1] & EOC) != EOC)
        {
            printf("Inconsistent file system: FAT[1] should be %08X, but is 0x%08X\n", EOC, (first2FAT[1] & EOC));
        }

        // printf("num of bytes in FAT: %d\n", (bootSector.BPB_FATSz32 * bootSector.BPB_BytesPerSec));

        // count free clusters
        freeSectors = 0;
        for (int i = 2; i < (int)(bootSector.BPB_FATSz32 * bootSector.BPB_BytesPerSec); i++)
        {
            fread(&nextFAT, sizeof(nextFAT), 1, volume);
            if (nextFAT == 0)
            {
                freeSectors++;
            }
        }

        // print free space info
        printf("FSI says %d free clusters, FAT count is %d\n", fsInfoSector.free_count, freeSectors);

    }

    printf("\n\nEnd of Process.\n");
    return 0;
}
