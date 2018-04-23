#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define LEFT (0)
#define RIGHT (1)
#define NUM_CHILDREN (2)


typedef struct node{
    int value;
    struct node * children[NUM_CHILDREN];
} Node;


Node * treeCreate (Node *t, int n){
    Node *t1 = malloc(sizeof(Node));
    t1->value = n;

    if (n == 1){
        t1->children[LEFT] = 0;
        t1->children[RIGHT] = 0;
        return t1;
    } else{
        t1->children[LEFT] = treeCreate(t1->children[LEFT], n-n/2);
        t1->children[RIGHT] = treeCreate(t1->children[RIGHT], n/2);
    }
    return t1;
}


void treeDestroy (Node *n){
    if (n == 0){
        return;
    } else {
        treeDestroy(n->children[LEFT]);
        treeDestroy(n->children[RIGHT]);
        free(n);
    }
}


void treePrint (Node * n){
    if(n == 0){
        return;
    } else {
        treePrint(n->children[LEFT]);
        for(int i = 0; i < n->value; i++){
            printf(" ");
        }
        printf("%i\n", n->value);
        treePrint(n->children[RIGHT]);
    }
}


int main(int argc, char **argv){
    if (argc != 2 || !atoi(argv[1]) || atoi(argv[1]) < 1){
        fprintf(stderr, "Usage: ./tree [size]\n");
        exit(1);
    }
    Node *t = treeCreate(0, atoi(argv[1]));
    treePrint(t);
    treeDestroy(t);
}
