#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "BinarySearchTree.h"
#include "LinkedList.h"
#include "City.h"
#include "findCities.h"

/*---------------------------------------PROTOTYPESS-----------------------------------------*/

/*-------------------------------------------filter-----------------------------------------------
*
* Filtre une liste liée pour ne garder que les villes comprises entre deux longitudes.
*
* ARGUMENTS :
*	ll					Pointeur valide vers une liste liée contenant les villes à filtrer
*	longitudeMin		La longitude minimum définie
*	longitudeMax		La longitude maximum définie
*
* RETOURNE :
*	filtered		La liste filtrée contenant les villes comprises entre deux longitudes
*------------------------------------------------------------------------------------------------*/
LinkedList* filter(LinkedList* ll, double longitudeMin, double longitudeMax);


/*------------------------------------------comparison_double-------------------------------------*
*
* Fonction de comparaison pour des "double" (latitude).
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
int comparison_double(const void* a, const void* b);



/*--------------------------------------IMPLEMENTATION------------------------------------------*/

LinkedList* findCities(LinkedList* cities, double latitudeMin, double latitudeMax, 
										   double longitudeMin, double longitudeMax)
{
	//Construction de l'arbre bianire dont les clés sont les latitudes
	BinarySearchTree* citiesBST = newBST(&comparison_double);
    if(citiesBST == NULL)
        return NULL;

    // Remplissage de l'arbre
	LLNode* current = cities->head;
	const City* city;
	while(current)
	{
		city = (const City*)current->value;
		if(!insertInBST(citiesBST, &(city->latitude), city))
        {
            freeBST(citiesBST, false);
            return NULL;
        }
		current = current->next;
	}

	// Crée une liste liée contenant l'ensemble des villes comprises entre deux latitudes
	LinkedList* citiesLat = getInRange(citiesBST, &latitudeMin, &latitudeMax);
    if(citiesLat == NULL){
        freeBST(citiesBST, false);
        return NULL;
    }
    
	// Filtre les villes comprises entre deux longitudes
	LinkedList* filteredList = filter(citiesLat, longitudeMin, longitudeMax);
    if(filteredList == NULL){
        freeLinkedList(citiesLat, false);
        return NULL;
    }

    // Libère la mémoire allouée au cours de la fonction
    freeBST(citiesBST, false);
    freeLinkedList(citiesLat, false);
    
	return filteredList;
}



LinkedList* filter(LinkedList* ll, double longitudeMin, double longitudeMax)
{	
    LinkedList* filtered = newLinkedList();
    if (!filtered)
        return NULL;

    // Filtre les villes
    LLNode* current = ll->head;

    bool error = false;
    const City* city;

    while(!error && current != NULL)
    {
        city = (const City*)current->value;

        if(longitudeMin <= city->longitude && city->longitude <= longitudeMax)
            error = error  || !insertInLinkedList(filtered, current->value);
        current = current->next;
    }
    // Libère la mémoire en cas d'erreur
    if(error)
    {
        freeLinkedList(filtered, false);
        return NULL;
    }
    return filtered;
}



int comparison_double(const void* a, const void* b)
{
	const double* a_ = (const double*)a;
 	const double* b_ = (const double*)b;
 	return (*a_ > *b_) - (*a_ < *b_);
}