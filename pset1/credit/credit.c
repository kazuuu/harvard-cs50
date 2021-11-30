#include <cs50.h>
#include <stdio.h>

int main(void)
{

    long credit = get_long("Number: ");

    int number_of_digits = 0;
    int sumMultiplicated = 0;
    int sumNonMultiplicated = 0;
    int firstDigit = 0;
    int firstTwoDigits = 0;

    bool bMultiply = false;

    // Make a loop in each digit of the card number
    while (((credit / 10) > 0) || (((credit / 10) == 0) && ((credit % 10) > 0)))
    {
        // Counter to evaluate total digit number of the card.
        number_of_digits++;

        // bool to Multiply every other digit by 2, starting with the number’s second-to-last digit,
        if (bMultiply)
        {
            int x = (credit % 10) * 2;

            // if the product number has more than one digit, sum each digit separately.
            if (x > 9)
            {
                sumMultiplicated += (x % 10);
                sumMultiplicated += (x / 10);
            }
            else
            {
                sumMultiplicated += ((credit % 10) * 2);
            }
        }
        else
        {
            sumNonMultiplicated += (credit % 10);
        }

        // set the first two digits of the card
        firstTwoDigits = firstDigit;
        firstDigit = credit;
        credit = credit / 10;
        bMultiply = !bMultiply;
    }

    // Calculate the verifier number.
    int verifyNumber = sumMultiplicated + sumNonMultiplicated;

    // if Verify number is ok, check whats the card type.
    if (verifyNumber % 10 == 0)
    {
        if (((firstTwoDigits == 34) || (firstTwoDigits == 37)) && (number_of_digits == 15))
        {
            printf("AMEX\n");
        }
        else if (((firstTwoDigits / 10) == 4) && ((number_of_digits == 13) || (number_of_digits == 16)))
        {
            printf("VISA\n");
        }
        else if (((firstTwoDigits >= 51) && (firstTwoDigits <= 55)) && (number_of_digits == 16))
        {
            printf("MASTERCARD\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }

}