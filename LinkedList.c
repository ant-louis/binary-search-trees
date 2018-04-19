/* ========================================================================= *
 * LinkedList definition
 * ========================================================================= */


#include <stddef.h>
#include <stdlib.h>
#include "LinkedList.h"



LinkedList* newLinkedList(void)
{
    LinkedList* ll = malloc(sizeof(LinkedList));
    if (!ll)
        return NULL;
    ll->head = NULL;
    ll->last = NULL;
    ll->size = 0;
    return ll;
}



void freeLinkedList(LinkedList* ll, bool freeContent)
{
    // Free LLNodes
    LLNode* node = ll->head;
    LLNode* prev = NULL;
    while(node != NULL)
    {
        prev = node;
        node = node->next;
        if(freeContent)
            free((void*)prev->value); // Discard const qualifier
        free(prev);
    }
    // Free LinkedList sentinel
    free(ll);
}


size_t sizeOfLinkedList(const LinkedList* ll)
{
    return ll->size;
}


bool insertInLinkedList(LinkedList* ll, const void* value)
{
    LLNode* node = malloc(sizeof(LLNode));
    if(!node)
        return false;
    // Initialisation
    node->next = NULL;
    node->value = value;
    // Adding the node to the list
    if(!ll->last)
    {
        // First element in the list
        ll->last = node;
        ll->head = node;
    } else {
        //At least one element in the list
        ll->last->next = node;
        ll->last = node;
    }
    // In both cases, increment size
    ll->size++;
    return true;
}

LinkedList* filterLinkedList(LinkedList* ll,  bool keepIt_fn_t(const void*))
{
    LinkedList* filtered = newLinkedList();
    if (!filtered)
        return NULL;
    LLNode* curr = ll->head;
    bool error = false;
    while(!error && curr != NULL)
    {
        if(keepIt_fn_t(curr->value))
            error = error  || !insertInLinkedList(filtered, curr->value);
        curr = curr->next;
    }
    if(error)
    {
        freeLinkedList(filtered, false);
        return NULL;
    }
    return filtered;
}
