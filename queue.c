#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct elt {
    char *string;
    struct elt *next;
} Elt;

typedef struct queue {
    struct elt *head;
    struct elt *tail;
    int n;
} Queue;

Queue * queueCreate(void){
    Queue *q;
    q = malloc(sizeof(Queue));
    q->head = 0;
    q->tail = 0;
    q->n = 0;
    return q;
}

void enqueue(char *a, Queue *q){
    Elt *e = malloc(sizeof(Elt));
    e->next = 0;

    int length = strlen(a);
    e->string = malloc(length+1);
    strcpy(e->string, a);

    if (q->n == 0){
        q->head = q->tail = e;
    } else {
        q->tail->next = e;
        q->tail = e;
    }

    q->n++;
}

Elt * dequeue(Queue *q){
    Elt *e1 = q->head;
    q->head = e1->next;
    q->n--;
    return e1;
}

int queueEmpty(Queue *q){
    return(q->n == 0);
}

int main(int argc, char **argv){
    Queue *q = queueCreate();
    for(int i = 1; i < argc; i++){
        enqueue(argv[i], q);
    }

    while(!queueEmpty(q)){
        Elt *e = dequeue(q);
        printf("%s\n", e->string);
        free(e->string);
        free(e);
    }
    free(q);
}
