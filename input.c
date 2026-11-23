
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "input.h"

void readString(const char *prompt, char *buffer, int size)
{
    while (1)
    {
        printf("%s", prompt);

        if (fgets(buffer, size, stdin) == NULL)
        {
            printf("\nInput error. Please restart the program.\n");
            exit(EXIT_FAILURE);
        }

        size_t length = strlen(buffer);

        if (length > 0 && buffer[length - 1] == '\n')
        {
            buffer[length - 1] = '\0';
        }
        else
        {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF)
            {
                /* Clear remaining input */
            }
        }

        if (buffer[0] == '\0')
        {
            printf("Input cannot be empty. Try again.\n");
            continue;
        }

        return;
    }
}


int readInteger(const char *prompt, int min, int max)
{
    char buffer[100];
    char *endPointer;
    long value;

    while (1)
    {
        printf("%s", prompt);

        if (fgets(buffer, sizeof(buffer), stdin) == NULL)
        {
            printf("\nInput error. Please restart the program.\n");
            exit(EXIT_FAILURE);
        }

        value = strtol(buffer, &endPointer, 10);

        while (isspace((unsigned char)*endPointer))
        {
            endPointer++;
        }

        if (endPointer == buffer || *endPointer != '\0')
        {
            printf("Invalid input! Enter a valid integer.\n");
            continue;
        }

        if (value < min || value > max)
        {
            printf("Please enter a value between %d and %d.\n", min, max);
            continue;
        }

        return (int)value;
    }
}
