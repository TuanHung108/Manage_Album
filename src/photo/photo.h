#ifndef PHOTO_H
#define PHOTO_H

#include "types.h"
#include "album/album.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


Photo makePhoto(int n);
Infor makeInfor(char *name, char *location, char *description);

void editPhotoDetails(Infor *photo);
void accessAndEditPhoto(Album *album, const char *album_name);

void insertPhoto(Album *head, const char *album_name);
void addtoAnotherAlbum(Album *head, Infor *newPhoto);

void clearPhotos(Album *head, const char *album_name);
void removePhoto(Album *head, const char *album_name);

void sortPhoto(Album *head, const char *album_name);
void swapPhoto(Infor *a, Infor *b);
int binarySearch(Album *head, const char *album_name, const char *name);
int partition(Infor *photos, int low, int high);
void qSortPhotos(Infor *photos, int low, int high);

void findPhotobyName(Album *head, const char *album_name, const char *name);
void findPhotobyIndex(Album *head, const char *album_name, int index);
void NameOrIndex(Album *head, const char *album_name);

void listPhoto(Album *head, const char *album_name);
void showPhotosInTable(Album *album, int indices[], int count);
void showPhotobyIndex(Album *head, const char *album_name, int index);

#endif 