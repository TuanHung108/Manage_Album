#include "album.h"

Album *makeAlbum(const char* album_name) {
    Album *newAlbum = (Album *)malloc(sizeof(Album));
    if (newAlbum == NULL) {
        printf("Cannot create new album!\n");
        return NULL;
    }
    strncpy(newAlbum->album_name, album_name, sizeof(newAlbum->album_name) - 1);
    newAlbum->album_name[sizeof(newAlbum->album_name) - 1] = '\0';    
    
    newAlbum->photos.num_photos = 0;
    newAlbum->photos.photo = NULL;
    newAlbum->next = NULL;
    newAlbum->prev = NULL;
    return newAlbum;
}

int len(Album *head) {
    if (head == NULL) {
        return 0;
    }
    int count = 0;
    while (head != NULL) {
        count++;
        head = head->next;
    }
    return count;
}

void listAlbum(Album *head) {
    if (head == NULL) {
        printf("There are no albums!\n");
        return;
    }
    int stt = 0;
    printf("List of albums:\n");
    printf("| %-3s | %-15s | %-20s |\n", "STT", "Album Name", "Number of Photos");
    printf("------------------------------------------\n");
    while (head != NULL) {
        printf("| %-3d | %-15s | %10d%10s |\n", ++stt, head->album_name, head->photos.num_photos, "");
        head = head->next;
    }
    printf("\n");
}

void insertAlbum(Album **head, const char *album_name) {
    if (album_name == NULL || strlen(album_name) == 0) {
        printf("Album name cannot be empty. Please try again.\n");
        return;
    }
    Album *newAlbum = makeAlbum(album_name);
    if (newAlbum == NULL) {
        printf("Failed to create new album.\n");
        return;
    }

    if (*head == NULL) {
        *head = newAlbum;
        return;
    }

    Album *current = *head;
    Album *prev = NULL;

    while (current != NULL && strcmp(current->album_name, album_name) < 0) {
        prev = current;
        current = current->next;
    }

    if (current != NULL && strcmp(current->album_name, album_name) == 0) {
        printf("'%s' already exists. Please try another name.\n", album_name);
        free(newAlbum);
        return;
    }

    if (prev == NULL) {
        newAlbum->next = *head;
        (*head)->prev = newAlbum;
        *head = newAlbum;
    } else {
        newAlbum->next = current;
        newAlbum->prev = prev;
        prev->next = newAlbum;
        if (current != NULL) current->prev = newAlbum;
    }

    printf("Album %s has been added.\n", album_name);
}

void removeAlbum(Album **head, const char *album_name) {
    if (*head == NULL) {
        printf("No albums to delete!\n");
        return;

    }

    char message[MAX_LENGTH];
    snprintf(message, sizeof(message), "Are you sure you want to delete album %s?", album_name);    
    if (confirm(message)) {
        printf("Delete confirmed.\n");
    } else {
        printf("Delete cancelled.\n");
        return;
    }

    Album *delAlbum = findAlbum(*head, album_name);

    if (delAlbum == NULL) {
        printf("Album %s not found.\n", album_name);
        return;
    }
    
    if (delAlbum == *head) {
        *head = delAlbum->next;
        if (*head != NULL) (*head)->prev = NULL;
    } else {
        if (delAlbum->next != NULL) delAlbum->next->prev = delAlbum->prev;
        if (delAlbum->prev != NULL) delAlbum->prev->next = delAlbum->next; 
    }
    free(delAlbum->photos.photo);
    free(delAlbum);
    printf("Album %s deleted.\n", album_name);
}

void accessAlbum(Album *head, const char *album_name) {
    Album *album = findAlbum(head, album_name);
    if (album == NULL) {
        printf("Album %s does not exist.\n", album_name);
        return;
    }

    printf("Album: %s\n", album_name);
    printf("Number of photos: %d\n", album->photos.num_photos);
    photoMenu(head, album_name);
}

void clearAlbum(Album **head) {
    if (*head == NULL) {
        printf("No albums to delete!\n");
        return;
    }
    while (getchar() != '\n' && !feof(stdin));    
    char message[MAX_LENGTH];
    snprintf(message, sizeof(message), "Are you sure you want to clear all albums?");
    if (confirm(message)) {
        printf("Delete confirmed.\n");
    } else {
        printf("Delete cancelled.\n");
        return;
    }

    freeAll(head);
    printf("All albums are deleted.\n");
}

Album *findAlbum(Album *head, const char *album_name) {
    while (head != NULL) {
        if (strcmp(head->album_name, album_name) == 0) {
            return head;
        }
        head = head->next;
    }
    return NULL;
}

void freeAll(Album **head) {
    Album *current = *head;
    Album *nextAlbum;

    while (current != NULL) {
        nextAlbum = current->next;

        if (current->photos.photo != NULL) {
            free(current->photos.photo);
            free(current);
        }
        current = nextAlbum;
    }
    *head = NULL;
}
