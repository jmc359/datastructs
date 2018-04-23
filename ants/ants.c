#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#define ARMY_SIZE (256)
#define MULTIPLIER (113)
#define INITIAL_SIZE (1024)
#define GROW_MULTIPLIER (2)
#define MAX_LOAD_FACTOR (1)
#define HASHES (3)

// Ant data structure
typedef struct ant {
    int x;
    int y;
    int z;
    int value;
} Ant;

// individual element in Hashtable
struct elt {
    int x_pos;
    int y_pos;
    int z_pos;
    int value;
    struct elt *next;
};

// Hashtable with chaining
typedef struct Hash {
    int size;
    int n;
    struct elt **table;
} Hash;

// Ant army array
Ant army[ARMY_SIZE];

// Function declarations
Hash * HashCreate(int size);
void HashDestroy(Hash *h);
static unsigned long HashFunction(int x, int y, int z);
static void HashGrow(Hash *h);
void HashInsert(Hash *h, int x, int y, int z, int value);
int HashSearch(Hash *h, const int x, const int y, const int z);


/* Hashtable initialization */
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


/* destroy Hashtable */
void
HashDestroy(Hash *h)
{
    struct elt *e;
    struct elt *next;

    // free all occupied positions in table
    for(int i = 0; i < h->size; i++) {
        for(e = h->table[i]; e != 0; e = next) {
            next = e->next;
            free(e);
        }
    }

    free(e);
    free(h->table);
    free(h);
}


/* hash coordinates with original hash function */
static unsigned long
HashFunction(int x, int y, int z)
{
    unsigned long hash = 0;
    for(unsigned int i = 0; i < HASHES; i++) {
        hash = hash * MULTIPLIER + (97 ^ i) * 13 * (57 ^ x) + (359 ^ y) + (697 ^ z);
    }
    return hash;
}


/* grow function for when table becomes too full */
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
            /* recopy everything */
            HashInsert(h2, e->x_pos, e->y_pos, e->z_pos, e->value);
        }
    }

    /* swap contents and destroy h2 */
    tmp = *h;
    *h = *h2;
    *h2 = tmp;

    HashDestroy(h2);
}


/* insert new coordinates and value */
void
HashInsert(Hash *h, int x, int y, int z, int value)
{
    struct elt *e;
    unsigned long i;

    e = malloc(sizeof(*e));
    assert(e);

    // set proper values for position in table
    e->x_pos = x;
    e->y_pos = y;
    e->z_pos = z;
    e->value = value;

    i = HashFunction(x, y, z) % h->size;

    e->next = h->table[i];
    h->table[i] = e;

    h->n++;

    if(h->n >= h->size * MAX_LOAD_FACTOR) {
        HashGrow(h);
    }
}


/* return value of given coordinates */
/* or -1 if no matching coordinates are present */
int
HashSearch(Hash *h, const int x, const int y, const int z)
{
    struct elt *e;
    // search hash location and return found value
    for(e = h->table[HashFunction(x,y,z) % h->size]; e != 0; e = e->next) {
        if(x == e->x_pos && y == e->y_pos && z == e->z_pos) {
            return e->value;
        }
    }
    // if not found return -1
    return -1;
}


int
main(void){
    // fill ant army array
    for (int i = 0; i < ARMY_SIZE; i++){
        army[i].value = i;
        army[i].x = 0;
        army[i].y = 0;
        army[i].z = 0;
    }

    // create Hashtable
    Hash *universe = HashCreate(INITIAL_SIZE);

    // parse input
    int firstchar;
    while((firstchar = getchar()) != EOF){
        int command;
        while((command = getchar()) != '\n'){
            switch(command){
                // end program
                case EOF:
                    HashDestroy(universe);
                    return 0;
                    break;
                // perform various operations to ant position
                case 'h':
                    army[firstchar].x -= 1;
                    break;
                case 'j':
                    army[firstchar].y -= 1;
                    break;
                case 'k':
                    army[firstchar].y += 1;
                    break;
                case 'l':
                    army[firstchar].x += 1;
                    break;
                case '<':
                    army[firstchar].z += 1;
                    break;
                case '>':
                    army[firstchar].z -= 1;
                    break;
                case '*':
                    army[firstchar].x *= 2;
                    army[firstchar].y *= 2;
                    army[firstchar].z *= 2;
                    break;
                case '.':
                    // hash coordinates and store value
                    if (HashSearch(universe, army[firstchar].x, army[firstchar].y, army[firstchar].z) == -1){
                        HashInsert(universe, army[firstchar].x, army[firstchar].y, army[firstchar].z, army[firstchar].value);
                    } else{
                        struct elt *e;
                        for(e = universe->table[HashFunction(army[firstchar].x, army[firstchar].y, army[firstchar].z) % universe->size]; e != 0; e = e->next) {
                            if(army[firstchar].x == e->x_pos && army[firstchar].y == e->y_pos && army[firstchar].z == e->z_pos) {
                                e->value = army[firstchar].value;
                            }
                        }
                    }
                    break;
                case '?':
                    // hash lookup and print
                    if (HashSearch(universe, army[firstchar].x, army[firstchar].y, army[firstchar].z) == -1){
                        putchar(' ');
                    } else {
                        putchar(HashSearch(universe, army[firstchar].x, army[firstchar].y, army[firstchar].z));
                    }
                    break;
                default:
                    break;
            }
        }
    }
    HashDestroy(universe);
    return 0;
}
