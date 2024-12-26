#include "file_io.h"

void savetoFile(Album *head, const char *file_name) {
    FILE *file = fopen(file_name, "w");
    if (file == NULL) {
        printf("Error saving data.\n");
        return;
    }

    fprintf(file, "Album Name,Photo Name,Date,Time,Location,Description\n");

    Album *current = head;
    while (current != NULL) {
        if (current->photos.num_photos > 0) {
            for (int i = 0; i < current->photos.num_photos; i++) {
                Infor *photo = &current->photos.photo[i];
                fprintf(file, "%s,%s,%s,%s,%s,%s\n",
                        current->album_name,
                        photo->name[0] ? photo->name : "Empty",
                        photo->date[0] ? photo->date : "Empty",
                        photo->time[0] ? photo->time : "Empty",
                        photo->location[0] ? photo->location : "Unknown",
                        photo->description[0] ? photo->description : "Empty");
            }
        } else {
            fprintf(file, "%s\n", current->album_name);
        }
        current = current->next;
    }

    fclose(file);
    printf("Data saved to file '%s' successfully.\n", file_name);
}

void loadfromFile(Album **head, const char *file_name) {
    FILE *file = fopen(file_name, "r");
    if (file == NULL) {
        printf("Error reading data in file '%s'.\n", file_name);
        return;
    }

    char line[256];
    if (fgets(line, sizeof(line), file) == NULL) {
        printf("File '%s' is empty.\n", file_name);
        fclose(file);
        return;
    }

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0';

        char *token = strtok(line, ",");
        if (token == NULL) continue;
        char *album_name = token;

        Album *existingAlbum = findAlbum(*head, album_name);
        if (existingAlbum == NULL) {
            // Nếu chưa tồn tại, tạo album mới và thêm vào danh sách
            Album *newAlbum = makeAlbum(album_name);
            if (*head == NULL) {
                *head = newAlbum;
            } else {
                Album *tail = *head;
                while (tail->next != NULL) {
                    tail = tail->next;
                }
                tail->next = newAlbum;
                newAlbum->prev = tail;
            }
            existingAlbum = newAlbum;
        }        
        //Tải dữ liệu ảnh của albumalbum
        Infor newPhoto = {0};
        int validPhoto = 0;
        
        token = strtok(NULL, ",");
        if (token && strlen(token) > 0) {
            strcpy(newPhoto.name, token);
            validPhoto = 1;
        }
        token = strtok(NULL, ",");
        if (token && strlen(token) > 0) {
            strncpy(newPhoto.date, token, sizeof(newPhoto.date) - 1);
            newPhoto.date[sizeof(newPhoto.date) - 1] = '\0';
            validPhoto = 1;
        }
        token = strtok(NULL, ",");
        if (token && strlen(token) > 0) {
            strncpy(newPhoto.time, token, sizeof(newPhoto.time) - 1);
            newPhoto.time[sizeof(newPhoto.time) - 1] = '\0';
            validPhoto = 1;
        }
        token = strtok(NULL, ",");
        if (token && strlen(token) > 0) {
            strcpy(newPhoto.location, token);
            validPhoto = 1;
        }
        token = strtok(NULL, ",");
        if (token && strlen(token) > 0) {
            strcpy(newPhoto.description, token);
            validPhoto = 1;
        }
        
        if (validPhoto) {
            existingAlbum->photos.num_photos++;
            Infor *temp = realloc(existingAlbum->photos.photo, existingAlbum->photos.num_photos * sizeof(Infor));
            if (temp != NULL) {
                existingAlbum->photos.photo = temp;
                existingAlbum->photos.photo[existingAlbum->photos.num_photos - 1] = newPhoto;
            } else {
                printf("Memory allocation failed while loading photo.\n");
                existingAlbum->photos.num_photos--;  // Khôi phục số lượng ảnh
            }
        }
    }
    fclose(file);
    printf("Data loaded successfully from file '%s'.\n", file_name);
}
