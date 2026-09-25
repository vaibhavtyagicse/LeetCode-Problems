#include <stdlib.h>

#define HASH_SIZE 100003
#define MAX_FREQ 20005

typedef struct Node {
    int key;
    int value;
    int freq;

    struct Node *prev;
    struct Node *next;

    struct Node *hashNext;
} Node;

typedef struct {
    int capacity;
    int size;
    int minFreq;

    Node **hashTable;

    // Head and tail of every frequency list
    Node **head;
    Node **tail;
} LFUCache;


/* ---------------- HASH FUNCTION ---------------- */

int hashFunction(int key)
{
    return key % HASH_SIZE;
}


/* ---------------- FIND NODE ---------------- */

Node* findNode(LFUCache *obj, int key)
{
    int index = hashFunction(key);

    Node *temp = obj->hashTable[index];

    while (temp != NULL)
    {
        if (temp->key == key)
            return temp;

        temp = temp->hashNext;
    }

    return NULL;
}


/* ---------------- ADD TO HASH TABLE ---------------- */

void addToHash(LFUCache *obj, Node *node)
{
    int index = hashFunction(node->key);

    node->hashNext = obj->hashTable[index];
    obj->hashTable[index] = node;
}


/* ---------------- REMOVE FROM HASH TABLE ---------------- */

void removeFromHash(LFUCache *obj, Node *node)
{
    int index = hashFunction(node->key);

    Node *temp = obj->hashTable[index];
    Node *prev = NULL;

    while (temp != NULL)
    {
        if (temp == node)
        {
            if (prev == NULL)
                obj->hashTable[index] = temp->hashNext;
            else
                prev->hashNext = temp->hashNext;

            return;
        }

        prev = temp;
        temp = temp->hashNext;
    }
}


/* ---------------- ADD TO FREQUENCY LIST ---------------- */
/*
   New nodes are added at the front.
   Therefore, the tail is the LRU node.
*/

void addToList(LFUCache *obj, Node *node)
{
    int f = node->freq;

    node->prev = NULL;
    node->next = obj->head[f];

    if (obj->head[f] != NULL)
        obj->head[f]->prev = node;
    else
        obj->tail[f] = node;

    obj->head[f] = node;
}


/* ---------------- REMOVE FROM FREQUENCY LIST ---------------- */

void removeFromList(LFUCache *obj, Node *node)
{
    int f = node->freq;

    if (node->prev != NULL)
        node->prev->next = node->next;
    else
        obj->head[f] = node->next;

    if (node->next != NULL)
        node->next->prev = node->prev;
    else
        obj->tail[f] = node->prev;

    node->prev = NULL;
    node->next = NULL;
}


/* ---------------- CREATE CACHE ---------------- */

LFUCache* lFUCacheCreate(int capacity)
{
    LFUCache *obj = malloc(sizeof(LFUCache));

    obj->capacity = capacity;
    obj->size = 0;
    obj->minFreq = 0;

    obj->hashTable =
        calloc(HASH_SIZE, sizeof(Node*));

    obj->head =
        calloc(MAX_FREQ, sizeof(Node*));

    obj->tail =
        calloc(MAX_FREQ, sizeof(Node*));

    return obj;
}


/* ---------------- GET ---------------- */

int lFUCacheGet(LFUCache* obj, int key)
{
    Node *node = findNode(obj, key);

    // Key not found
    if (node == NULL)
        return -1;

    int oldFreq = node->freq;

    // Remove from old frequency list
    removeFromList(obj, node);

    // Increase frequency
    node->freq++;

    // Add to new frequency list
    addToList(obj, node);

    // If old frequency became empty,
    // update minFreq
    if (obj->head[oldFreq] == NULL)
    {
        if (obj->minFreq == oldFreq)
            obj->minFreq = node->freq;
    }

    return node->value;
}


/* ---------------- PUT ---------------- */

void lFUCachePut(LFUCache* obj, int key, int value)
{
    // Capacity is zero
    if (obj->capacity == 0)
        return;

    Node *node = findNode(obj, key);

    /*
       KEY ALREADY EXISTS
    */
    if (node != NULL)
    {
        node->value = value;

        int oldFreq = node->freq;

        removeFromList(obj, node);

        node->freq++;

        addToList(obj, node);

        if (obj->head[oldFreq] == NULL)
        {
            if (obj->minFreq == oldFreq)
                obj->minFreq = node->freq;
        }

        return;
    }


    /*
       CACHE IS FULL
    */
    if (obj->size == obj->capacity)
    {
        /*
           tail[minFreq] is the LRU node
           among the nodes having minimum frequency.
        */
        Node *victim = obj->tail[obj->minFreq];

        removeFromList(obj, victim);
        removeFromHash(obj, victim);

        free(victim);

        obj->size--;
    }


    /*
       CREATE NEW NODE
    */

    node = malloc(sizeof(Node));

    node->key = key;
    node->value = value;
    node->freq = 1;

    node->prev = NULL;
    node->next = NULL;
    node->hashNext = NULL;

    // Add to hash table
    addToHash(obj, node);

    // Add to frequency 1 list
    addToList(obj, node);

    // Minimum frequency becomes 1
    obj->minFreq = 1;

    obj->size++;
}


/* ---------------- FREE CACHE ---------------- */

void lFUCacheFree(LFUCache* obj)
{
    for (int i = 0; i < HASH_SIZE; i++)
    {
        Node *temp = obj->hashTable[i];

        while (temp != NULL)
        {
            Node *next = temp->hashNext;
            free(temp);
            temp = next;
        }
    }

    free(obj->hashTable);
    free(obj->head);
    free(obj->tail);
    free(obj);
}