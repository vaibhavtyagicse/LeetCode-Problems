struct Node* copyRandomList(struct Node* head) {
    struct Node *c, *d;

    if (head == NULL)
        return NULL;

    // Step 1: Insert copied nodes after original nodes
    c = head;

    while (c != NULL) {
        d = malloc(sizeof(struct Node));

        d->val = c->val;
        d->random = NULL;

        d->next = c->next;
        c->next = d;

        c = d->next;
    }

    // Step 2: Set random pointers of copied nodes
    c = head;

    while (c != NULL) {
        if (c->random != NULL)
            c->next->random = c->random->next;

        c = c->next->next;
    }

    // Step 3: Separate the two lists
    struct Node *newHead = head->next;

    c = head;
    d = newHead;

    while (c != NULL) {
        c->next = c->next->next;

        if (d->next != NULL)
            d->next = d->next->next;

        c = c->next;
        d = d->next;
    }

    return newHead;
}
