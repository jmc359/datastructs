#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "array.h"

#define LEFT (0)
#define RIGHT (1)
#define CHILDREN (2)

// Binary tree structure for array
struct array {
    size_t size;
    int sum;
    int (*combine)(int, int);
    struct array *child[CHILDREN];
};


// // Create a new array holding n values, all initially 0.
// // Behavior is undefined if n == 0.
// // Cost: O(n).
Array *arrayCreate(int (*combine)(int, int), size_t n){
    if(n == 0){
        return 0;
    }

    Array *a = malloc(sizeof(Array));
    a->size = n;
    a->combine = combine;

    if (n == 1){
        // child node
        a->child[LEFT] = 0;
        a->child[RIGHT] = 0;
        a->sum = 0;
        return a;
    } else{
        // recurse for parent nodes
        a->child[LEFT] = arrayCreate(combine, n/2);
        a->child[RIGHT] = arrayCreate(combine, n-n/2);
        a->sum = a->combine(a->child[LEFT]->sum, a->child[RIGHT]->sum);
    }
    return a;
}


// // Free all space used by an array.
// // Cost: O(n).
void arrayDestroy(Array *a){
    if(a == 0){
        return;
    } else{
        // recursively destroy
        arrayDestroy(a->child[LEFT]);
        arrayDestroy(a->child[RIGHT]);
        free(a);
    }
}


// // Return the number of size of an array.
// // Cost: O(1).
size_t arraySize(const Array *a){
    return a->size;
}


// // Return the i-th element of an array
// // or 0 if i is out of range.
// // Cost: O(log n).
int arrayGet(const Array *a, size_t i){
    if(a == 0 || i >= a->size || i < 0){
        // out of range
        return 0;
    } else if (a->size == 1){
        // found it
        return a->sum;
    } else if(i < a->child[LEFT]->size){
        // search left subtree
        return arrayGet(a->child[LEFT], i);
    } else {
        // search right subtree
        return arrayGet(a->child[RIGHT], i-a->child[LEFT]->size);
    }
}


// // Set the i-th element of an array to v.
// // No effect if i is out of range.
// // Cost: O(log n).
void arraySet(Array *a, size_t i, int v){
    if(a == 0 || i >= a->size || i < 0){
        // out of range
        return;
    } else if (a->size == 1){
        // set sum of leaf
        a->sum = v;
        return;
    } else if(i < a->child[LEFT]->size){
        // recurse and then aggregate sums of leaves
        arraySet(a->child[LEFT], i, v);
        a->sum = a->combine(a->child[LEFT]->sum, a->child[RIGHT]->sum);
    } else {
        // recurse and then aggregate sums of leaves
        arraySet(a->child[RIGHT], i-a->child[LEFT]->size, v);
        a->sum = a->combine(a->child[LEFT]->sum, a->child[RIGHT]->sum);
    }
}


// Return the result of aggregating the first k size
// of an array in order using the combine combine.
// If k is zero or greater than size, returns combination of all size.
// Cost: O(log n).
int arrayCombine(const Array *a, size_t k){
    if (a == 0){
        // empty input array
        return 0;
    } else if(k >= a->size || k == 0){
        // return found sum
        return a->sum;
    } else if(k <= a->child[LEFT]->size){
        // traverse left subtree
        return arrayCombine(a->child[LEFT], k);
    } else if(k > a->child[LEFT]->size){
        // combine sums of left and the result of the recursive call on right
        return a->combine(a->child[LEFT]->sum, arrayCombine(a->child[RIGHT], k - a->child[LEFT]->size));
    } else {
        return a->sum;
    }
}
