#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MULTIPLIER (113)
#define INITIAL_SIZE (2048)
#define GROW_MULTIPLIER (2)
#define MAX_LOAD_FACTOR (1)
#define VOWELS "aeiou"
#define VOWEL_SIZE (5)

// link in stack
struct elt {
    char *string;
    struct elt *next;
};

// Hashtable
typedef struct Hash {
    int size;
    int n;
    struct elt **table;
} Hash;

// stack
typedef struct stack {
    int n;
    struct elt *next;
} Stack;

// function declarations
Hash * HashCreate(int size);
void HashDestroy(Hash *h);
static unsigned long HashFunction(char *string);
static void HashGrow(Hash *h);
void HashInsert(Hash *h, char *string);
int HashSearch(Hash *h, char *string);

Stack * stackCreate(void);
void stackPush(Stack *stack, char *string);
struct elt * stackPop(Stack *stack);
int stackEmpty(Stack *stack);

void stringTest(char *a, Hash *h, Stack *s, int length);
void dfs(char *string, Hash *h, Stack *s, int length);


// Hashtable initialization
Hash *
HashCreate(int size)
{
    Hash *h;
    h = malloc(sizeof(Hash));

    assert(h != 0);

    // initialize Hashtable attributes
    h->size = size;
    h->n = 0;
    h->table = malloc(sizeof(struct elt *) * h->size);

    assert(h->table != 0);

    for(int i = 0; i < h->size; i++){
        h->table[i] = 0;
    }
    return h;
}


// destroy Hashtable
void
HashDestroy(Hash *h)
{
    struct elt *e;
    struct elt *next;

    // free all occupied positions in table
    for(int i = 0; i < h->size; i++) {
        for(e = h->table[i]; e != 0; e = next) {
            next = e->next;
            free(e->string);
            free(e);
        }
    }

    free(e);
    free(h->table);
    free(h);
}


// hash coordinates with original hash function
static unsigned long
HashFunction(char *string)
{
    int n = strlen(string);
    unsigned long hash = 0;
    for(unsigned int i = 0; i < n; i++) {
        hash = hash * MULTIPLIER + (97 ^ string[i]) * 13 * (57 ^ string[i]) + (359 ^ i) + (697 ^ string[i]);
    }
    return hash;
}


// grow function for when table becomes too full
static void
HashGrow(Hash *h)
{
    Hash *h2;
    Hash tmp;
    int i;
    struct elt *e;

    h2 = HashCreate(h->size * GROW_MULTIPLIER);

    for(i = 0; i < h->size; i++) {
        for(e = h->table[i]; e != 0; e = e->next) {
            // recopy everything
            HashInsert(h2, e->string);
        }
    }

    // swap contents and destroy d2
    tmp = *h;
    *h = *h2;
    *h2 = tmp;

    HashDestroy(h2);
}


// insert new coordinates and string
void
HashInsert(Hash *h, char *string)
{
    struct elt *e;
    unsigned long i;

    e = malloc(sizeof(*e));
    assert(e);

    // set string for position in table
    e->string = malloc(strlen(string) + 1);
    strcpy(e->string, string);

    i = HashFunction(string) % h->size;

    e->next = h->table[i];
    h->table[i] = e;

    h->n++;

    if(h->n >= h->size * MAX_LOAD_FACTOR) {
        HashGrow(h);
    }
}


// return string of given coordinates
// or -1 if no matching coordinates are present
int
HashSearch(Hash *h, char *string)
{
    struct elt *e;

    // search hash location and return found string
    for(e = h->table[HashFunction(string) % h->size]; e != 0; e = e->next) {
        if(strcmp(e->string, string) == 0){
            return 1;
        }
    }

    // if not found return -1
    return -1;
}


// create new stack
Stack *
stackCreate(void)
{
    Stack *stack;
    stack = malloc(sizeof(Stack));
    stack->n = 0;
    stack->next = 0;
    return stack;
}


// push link onto stack
void
stackPush(Stack *stack, char *string)
{
    struct elt *stack2;
    stack2 = malloc(sizeof(struct elt));
    stack2->string = malloc(strlen(string) + 1);
    strcpy(stack2->string, string);
    stack2->next = stack->next;
    stack->next = stack2;
    stack->n++;
}


// pop link off of stack
struct elt *
stackPop(Stack *stack)
{
    struct elt *stack2;
    stack2 = stack->next;
    stack->next = stack2->next;
    stack->n--;
    return stack2;
}


// check if stack is empty
int
stackEmpty(Stack *stack)
{
    return (stack->n == 0);
}


// test for valid moves
void
stringTest(char *a, Hash *h, Stack *s, int length){
    int n = strlen(a);
    if (s->n == length){
        return;
    }

    for(int i = 0; i < n-1; i++){

        // put i+1 on i
        if(a[i] - a[i+1] <= 5 && a[i] - a[i+1] >= -5){
            char *b = calloc(n+3, sizeof(char));
            strncpy(b, a, i);
            strncpy(b+i, a+i+1, n-i);

            dfs(b, h, s, length);
            HashInsert(h, b);
            free(b);
            if (s->n == length){
                return;
            }
        }

        for(int j = 0; j < VOWEL_SIZE; j++){
            if(a[i] == VOWELS[j]){
                for (int k = 0; k < VOWEL_SIZE;k++){
                    if(a[i+1] == VOWELS[k]){
                        char *b = calloc(n+3, sizeof(char));
                        strncpy(b, a, i);
                        strncpy(b+i, a+i+1, n-i);

                        dfs(b, h, s, length);
                        HashInsert(h, b);
                        free(b);
                        if (s->n == length){
                            return;
                        }
                    }
                }
            }
        }

        // put i+3 on i
        if(i < n-3){
            if(a[i] - a[i+3] <= 5 && a[i] - a[i+3] >= -5){
                char *b = calloc(n+3, sizeof(char));
                strncpy(b, a, i);
                b[i] = a[i+3];
                strncpy(b+i+1, a+i+1, 2);
                strncpy(b+i+3, a+i+4, n-i-2);

                dfs(b, h, s, length);
                HashInsert(h, b);
                free(b);
                if (s->n == length){
                    return;
                }
            }


            for(int j = 0; j < VOWEL_SIZE; j++){
                if(a[i] == VOWELS[j]){
                    for (int k = 0; k < VOWEL_SIZE; k++){
                        if(a[i+3] == VOWELS[k]){
                            char *b = calloc(n+3, sizeof(char));
                            strncpy(b, a, i);
                            b[i] = a[i+3];
                            strncpy(b+i+1, a+i+1, 2);
                            strncpy(b+i+3, a+i+4, n-i-2);

                            dfs(b, h, s, length);
                            HashInsert(h, b);
                            free(b);
                            if (s->n == length){
                                return;
                            }
                        }
                    }
                }
            }
        }
    }
    // pop bad combinations
    struct elt *tmp = stackPop(s);
    free(tmp->string);
    free(tmp);
}


// depth first search
void
dfs(char *string, Hash *h, Stack *s, int length)
{
    stackPush(s, string);
    if(!stackEmpty(s) && s->n != length){
        if(HashSearch(h, s->next->string) == -1){
            // search if not done before
            stringTest(s->next->string, h, s, length);
        }
        else{
            // pop if bad
            struct elt *tmp = stackPop(s);
            free(tmp->string);
            free(tmp);
        }
    }
}


int
main(int argc, char **argv)
{
    if(argc != 2){
        fprintf(stderr, "Usage: ./shrink [lowercase word]\n");
        exit(1);
    }

    Hash *h = HashCreate(INITIAL_SIZE);
    Stack *s = stackCreate();
    dfs(argv[1], h, s, strlen(argv[1]));
    HashDestroy(h);

    // create new stack for printing
    Stack *s1 = stackCreate();
    while(!stackEmpty(s)){
        struct elt *tmp = stackPop(s);
        stackPush(s1, tmp->string);
        free(tmp->string);
        free(tmp);
    }
    free(s);

    // print
    for(struct elt *e = s1->next; e != 0; e = e->next){
        printf("%s\n", e->string);
    }

    // destroy stack
    while(!stackEmpty(s1)){
        struct elt *tmp = stackPop(s1);
        free(tmp->string);
        free(tmp);
    }
    free(s1);
}
