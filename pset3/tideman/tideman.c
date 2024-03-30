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
bool loop(pair initial_pair, pair current_pair, int locked_pairs_count, pair locked_pairs[], int checks);

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
    for (int q = 0; q < candidate_count; q++)
    {
        if (strcmp(candidates[q], name) == 0)
        {
            ranks[rank] = q;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int x = 0; x < candidate_count; x++)
    {
        preferences[ranks[x]][ranks[x]] = 0; 
        
        for (int y = x + 1; y < candidate_count; y++)
        {
            preferences[ranks[x]][ranks[y]]++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    int spot = 0;
    pair_count = 0;
    
    for (int a = 0; a < candidate_count; a++)
    {
        for (int b = a + 1; b < candidate_count; b++)
        {
            if ((preferences[a][b] - preferences[b][a]) > 0)
            {
                pairs[spot].winner = a;
                pairs[spot].loser = b;
                spot++;
                pair_count++;
            }
            
            if ((preferences[a][b] - preferences[b][a]) < 0)
            {
                pairs[spot].winner = b;
                pairs[spot].loser = a;
                spot++;
                pair_count++;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    pair holder;
    int swaps = -1;
    while (swaps != 0)
    {
        swaps = 0;
        for (int k = 0; k < pair_count; k++)
        {
            int c = preferences[pairs[k].winner][pairs[k].loser] - preferences[pairs[k].loser][pairs[k].winner];
            int d = preferences[pairs[k + 1].winner][pairs[k + 1].loser] - preferences[pairs[k + 1].loser][pairs[k + 1].winner];
         
            if (c < d)
            {
                holder = pairs[k];
                pairs[k] = pairs[k + 1];
                pairs [k + 1] = holder;
                swaps++; 
            }
        }
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    pair locked_pairs[pair_count];
    int locked_pairs_count = 0;
    for (int i = 0; i < pair_count; i++)
    {
        int checks = 0;
        pair current_pair = pairs[i];
        pair initial_pair = pairs[i];
        if (loop(initial_pair, current_pair, locked_pairs_count, locked_pairs, checks))
        {
            locked[pairs[i].winner][pairs[i].loser] = false;
        }
        
        else
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
            locked_pairs[locked_pairs_count] = pairs[i]; 
            locked_pairs_count++; 
        }
        
    }
    
    return;
}

// Print the winner of the election
void print_winner(void)
{
    //Checks whether the candidate loses to someone
    int lost;
    for (int i = 0; i < candidate_count; i++)
    {
        lost = 0;
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i] == true)
            {
                lost++;
            }
        }
        
        //If the candidate has lost to no one then it prints their name
        if (lost == 0) 
        {
            printf("%s\n", candidates[i]);
        }
    }
    return;
}

bool loop(pair initial_pair, pair current_pair, int locked_pairs_count, pair locked_pairs[], int checks)
{
    //Base Case
    if (locked[current_pair.winner][initial_pair.winner]) 
    {
        return true;
    }
    
    //Recursive case
    for (int s = 0; s < locked_pairs_count; s++) 
    {
        if (current_pair.loser == locked_pairs[s].winner) 
        {
            if (loop(initial_pair, locked_pairs[s], locked_pairs_count, locked_pairs, checks))
            {    
                return true;
            }    
            
            else
            {
                return false;
            }
        }
    }
    
    //Base case
    return false;

}

