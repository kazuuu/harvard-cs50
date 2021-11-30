# Pset6: DNA

import csv
import sys
import random


def main():

    # Ensure correct usage
    if len(sys.argv) != 3:
        sys.exit("Usage: python dna.py DATABASE SEQUENCY")

    # Reads columns names as list of possibles STR
    list_str = []

    with open(sys.argv[1]) as csvfile:
        file = csv.reader(csvfile, delimiter=',')

        # loop to iterate thorugh the rows of csv
        for row in file:
            # adding the first row
            list_str.append(row)

            # breaking the loop after the
            # first iteration itself
            break
    csvfile.close()

    # Read database into memory from file
    database = []

    with open(sys.argv[1], newline='') as csvfile:
        file = csv.DictReader(csvfile)
        for line in file:
            database.append(line)
    csvfile.close()

    sequency = ""
    file = open(sys.argv[2], "r")
    sequency = file.read()

    # Count STR from sequency
    dna_count = []
    nucleotideDict = {}
    for i in range(1, len(list_str[0])):
        result = 0
        count = 0
        while (result != -1):
            result = sequency.find(list_str[0][i] * (count + 1))
            if (result != -1):
                count += 1
        nucleotideDict["str"] = list_str[0][i]
        nucleotideDict["count"] = count
        dna_count.append(nucleotideDict)
        # print("{0}: {1}".format(list_str[0][i], nucleotideDict[list_str[0][i]]))
        count = 0
        result = 0
        nucleotideDict = {}
    # for item in dna_count:
    #     print("{0}: {1}".format(item.get("str"), item.get("count")))

    # Check each person in database with the dna to find
    bFind = True
    bMatch = False
    for person in database:
        for item in dna_count:
            if (int(item.get("count")) != int(person.get(item.get("str")))):
                # print("{0} = {1}".format(item.get("count"), person.get(item.get("str"))))
                bFind = False
        if (bFind):
            print(person.get("name"))
            bMatch = True
            # print("{0}: {1}".format(item.get("str"), person.get(item.get("str"))))
        bFind = True

    if (not bMatch):
        print("No match")


if __name__ == "__main__":
    main()
