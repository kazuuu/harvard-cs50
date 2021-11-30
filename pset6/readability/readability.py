# Pset6: Readability
from cs50 import get_string


def main():
    # Asking text
    text = get_string("Text: ")

    iLetters = count_letters(text)
    iWords = count_words(text)
    iSentences = count_sentences(text)

    # Printing greeting
    # print("{0} letter(s)".format(iLetters))
    # print("{0} word(s)".format(iWords))
    # print("{0} sentence(s)".format(iSentences))

    L = (iLetters / iWords) * 100
    S = (iSentences / iWords) * 100

    # Calculate Coleman-Liau index i
    index = (0.0588 * L) - (0.296 * S) - 15.8

    # Output message
    if (index < 1):
        print("Before Grade 1")
    elif (index > 16):
        print("Grade 16+")
    else:
        print("Grade {0}".format(round(index)))


# Count Letters
def count_letters(text):
    sumLetters = 0

    # check each letter
    for i in range(0, len(text)):
        # convert all letters to uppercase
        c = text[i].upper()
        if ((ord(c) >= 65) and (ord(c) <= 90)):
            sumLetters += 1

    return sumLetters


# Count words
def count_words(text):
    return len(text.split())


# Count Sentences
def count_sentences(text):
    sumSentences = len(text.split(".")) - 1
    sumSentences = sumSentences + len(text.split("!")) - 1
    sumSentences = sumSentences + len(text.split("?")) - 1

    return sumSentences


if __name__ == "__main__":
    main()
