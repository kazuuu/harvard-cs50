#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int validateKey(string key);

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    string KEY = argv[1];

    if (validateKey(KEY) > 0)
    {
        return 1;
    }

    string plain = get_string("plaintext: ");

    printf("ciphertext: ");

    for (int i = 0; i < strlen(plain); i++)
    {
        // If Upper Case Letters
        if ((plain[i] >= 65) && (plain[i] <= 90))
        {
            printf("%c", toupper(KEY[plain[i] - 65]));
        }
        // If Lower Case Letters
        else if ((plain[i] >= 97) && (plain[i] <= 122))
        {
            printf("%c", tolower(KEY[plain[i] - 65 - 32]));
        }
        else
        {
            printf("%c", plain[i]);
        }
    }

    printf("\n");
    return 0;
}

int validateKey(string key)
{
    // Check key lenght
    if (strlen(key) != 26)
    {
        // Invalid lenght
        printf("Key must contain 26 characters.\n");
        return 1;
    }

    // Temporary array to check if there are all letters
    char map[27] = "**************************";

    // Add char to Map and Check if is invalid char
    for (int i = 0; i < 26; i++)
    {
        char c = toupper(key[i]);

        if ((c < 65) || (c > 90))
        {
            printf("Key must contain only letters.\n");
            return 1;
        }

        map[(int) c - 65] = c;
    }

    // Check if there are all letters in key
    for (int i = 0; i < 26; i++)
    {
        if (map[i] == 42)
        {
            printf("Key letters is missing.\n");
            return 1;
        }
    }

    // Key its Ok
    return 0;
}
