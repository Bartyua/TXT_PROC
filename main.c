#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "rand_malloc.h"

// Function to read a line from standard input
char *getLine(int *success)
{
    int bufferSize = 10; // Initial buffer size
    int position = 0;
    char *buffer = malloc(sizeof(char) * bufferSize);
    int c;

    if (!buffer)
    {
        *success = 0;
        fprintf(stderr, "Unable to allocate buffer\n");
        return NULL;
    }

    while (1)
    {
        // Read a character
        c = getchar();

        // If we hit EOF, replace it with a null character and return.
        if (c == EOF)
        {
            if (position == 0)
            {
                free(buffer);
                return NULL;
            }
            break;
        }

        // If we hit a newline, replace it with a null character and return.
        if (c == '\n')
        {
            break;
        }

        // Resize the buffer if necessary
        if (position + 1 >= bufferSize)
        {
            bufferSize *= 2;
            char *temp = realloc(buffer, bufferSize);
            if (!temp)
            {
                fprintf(stderr, "Unable to reallocate buffer\n");
                free(buffer);
                return NULL;
            }
            buffer = temp;
        }

        buffer[position++] = c;
    }

    buffer[position] = '\0';
    return buffer;
}

// Function to check if a string is a valid binary number
int isValidBinary(const char *str)
{
    int flag = 0;
    while (*str)
    {   
        if (*str != '0' && *str != '1' && !isspace(*str))
        {
            return 0;
        }
        if (isspace(*str)) {
            if (flag) { // If we've seen a digit before
            flag = 2; // Mark that we've seen whitespace after digits
            }
        } else {
            if (flag == 2) { // If we see a digit after whitespace
            return 0;    // Invalid format
            }
            flag = 1; // Mark that we've seen a digit
        }
        str++;
    }
    return 1;
}

// Function to remove whitespace from a string
void removeWhitespace(char *str)
{
    if (!str)
        return;
    char *dest = str;
    while (*str)
    {
        if (!isspace(*str))
        {
            *dest++ = *str;
        }
        str++;
    }
    *dest = '\0';
}

// Function to add two binary strings
char *addBinary(const char *a, const char *b)
{
    if (!a || !b)
        return NULL;

    int lenA = strlen(a);
    int lenB = strlen(b);
    int maxLen = (lenA > lenB) ? lenA : lenB;
    char *result = malloc(maxLen + 2); // +1 for carry, +1 for null terminator

    if (!result)
    {
        fprintf(stderr, "Unable to allocate memory for addition result\n");
        return NULL;
    }

    char carry = '0';
    result[maxLen + 1] = '\0';
    int k = maxLen;

    // Process digits from right to left
    int i = lenA - 1;
    int j = lenB - 1;

    while (i >= 0 || j >= 0 || carry == '1')
    {
        char bitA = (i >= 0) ? a[i] : '0';
        char bitB = (j >= 0) ? b[j] : '0';

        if (bitA == '1' && bitB == '1')
        {
            result[k] = (carry == '1') ? '1' : '0';
            carry = '1';
        }
        else if (bitA == '0' && bitB == '0')
        {
            result[k] = carry;
            carry = '0';
        }
        else
        {
            result[k] = (carry == '1') ? '0' : '1';
        }

        k--;
        i--;
        j--;
    }

    // Shift result if there's no carry
    if (k >= 0)
    {
        memmove(result, result + k + 1, maxLen - k);
        result[maxLen - k] = '\0';
        return result;
    }

    return result + k + 1;
}

// Cleanup functions
void cleanup_numbers(char **numbers, int count)
{
    if (numbers)
    {
        for (int i = 0; i < count; i++)
        {
            free(numbers[i]);
        }
        free(numbers);
    }
}

void cleanup_and_exit(char *line, char *sum, char **numbers, int count)
{
    if (line)
        free(line);
    if (sum)
        free(sum);
    cleanup_numbers(numbers, count);
    exit(1);
}

int main()
{
    char *line = NULL;
    char *sum = strdup("0");
    char **numbers = NULL;
    int numCount = 0;
    int success = 1; // Track if all operations succeeded

    if (!sum)
    {
        fprintf(stderr, "Unable to allocate memory for sum\n");
        return 1;
    }

    while ((line = getLine(&success)) != NULL)
    {
        if (strlen(line) == 0)
        {
            free(line);
            continue;
        }

        if (!isValidBinary(line))
        {
            fprintf(stderr, "Error: Invalid binary number format\n");
            cleanup_and_exit(line, sum, numbers, numCount);
        }

        removeWhitespace(line);
        char **temp = realloc(numbers, (numCount + 1) * sizeof(char *));
        if (!temp)
        {
            fprintf(stderr, "Unable to reallocate memory for numbers\n");
            cleanup_and_exit(line, sum, numbers, numCount);
        }
        numbers = temp;
        numbers[numCount] = line; // Store the original line pointer

        char *newSum = addBinary(sum, line);
        if (!newSum)
        {
            fprintf(stderr, "Unable to calculate sum\n");
            cleanup_and_exit(line, sum, numbers, numCount);
        }

        free(sum);
        sum = newSum;
        numCount++;
    }
    if (success && numCount == 0)
    {
        printf("Empty Input\n");
    }

    // Only print results if all operations succeeded
    if (success && numbers && numCount > 0)
    {
        printf("Sum:\n%s\n", sum);
        printf("Input numbers:\n");
        for (int i = 0; i < numCount; i++)
        {
            printf("%s\n", numbers[i]);
        }
    }

    cleanup_numbers(numbers, numCount);
    free(sum);
}
