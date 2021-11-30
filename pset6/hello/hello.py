# Pset6: Print Hello
from cs50 import get_string


def main():
    # Ask for name
    name = get_string("What´s your name? ")
    # Print Hello Name
    print("hello, {0}".format(name))


if __name__ == "__main__":
    main()
