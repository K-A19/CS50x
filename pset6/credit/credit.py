from cs50 import get_int
import re

numbers = []


def main():
    length = credit_card_number()

    if invalid(length):
        print("INVALID")
        return

    card_type(length)


def credit_card_number():

    card = get_int("Number: ")  # Gets card number from user
    copy = card

    length = 0
    while card != 0:
        digit = card % 10       # Finds the last digit
        numbers.append(digit)   # Adds last digit to the numbers list
        card -= digit           # Makes it easier to find the next digit
        card /= 10              # Changes the original digit by shifting it right once
        length += 1             # Adds  one to the length of the card number as digits are added to the card number list

    return length


def invalid(length):

    if length not in [13, 15, 16]:  # Causes the printing of invalid if the card number length is of any type
        return True
    total = 0

    for num in range(1, length, 2):                    # Iterates over every other digit in the card number starting with the second to last one
        n = numbers[num]
        n *= 2

        digits = []
        while n >= 1:                                  # Createes a list of the product individual digits
            dig = n % 10
            digits.append(dig)
            n -= dig
            n = int(n / 10)

        for i in range(len(digits)):                   # Adds each digit in the product to the whole card number total
            total += digits[i]

    for x in range(0, length, 2):                      # Adds the other digits which weren't multiplied to the whole card number total
        total += numbers[x]

    if total % 10 == 0:                                # Return true if the last digit of the total is 0 as per Luhn's Algorithm and false otherwise
        return False
    else:
        return True


def card_type(length):
    if length == 15:
        print("AMEX")
    elif length == 13:
        print("VISA")
    else:
        if numbers[length - 1] == 4:
            print("VISA")
        else:
            print("MASTERCARD")


main()