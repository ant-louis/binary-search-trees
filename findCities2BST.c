#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "intersect.h"
#include "BinarySearchTree.h"
#include "LinkedList.h"
#include "City.h"
#include "findCities.h"

/*---------------------------------------PROTOTYPESS------------------------------------------------------*/

/*------------------------------------------comparison_double-------------------------------------*
*
* Fonction de comparaison pour des "double" (latitude et longitude).
*
* ARGUMENTS :
*	a                Premier élément à comparer
*	b                Second élément à comparer
*
* RETOURNE :
*	res              Un entier négatif si a < b.
*					 Un entier positif si a > b.
*					 0 si a = b.
*---------------------------------------------------------------------------------------------*/
static int comparison_double(const void* a, const void* b);


/*------------------------------------------comparison_city-------------------------------------*
*
* Fonction de comparaison pour des structures de données "City".
*
* ARGUMENTS :
*	a                Premier élément à comparer
*	b                Second élément à comparer
*
* RETOURNE :
*	res           	  0 si a == b, 1 sinon.
*---------------------------------------------------------------------------------------------*/
static int comparison_city(const void* a, const void* b);



/*--------------------------------------IMPLEMENTATION---------------------------------------------------*/


LinkedList* findCities(LinkedList* cities, double latitudeMin, double latitudeMax, 
										   double longitudeMin, double longitudeMax)
{
	//Création des deux arbres binaires
	BinarySearchTree* citiesBSTLat = newBST(&comparison_double);
	if(citiesBSTLat == NULL)
		return NULL;
	BinarySearchTree* citiesBSTLong = newBST(&comparison_double);
	if(citiesBSTLong == NULL){
		freeBST(citiesBSTLat, false);
		return NULL;
	}
	// Remplissage des arbres 
	LLNode* current = cities->head;
	const City* city;
	while(current)
	{
		city = (const City*)current->value;

		if(!insertInBST(citiesBSTLat, &(city->latitude), city))
		{
			freeBST(citiesBSTLat, false);
        	return NULL;
		}

		if(!insertInBST(citiesBSTLong, &(city->longitude), city))
		{
			freeBST(citiesBSTLong, false);
        	return NULL;
		}
		current = current->next;
	}

	// Crée une liste liée contenant l'ensemble des villes comprises entre deux latitudes
	LinkedList* citiesLat = getInRange(citiesBSTLat, &latitudeMin, &latitudeMax);
	if(citiesLat == NULL){
		freeBST(citiesBSTLat, false);
		freeBST(citiesBSTLong, false);
		return NULL;
	}
	// Crée une liste liée contenant l'ensemble des villes comprises entre deux longitudes
	LinkedList* citiesLong = getInRange(citiesBSTLong, &longitudeMin, &longitudeMax);
	if(citiesLong == NULL){
		freeBST(citiesBSTLat, false);
		freeBST(citiesBSTLong, false);
		freeLinkedList(citiesLat, false);
		return NULL;
	}
	// Calcule l'intersection des deux listes
	LinkedList* intersectList = intersect(citiesLat, citiesLong, &comparison_city);
	if(intersectList == NULL){
		freeBST(citiesBSTLat, false);
		freeBST(citiesBSTLong, false);
		freeLinkedList(citiesLat, false);
		freeLinkedList(citiesLong, false);
		return NULL;
	}

    // Libère la mémoire allouée au cours de cette fonction
	freeBST(citiesBSTLat, false);
	freeBST(citiesBSTLong, false);
	freeLinkedList(citiesLat, false);
	freeLinkedList(citiesLong, false);

	return intersectList;
}


static int comparison_double(const void* a, const void* b)
{
	const double* a_ = (const double*)a;
 	const double* b_ = (const double*)b;
 	return (*a_ > *b_) - (*a_ < *b_);
}


static int comparison_city(const void* a, const void* b)
{
	const City* A = (const City*)a;
 	const City* B = (const City*)b;
 	if((A->latitude == B->latitude) && (A->longitude == B->longitude))
 		return 0;
 	return 1;
}