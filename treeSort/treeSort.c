#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#define INITIAL_SIZE (32)

/* Tree data structure */
typedef struct tree {
    int size;
    int error;
    int treeSize;
    int numChildren;
    struct tree **children;
} Tree;

/* function declarations */
Tree * TreeCreate(Tree *t);
int compare(const void *size1, const void *size2);
Tree * TreeSort(Tree *t);
void TreePrint(Tree *t);
void TreeDestroy(Tree *t);

/* keep track of left and right bracket amounts */
int LEFT = 0;
int RIGHT = 0;

/* create tree from stdin */
Tree *
TreeCreate(Tree *t){
    // initialize tree node
    int size = INITIAL_SIZE;
    t = malloc(sizeof(Tree));
    t->size = size;
    t->error = 0;
    t->treeSize = 0;
    t->numChildren = 0;
    t->children = malloc(size * sizeof(Tree *));
    for (int i = 0; i < size; i++){
        t->children[i] = 0;
    }

    int c;
    while((c = getchar()) != EOF){
        if (c == '['){
            LEFT++;
            t->numChildren++;
            t->treeSize++;

            // if too many children, grow array
            if (t->numChildren > t->size){
                t->size *= 2;
                t->children = realloc(t->children, t->size * sizeof(Tree *));
                for(int i = t->size/2; i < t->size; i++){
                    t->children[i] = 0;
                }
            }

            // create child
            t->children[t->numChildren-1] = TreeCreate(t->children[t->numChildren-1]);
            t->error += t->children[t->numChildren-1]->error;

            // keep track of size
            if (t->children[t->numChildren-1] != 0){
                t->treeSize += t->children[t->numChildren-1]->treeSize;
            }
        } else if (c == ']'){
            // return tree node
            RIGHT++;
            return t;
        } else if (LEFT == RIGHT && LEFT != 0){
            // ignore any characters after valid tree input
        } else {
            // something went wrong
            t->error = 1;
            return t;
        }
    }
    if(LEFT != RIGHT) {
        t->error = 1;
        return t;
    }
    return t;
}

/* qsort compare function */
int
compare(const void *t1, const void *t2){
    int a = (*(Tree **)t1)->treeSize;
    int b = (*(Tree **)t2)->treeSize;
    return b-a;
}

/* sort tree recursively */
Tree *
TreeSort(Tree *t){
    if(t == 0){
        return 0;
    } else {
        qsort(t->children, t->numChildren, sizeof(Tree *), compare);
        for(int i = 0; i < t->numChildren; i++){
            TreeSort(t->children[i]);
        }
        return t;
    }
}

/* print tree recursively */
void
TreePrint(Tree *t){
    if(t == 0){
        return;
    } else {
        putchar('[');
        for(int i = 0; i < t->numChildren; i++){
            TreePrint(t->children[i]);
        }
        putchar(']');
    }
}


/* destroy tree recursively */
void
TreeDestroy(Tree *t){
    if (t == 0){
        return;
    }
    for (int i = 0; i < t->numChildren; i++){
        TreeDestroy(t->children[i]);
    }
    free(t->children);
    free(t);
}


int
main(void){
    Tree *t = TreeCreate(0);
    if(t->error == 1 || LEFT > RIGHT || LEFT == 0){
        TreeDestroy(t);
        exit(1);
    }
    t = TreeSort(t);
    TreePrint(t->children[0]);
    TreeDestroy(t);
}
