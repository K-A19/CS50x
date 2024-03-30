// Implements a dictionary's functionality

#include <stdbool.h>
#include <strings.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 676;

// Hash table
node *table[N];

//Number of words from dictionary stored in memory
int dict_words = 0;

//Helps other functions determine if the dictionary was properly loaded
bool loaded = false;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    //Gets the hash to find the linked list the word would be in, creates a pointer to head of that list
    int pointer = hash(word);
    node *trav = table[pointer];
    
    if (trav == NULL)
    {
        return false;
    } 
    
    while (strcasecmp(word, trav->word) != 0)
    {
        if (trav == NULL)
        {
            return false;
        }
        
        trav = trav->next;
    }
    
    return true;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    //Makes a copy of word that is in lowercase
    char *copy = malloc(strlen(word) + 1);
    strcpy(copy, word);
    
    for (int a = 0, b = strlen(copy); a < b; a++)
    {
        copy[a] = tolower(copy[a]);
    }
    
    int hash = 0;
    
    //Multiplies each character by 31 after subtracting 92 from them
    for (int i = 0, n = strlen(word); i < n; i++)
    {
        char c = copy[i];
        c -= 97;
        c *= 31;
        c = abs(c);
        hash += c;
    }
    
    free(copy); 
    
    //Divides by 676 to make sure it is a valid indes in the array
    return (hash % 676);
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    //Makes sure all the pointers in the hashtable are initially set to NULL
    for (int i = 0; i < N; i++)
    {
        table[i] = NULL;
    }
    
    //Opens fictionary and checks if it opened successfully
    FILE *dict = fopen(dictionary, "r");
    if (dict == NULL)
    {
        return false;
    }
    
    //Allocates memmory for a word buffer and creates a variable for insertion into hashtable
    char *buffer = malloc(LENGTH * sizeof(char));
    int index = 0;
    
    //Copys words (in the form of nodes) from the dictionary into the hashtable
    while (fscanf(dict, "%s", buffer) != EOF)
    {
        //Checks to make sure that a node is successfully malloced
        node *word_pointer = malloc(sizeof(node));
        if (word_pointer == NULL)
        {
            fclose(dict);
            return false;
        }
        
        //Copies the read string into the newly created node 
        strcpy(word_pointer->word, buffer);
        
        //Inserts node into hashtable through call of hash func and rearrangement of pointers
        strcpy(word_pointer->word, buffer);
        index = hash(word_pointer->word);
        word_pointer->next = table[index];
        table[index] = word_pointer;
        dict_words++;
    }
    
    free(buffer);
    loaded = true;
    fclose(dict);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    //If the dictionary was previously loaded it return the words counted during the load
    if (loaded) 
    {
        return dict_words; 
    }
    
    //If dictionary wasn't previously loaded then there is a default return of 0
    return 0;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    //Checks if the dictionary was able to actually load in the first place
    if (!loaded) 
    {
        return false;
    }
    
    //Iterates over each element in the array
    for (int z = 0; z < N; z++)
    {
        node *trav = table[z];
        node *tmp = table[z];
        
        //Iterates over each node in ech linked list and frees them
        while (trav != NULL)
        {
            trav = trav->next;
            free(tmp);
            tmp = trav;
        }
    }
    
    return true;
}
