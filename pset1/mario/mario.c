#include <stdio.h>
#include <cs50.h>

int main(void)
{
    // Getting height from the user
    int height = 0;
    do
    {
        height = get_int("Height: ");
    }
    while (height < 1 || 8 < height);

    //Printing pyramid specified by height inputted
    for (int i = 1; i <= height; i++)
    {
        //Prints first half orf the pyramid
        for (int s = 0; s < height - i; s++) 
        {
            printf(" ");
        }
        for (int j = 0; j < i; j++)
        {
            printf("#");
        }

        //Prints the space between the two pyramid halves
        printf("  ");

        //Prints second pyramid half
        for (int k = 0; k < i; k++)
        {
            printf("#");
        }
        
        //Prints them in rows
        printf("\n");
    }
}