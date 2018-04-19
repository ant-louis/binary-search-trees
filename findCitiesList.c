/* ========================================================================= *
 * findCities: list implementation
 * ========================================================================= */

#include "City.h"
#include "findCities.h"
#include "LinkedList.h"

LinkedList* findCities(LinkedList* cities,
                       double latitudeMin,
                       double latitudeMax,
                       double longitudeMin,
                       double longitudeMax)
{
    // Create return object
    LinkedList* filtered = newLinkedList();
    if (!filtered)
        return NULL;

    // Filter the cities
    LLNode* curr = cities->head;
    bool error = false;
    const City* city;
    while(!error && curr != NULL)
    {
        city = (const City*)curr->value;
        if(latitudeMin <= city->latitude && city->latitude <= latitudeMax &&
           longitudeMin <= city->longitude && city->longitude <= longitudeMax)
            error = error  || !insertInLinkedList(filtered, curr->value);
        curr = curr->next;
    }
    // Free in case of error
    if(error)
    {
        freeLinkedList(filtered, false);
        return NULL;
    }
    return filtered;
}


