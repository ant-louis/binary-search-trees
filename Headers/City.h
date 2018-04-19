/* ========================================================================= *
 * City interface
 * ========================================================================= */

#ifndef _CITY_H_
#define _CITY_H_

/** Data structure for storing city information */
typedef struct city_t {
    char* name;
    double latitude; // latitude
    double longitude; // longitude
} City;

#endif // !_CITY_H_
