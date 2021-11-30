#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Points assigned to each letter of the alphabet
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

int compute_score(string word);

int main(void)
{
    // Get input words from both players
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    // Score both words
    int score1 = compute_score(word1);
    int score2 = compute_score(word2);

    // Check if Player1 wins
    if (score1 > score2)
    {
        printf("Player 1 wins!\n");
    }
    // Check if Player2 wins
    else if (score2 > score1)
    {
        printf("Player 2 wins!\n");
    }
    // If no players wins, its a tie
    else
    {
        printf("Tie!\n");
    }
}


// Calculate the word Score
int compute_score(string word)
{
    // List of possible characters
    string ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int score = 0;

    // Check each letter of the word
    for (int i = 0; i < strlen(word); i++)
    {
        // Do a linear search on possible alphabets
        for (int i1 = 0; i1 < strlen(ALPHABET); i1++)
        {
            // Check if found
            if (toupper(word[i]) == toupper(ALPHABET[i1]))
            {
                // Add scores to sum.
                score = score + POINTS[i1];
            }
        }
    }

    return score;
}
