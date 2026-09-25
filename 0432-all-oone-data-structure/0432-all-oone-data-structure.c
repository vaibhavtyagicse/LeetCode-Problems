#include <stdlib.h>
#include <string.h>

#define MAX_KEYS 50005
#define KEY_LEN 11
#define HASH_SIZE 100003

typedef struct KeyNode {
    char key[KEY_LEN];
    struct Bucket *bucket;
    struct KeyNode *nextHash;
    struct KeyNode *nextKey;
} KeyNode;

typedef struct Bucket {
    int count;

    KeyNode *keys;

    struct Bucket *prev;
    struct Bucket *next;
} Bucket;

typedef struct {
    Bucket *head;
    Bucket *tail;

    KeyNode *hash[HASH_SIZE];
} AllOne;


/* ---------------- HASH FUNCTION ---------------- */

unsigned int hashFunction(char *key)
{
    unsigned long hash = 5381;

    while (*key)
    {
        hash = ((hash << 5) + hash) + *key;
        key++;
    }

    return hash % HASH_SIZE;
}


/* ---------------- FIND KEY ---------------- */

KeyNode* findKey(AllOne* obj, char* key)
{
    unsigned int index = hashFunction(key);

    KeyNode *temp = obj->hash[index];

    while (temp != NULL)
    {
        if (strcmp(temp->key, key) == 0)
            return temp;

        temp = temp->nextHash;
    }

    return NULL;
}


/* ---------------- ADD KEY TO HASH ---------------- */

void addToHash(AllOne* obj, KeyNode* node)
{
    unsigned int index = hashFunction(node->key);

    node->nextHash = obj->hash[index];
    obj->hash[index] = node;
}


/* ---------------- REMOVE KEY FROM HASH ---------------- */

void removeFromHash(AllOne* obj, char* key)
{
    unsigned int index = hashFunction(key);

    KeyNode *temp = obj->hash[index];
    KeyNode *prev = NULL;

    while (temp != NULL)
    {
        if (strcmp(temp->key, key) == 0)
        {
            if (prev == NULL)
                obj->hash[index] = temp->nextHash;
            else
                prev->nextHash = temp->nextHash;

            return;
        }

        prev = temp;
        temp = temp->nextHash;
    }
}


/* ---------------- CREATE BUCKET ---------------- */

Bucket* createBucket(int count)
{
    Bucket *bucket = malloc(sizeof(Bucket));

    bucket->count = count;
    bucket->keys = NULL;

    bucket->prev = NULL;
    bucket->next = NULL;

    return bucket;
}


/* ---------------- ADD KEY TO BUCKET ---------------- */

void addKeyToBucket(Bucket *bucket, KeyNode *node)
{
    node->nextKey = bucket->keys;
    bucket->keys = node;

    node->bucket = bucket;
}


/* ---------------- REMOVE KEY FROM BUCKET ---------------- */

void removeKeyFromBucket(Bucket *bucket, KeyNode *node)
{
    KeyNode *temp = bucket->keys;
    KeyNode *prev = NULL;

    while (temp != NULL)
    {
        if (temp == node)
        {
            if (prev == NULL)
                bucket->keys = temp->nextKey;
            else
                prev->nextKey = temp->nextKey;

            node->nextKey = NULL;

            return;
        }

        prev = temp;
        temp = temp->nextKey;
    }
}


/* ---------------- REMOVE BUCKET ---------------- */

void removeBucket(AllOne *obj, Bucket *bucket)
{
    bucket->prev->next = bucket->next;
    bucket->next->prev = bucket->prev;

    free(bucket);
}


/* ---------------- CREATE ALLONE ---------------- */

AllOne* allOneCreate()
{
    AllOne *obj = malloc(sizeof(AllOne));

    memset(obj->hash, 0, sizeof(obj->hash));

    obj->head = createBucket(0);
    obj->tail = createBucket(0);

    obj->head->next = obj->tail;
    obj->tail->prev = obj->head;

    return obj;
}


/* ---------------- INC ---------------- */

void allOneInc(AllOne* obj, char* key)
{
    KeyNode *node = findKey(obj, key);

    /* Key does not exist */
    if (node == NULL)
    {
        node = malloc(sizeof(KeyNode));

        strcpy(node->key, key);

        node->bucket = NULL;
        node->nextHash = NULL;
        node->nextKey = NULL;

        addToHash(obj, node);

        Bucket *bucket;

        /* Check if bucket with count 1 exists */
        if (obj->head->next->count == 1)
        {
            bucket = obj->head->next;
        }
        else
        {
            bucket = createBucket(1);

            bucket->next = obj->head->next;
            bucket->prev = obj->head;

            obj->head->next->prev = bucket;
            obj->head->next = bucket;
        }

        addKeyToBucket(bucket, node);

        return;
    }

    /* Existing key */
    Bucket *current = node->bucket;

    Bucket *nextBucket;

    if (current->next->count == current->count + 1)
    {
        nextBucket = current->next;
    }
    else
    {
        nextBucket = createBucket(current->count + 1);

        nextBucket->next = current->next;
        nextBucket->prev = current;

        current->next->prev = nextBucket;
        current->next = nextBucket;
    }

    removeKeyFromBucket(current, node);

    addKeyToBucket(nextBucket, node);

    if (current->keys == NULL)
        removeBucket(obj, current);
}


/* ---------------- DEC ---------------- */

void allOneDec(AllOne* obj, char* key)
{
    KeyNode *node = findKey(obj, key);

    if (node == NULL)
        return;

    Bucket *current = node->bucket;

    /* Count becomes zero */
    if (current->count == 1)
    {
        removeKeyFromBucket(current, node);

        removeFromHash(obj, key);

        free(node);

        if (current->keys == NULL)
            removeBucket(obj, current);

        return;
    }

    Bucket *prevBucket;

    if (current->prev->count == current->count - 1)
    {
        prevBucket = current->prev;
    }
    else
    {
        prevBucket = createBucket(current->count - 1);

        prevBucket->prev = current->prev;
        prevBucket->next = current;

        current->prev->next = prevBucket;
        current->prev = prevBucket;
    }

    removeKeyFromBucket(current, node);

    addKeyToBucket(prevBucket, node);

    if (current->keys == NULL)
        removeBucket(obj, current);
}


/* ---------------- GET MAX KEY ---------------- */

char* allOneGetMaxKey(AllOne* obj)
{
    if (obj->tail->prev == obj->head)
        return "";

    return obj->tail->prev->keys->key;
}


/* ---------------- GET MIN KEY ---------------- */

char* allOneGetMinKey(AllOne* obj)
{
    if (obj->head->next == obj->tail)
        return "";

    return obj->head->next->keys->key;
}


/* ---------------- FREE ---------------- */

void allOneFree(AllOne* obj)
{
    Bucket *bucket = obj->head;

    while (bucket != NULL)
    {
        Bucket *next = bucket->next;

        KeyNode *node = bucket->keys;

        while (node != NULL)
        {
            KeyNode *nextNode = node->nextKey;
            free(node);
            node = nextNode;
        }

        free(bucket);

        bucket = next;
    }

    free(obj);
}