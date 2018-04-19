/* ------------------------------------------------------------------------- *\
 * NOM
 *      boxsearch
 * SYNOPSIS
 *      boxsearch cities.csv lat1 lon1 lat2 lon2
 * DESCIRPTION
 *      Loads filter the cities outside the box (lat1, lon1), (lat1, lon2)
 *      (lat2, lon2), (lat2, lon1).
 * USAGE
 *      ./boxsearch cities.csv lat1 lon1 lat2 lon2
 \* ------------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "City.h"
#include "LinkedList.h"
#include "findCities.h"

// Max length of a single line in a .csv file
static const size_t LINE_SIZE = 512;




/* ------------------------------------------------------------------------- *
 * Parse a CSV file containing cities.
 * This CSV must start with a header row and have three columns:
 *   1) Name: the name of the city (delimited with double quotes)
 *   2) Latitude: the latitude of the city in degree (floating-point value)
 *   3) Longitude: the longitude of the city in degree (floating-point value)
 *
 * PARAMETERS
 * filename     A null-terminated string containing the name of the CSV file
 *
 * RETURN
 * cities       A linked list containing the cities
 * ------------------------------------------------------------------------- */
static LinkedList* parseCsv(const char* filename)
{
    // Opens the file
    FILE* fileObj = fopen(filename, "r");
    if (fileObj == NULL)
    {
        fprintf(stderr, "Could not open file '%s'. Exiting...\n", filename);
        exit(EXIT_FAILURE);
    }
    LinkedList* cities = newLinkedList();
    if (!cities)
    {
        fprintf(stderr, "Allocation error. Exiting...\n");
        exit(EXIT_FAILURE);
    }


    // Start reading the file, line by line (N.B.: fgets() reads until next \n or EOF character)
    char line[LINE_SIZE];
    size_t nbLine = 0;
    size_t nbCity = 0;
    char delim = ',';
    char stringDelim = '"';
    City* city;
    size_t currChar, nameStart, nameEnd, nameLen, latitudeStart, longitudeStart;
    while (fgets(line, sizeof(line), fileObj) != NULL)
    {
        if (nbLine == 0) { nbLine++; continue; } // Skip header

        city = malloc(sizeof(City));
        if(!city)
        {
            fprintf(stderr, "Allocation error at line %zu. Exiting...\n", nbLine);
            exit(EXIT_FAILURE);
        }
        city->latitude = 0.0;
        city->longitude = 0.0;
        city->name = NULL;


        currChar = 0;

        // Skip anything before the first string delimiter of the city name
        while (line[currChar++] != stringDelim) { }

        nameStart = currChar;

        // Find position of the end of the city name
        while (line[currChar] != stringDelim) { currChar++; }

        nameEnd = currChar, nameLen = nameEnd - nameStart; // Index one past the end of the name

        // City name
        city->name = malloc(sizeof(char) * (nameLen + 1));
        if (!city->name) {
            fprintf(stderr, "Allocation error: city name at line %zu. Exiting...\n", nbLine);
            exit(EXIT_FAILURE);
        }
        memcpy(city->name, line + nameStart, sizeof(char) * nameLen);
        city->name[nameLen] = '\0';

        // Skip until the latitude start
        while (line[currChar++] != delim) { }

        latitudeStart = currChar;

        // Latitude
        city->latitude = strtod(line + latitudeStart, NULL);

        // Skip until delimiter
        while (line[currChar++] != delim) { }

        longitudeStart = currChar;

        // Longitude
        city->longitude = strtod(line + longitudeStart, NULL);

        if(!insertInLinkedList(cities, (const void*)city))
        {
            fprintf(stderr, "Allocation error: insertion at line %zu. Exiting...\n", nbLine);
            exit(EXIT_FAILURE);
        }

        nbLine++;
        nbCity++;
    }
    fclose(fileObj);
    return cities;
}



int main(int argc, char** argv)
{
    // Parse command line input
    if (argc  != 6)
    {
        fprintf(stderr, "%s is expecting exactly five parameters: "
                        "(1) a .csv file containing the cities and their coordinates, "
                        "(2) the first latitude, (3) the first longitude, "
                        "(4) the second latitude and (5) the second longitude\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    char* fileName = argv[1];
    double latitudeA = strtod(argv[2], NULL);
    double longitudeA = strtod(argv[3], NULL);
    double latitudeB = strtod(argv[4], NULL);
    double longitudeB = strtod(argv[5], NULL);

    // Compute min/max latitude/longitude
    double latitudeMin = (latitudeA < latitudeB)? latitudeA: latitudeB;
    double latitudeMax = (latitudeA < latitudeB)? latitudeB: latitudeA;
    double longitudeMin = (longitudeA < longitudeB)? longitudeA: longitudeB;
    double longitudeMax = (longitudeA < longitudeB)? longitudeB: longitudeA;

    // Parse csv file
    LinkedList* cities = parseCsv(fileName);
    long unsigned int nbCities = sizeOfLinkedList(cities);
    printf("Number of cities: %lu\n", nbCities);


    // Compute the cities in the box
    LinkedList* citiesInBox = findCities(cities, latitudeMin, latitudeMax,
                                         longitudeMin, longitudeMax);
    if(!citiesInBox)
    {
        fprintf(stderr, "Allocation error while finding the cities. Exiting...\n");
        exit(EXIT_FAILURE);
    }

    
    // Print stuff
    nbCities = sizeOfLinkedList(citiesInBox);
    printf("Number of cities after filtering: %lu\n", nbCities);
    if(nbCities <= 50)
    {
        LLNode* node = citiesInBox->head;
        City* city;
        while(node != NULL)
        {
            city = (City*)node->value;
            printf("%s (%f, %f)\n", city->name, city->latitude, city->longitude);
            node = node->next;
        }
    }

    // Free the linked list: the name must be freed beforehand
    freeLinkedList(citiesInBox, false);
    LLNode* node = cities->head;
    City* city;
    while(node != NULL)
    {
        city = (City*)node->value;
        free(city->name);
        node = node->next;
    }
    freeLinkedList(cities, true);

    exit(EXIT_SUCCESS);
}
