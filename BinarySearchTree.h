/* ========================================================================= *
 * BinarySearchTree interface
 * ========================================================================= */

#ifndef _BINARY_SEARCH_TREE_H_
#define _BINARY_SEARCH_TREE_H_

#include <stddef.h>
#include <stdbool.h>
#include "LinkedList.h"

/* Opaque Structure */
typedef struct tree_t BinarySearchTree;

/* ------------------------------------------------------------------------- *
 * Creates an empty BinarySearchTree (or BST).
 *
 * The BST must later be deleted by calling freeBinarySearchTree().
 *
 * ARGUMENT
 * comparison_fn_t      A comparison function
 *
 * RETURN
 * bst                  A pointer to the BinarySearchTree, or NULL in case of
 *                      error
 *
 * COMPARISON FUNCTION
 * comparison_fn_t(a, b) < 0    <=> a < b
 * comparison_fn_t(a, b) = 0    <=> a == b
 * comparison_fn_t(a, b) > 0    <=> a > b
 *
 * USAGE (example for doubles: http://www.gnu.org/software/libc/manual/html_node/Comparison-Functions.html)
 * int compare_doubles(const void* a, const void* b)
 * {
 *     const double* a_ = (const double*)a;
 *     const double* b_ = (const double*)b;
 *     return (*a_ > *b_) - (*a_ < *b_)
 * }
 * ...
 * BinarySearchTree bst = newBST(&compare_doubles);
 * ------------------------------------------------------------------------- */

BinarySearchTree* newBST(int comparison_fn_t(const void *, const void *));

/* ------------------------------------------------------------------------- *
 * Frees the allocated memory of the given BinarySearchTree.
 *
 * PARAMETERS
 * bst          A valid pointer to a BinarySearchTree object
 * freeContent  Whether to free the content as well.
 *
 * NOTE
 * The const qualifier will be exceptionally discarded if freeContent == true
 * to allow the deletion of the content.
 * ------------------------------------------------------------------------- */

void freeBST(BinarySearchTree* bst, bool freeContent);

/* ------------------------------------------------------------------------- *
 * Counts the number of elements/nodes stored in the given BinarySearchTree.
 *
 * PARAMETERS
 * bst          A valid pointer to a BinarySearchTree object
 *
 * RETURN
 * nb           The amount of elements stored in bst
 * ------------------------------------------------------------------------- */

size_t sizeOfBST(const BinarySearchTree* bst);



/* ------------------------------------------------------------------------- *
 * Inserts a new key-value pair in the provided BinarySearchTree. This
 * specific implementation of the BST must handle duplicate keys.
 *
 * PARAMETERS
 * bst          A valid pointer to a BinarySearchTree object
 * key          The key of the new element or of the element to update
 * value        The value to store
 *
 * RETURN
 * res          A boolean equal to true if the new element was successfully
 *              inserted, false otherwise
 * ------------------------------------------------------------------------- */

bool insertInBST(BinarySearchTree* bst, const void* key, const void* value);



/* ------------------------------------------------------------------------- *
 * Return the value associated to that key, if any
 *
 * PARAMETERS
 * bst          A valid pointer to a BinarySearchTree object
 * key          The key to look for
 *
 * RETURN
 * res          One of the value corresping to that key. Or NULL if the key
 *              is not present in the BST
 * ------------------------------------------------------------------------- */

const void* searchBST(BinarySearchTree* bst, const void* key);

/* ------------------------------------------------------------------------- *
 * Finds a set of elements in the provided BinarySearchTree whose the keys
 * are included in a range [key1, key2] and returns their values. The values
 * are sorted in the increasing order of the keys.
 *
 * PARAMETERS
 * bst          A valid pointer to a BinarySearchTree object
 * keyMin       Lower bound of the range (inclusive)
 * keyMax       Upper bound of the range (inclusive)
 *
 * RETURN
 * ll           A linkedList containing the element in the given range, or
 *              NULL in case of allocation error.
 *
 * NOTES
 * The linkedList must be freed but not its content
 * If no elements are in the range, the function returns an empty linked-list
 * ------------------------------------------------------------------------- */

LinkedList* getInRange(const BinarySearchTree* bst, void* keyMin, void* keyMax);


#endif // !_BINARY_SEARCH_TREE_H_
