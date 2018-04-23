// implementation of a deck queue
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>

// Standard suits and ranks
#define SUITS "CDHS"
#define RANKS "A23456789TJQK"
#define SUIT_SIZE (4)
#define RANK_SIZE (13)
#define DECK_SIZE (52)

// A single card
// This is small enough that we usually pass it
// around by copying instead of using pointers.
typedef struct card {
    char rank;   /* from RANKS */
    char suit;   /* from SUITS */
} Card;

// links in deck queue
typedef struct link {
    struct link *next;
    Card card;
} Link;

// A deck of cards
typedef struct deck {
    struct link *head;
    struct link *tail;
    unsigned int size;
} Deck;


// Create a new unshuffled deck of 52 cards,
// ordered by suit then rank:
// AC 2C 3C ... KC AD 2D 3D ... KD AH 2H 3H ... KS
Deck *deckCreate(void){
    Deck *deck;
    deck = malloc(sizeof(Deck));
    deck->head = 0;

    for (int i = 0; i < SUIT_SIZE; i++) {
        for (int j = 0; j < RANK_SIZE; j++) {
            /* malloc link in queue */
            Link *link;
            link = malloc(sizeof(Link));
            assert(link);

            /* set rank and suit */
            link->card.suit = SUITS[i];
            link->card.rank = RANKS[j];

            /* tail has no next */
            link->next = 0;

            if(deck->head == 0) {
                /* If the queue was empty, becomes head */
                deck->head = link;
            } else {
                /* Otherwise gets in line */
                deck->tail->next = link;
            }

            /* Becomes new tail */
            deck->tail = link;
        }
    }
    deck->size = DECK_SIZE;
    return deck;
}

// Return true if deck is not empty.
// Running time should be O(1).
int deckNotEmpty(const Deck *d){
    return (!(d->head == 0));
}

// Remove and return the top card of a deck.
// If deck is empty, behavior is undefined.
// Running time should be O(1).
Card deckGetCard(Deck *d){
    Card draw;
    Link *tmp;

    // standard dequeue
    draw = d->head->card;
    tmp = d->head;
    d->head = tmp->next;
    free(tmp);
    d->size--;
    return draw;
}

// Free all space used by d.
// Running time should be O(length of deck)
void deckDestroy(Deck *d){
    Card *tmp = malloc(sizeof(Card));
    while(deckNotEmpty(d)){
        *tmp = deckGetCard(d);
    }
    free(tmp);
    free(d);
}

// Add a card to the bottom of a deck.
// This is not required to do anything special if the card is bogus
//   (e.g. "1C", "?X", "A-").
// Running time should be O(1).
void deckPutCard(Deck *d, Card c){
    Link *link;

    link = malloc(sizeof(Link));
    assert(link);

    link->card = c;

    /* Being the tail, it has no next */
    link->next = 0;

    if(d->head == 0) {
        /* If queue is empty, becomes head */
        d->head = link;
    } else {
        /* Otherwise follow old tail */
        d->tail->next = link;
    }

    /* Become the new tail */
    d->tail = link;
    d->size++;
}


// Split a deck into two piles:
//    *d1 is new deck with first n cards in d.
//    *d2 is new deck with remaining cards in d.
// Order of cards is preserved.
// If d contains fewer than n cards, put them all in d1.
// Destroys d.
// Running time should be O(n).
void deckSplit(Deck *d, int n, Deck **d1, Deck **d2){
    *d1 = malloc(sizeof(Deck));
    *d2 = malloc(sizeof(Deck));
    (*d1)->head = 0;
    (*d2)->head = 0;
    (*d1)->size = (*d2)->size = 0;

    // fill first deck with n cards
    for (int i = 0; i < n && d->size > 0; i++){
        deckPutCard(*d1, deckGetCard(d));
    }
    // fill second deck with rest
    if (d->size > 0){
        memcpy(*d2, d, sizeof(Deck));
    }
    free(d);
}

// Shuffle two decks together by alternating cards from
// d1 and d2 to obtain new deck.
//
// If d1 is X X X X
// and d2 is Y Y Y Y Y Y Y,
// return value is X Y X Y X Y X Y Y Y Y.
//
// If d1 is X X X X
// and d2 is Y Y,
// return value is X Y X Y X X.
//
// Running time should be O(length of shorter deck).
// Destroys d1 and d2.
Deck *deckShuffle(Deck *d1, Deck *d2){
    // return nonempty input if one deck is empty
    Deck *d3 = malloc(sizeof(Deck));
    d3->head = 0;
    d3->size = 0;
        if (d2->size < d1->size){
            // alternate between queues until shorter one is exhausted
            while (d2->size > 0){
                deckPutCard(d3, deckGetCard(d1));
                deckPutCard(d3, deckGetCard(d2));
            }
            // fill in elements of longer queue
            if (d1->size > 0){
                if (d3->size == 0){
                    memcpy(d3, d1, sizeof(Link));
                } else {
                    d3->tail->next = d1->head;
                    memcpy(d3->tail->next, d1->head, sizeof(Link));
                }
                d3->size += d1->size;
            }
        } else {
            // alternate between queues until shorter one is exhausted
            while (d1->size > 0){
                deckPutCard(d3, deckGetCard(d1));
                deckPutCard(d3, deckGetCard(d2));
            }
            // fill in elements of longer queue
            if (d2->size > 0){
                if (d3->size == 0){
                    memcpy(d3, d2, sizeof(Link));
                } else {
                    d3->tail->next = d2->head;
                    memcpy(d3->tail->next, d2->head, sizeof(Link));
                }
                d3->size += d2->size;
            }
        }
        free(d1);
        free(d2);
        return d3;
}

// Print the contents of deck to f as sequence of ranks/suits
// separated by spaces.
// Example output: "AS TC 9D 3H 5S" (without quotes)
// Running time should be O(length of deck).
void deckPrint(const Deck *d, FILE *f) {
    Link *copy = d->head;
    // print cards
    for(int i = 0; i < d->size; i++, copy = copy->next) {
        fprintf(f, "%c%c", copy->card.rank, copy->card.suit);
        // print spaces between cards
        if (i != d->size - 1){
            fprintf(f, " ");
        }
    }
    free(copy);
}
