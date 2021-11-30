#include <cs50.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    int height;

    // Ask the height
    do
    {
        height = get_int("Height : ");
    }
    while ((height < 1) || (height > 8));



    // build line
    for (int i = 0; i < height; i++)
    {
        for (int col = 0; col < ((height * 2) + 2); col++)
        {

            // if is the first column
            if (col < height)
            {
                if (col < height - i - 1)
                {
                    printf(" ");
                }
                else
                {
                    printf("#");
                }
            }
            // if is the second column
            else if (col == height)
            {
                printf("  ");
            }
            else
            {
                if (col < height + 2 + i)
                {
                    printf("#");
                }
            }
        }

        // Go to next line
        printf("\n");
    }
}