#include <stdio.h>
#include <cs50.h>
#include <math.h>

//Prototypes
long creditnum();
int creditlength(long num);
int numfind(int n, long num);
bool checksum(long num, int len);
void cardtype(int len, long num);

//Actual functions used
int main(void)
{
    long credit_num = creditnum();
    int length = creditlength(credit_num);
    cardtype(length, credit_num);
}

//Gets credit card number from user
long creditnum()
{
    long num;
    do
    {
        num = get_long("Number: ");
    }
    while (num < 0);
    return num;
}

//Finds the length of the credit card number
int creditlength(long num)
{
    int length = 0;
    while (num != 0)
    {
        num /= 10;
        length++;
    }
    return length;
}

//Finds the number in a specific place
int numfind(int n, long num)
{
    int number;
    
    if (n != 0)
    {
        long multi = pow(10, n);
        number = (num % multi) / (multi / 10);
    
    }
    
    else
    {
        number = num % 10;
    }
    
    return number;
}

//Checks if the credit card number is valid
bool checksum(long num, int len)
{
    int invalid;
    int total = 0;
    int y;
    int x;
    int sx;
    int xl;
    for (int i = 2; i <= len; i += 2)
    {
        x = numfind(i, num);
        x *= 2;

        xl = creditlength(x);
        
        for (int p = 1; p <= xl; p++)
        {
            sx = numfind(p, x);
            total += sx;
        }
        
    }
    
    
    for (int j = 1; j <= len; j += 2)
    {
        y = numfind(j, num);
        total += y;
    }
    
    //Veryfies whether if follows Luhn's algo or not
    if ((total % 10) == 0) // follows
    {
        invalid = false;
    }
    
    else
    {
        invalid = true; // doesn't follow
    }
    
    return invalid;
}

//Checks credit card type
void cardtype(int len, long num)
{
    int l = len;
    int first = numfind(len, num);
    int second = numfind(len - 1, num);
    
    //Outputs whether the card is valid or not
    if (checksum(num, len)) // nested function
    {
        printf("INVALID\n");
    }
    
    else if ((len == 15) && ((first == 3) && (second == 4 || second == 7)))
    {
        printf("AMEX\n");
    }
    
    else if ((len == 16) && ((first == 5) && (second == 1 || second == 2 || second == 3 || second == 4 || second == 5)))
    {
        printf("MASTERCARD\n");
    }
    
    else if ((len == 13 || len == 16) && (first == 4))
    {
        printf("VISA\n");
    }
    
    else
    {
        printf("INVALID\n");
    }
}