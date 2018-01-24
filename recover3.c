#include <stdio.h>
#include <stdlib.h>
#include "buffer.h"

int main(int argc, char* argv[])
{
    // Ensuring that the input arguments are correct
    if(argc != 2)
    {
        fprintf(stderr,"Usage: ./recover image \n");
        return 1;
    }
    
    // Saving the input memory card
    char* card = argv[1]; 
    
    // Opening the memory card to read 
    FILE* inptr = fopen(card,"r");
    
    //Ensuring that that the file actually opened
    if(inptr == NULL)
    {
        fprintf(stderr,"Could not open file %s. \n", card);
        return 2;
    }
    
    // Creating the 512 byte space that will be read each time
    buffer buffer[512];
    int x = 0;
    
    //Reading the file until end of file 
    char filename[10];
    sprintf(filename,"%03i.jpg",x);
    FILE *img = fopen(filename,"w");
    
    while(fread(buffer, 1, 512, inptr) == 512)
    {
        // Checking the 4 bytes of the jpeg and ensuring it meets the requirements
        if( buffer[0].byte == 0xff && buffer[1].byte == 0xd8 && buffer[2].byte == 0xff && (buffer[3].byte & 0xf0) == 0xe0)
        {
            if(x == 0)
            {
                fwrite(buffer, 1, 512, img);
                x++;
            }
            
            else
            {
                fclose(img);
                sprintf(filename,"%03i.jpg",x);
                fopen(filename,"w");
                fwrite(buffer, 1, 512, img);
                x++;
            }

        }
        
        else
        {
            if(x != 0)
            {
             fwrite(buffer, 1, 512, img);
            }
        }
    }
    
    fclose(img);
    fclose(inptr);
    return 0;
}