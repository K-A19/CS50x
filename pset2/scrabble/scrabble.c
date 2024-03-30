#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Points assigned to each letter of the alphabet
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

int compute_score(string word);
void winner_check(int w1, int w2);

int main(void)
{
    // Get input words from both players
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    // Score both words
    int score1 = compute_score(word1);
    int score2 = compute_score(word2);

    //Checks for the winner
    winner_check(score1, score2);
    
    return 0;
}

void winner_check(int w1, int w2)
{
    //Checks the case of score1 being greater than score2
    if (w1 > w2)
    {
        printf("Player 1 wins!\n");
    }
    
    //Checks the case of score2 being greater than score1
    else if (w1 < w2)
    {
        printf("Player 2 wins!\n");
    }
    
    // If both previous checks are false, assumtion of a tie
    else
    {
        printf("Tie!\n");
    }
}

int compute_score(string word)
{
    int total = 0;
    //Iterates through characters in the word used as input
    for (int i = 0,  n = strlen(word); i < n; i++)
    {
        //Checks if word is an actual letter
        if ((word[i] >= 'A' && word[i] <= 'Z') || (word[i] >= 'a' && word[i] <= 'z'))
        {
            //Turns uppercase letters to lowercase letters
            word[i] = tolower(word[i]);
            
            //Adds up the score
            total += POINTS[word[i] - 97];
        }
    }
    
    return total;
}