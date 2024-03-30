import csv
import sys


def main():

    database = sys.argv[1]              # Sets database to the second command line argument
    sequence = sys.argv[2]              # sets the dna sequence to the third command line argument

    with open(sequence, "r") as s:
        dna = s.readline()              # Reads the first line of the sequence text file to get the dna sequence

    fields = []
    with open(database, "r") as db:
        reader = csv.reader(db)
        fields = next(reader)           # Opens the database in order to get the strs to check for from the header
        del fields[0]

    dna_count = strand(dna, database, fields)
    match = compare(dna_count, database, fields)
    print(match)


def strand(dna, database, fields):      # Gets the short tandem repeats (str) of the given dna sequence and returns a dictionary of it

    dna_count = {}
    for repeat in range(len(fields)):
        field = fields[repeat]
        length = len(field)
        repeats = [0]
        slices = [""] * length
        for sub in range(len(dna) - length,):
            if dna[sub:(sub + length)] == field:        # Checks if the current field is equal to the str currently being checked for
                if slices[-length] != field:            # If the past str was not equal as well, then a zero is added to the back of the repeats list
                    repeats.append(0)
                repeats[-1] += 1                        # It adds 1 to the repeats list element to keep track of str

            slices.append(dna[sub:(sub + length)])      # Adds to the slice list to be able to easily check past slices

        repeats = sorted(repeats, reverse=True)         # Sorts the repeats list from largest to find the largest str
        dna_count[field] = repeats[0]                   # Adds the largest str to the dna's str count list to be used for comparison

    return dna_count                                    # Returns the dna's count list as a dictionary


def compare(dna_count, database, fields):

    people = []
    with open(database, "r") as db:
        reader = csv.DictReader(db)                 # Gets each person's dna strs as a dictionary
        for row in reader:
            people.append(row)                      # Adds each person's dictionary to a people list

    for person in people:
        similarity = 0
        for STR in fields:
            individual = str(person[STR])
            strand = str(dna_count[STR])
            if individual == strand:                # Checks if the persons dna strs is equal to the given dna's strs
                similarity += 1

        if similarity == len(dna_count):            # If the current person has exactly the same strs ad the dna sequence their name is printed
            return person["name"]

    return "No match"                               # Print 'No match' if no one's dna strs is equal to the given dna's strs


main()