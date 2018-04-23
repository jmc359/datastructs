#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

/*
 * High-precision arithmetic on non-negative number in base 10.
 */

typedef struct num{
    unsigned int length;
    int a[];
} Num;

/* constructs a Num from a string */
/* string contains representation of the number
 * in base 10, e.g. numCreate("314159");
 * Returns 0 if the string contains any non-digits.
 * Leading zeros are OK: numCreate("012") parses as 12.
 * Empty string parses as 0 */

Num * numCreate(const char *s){
    Num *number;
    unsigned int size = strlen(s);

    number = malloc(sizeof(Num) + sizeof(int) * size);
    assert(number);

    /* take care of 0 edge case */
    number -> length = size;
    if (size == 0){
        number -> a[0] = 0;
    } else {
        /* fill array with digits in least significant to most significant order */
        for (int i = 0; i < size; i++){
            if (isdigit(s[i])){
                number -> a[size - i - 1] = s[i] - '0';
            } else {
                free(number);
                return 0;
            }
        }
    }
    return number;
}


/* Free all resources used by a Num */
void numDestroy(Num *n){
    free(n);
}


/* Get the value of the i-th least significant digit of a Num.
 * Returns 0 if i is out of range.
 * Example:
 *   n = numCreate("12345");
 *   numGetDigit(n, 0) == 5
 *   numGetDigit(n, 3) == 2
 *   numGetDigit(n, 17) == 0
 *   numGetDigit(n, -12) == 0
 */
int numGetDigit(const Num *n, int i){
    /* make sure valid index is entered */
    if ((i > n -> length - 1) || i < 0){
        return 0;
    } else {
        return n -> a[i];
    }
}


/* creates output Num for add and multiply functions */
Num * numOutputCreate(const Num *x, const Num *y){
    char* input;
    Num *z;
    /* creates output of all 0s of length 2x that of the longest input */
    if (x -> length >=  y -> length){
        input = malloc(sizeof(char) * (2 * x -> length + 1));
        for (int i = 0; i < 2 * x -> length; i++){
            input[i] = '0';
        }
        input[x -> length * 2] = '\0';
    } else{
        input = malloc(sizeof(char) * (2 * y -> length + 1));
        for (int i = 0; i < 2 * y -> length; i++){
            input[i] = '0';
        }
        input[y -> length * 2] = '\0';
    }
    z = numCreate(input);
    free(input);
    return z;
}


/* add two Nums, returning a new Num */
/* does not destroy its inputs, caller must destroy output */
Num * numAdd(const Num *x, const Num *y){
    int carry = 0;
    Num *z = numOutputCreate(x, y);

    /* account for all lengths of x and y inputs */
    for (int i = 0; i < z -> length; i++){
        if (i > x -> length - 1 && i > y -> length - 1){
            z -> a[i] = 0;
        } else if (i > x -> length - 1){
            z -> a[i] = y -> a[i];
        } else if (i > y -> length - 1){
            z -> a[i] = x -> a[i];
        } else {
            z -> a[i] = x -> a[i] + y -> a[i];
        }

        /* keep track of carry if individual sum is greater than 10 */
        if (carry == 1){
            z -> a[i-1] -= 10;
            z -> a[i] += 1;
        }

        /* execute carry */
        if (z -> a[i] > 9) {
            carry = 1;
        } else {
            carry = 0;
        }

    }
    return z;
}


/* multiply two Nums, returning a new Num */
/* does not destroy its inputs, caller must destroy output */
Num * numMultiply(const Num *x, const Num *y){
    Num *z = numOutputCreate(x, y);
    /* standard multiplication with carry when individual products > 10 */
    for (int i = 0; i < y -> length; i++){
        for (int j = 0; j < x -> length; j++){
            z -> a[j+i] += x -> a[j] * y -> a[i];
            while (z -> a[j+i] > 9){
                z -> a[j+i] -= 10;
                z -> a[j+i+1] += 1;
            }
        }
    }
    return z;
}


/* Print the digits of a number to file.
 * Do not print any leading zeros unless n is zero. */
void numPrint(const Num *n, FILE *f){
    /* printing out edge case when Num == 0 */
    int nonzero = 0;
    int zeroes = 0;
    for (int j = 0; j < n -> length; j++){
        if (n -> a[j] == 0){
            zeroes++;
        }
    }
    if (zeroes == n -> length){
        fprintf(f, "%i", n -> a[0]);
    } else{
        /* print number out in most significant digit order */
        for (int i = n -> length - 1; i >= 0; i--){
            /* ignore zeroes until most significant digit is read */
            if (n -> a[i] != 0){
                nonzero = 1;
            }
            /* print rest of digits */
            if (nonzero == 1) {
                fprintf(f, "%i", n -> a[i]);
            }
        }
    }
}
