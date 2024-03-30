//Header files needed
#include <stdio.h>
#include <math.h>
#include <cs50.h>
#include <ctype.h>
#include <string.h> 

//Prototypes
int num_of_words(string s);
int num_of_letters(string s);
int num_of_sentences(string s);
void reading_level(int x, int y, int z);

int main(void)
{
    //Getting input of text, which will be checked for its reading level, from user
    string text = get_string("Text: ");
    
    //Initializing variables need for the Coleman-Liau index
    int letters = num_of_letters(text);
    int words = num_of_words(text);
    int sentences = num_of_sentences(text);
    
    //Calculation of reading level and printing it to screen
    reading_level(letters, words, sentences);
    
    //Shows sucessful exit
    return 0;
}

int num_of_letters(string s)
{
    int letters = 0;
    
    for (int i = 0, n = strlen(s); i < n; i++)
    {
        //Checks if character is a letter
        if (isalpha(s[i]))
        {
            letters++;
        }
    }
    
    return letters; 
}

int num_of_words(string s)
{
    int words = 1;
    
    for (int i = 0, n = strlen(s); i < n; i++)
    {
        //Checks if character is a whitespace in order to determine a word
        if (isspace(s[i]))
        {
            words++;
        }
    }
    
    return words; 
}

int num_of_sentences(string s)
{
    int sentences = 0;
    
    for (int i = 0, n = strlen(s); i < n; i++)
    {
        //Checks if character is '.', '?' or '!'
        if (s[i] == '.' || s[i] == '!' || s[i] == '?')
        {
            sentences++;
        }
    }
    
    return sentences; 
}

void reading_level(int x, int y, int z)
{
    //Coleman-Liau index
    double L = (x / (float) y) * 100;
    double S = (z / (float) y) * 100;
    double index = 0.0588 * L - 0.296 * S - 15.8;
    
    int level = round(index);
    
    //Checks the range the level lies in
    if (level > 1 && level < 16)
    {
        printf("Grade %i\n", level);
    }
    
    else if (level >= 16)
    {
        printf("Grade 16+\n");
    }
    
    else  
    {
        printf("Before Grade 1\n");
    }
}