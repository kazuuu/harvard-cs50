#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

int main(void)
{
    // Asking text
    string text = get_string("Text: ");

    int iLetters = count_letters(text);
    int iWords = count_words(text);
    int iSentences = count_sentences(text);

    // Printing greeting
    printf("%i letter(s)\n", iLetters);
    printf("%i word(s)\n", iWords);
    printf("%i sentence(s)\n", iSentences);


    float L = ((float) iLetters / iWords) * 100.0;
    float S = ((float) iSentences / iWords) * 100.0;

    // Calculate Coleman-Liau index i
    float index = (0.0588 * L) - (0.296 * S) - 15.8;

    // Output message
    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index > 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %.0f\n", round(index));
    }
}


// Count Letters
int count_letters(string text)
{
    int sumLetters = 0;

    // check each letter
    for (int i = 0; i < strlen(text); i++)
    {
        // convert all letters to uppercase
        char c = toupper(text[i]);
        if ((c >= 65) && (c <= 90))
        {
            sumLetters++;
        }
    }

    return sumLetters;
}

// Count words
int count_words(string text)
{
    int sumWords = 0;
    bool newWord = true;

    // check each letter
    for (int i = 0; i < strlen(text); i++)
    {
        // convert all letters to uppercase
        char c = toupper(text[i]);

        if (((c >= 65) && (c <= 90)) && (newWord))
        {
            sumWords++;
            newWord = false;
        }
        else if ((newWord == false) && (c == 32))
        {
            newWord = true;
        }
    }

    return sumWords;
}


// Count Sentences
int count_sentences(string text)
{
    int sumSentences = 0;
    bool newSentence = true;


    // check each letter
    for (int i = 0; i < strlen(text); i++)
    {
        // convert all letters to uppercase
        char c = toupper(text[i]);

        if (((c >= 65) && (c <= 90)) && (newSentence))
        {
            sumSentences++;
            newSentence = false;
        }
        else if ((newSentence == false) && ((c == 46) || (c == 33) || (c == 63)))
        {
            newSentence = true;
        }
    }

    return sumSentences;
}