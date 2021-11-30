// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of nodes in hash table
unsigned int nodeCount = 0;

// Number of buckets in hash table
const unsigned int N = 5381;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // Convert word to UpperCase
    // char *checkWord = malloc(sizeof(char) * LENGTH);
    // strcpy(checkWord, word);
    // for (int i = 0; i < (LENGTH + 1); i++) {
    //     checkWord[i] = toupper((char) checkWord[i]);
    // }

    unsigned int index = hash(word);

    node *checkNode = table[index];

    // printf("check 1: %i - %s \n", index, word);

    while (checkNode != NULL)
    {
        // printf("check 2: %s = %s\n", checkNode->word, word);

        if (strcasecmp(checkNode->word, word) == 0)
        {
            // printf("check true: %i \n", index);
            return true;
        }

        checkNode = checkNode->next;
    }

    // printf("check 3: false %i\n", index);
    return false;
}

// Hashes word to a number 0 to (N-1)
unsigned int hash(const char *word)
{

    unsigned int index = 5381;

    for (int i = 0; i < strlen(word); i++)
    {
        // index = ((index << 5) + index) + word[i]; /* hash * 33 + c */
        index = ((index << 5) + index) + toupper((unsigned char) word[i]); /* hash * 33 + c */
    }

    if (index >= N)
    {
        index = index % N;
    }

    return index;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", dictionary);
        return false;
    }

    char w[LENGTH + 1];

    while (fscanf(file, "%s", w) != EOF)
    {
        node *newNode = malloc(sizeof(node));

        // Convert Word to uppercase and Copy to node.
        // for (int i = 0; i < (LENGTH + 1); i++) {
        //     newNode->word[i] = toupper((unsigned char) w[i]);
        // }

        // Copy w uppercase to newNode
        strcpy(newNode->word, w);

        newNode->next = NULL;

        unsigned int h = hash(w);

        // printf("load hash: %i\n", h);

        if (table[h] == NULL)
        {
            // printf("load: %i \n", h);
            table[h] = newNode;
        }
        else
        {
            newNode->next = table[h]->next;
            table[h]->next = newNode;
        }

        // count + 1 in the total of nodes.
        nodeCount++;
    }

    fclose(file);

    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return nodeCount;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    node *unloadNode = NULL;
    node *cursor = NULL;

    for (int i = 0; i < N; i++)
    {
        // printf("unload 1: %i \n", i);

        if (table[i] == NULL)
        {
            // printf("unload 2: %i \n", i);
            free(table[i]);
            continue;
        }

        unloadNode = table[i];

        do
        {
            // printf("unload while: %i - %s \n", i, unloadNode->word);
            if (unloadNode->next == NULL)
            {
                free(unloadNode);
                break;
            }

            cursor = unloadNode->next;

            free(unloadNode);

            unloadNode = cursor;

        }
        while (unloadNode != NULL);

    }

    return true;
}
