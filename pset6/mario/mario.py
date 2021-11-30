# Pset6: Mario More
from cs50 import get_int


def main():
    height = -1

    # Ask the height
    while ((height < 1) or (height > 8)):
        height = get_int("Height : ")

    for i in range(0, height):
        for col in range(0,  ((height * 2) + 2)):
            # if is the first column
            if (col < height):
                if (col < height - i - 1):
                    print(" ", end='')
                else:
                    print("#", end='')
            # if is the second column
            elif (col == height):
                print("  ", end='')
            else:
                if (col < height + 2 + i):
                    print("#", end='')
        # Go to next line
        print("")


if __name__ == "__main__":
    main()
