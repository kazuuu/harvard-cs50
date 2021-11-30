# Pset6: Credit
from cs50 import get_int


def main():
    credit = get_int("Number: ")

    print(int(credit / 10))

    number_of_digits = 0
    sumMultiplicated = 0
    sumNonMultiplicated = 0
    firstDigit = 0
    firstTwoDigits = 0

    bMultiply = False

    # Make a loop in each digit of the card number
    while (((int(credit / 10)) > 0) or (((int(credit / 10)) == 0) and ((credit % 10) > 0))):
        # Counter to evaluate total digit number of the card.
        number_of_digits += 1

        # bool to Multiply every other digit by 2, starting with the number’s second-to-last digit,
        if (bMultiply):
            x = (credit % 10) * 2

            # if the product number has more than one digit, sum each digit separately.
            if (x > 9):
                sumMultiplicated += (x % 10)
                sumMultiplicated += int((x / 10))
            else:
                sumMultiplicated += ((credit % 10) * 2)
        else:
            sumNonMultiplicated += (credit % 10)

        # set the first two digits of the card
        firstTwoDigits = firstDigit
        firstDigit = credit
        credit = int(credit / 10)
        bMultiply = not bMultiply

    # Calculate the verifier number.
    verifyNumber = sumMultiplicated + sumNonMultiplicated

    # if Verify number is ok, check whats the card type.
    if (verifyNumber % 10 == 0):
        if (((firstTwoDigits == 34) or (firstTwoDigits == 37)) and (number_of_digits == 15)):
            print("AMEX")
        elif (((int(firstTwoDigits / 10)) == 4) and ((number_of_digits == 13) or (number_of_digits == 16))):
            print("VISA")
        elif (((firstTwoDigits >= 51) and (firstTwoDigits <= 55)) and (number_of_digits == 16)):
            print("MASTERCARD")
        else:
            print("INVALID")
    else:
        print("INVALID")


if __name__ == "__main__":
    main()
