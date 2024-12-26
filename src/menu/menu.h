#ifndef MENU_H
#define MENU_H

#include "constants.h"
#include "photo/photo.h"
#include <ctype.h>

struct Album;
typedef struct Album Album;

void Menu();
void photoMenu(Album *head, const char *album_name);
int confirm(const char *message);
int choice_checked();


#endif
