#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "rand_malloc.h"

// Function to read a line from standard input
char* getLine() {
    int bufferSize = 10; // Initial buffer size
    int position = 0;
    char* buffer = malloc(sizeof(char) * bufferSize);
    int c;

    if (!buffer) {
        fprintf(stderr, "Unable to allocate buffer\n");
        return NULL;
    }

    while (1) {
        // Read a character
        c = getchar();

        // If we hit EOF, replace it with a null character and return.
        if (c == EOF) {
            if (position == 0) {
                free(buffer);
                return NULL;
            }
            break;
        }

        // If we hit a newline, replace it with a null character and return.
        if (c == '\n') {
            break;
        }

        // Resize the buffer if necessary
        if (position + 1 >= bufferSize) {
            bufferSize *= 2;
            char* temp = realloc(buffer, bufferSize);
            if (!temp) {
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
int isValidBinary(const char* str) {
    while (*str) {
        if (*str != '0' && *str != '1' && !isspace(*str)) {
            return 0;
        }
        str++;
    }
    return 1;
}

// Function to remove whitespace from a string
void removeWhitespace(char* str) {
    if (!str) return;
    char* dest = str;
    while (*str) {
        if (!isspace(*str)) {
            *dest++ = *str;
        }
        str++;
    }
    *dest = '\0';
}

// Function to add two binary strings
char* addBinary(const char* a, const char* b) {
    if (!a || !b) return NULL;

    int lenA = strlen(a);
    int lenB = strlen(b);
    int maxLen = (lenA > lenB) ? lenA : lenB;
    char* result = malloc(maxLen + 2); // +1 for possible carry, +1 for null terminator
    if (!result) {
        fprintf(stderr, "Unable to allocate memory for addition result\n");
        return NULL;
    }

    int carry = 0;
    int i = lenA - 1, j = lenB - 1;
    int resIndex = maxLen; // Start from the end of the result string
    result[maxLen + 1] = '\0';

    while (i >= 0 || j >= 0 || carry) {
        int sum = carry;
        if (i >= 0) sum += a[i--] - '0';
        if (j >= 0) sum += b[j--] - '0';
        result[resIndex--] = (sum % 2) + '0';
        carry = sum / 2;
    }

    // If we have unused space at the start, shift everything left
    if (resIndex >= 0) {
        int shift = resIndex + 1;
        memmove(result, result + shift, maxLen - resIndex);
        result[maxLen - resIndex] = '\0';
    }

    return result;
}

int main() {
    char* line;
    char* sum = strdup("0"); // Initialize sum to "0"
    char** numbers = NULL;
    int numCount = 0;

    if (!sum) {
        fprintf(stderr, "Unable to allocate memory for sum\n");
        return 1;
    }

    while ((line = getLine()) != NULL) {
        if (strlen(line) == 0) {
            free(line);
            continue;
        }

        if (!isValidBinary(line)) {
            fprintf(stderr, "Error: Invalid binary number format\n");
            free(line);
            free(sum);
            for (int i = 0; i < numCount; i++) {
                free(numbers[i]);
            }
            free(numbers);
            return 1;
        }

        removeWhitespace(line);
        char** temp = realloc(numbers, (numCount + 1) * sizeof(char*));
        if (!temp) {
            fprintf(stderr, "Unable to reallocate memory for numbers\n");
            free(line);
            free(sum);
            for (int i = 0; i < numCount; i++) {
                free(numbers[i]);
            }
            free(numbers);
            return 1;
        }
        numbers = temp;
        numbers[numCount++] = line;

        char* newSum = addBinary(sum, line);
        if (!newSum) {
            free(line);
            free(sum);
            for (int i = 0; i < numCount; i++) {
                free(numbers[i]);
            }
            free(numbers);
            return 1;
        }
        free(sum);
        sum = newSum;
    }

    printf("Sum:\n%s\n", sum);
    printf("Input numbers:\n");
    for (int i = 0; i < numCount; i++) {
        printf("%s\n", numbers[i]);
        free(numbers[i]);
    }

    free(sum);
    free(numbers);
    return 0;
}
