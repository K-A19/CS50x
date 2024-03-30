from cs50 import get_string
import re


def main():

    text = get_string("Text: ")
    details = detail(text)
    calculation(details)


def detail(text):
    details = [0, 1, 0]
    for char in text:
        if char.isalpha():                # If the current character is a letter it adds to the number of letters of the text
            details[0] += 1

        elif char.isspace():             # If the current character is a space it adds to the number of words for the text
            details[1] += 1

        elif char in ["?", ".", "!"]:     # If the current character is punctuation indicating the end of a sentence, the number of sentences
            details[2] += 1                     # of the text is increased by one

    return details


def calculation(details):

    letters = details[0]                        # Initializes letters with its corespondant in details
    words = details[1]                          # Initializes words with its correspondant in details
    sentences = details[2]                      # Initializes sentences with its correspondant in details

    L = (letters / words) * 100                 # Finds the number of letters per 100 words in the text
    S = (sentences / words) * 100               # Finds the number of sentences per 100 words in the text

    grade = 0.0588 * L - 0.296 * S - 15.8       # Performs the Coleman-Liau index
    grade = round(grade)

    if grade < 1:
        print("Before Grade 1")

    elif grade >= 16:
        print("Grade 16+")

    else:
        print(f"Grade {grade}")


main()