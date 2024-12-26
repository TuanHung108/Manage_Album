#include "photo.h"


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

Infor makeInfor(char *name, char *location, char *description) {
    Infor newInfor;

    strncpy(newInfor.name, name, sizeof(newInfor.name) - 1);
    newInfor.name[sizeof(newInfor.name) - 1] = '\0';

    strncpy(newInfor.location, location, sizeof(newInfor.location) - 1);
    newInfor.location[sizeof(newInfor.location) - 1] = '\0';
    

    strncpy(newInfor.description, description, sizeof(newInfor.description) - 1);
    newInfor.description[sizeof(newInfor.description) - 1] = '\0';

    return newInfor;
}

void insertPhoto(Album *head, const char *album_name) {
    Album *album = findAlbum(head, album_name);
    album->photos.num_photos++;
    album->photos.photo = realloc(album->photos.photo, album->photos.num_photos * sizeof(Infor));
    if (album->photos.photo == NULL) {
        printf("Cannot insert new photo!\n");
        album->photos.num_photos--;
        return;
    }

    //Enter information for new photo
    Infor *newPhoto  = &album->photos.photo[album->photos.num_photos - 1];
    
    printf("Enter name: ");
    scanf(" %[^\n]s", newPhoto->name);
    getchar();

    printf("Enter location (press Enter to skip): ");
    fgets(newPhoto->location, sizeof(newPhoto->location), stdin);
    size_t len = strlen(newPhoto->location);
    if (len > 0 && newPhoto->location[len - 1] == '\n') {
        newPhoto->location[len - 1] = '\0';
    }
    if (strlen(newPhoto->location) == 0) {
        strcpy(newPhoto->location, "Unknown Location");
    }

    printf("Enter description (press Enter to skip): ");
    fgets(newPhoto->description, sizeof(newPhoto->description), stdin);
    len = strlen(newPhoto->description);
    if (len > 0 && newPhoto->description[len - 1] == '\n') {
        newPhoto->description[len - 1] = '\0';
    }
    if (strlen(newPhoto->description) == 0) {
        strcpy(newPhoto->description, "Empty");
    }

    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    snprintf(newPhoto->date, sizeof(newPhoto->date), "%02d/%02d/%04d", tm->tm_mday, tm->tm_mon + 1, tm->tm_year + 1900);
    snprintf(newPhoto->time, sizeof(newPhoto->time), "%02d:%02d:%02d", tm->tm_hour, tm->tm_min, tm->tm_sec);
    
    printf("New photo has been added to Album %s!\n", album->album_name);
    addtoAnotherAlbum(head, newPhoto);
    return;
}

void addtoAnotherAlbum(Album *head, Infor *newPhoto) {
    char choice;
    printf("\nDo you want to add this photo to another album? (Y/N): ");

    while (1) {
        scanf("%c", &choice);
        if (choice == 'Y' || choice == 'y') {
            char album_name[MAX_LENGTH];
            printf("Enter album name to add: ");
            scanf(" %[^\n]s", album_name);
            Album *another_album = findAlbum(head, album_name);
            if (another_album == NULL) {
                printf("Album %s does not exist. \nPlease enter again: ", album_name);
                scanf(" %[^\n]s", album_name);
                continue;
            }

            another_album->photos.num_photos++;
            another_album->photos.photo = realloc(another_album->photos.photo, another_album->photos.num_photos * sizeof(Infor));
            if (another_album->photos.photo == NULL) {
                printf("Cannot insert photo into album %s!\n", album_name);
                another_album->photos.num_photos--;
            } else {
                Infor *copiedPhoto = &another_album->photos.photo[another_album->photos.num_photos - 1];
                *copiedPhoto = *newPhoto;
                time_t t = time(NULL);
                struct tm *tm = localtime(&t);
                snprintf(copiedPhoto->time, sizeof(copiedPhoto->time), "%02d:%02d:%02d", tm->tm_hour, tm->tm_min, tm->tm_sec);
                snprintf(copiedPhoto->date, sizeof(copiedPhoto->date), "%02d/%02d/%04d", tm->tm_mday, tm->tm_mon + 1, tm->tm_year + 1900);
                printf("This photo has been added to Album %s!\n", another_album->album_name);
            }

            printf("\nDo you want to add this photo to another album? (Y/N): ");
            getchar();
        } else if (choice == 'N' || choice == 'n') {
            return;
        } else {
            printf("Invalid input. Please enter Y or N: ");
            while (getchar() != '\n');
        }
    }
}

void removePhoto(Album *head, const char *album_name) {
    Album *album = findAlbum(head, album_name);
    char message[MAX_LENGTH];
    
    char photo_name[MAX_LENGTH];
    printf("Enter photo name: ");
    getchar();
    fgets(photo_name, MAX_LENGTH, stdin);
    photo_name[strcspn(photo_name, "\n")] = '\0'; 

    int indices[MAX_PHOTO];
    int count = 0;
    for (int i = 0; i < album->photos.num_photos; i++) {
        if (strcmp(album->photos.photo[i].name, photo_name) == 0) {
            indices[count++] = i; //save photo index
        }
    }

    if (count == 0) {
        printf("Photo with the name '%s' does not exist.\n", photo_name);
        return;
    }
    if (count == 1) {
        printf("\nOnly one photo found:\n");
        showPhotobyIndex(head, album_name, indices[0]);
        snprintf(message, sizeof(message), "Are you sure you want to delete photo %s?", photo_name);
        if (confirm(message)) {
            printf("Delete confirmed.\n");
        } else {
            printf("Delete cancelled.\n");
            return;
        }

        for (int i = indices[0]; i < album->photos.num_photos - 1; i++) {
            album->photos.photo[i] = album->photos.photo[i + 1];
        }
        album->photos.num_photos--;
        if (album->photos.num_photos > 0) {
            Infor *temp = realloc(album->photos.photo, album->photos.num_photos * sizeof(Infor));
            if (temp != NULL) {
                album->photos.photo = temp;
            } else {
                printf("Memory reallocation failed. Operation incomplete.\n");
                album->photos.num_photos++;
                return;
            }
        } else {
            free(album->photos.photo);
            album->photos.photo = NULL;
        }

        printf("Photo '%s' deleted successfully.\n", photo_name);
        return;
    }

    showPhotosInTable(album, indices, count);
    int number;
    printf("\nEnter photo ID to delete: ");
    scanf("%d", &number);
    getchar();

    int valid = 0;
    for (int i = 0; i < count; i++) {
        if (indices[i] == number - 1) {
            valid = 1;
            break;
        }
    }
    if (!valid) {
        printf("Invalid ID selected. Please try again.\n");
        return;
    }

    snprintf(message, sizeof(message), "Are you sure you want to delete photo %s - ID %d?", photo_name, number);
    if (confirm(message)) {
        printf("Delete confirmed.\n");
    } else {
        printf("Delete cancelled.\n");
        return;
    }

    for (int i = number - 1; i < album->photos.num_photos - 1; i++) {
        album->photos.photo[i] = album->photos.photo[i + 1];
    }
    album->photos.num_photos--;
    if (album->photos.num_photos > 0) {
        Infor *temp = realloc(album->photos.photo, album->photos.num_photos *sizeof(Infor));
        if (temp != NULL) {
            album->photos.photo = temp;
        } else {
            printf("Memory reallocation failed. Operation incomplete.\n");
            album->photos.num_photos++;
            return;
        }
    } else {
        free(album->photos.photo);
        album->photos.photo = NULL;
    }

    printf("Photo ID %d with the name '%s' deleted successfully.\n", number, photo_name);
}

void clearPhotos(Album *head, const char *album_name) {
    while (getchar() != '\n' && !feof(stdin));
    char message[MAX_LENGTH];
    snprintf(message, sizeof(message), "Are you sure you want to clear all photos?");
    if (confirm(message)) {
        printf("Delete confirmed.\n");
    } else {
        printf("Delete cancelled.\n");
        return;
    }

    Album *album = findAlbum(head, album_name);
    if (album->photos.photo != NULL) {
        printf("...Deleting photos in album %s...\n", album_name);
        free(album->photos.photo);
        album->photos.photo = NULL;
        album->photos.num_photos = 0;
        printf("All photos are deleted!\n");
    } else {
        printf("No photos to delete\n");
    }
    return;
}

void editPhotoDetails(Infor *photo) {
    printf("\nEditing photo '%s'.\n", photo->name);
    Infor originalPhoto = *photo;

    while (1) {
        printf("\nCurrent Details:\n");
        printf("Name: %s\n", photo->name);
        printf("Date: %s\n", photo->date);
        printf("Time: %s\n", photo->time);
        printf("Location: %s\n", photo->location);
        printf("Description: %s\n", photo->description);

        printf("\nWhat do you want to edit?\n");
        printf("1. Name\n");
        printf("2. Location\n");
        printf("3. Description\n");
        printf("0. Exit and save changes\n");
        printf("Choose an option: ");

        int choice;
        while (scanf("%d", &choice) != 1 || choice < 0 || choice > 3) {
            printf("Invalid choice. Please enter a valid option: ");
            while (getchar() != '\n');
        }
        getchar();

        if (choice == 0) {
            if (confirm("Do you want to save changes before existing?")) {
                printf("Changes saved.\n");
                return;
            } else {
                *photo = originalPhoto;
                printf("Changes discarded.\n");
                return;
            }
        }
        
        switch (choice) {
            case 1:
                printf("Enter new name: ");
                fgets(photo->name, MAX_LENGTH, stdin);
                photo->name[strcspn(photo->name, "\n")] = '\0';
                break;
            case 2:
                printf("Enter new location: ");
                fgets(photo->location, MAX_LENGTH, stdin);
                photo->location[strcspn(photo->location, "\n")] = '\0';
                break;
            case 3:
                printf("Enter new description: ");
                fgets(photo->description, MAX_LENGTH, stdin);
                photo->description[strcspn(photo->description, "\n")] = '\0';
                break;
        }
    }
}

void accessAndEditPhoto(Album *head, const char *album_name) {
    Album *album = findAlbum(head, album_name);

    char photo_name[MAX_LENGTH];
    printf("Enter photo name to access: ");
    getchar();
    fgets(photo_name, MAX_LENGTH, stdin);
    photo_name[strcspn(photo_name, "\n")] = '\0';

    int indices[MAX_PHOTO];
    int count = 0;

    for (int i = 0; i < album->photos.num_photos; i++) {
        if (strcmp(album->photos.photo[i].name, photo_name) == 0) {
            indices[count++] = i;
        }
    }

    if (count == 0) {
        printf("No photos with the name '%s' found.\n", photo_name);
        return;
    }

    int selected_id;
    if (count == 1) {
        printf("\nFound 1 photo with the name '%s'.\n", photo_name);
        selected_id = indices[0];
    } else {
        // Show anh cung ten
        showPhotosInTable(album, indices, count);
        while (1) {
            printf("\nEnter photo ID to edit (ID from %d to %d): ", indices[0] + 1, indices[count - 1] + 1);  // Hiển thị ID thực tế
            if (scanf("%d", &selected_id) != 1) {
                printf("Please enter a number.\n");
                while (getchar() != '\n');
                continue;
            }
            getchar();  // Đọc ký tự '\n' còn lại

            // Kiểm tra xem ID có hợp lệ không
            int valid = 0;
            for (int i = 0; i < count; i++) {
                if (selected_id == indices[i] + 1) {  // ID thực tế từ mảng indices (bắt đầu từ 1)
                    selected_id = indices[i];
                    valid = 1;
                    break;
                }
            }

            if (valid) {
                break;  // Chọn ID hợp lệ
            } else {
                printf("Invalid ID selected. Please try again.\n");
            }
        }
    }
    Infor *photo = &album->photos.photo[selected_id];
    editPhotoDetails(photo);
}

void swapPhoto(Infor *a, Infor *b) {
    Infor temp = *a;
    *a = *b;
    *b = temp;
}

int partition(Infor *photos, int low, int high) {
    int mid = low + (high - low) / 2;
    if (strcmp(photos[low].name, photos[mid].name) > 0) swapPhoto(&photos[low], &photos[mid]);
    if (strcmp(photos[low].name, photos[high].name) > 0) swapPhoto(&photos[low], &photos[high]);
    if (strcmp(photos[mid].name, photos[high].name) > 0) swapPhoto(&photos[mid], &photos[high]);
    swapPhoto(&photos[mid], &photos[high]);

    char pivot[MAX_LENGTH];
    strcpy(pivot, photos[high].name);
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (strcmp(photos[j].name, pivot) < 0) {
            i++;
            swapPhoto(&photos[i], &photos[j]);
        }
    }
    swapPhoto(&photos[i + 1], &photos[high]);
    return i + 1;
}

void qSortPhotos(Infor *photos, int low, int high) {
    if (low < high) {
        int pivotIndex = partition(photos, low, high);
        qSortPhotos(photos, low, pivotIndex - 1);
        qSortPhotos(photos, pivotIndex + 1, high);
    }
}

void sortPhoto(Album *head, const char *album_name) {
    Album *album = findAlbum(head, album_name);
    if (album == NULL || album->photos.num_photos <= 1) return;

    qSortPhotos(album->photos.photo, 0, album->photos.num_photos - 1);
}

int binarySearch(Album *head, const char *album_name, const char *name) {
    Album *album = findAlbum(head, album_name);
    int left = 0, right = album->photos.num_photos - 1;
    
    while (left <= right) {
        int mid = left + (right - left) / 2;
        int cmp = strcmp(album->photos.photo[mid].name, name);
        
        if (cmp == 0) return mid;
        if (cmp < 0) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return -1;
}

void showPhotobyIndex(Album *head, const char *album_name, int index) {
    Album *album = findAlbum (head, album_name);
    if (index < 0 || index >= album->photos.num_photos) {
        printf("Invalid index.\n");
        return;
    }

    printf("Photo %d:\n", index + 1);
    printf("Name: %s\n", album->photos.photo[index].name);
    printf("Location: %s\n", album->photos.photo[index].location);
    printf("Date: %s\n", album->photos.photo[index].date);
    printf("Time: %s\n", album->photos.photo[index].time);
    printf("Description: %s\n", album->photos.photo[index].description);
    return;    
}

void findPhotobyName(Album *head, const char *album_name, const char *name) {
    Album *album = findAlbum(head, album_name);
    int indices[MAX_PHOTO];
    int count = 0;
    for (int i = 0; i < album->photos.num_photos; i++) {
        if (strcmp(album->photos.photo[i].name, name) == 0) {
            indices[count++] = i;
        }
    }

    if (count == 0) {
        printf("Photo with name '%s' not found.\n", name);
        return;
    } else if (count == 1) {
        int index = binarySearch(head, album_name, name);
        showPhotobyIndex(head, album_name, index);
        return;
    }

    printf("\nFound %d photos with the name '%s'.\n", count, name);
    char date[DATE] = "", location[MAX_LENGTH] = "";
    printf("\nEnter addtional criteria to refine search (press Enter to skip): \n");
    printf("Date (dd/mm/yyyy): ");
    fgets(date, sizeof(date), stdin);
    date[strcspn(date, "\n")] = '\0';

    printf("Location: ");
    fgets(location, sizeof(location), stdin);
    location[strcspn(location, "\n")] = '\0';

    //Skip all criteria
    if (strlen(date) == 0 && strlen(location) == 0) {
        printf("\nNo additional citeria entered. Displaying all photos with name '%s':\n", name);
        showPhotosInTable(album, indices, count);
        return;
    }
    //Trường hợp có nhiều ảnh phù hợp với tiêu chí hoặc khác 
    int filtered_indices[MAX_PHOTO];
    int filtered_count = 0;

    for (int i = 0; i < count; i++) {
        int index = indices[i];
        if ((strlen(date) == 0 || strcmp(album->photos.photo[index].date, date) == 0) && 
            (strlen(location) == 0 || strcmp(album->photos.photo[index].location, location) == 0)) {
                filtered_indices[filtered_count++] = index;
        }
    }

    if (!filtered_count) {
        printf("No photos with the name '%s' match with the given criteria:\n", name);
        if (strlen(date) > 0) printf("- Date : %s\n", date);
        if (strlen(location) > 0) printf("- Location: %s\n", location);
        return;
    } else {
        printf("\nMatching photo found with the given criteria:\n");
        showPhotosInTable(album, filtered_indices, filtered_count);
    }
}

void findPhotobyIndex(Album *head, const char *album_name, int index) {
    Album *album = findAlbum(head, album_name);
    if (album == NULL) {
        printf("Album %s not found.\n", album_name);
        return;
    }

    if (index < 1 || index > album->photos.num_photos) {
        printf("Index %d is out of range. Album contains %d photos.\n", index, album->photos.num_photos);
        return;
    }
    showPhotobyIndex(head, album_name, index - 1);
}

void NameOrIndex(Album *head, const char *album_name) {
    int searchChoice;
    printf("Do you want to find by Name or Index? (1. Name, 2. Index): ");
    scanf("%d", &searchChoice);
    getchar();
    if (searchChoice == 1) {
        char photo_name[MAX_LENGTH];
        printf("Enter photo name: ");
        fgets(photo_name, MAX_LENGTH, stdin);
        photo_name[strcspn(photo_name, "\n")] = '\0'; 
        findPhotobyName(head, album_name, photo_name);
    } else if (searchChoice == 2) {
        int index;
        printf("Enter photo index: ");
        scanf("%d", &index);
        findPhotobyIndex(head, album_name, index);
    } else {
        printf("Invalid choice. Please choose again.\n");
    }
}

void showPhotosInTable(Album *album, int indices[], int count) {
    printf("\nPhotos with the name '%s':\n", album->photos.photo[indices[0]].name);
    printf("%-3s | %-15s | %-20s | %-15s | %-15s | %-20s\n", 
        "ID", "Name", "Location", "Date", "Time", "Description");
        printf("---------------------------------------------------------------------------------------------\n");

        for (int i = 0; i < count; i++) {
            int idx = indices[i];
            printf("%-3d | %-15s | %-20s | %-15s | %-15s | %-20s\n",
                idx + 1,  // ID (index + 1)
                album->photos.photo[idx].name, 
                album->photos.photo[idx].location, 
                album->photos.photo[idx].date, 
                album->photos.photo[idx].time, 
                album->photos.photo[idx].description);
        }
}

void listPhoto(Album *head, const char *album_name) {
    Album *album = findAlbum(head, album_name);
    if (album->photos.num_photos == 0 || album->photos.photo == NULL) {
        printf("No photos in Album %s!\n", album_name);
        return;
    }

    printf("List photos of Album %s:\n", album_name);
    printf("%-3s | %-15s | %-20s | %-15s | %-15s | %-20s\n", 
        "ID", "Name", "Location", "Date", "Time", "Description");
    printf("---------------------------------------------------------------------------------------------\n");
    for (int i = 0; i < album->photos.num_photos; i++) {
        printf("%-3d | %-15s | %-20s | %-15s | %-15s | %-20s\n",
            i + 1, 
            album->photos.photo[i].name, 
            album->photos.photo[i].location, 
            album->photos.photo[i].date, 
            album->photos.photo[i].time, 
            album->photos.photo[i].description);
    }
    printf("\n");
    return;
}
