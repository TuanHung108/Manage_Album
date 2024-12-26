#ifndef TYPES_H
#define TYPES_H

#include "constants.h"

typedef struct {
    char name[MAX_LENGTH];
    char location[MAX_LENGTH];
    char date[DATE];
    char time[TIME];
    char description[MAX_LENGTH];
} Infor;

typedef struct {
    int num_photos;
    Infor *photo;
} Photo;

typedef struct Album {
    char album_name[MAX_LENGTH];
    Photo photos;
    struct Album *next;
    struct Album *prev;
} Album;


#endif




