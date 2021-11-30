#include <stdio.h>
#include <cs50.h>

int main(void)
{
    // Asking first name
    string name = get_string("Whats your first name?\n");

    // Printing greeting
    printf("hello, %s\n", name);
}