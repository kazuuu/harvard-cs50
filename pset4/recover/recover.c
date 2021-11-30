#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <cs50.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{

    // Ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: recover forensic_image\n");
        return 1;
    }

    // Remember filenames
    char *infile = argv[1];

    // Open input file
    FILE *inptr = fopen(infile, "r");

    // Output file image
    FILE *img;

    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 1;
    }

    const int BLOCK_SIZE = 512;

    BYTE buffer[BLOCK_SIZE + 1];

    int total_photos = 0;

    bool found = false;

    // Loop until the end of file
    while (fread(&buffer, sizeof(BYTE), BLOCK_SIZE, inptr))
    {
        // check if it´s jpg header
        if (((buffer[0] == 0xff) && (buffer[1] == 0xd8) && (buffer[2] == 0xff)) && ((buffer[3] & 0xf0) == 0xe0))
        {
            // if it is, add 1 to the total of photos found
            total_photos++;

            char *filename = malloc(sizeof(char) * 7);
            sprintf(filename, "%03i.jpg", total_photos - 1);

            // If it´s not the first photo, close the file before.
            if (found)
            {
                fclose(img);
            }

            // create a new file with the number in the filename.
            img = fopen(filename, "w");

            if (img == NULL)
            {
                fclose(inptr);
                fprintf(stderr, "Could not create %s.\n", filename);
                return 1;
            }

            fwrite(&buffer,  sizeof(BYTE), BLOCK_SIZE, img);

            // Mark as found the first file to close the output file before open it again.
            found = true;
            free(filename);
        }
        else if (found)
        {
            fwrite(&buffer,  sizeof(BYTE), BLOCK_SIZE, img);
        }

    }

    fclose(inptr);
    // printf("Found %i photos! \n", total_photos);

}