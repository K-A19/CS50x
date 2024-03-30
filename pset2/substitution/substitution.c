#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h> 
#include <stdlib.h>  

int key_length(string array[]);
bool repeated_letter(string argv[]);
bool valid(int argc, string argv[]);
void encryption(string argv[], string key);

int main(int argc, string argv[])
{
    string key = argv[1];
    
    //Stops program and outputs error message if valid is false
    if (!valid(argc, argv))
    {
        return 1;
    }
    
    encryption(argv, key);
    
    return 0;
}

//Checks the number of characters in the key that are actually aphabetical
int key_length(string array[])
{
    int length = 0;
    for (int i = 0, n = strlen(array[1]); i < n; i++)
    {
        if (isalpha(array[1][i]))
        {
            length++;
        }
    }
    
    return length;
}

//Checks to see if any character is repeated
bool repeated_letter(string argv[])
{
    const int length = strlen(argv[1]);
    string alphabet = "abcdefghijklmnopqrstuvwxyz";
    string ky = "";
    string k = malloc(strlen(ky) + 1);
    
    //Makes all characters in the key lowercase for easy comparison
    for (int i = 0; i < length; i++)
    {
        argv[1][i] = tolower(argv[1][i]);
    }
    
    ky = argv[1];
    
    //Checks for repitition or omission of a letter in the key
    char letter;
    for (int i = 0; i < length; i++)
    {
        strcpy(k, ky);
        
        letter = alphabet[i];
        
        string set1 = strchr(k, letter);
        
        //Case of the letter not being in the string at all
        if (set1 == 0)
        {
            return true;
        }
        
        //Case of the letter being in the string but repeated
        set1[0] = '.';
        string set2 = strchr(set1, letter);
        
        if (set2 != 0)
        {
            return true; 
        }
    }

    return false;
}

//Checks whether the key inputted is valid
bool valid(int argc, string argv[])
{
    //Checks to make sure not more than the key is given as input
    if (argc != 2)
    {
        printf("Usage: ./substitution KEY\n");
        return false;
    }
    
    int acckey_length = key_length(argv);
    int key_length = strlen(argv[1]);
    
    //Checks to see if the key is 26 characters long
    if (key_length != 26)
    {
        printf("Key must contain 26 alphabetical characters.\n");
        return false;
    }
    
    //Checks to see if there are only 26 characters that are letters
    if (key_length > acckey_length)
    {
        printf("Key must contain only alphabetical characters.\n");
        return false;
    }
    
    //Checks to see if character is repeated
    if (repeated_letter(argv))
    {
        printf("Key must not contain repeated characters.\n");
        return false;
    }
    
    //If key passes previous checks, it is returned as valid
    else
    {
        return true;
    }
}

//After key is deemed valid, does the encryption
void encryption(string argv[], string key)
{
    string plain_text = get_string("plaintext: ");
    
    printf("ciphertext: ");
    
    for (int i = 0, n = strlen(plain_text); i < n; i++)
    {
        //Checks if the character in question is a letter
        if (isalpha(plain_text[i]))
        {
            string alpha = "abcdefghijklmnopqrstuvwxyz";
            
            //Keeps case of letter depending on plaintext inputted
            if (islower(plain_text[i]))
            {
                for (int j = 0, k = strlen(alpha); j < k; j++)
                {
                    if (plain_text[i] == alpha[j])
                    {
                        printf("%c", tolower(key[j]));
                    }
                }
            }
            
            else
            {
                for (int j = 0, k = strlen(alpha); j < k; j++)
                {
                    if (tolower(plain_text[i]) == alpha[j])
                    {
                        printf("%c", toupper(key[j]));
                    }
                }
            }
        }
        
        //Leaves letter as is if it's not a letter
        else
        {
            printf("%c", plain_text[i]);
        }
    }
    
    printf("\n");
    
}