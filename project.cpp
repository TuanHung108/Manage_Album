#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

//Information of Photos
typedef struct {
    int year;
    int month;
    int day;
} Date;

typedef struct {
    int hour;
    int minute; 
    int second;
} Time;

typedef struct {
    char name[50];
    char location[50];
    Date date;
    Time time;
    char description[100];
} Infor;

//Manage photos in one album
typedef struct {
    int num_photos;
    Infor *photo;
} Photo;

//Albums
typedef struct Album {
    int id;
    Photo photos;
    struct Album *next;
    struct Album *prev;
} Album;

//Done
Album *makeAlbum(int id);
Photo makePhoto(int n);
Infor makeInfor(char *name, char *location, Date date, Time time, char *description);
void freeAll(Album **head);

int len(Album *head);
void listAlbum(Album *head);
void insertAlbum(Album **head, int id);
void clearAlbum(Album **head);
void removeAlbum(Album **head, int id);
void showAlbum(Album *head, int id);

void listPhoto(Album *head, int id);
void showPhoto(Album *head, int album_id, char *name);
void clearPhotos(Album *head, int album_id);
void insertNewPhoto(Album *head, int id);
void removePhoto(Album *head, int id, char *name);
void findPhoto(Album *head, int id);

void Menu();
void photoMenu(Album *head, int id);

int main() {
    Album *headAlbum = NULL;
    int choice, id;
    char name[50];

    while (1) {
        Menu();
        printf("Choose an option: ");
        scanf("%d", &choice);
        system("cls");
        switch(choice) {
            case 0:
                freeAll(&headAlbum);
                return 0;
            case 1:
                printf("Enter Album ID to acces: ");
                scanf("%d", &id);
                showAlbum(headAlbum, id);
                photoMenu(headAlbum, id);
                break;
            case 2:
                printf("Enter new Album ID: ");
                scanf("%d", &id);
                insertAlbum(&headAlbum, id);
                break;
            case 3:
                printf("Enter Album ID to delete: ");
                scanf("%d", &id);
                removeAlbum(&headAlbum, id);
                break;
            case 4:
                clearAlbum(&headAlbum);
                break;
            case 5:
                listAlbum(headAlbum);
                break;
            case 6:
                printf("You have %d albums in your device.\n", len(headAlbum));
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
    return 0;
}



void Menu() {
    printf("---------------------------\n");
    printf("0. Exit\n");
    printf("1. Access album\n");
    printf("2. Insert new album\n");
    printf("3. Delete album\n");
    printf("4. Clear all albums\n");
    printf("5. List of albums\n");
    printf("6. Number of albums\n");
    printf("---------------------------\n");
}

void photoMenu(Album *head, int album_id) {
    int choice, id = album_id;
    char name[50];
    while (1) {
        printf("---------------------------\n");
        printf("0. Back to main menu\n");
        printf("1. Access photo\n");
        printf("2. Insert new photo\n");
        printf("3. Remove photo\n");
        printf("4. Clear all photos\n");
        printf("5. Find photo\n");
        printf("7. List photos\n");
        printf("---------------------------\n");
        
        printf("Choose an option: ");
        scanf("%d", &choice);

        system("cls");

        switch(choice) {
            case 0:
                return;
            case 1:
                printf("Enter photo name to access: ");
                scanf("%s", name);
                showPhoto(head, id, name);
                break;
            case 2:
                insertNewPhoto(head, id);
                break;
            case 3:
                printf("Enter photo name to delete: ");
                scanf("%s", name);
                removePhoto(head, id, name);
                break;
            case 4:
                clearPhotos(head, id);
                break;
            case 5:
                findPhoto(head, id);
                break;
            case 7:
                listPhoto(head, id);
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}

//Mode: Find photo based on name/location/date/time/description
void findPhoto(Album *head, int id) {
    char feature[20], search[50];
    printf("Which features do you want to find?\n");
    printf("(Name/Location/Description)\n");
    printf("Choose feature: ");
    scanf("%s", feature);

    printf("Enter information to search: ");
    scanf("%s", search);

    while (head != NULL) {
        if (head->id == id) {
            for (int i = 0; i < head->photos.num_photos; i++) {
                int match = 0;
                if (strcmp("Name", feature) == 0 && strcmp(head->photos.photo[i].name, search) == 0) {
                    match = 1;
                }
                if (strcmp("Location", feature) == 0 && strcmp(head->photos.photo[i].location, search) == 0) {
                    match = 1;
                }
                if (strcmp("Description", feature) == 0 && strcmp(head->photos.photo[i].description, search) == 0) {
                    match = 1;
                }
                if (match) {
                    printf("Photo %d:\n", i + 1);
                    printf("Name: %s\n", head->photos.photo[i].name);
                    printf("Location: %s\n", head->photos.photo[i].location);
                    printf("Date: %d/%d/%d\n", head->photos.photo[i].date.day, head->photos.photo[i].date.month, head->photos.photo[i].date.year);
                    printf("Time: %d:%d:%d\n", head->photos.photo[i].time.hour, head->photos.photo[i].time.minute, head->photos.photo[i].time.second);
                    printf("Description: %s\n", head->photos.photo[i].description);
                    printf("\n");
                }
            }
            return;
        }
        head = head->next;
    }
}


//Mode: Access (which album and photo)
void showAlbum(Album *head, int id) {
    if (head == NULL) {
        printf("There are no albums!\n");
        return;
    }
    while (head != NULL) {
        if (head->id == id) {
            printf("Album ID: %d\n", id);
            printf("Number of photos: %d\n", head->photos.num_photos);
            return;
        }
        head = head->next;
    }
    printf("Album ID %d does not exist.\n", id);
}

void showPhoto(Album *head, int album_id, char *name) {
    while (head != NULL) {
        if (head->id == album_id) {
            for (int i = 0; i < head->photos.num_photos; i++) {
                if (strcmp(head->photos.photo[i].name, name) == 0) {
                    printf("Photo %d:\n", i + 1);
                    printf("Name: %s\n", head->photos.photo[i].name);
                    printf("Location: %s\n", head->photos.photo[i].location);
                    printf("Date: %d/%d/%d\n", head->photos.photo[i].date.day, head->photos.photo[i].date.month, head->photos.photo[i].date.year);
                    printf("Time: %d:%d:%d\n", head->photos.photo[i].time.hour, head->photos.photo[i].time.minute, head->photos.photo[i].time.second);
                    printf("Description: %s\n", head->photos.photo[i].description);
                }
                return;
            }
            printf("Photo %s does not exist\n", name);
            return;
        }
        head = head->next;
    }
}

//Mode: Insert
void insertAlbum(Album **head, int id) {
    Album *newAlbum = makeAlbum(id);
    if (*head == NULL) {
        *head = newAlbum;
        return;
    }
    Album *temp = *head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = newAlbum;
    newAlbum->prev = temp;
}

void insertNewPhoto(Album *head, int id) {
    while (head != NULL) {
        if (head->id == id) {
            head->photos.num_photos++;
            head->photos.photo = (Infor *)realloc(head->photos.photo, head->photos.num_photos * sizeof(Infor));
            if (head->photos.photo == NULL) {
                printf("Cannot insert new photo!\n");
                head->photos.num_photos--;
                return;
            }

            //Enter information for new photo
            Infor *newPhoto  = &head->photos.photo[head->photos.num_photos - 1];
            printf("Enter name: ");
            scanf(" %[^\n]s", newPhoto->name);
            printf("Enter location: ");
            scanf(" %[^\n]s", newPhoto->location);

            printf("Enter date (yyyy mm dd): ");
            scanf("%d %d %d", &newPhoto->date.year, &newPhoto->date.month, &newPhoto->date.day);

            printf("Enter time (hh mm ss): ");
            scanf("%d %d %d", &newPhoto->time.hour, &newPhoto->time.minute, &newPhoto->time.second);

            printf("Enter description: ");
            scanf(" %[^\n]s", newPhoto->description);

            printf("New photo has been added to Album ID %d!\n", head->id);
        }
        head = head->next;
    }
}

//Mode: Delete
void removeAlbum(Album **head, int id) {
    if (*head == NULL) {
        printf("No albums to delete!\n");
        return;
    }
    Album *delAlbum = *head;
    while (delAlbum != NULL) {
        if (delAlbum->id == id) {
            if (delAlbum == *head) {
                *head = delAlbum->next;
                if (*head != NULL) (*head)->prev = NULL;
            }
            else {
                if (delAlbum->next != NULL) {
                    delAlbum->next->prev = delAlbum->prev;
                }
                if (delAlbum->prev != NULL) {
                    delAlbum->prev->next = delAlbum->next;
                }
            }
            free(delAlbum->photos.photo);
            free(delAlbum);
            printf("Album with ID %d deleted.\n", id);
            return;
        }
        delAlbum = delAlbum->next;
    }
    printf("Album with ID %d not found.\n", id);
}

void removePhoto(Album *head, int id, char *name) {
    while (head != NULL) {
        if (head->id == id) {
            int found = 0;
            for (int i = 0; i < head->photos.num_photos; i++) {
                if (strcmp(head->photos.photo[i].name, name) == 0) {
                    found = 1;  //mark when find photo
                    //move remaining photos up one position
                    for (int j = i; j < head->photos.num_photos - 1; j++) {
                        head->photos.photo[j] = head->photos.photo[j + 1];
                    }
                    head->photos.num_photos--;
                    //if there are any photos then decrease memory
                    if (head->photos.num_photos > 0) {
                        Infor *temp = (Infor *)realloc(head->photos.photo, head->photos.num_photos * sizeof(Infor));
                        if (temp != NULL) {
                            head->photos.photo = temp;
                        } else {
                            printf("Photo deletion error.\n");
                            return;
                        }
                    } else {
                        free(head->photos.photo);
                        head->photos.num_photos = 0;
                        head->photos.photo = NULL;
                    }
                    printf("Photo %s has been deleted!\n", name);
                    return;
                }
            }
            if (!found) {
                printf("Photo %s does not exist.\n", name);
            }
        }
        head = head->next;
    }
}

//Mode: Clear all
void clearAlbum(Album **head) {
    if (*head == NULL) {
        printf("No albums to delete!\n");
        return;
    }
    Album *delAlbum = *head;
    while (delAlbum != NULL) {
        printf("...Deleting album ID %d...\n", delAlbum->id);
        if (delAlbum->photos.photo != NULL) {
            free(delAlbum->photos.photo);
            delAlbum->photos.photo = NULL;
            delAlbum->photos.num_photos = 0;
        }
        Album *nextAlbum = delAlbum->next;
        free(delAlbum);
        delAlbum = nextAlbum;
    }
    *head = NULL;
    printf("All albums are deleted.\n");
}

void clearPhotos(Album *head, int album_id) {
    int id = album_id;
    while (head != NULL) {
        if (head->id == id) {
            if (head->photos.photo != NULL) {
                printf("...Deleting photos in album ID %d...\n", id);
                free(head->photos.photo);
                head->photos.photo = NULL;
                head->photos.num_photos = 0;
                printf("All photos are deleted!\n");
            } else {
                printf("No photos to delete\n");
            }
            return;
        }
        head = head->next;
    }
}

//Constructor ALbum, Photo and Information
Album *makeAlbum(int id) {
    Album *newAlbum = (Album *)malloc(sizeof(Album));
    if (newAlbum == NULL) {
        printf("Cannot create new album!\n");
        return NULL;
    }
    newAlbum->id = id;
    newAlbum->photos.num_photos = 0;
    newAlbum->photos.photo = NULL;
    newAlbum->next = NULL;
    newAlbum->prev = NULL;
    return newAlbum;
}

Photo makePhoto(int n) {
    Photo newPhoto;
    newPhoto.num_photos = n;
    newPhoto.photo = (Infor *)malloc(n*sizeof(Infor));
    if (newPhoto.photo == NULL) {
        printf("Cannot create new photo\n");
        newPhoto.num_photos = 0;
        return newPhoto;
    }
    return newPhoto;
}

Infor makeInfor(char *name, char *location, Date date, Time time, char *description) {
    Infor newInfor;
    strncpy(newInfor.name, name, sizeof(newInfor.name) - 1);
    newInfor.name[sizeof(newInfor.name) - 1] = '\0';

    strncpy(newInfor.location, location, sizeof(newInfor.location) - 1);
    newInfor.location[sizeof(newInfor.location) - 1] = '\0';
    
    newInfor.date = date;
    newInfor.time = time;

    strncpy(newInfor.description, description, sizeof(newInfor.description) - 1);
    newInfor.description[sizeof(newInfor.description) - 1] = '\0';

    return newInfor;
}

//Show List of Albums and Photos
void listAlbum(Album *head) {
    if (head == NULL) {
        printf("There are no albums!\n");
        return;
    }
    printf("List of albums:\n");
    while (head != NULL) {
        printf("Album ID: %d. Number of photos: %d\n", head->id, head->photos.num_photos);
        head = head->next;
    }
    printf("\n");
}

void listPhoto(Album *head, int id) {
    while (head != NULL) {
        if (head->id == id) {
            if (head->photos.num_photos == 0 || head->photos.photo == NULL) {
                printf("No photos in Album %d!\n", id);
                return;
            }

            printf("List photos of Album %d:\n", id);
            for (int i = 0; i < head->photos.num_photos; i++) {
                printf("Photo %d:\n", i + 1);
                printf("Name: %s\n", head->photos.photo[i].name);
                printf("Location: %s\n", head->photos.photo[i].location);
                printf("Date: %d/%d/%d\n", head->photos.photo[i].date.day, head->photos.photo[i].date.month, head->photos.photo[i].date.year);
                printf("Time: %d:%d:%d\n", head->photos.photo[i].time.hour, head->photos.photo[i].time.minute, head->photos.photo[i].time.second);
                printf("Description: %s\n", head->photos.photo[i].description);
                printf("-------------------------\n");
            }
            return;
        }
        head = head->next;
    }
}

//Other functions
void freeAll(Album **head) {
    Album *current = *head;
    Album *nextAlbum;
    while (current != NULL) {
        nextAlbum = current->next;
        free(current->photos.photo);
        free(current);
        current = nextAlbum;
    }
    *head = NULL;
    printf("All albums and photos have been freed.\n");
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
