#include <stdio.h>
#include <cs50.h>

int main(void)
{
    // TODO: Prompt for start size
    int startSize;
    do
    {
        startSize = get_int("Start Size: ");
    }
    while (startSize < 9);

    // TODO: Prompt for end size
    int endSize;
    do
    {
        endSize = get_int("End Size: ");
    }
    while (endSize < startSize);


    // Calculate number of years until we reach threshold
    int pop = startSize;
    int years = 0;

    while (pop < endSize)
    {
        int deaths = pop / 4;
        int births = pop / 3;
        pop = pop + births - deaths;
        years++;
    }

    // Print number of years
    printf("Years: %i\n", years);
}