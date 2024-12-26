#ifndef FILE_IO_H
#define FILE_IO_H

#include "album/album.h"
#include "photo/photo.h"
#include <stdio.h>
#include <stdlib.h>

void savetoFile(Album *head, const char *file_name);
void loadfromFile(Album **head, const char *file_name);

#endif