#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

void swap_pairs(int indexSource, int indexDestination);
int getAllLosers(int index);
int getLoser(int index, int th);
bool isCycleClosed(int indexOrigin, int indexTo);
void print_preferences(void);
void print_pairs(void);
void print_locked(void);
void setup(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();

    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // search for candidate in each item
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i], name) == 0)
        {
            // Set candidate´s index to ranking vote array.
            ranks[rank] = i;

            return true;
        }
    }

    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // loop through all head to head disputes possible int the respective ranking vote and compute a vote in the preferences array.
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]]++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            // check who is winner over other
            if ((i != j) && (preferences[i][j] > preferences[j][i]))
            {
                // add to the end of the array one more item
                // note that pair_count is using as index.. so do not need to add + 1 now.
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                // increase the number of existing pairs
                pair_count++;
            }
        }
    }

    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    int largestIndex;
    int largestVotes;

    // loop from each pair to check if its the largest item, and if not, swap to be.
    for (int i = 0; i < pair_count; i++)
    {
        largestIndex = i;
        largestVotes = preferences[pairs[i].winner][pairs[i].loser];
        bool toSwap = false;

        // loop to compare item i to others items to check if it is bigger
        for (int i1 = i + 1; i1 < pair_count; i1++)
        {
            if (preferences[pairs[i1].winner][pairs[i1].loser] > largestVotes)
            {
                largestIndex = i1;
                largestVotes = preferences[pairs[i1].winner][pairs[i1].loser];
                // Mark this variable to swap at the ende of this loop
                toSwap = true;
            }
        }

        // if swap with the largest number.
        if (toSwap)
        {
            swap_pairs(i, largestIndex);
            toSwap = false;
        }
    }

    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // loop all pairs to check if it can be locked
    for (int i = 0; i < pair_count; i++)
    {
        // check if there is an cycle closed
        if (!isCycleClosed(pairs[i].winner, pairs[i].loser))
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }

    return;
}

// Print the winner of the election
void print_winner(void)
{
    // Verify if the candidate has not lost and print it as winner.
    for (int i = 0; i < candidate_count ; i++)
    {
        bool hasLost = false;

        for (int i1 = 0; i1 < candidate_count ; i1++)
        {
            if (locked[i1][i] == true)
            {
                hasLost = true;
            };
        }
        if (!hasLost)
        {
            printf("%s\n", candidates[i]);
        }
    }

    return;
}

// Swap array pairs
void swap_pairs(int indexSource, int indexDestination)
{
    pair tempPair;

    // Save the destination pair to a temp variable
    tempPair = pairs[indexDestination];

    // overwrite the destination pair with the source variable.
    pairs[indexDestination] = pairs[indexSource];

    // put the temp variable with the old destination values to the source pair.
    pairs[indexSource] = tempPair;

    return;
}


// Check if is the last edge to not make a circle
bool isCycleClosed(int indexOrigin, int indexTo)
{
    for (int i = 0; i < pair_count; i++)
    {
        if ((pairs[i].winner == indexTo) && (locked[pairs[i].winner][pairs[i].loser]))
        {
            if (pairs[i].loser == indexOrigin)
            {
                return true;
            }
            else
            {
                if (isCycleClosed(indexOrigin, pairs[i].loser))
                {
                    return true;
                }
            }
        }
    }

    return false;
}

void print_preferences(void)
{
    printf("==== Preferences Table \n ");
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            printf("%i ", preferences[i][j]);
        }
        printf("\n");
    }
}

void print_pairs(void)
{
    printf("==== Pairs \n ");
    for (int i = 0; i < pair_count; i++)
    {
        printf("Pair %i: %i(%i) => %i(%i)\n", i, pairs[i].winner,
               preferences[pairs[i].winner][pairs[i].loser], pairs[i].loser,
               preferences[pairs[i].loser][pairs[i].winner]);
    }
}

void print_locked(void)
{
    printf("==== Locked \n");
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            printf("%i ", locked[i][j]);
        }
        printf("\n");
    }
}

void setup(void)
{
    candidate_count = 3;
    candidates[0] = "Alice";
    candidates[1] = "Bob";
    candidates[2] = "Charlie";

    int ranks[candidate_count];
    vote(0, "Alice", ranks);
    vote(1, "Bob", ranks);
    vote(2, "Charlie", ranks);
    record_preferences(ranks);

    vote(0, "Alice", ranks);
    vote(1, "Bob", ranks);
    vote(2, "Charlie", ranks);
    record_preferences(ranks);

    vote(0, "Alice", ranks);
    vote(1, "Bob", ranks);
    vote(2, "Charlie", ranks);
    record_preferences(ranks);

    vote(0, "Bob", ranks);
    vote(1, "Charlie", ranks);
    vote(2, "Alice", ranks);
    record_preferences(ranks);

    vote(0, "Bob", ranks);
    vote(1, "Charlie", ranks);
    vote(2, "Alice", ranks);
    record_preferences(ranks);

    vote(0, "Charlie", ranks);
    vote(1, "Alice", ranks);
    vote(2, "Bob", ranks);
    record_preferences(ranks);

    vote(0, "Charlie", ranks);
    vote(1, "Alice", ranks);
    vote(2, "Bob", ranks);
    record_preferences(ranks);

    vote(0, "Charlie", ranks);
    vote(1, "Alice", ranks);
    vote(2, "Bob", ranks);
    record_preferences(ranks);

    vote(0, "Charlie", ranks);
    vote(1, "Alice", ranks);
    vote(2, "Bob", ranks);
    record_preferences(ranks);
}