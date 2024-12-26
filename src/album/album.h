#ifndef ALBUM_H
#define ALBUM_H

#include "types.h"
#include "menu/menu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


Album *makeAlbum(const char *album_name);
int len(Album *head);
void listAlbum(Album *head);
void insertAlbum(Album **head, const char *album_name);
void clearAlbum(Album **head);
void removeAlbum(Album **head, const char *album_name);
void accessAlbum(Album *head, const char *album_name);
Album *findAlbum(Album *head, const char *album_name);
void freeAll(Album **head);


#endif