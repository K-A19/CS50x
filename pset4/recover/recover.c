#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;
 
int main(int argc, char *argv[])
{
    //Makes sure there is only one imputted command line argument 
    if (argc != 2)
    {
        printf("Usage: ./recover forensic.raw\n");
        return 1;
    }
    
    FILE *forensic = fopen(argv[1], "r");
    
    //Checks if forensic image file is properly opened
    if (forensic == NULL)
    {
        printf("Foresnsic image was unable to be opened\n");
        fclose(forensic);
        return 1;
    }
    
    int picture_count = 0;
    char filename[8];
    BYTE buffer[512];
    FILE *picture = NULL;
    
    while (fread(buffer, sizeof(BYTE), 512, forensic))
    {
        //Checks if current block has a JPEG signature signifying a new picture.
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && ((buffer[3] & 0xf0) == 0xe0))
        { 
            sprintf(filename, "%03i.jpg", picture_count);
            picture = fopen(filename, "w");
            fwrite(buffer, sizeof(BYTE), 512, picture);
            fclose(picture);
            picture_count++;
            continue; 
        }
        
        //Continue adding to the current picture if the current block doesnt have a JPEG signature
        if (picture_count > 0) 
        {
            picture = fopen(filename, "a");
            fwrite(buffer, sizeof(BYTE), 512, picture); 
            fclose(picture);
            continue;
        }
    }
    
    fclose(forensic);
    return 0;
} 
