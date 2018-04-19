#include <stddef.h>
#include <stdbool.h>
#include "intersect.h"
#include "LinkedList.h"

LinkedList* intersect(const LinkedList* listA, const LinkedList* listB, int comparison_fn_t(const void *, const void *))
{
	LinkedList* intersection = newLinkedList();
	if(intersection == NULL)
		return NULL;

	if(listA == NULL || listB == NULL)
		return intersection;

	LLNode* currentA = listA->head;
	LLNode* currentB = listB->head;
	
	while(currentA)
	{	
		// On repart du début de la liste B
		currentB = listB->head;
		// Boucle de recherche 
		while(currentB)
		{
			if(comparison_fn_t(currentA->value, currentB->value) == 0)
			{
				if(!insertInLinkedList(intersection, currentA->value))
				{
					freeLinkedList(intersection, false);
					return NULL;
				}
				break;
			}
			currentB = currentB->next;
		}
		// On passe à l'élément suivant de la liste A
		currentA = currentA->next;
		
	}

	return intersection;
}