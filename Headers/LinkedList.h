/* ========================================================================= *
 * LinkedList interface:
 * Note that contrary to the Binary Search Tree, the linked list structure
 * is not opaque so that you can manipulate it directly.
 * ========================================================================= */

#ifndef _LINKED_LIST_H_
#define _LINKED_LIST_H_

#include <stddef.h>
#include <stdbool.h>


typedef struct llnode_t {
    const void* value;
    struct llnode_t* next;

} LLNode;

typedef struct linkedlist_t {
    size_t size;
    LLNode* head;
    LLNode* last;
}LinkedList;

/* ------------------------------------------------------------------------- *
 * Creates an empty LinkedList
 *
 * The LinkeedList must later be deleted by calling freeLinkedList().
 *
 * RETURN
 * linkedList    A pointer to the LinkedList, or NULL in case of error
 *
 * ------------------------------------------------------------------------- */

LinkedList* newLinkedList(void);


/* ------------------------------------------------------------------------- *
 * Frees the allocated memory of the given LinkedList.
 *
 * PARAMETERS
 * ll           A valid pointer to a LinkedList object
 * freeContent  Whether to free the content as well.
 *
 * NOTE
 * The const qualifier will be exceptionally discarded if freeContent == true
 * to allow the deletion of the content.
 * ------------------------------------------------------------------------- */

void freeLinkedList(LinkedList* ll, bool freeContent);

/* ------------------------------------------------------------------------- *
 * Counts the number of elements stored in the given LinkedList.
 *
 * PARAMETERS
 * ll           A valid pointer to a LinkedList object
 *
 * RETURN
 * nb           The amount of elements stored in linked list
 * ------------------------------------------------------------------------- */

size_t sizeOfLinkedList(const LinkedList* ll);


/* ------------------------------------------------------------------------- *
 * Inserts a new element in the linked list.
 *
 * PARAMETERS
 * ll           A valid pointer to a LinkedList object
 * value        The value to store
 *
 * RETURN
 * res          A boolean equal to true if the new element was successfully
 *              inserted, false otherwise (error)
 * ------------------------------------------------------------------------- */

bool insertInLinkedList(LinkedList* ll, const void* value);


/* ------------------------------------------------------------------------- *
 * Return a new linked list containing (the pointer to) the element of the
 * original linked list for which the keepIt predicate is true.
 *
 * PARAMETERS
 * ll           A valid pointer to a LinkedList object
 * keepIt       A predicate operating on the values of the linked list
 *
 * RETURN
 * filtered     A (possibly empty) linked list or NULL in case of error
 *
 * USAGE (example for strings)
 * bool startByH(const void* string)
 * {
 *     const char* string_ = string;
 *     return string_[0] == 'h';
 * }
 * ...
 * LinkedList* filtered = filterLinkedList(ll, &startByH);
 * ------------------------------------------------------------------------- */

LinkedList* filterLinkedList(LinkedList* ll,  bool keepIt_fn_t(const void*));



#endif // !_LINKED_LIST_H_
