#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "BinarySearchTree.h"
#include "LinkedList.h"
#include "zscore.h"
#include "City.h"
#include "findCities.h"

/*--------------------------------------PROTOTYPES-----------------------------------------------*/

/*-------------------------------------------filter-----------------------------------------------
*
* Filtre une liste liée pour ne garder que les villes comprises entre deux longitudes.
*
* ARGUMENTS :
*	ll					Pointeur valide vers une liste liée contenant les villes à filtrer
*	latitudeMin      	La latitude minimum définie
* 	latitudeMax      	La latatitude maximum définie
*	longitudeMin		La longitude minimum définie
*	longitudeMax		La longitude maximum définie
*
* RETOURNE :
*	filtered		La liste filtrée contenant les villes comprises entre deux longitudes
*------------------------------------------------------------------------------------------------*/
LinkedList* filter(LinkedList* ll, double latitudeMin, double latitudeMax,
								   double longitudeMin, double longitudeMax);


/*------------------------------------------comparison_fn_t-------------------------------------*
*
* Fonction de comparaison
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
static int comparison_uint64_t(const void* a, const void* b);



/*-----------------------------------IMPLEMENTATION---------------------------------------------*/

LinkedList* findCities(LinkedList* cities, double latitudeMin, double latitudeMax, 
										   double longitudeMin, double longitudeMax)
{
	//Création de l'arbre bianire dont les clés sont une combinaison des coordonnées
	BinarySearchTree* citiesZBST = newBST(&comparison_uint64_t);
	if(citiesZBST == NULL)
		return NULL;

	// Remplissage de l'arbre
	LLNode* current = cities->head;
	const City* city;
	uint64_t* tabKeyZ = malloc(sizeof(uint64_t)*(cities->size));

	for(size_t i=0; current; i++)
	{
		city = (const City*)current->value;
		tabKeyZ[i] = zEncode(city->latitude, city->longitude);
		
		if(!insertInBST(citiesZBST, &tabKeyZ[i], city))
		{
			free(tabKeyZ);
			freeBST(citiesZBST, false);
        	return NULL;
		}
		current = current->next;
	}

    // Combinaisons min et max des clés
	uint64_t min = zEncode(latitudeMin, longitudeMin);
	uint64_t max = zEncode(latitudeMax, longitudeMax);

	// Crée une liste liée contenant l'ensemble des villes comprises entre min et max
	LinkedList* citiesList = getInRange(citiesZBST, &min, &max);
	if(citiesList == NULL){
		free(tabKeyZ);
		freeBST(citiesZBST, false);
		return NULL;
	}
	// Filtre les villes comprises entre les bonnes coordonnées
	LinkedList* filteredList = filter(citiesList, latitudeMin, latitudeMax, longitudeMin, longitudeMax);
	if(filteredList == NULL){
		free(tabKeyZ);
		freeBST(citiesZBST, false);
		freeLinkedList(citiesList, false);
		return NULL;
	}
	
	// Libère la mémoire allouée au cours de cette fonction
	free(tabKeyZ);
	freeBST(citiesZBST, false);
	freeLinkedList(citiesList, false);

	return filteredList;

}


LinkedList* filter(LinkedList* ll, double latitudeMin, double latitudeMax,
								   double longitudeMin, double longitudeMax)
{
    LinkedList* filtered = newLinkedList();
    if (!filtered)
        return NULL;

    LLNode* current = ll->head;
    bool error = false;
    const City* city;

    while(!error && current != NULL)
    {
        city = (const City*)current->value;

        if(latitudeMin <= city->latitude && city->latitude <= latitudeMax &&
           longitudeMin <= city->longitude && city->longitude <= longitudeMax)
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


static int comparison_uint64_t(const void* a, const void* b)
{
	const uint64_t* a_ = (const uint64_t*)a;
 	const uint64_t* b_ = (const uint64_t*)b;
 	return (*a_ > *b_) - (*a_ < *b_);
}