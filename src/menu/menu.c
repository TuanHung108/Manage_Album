#include "menu.h"

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

void photoMenu(Album *head, const char *album_name) {
    int choice;

    while (1) {
        printf("---------------------------\n");
        printf("0. Back to main menu\n");
        printf("1. Access photo\n");
        printf("2. Insert new photo\n");
        printf("3. Delete photo\n");
        printf("4. Clear all photos\n");
        printf("5. Find photo\n");
        printf("6. List photos\n");
        printf("---------------------------\n");
        
        choice = choice_checked();
        
        system("cls");

        switch(choice) {
            case 0:
                return;
            case 1:
                accessAndEditPhoto(head, album_name);
                break;
            case 2:
                insertPhoto(head, album_name);
                sortPhoto(head, album_name);
                break;
            case 3:
                removePhoto(head, album_name);
                break;
            case 4:
                clearPhotos(head, album_name);
                break;
            case 5:
                NameOrIndex(head, album_name);
                break;
            case 6:
                listPhoto(head, album_name);
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}

int confirm(const char *message) {
    char input[MAX_LENGTH];
    printf("%s (Y/N): ", message);
    while (1) {
        if (fgets(input, sizeof(input), stdin)) {
            input[strcspn(input, "\n")] = '\0';

            for (int i = 0; input[i]; i++) {
                input[i] = tolower(input[i]);
            }
            if (strcmp(input, "y") == 0 || strcmp(input, "yes") == 0) {
                return 1;
            } else if (strcmp(input, "n") == 0 || strcmp(input, "no") == 0) {
                return 0;
            } else {
                printf("Invalid input. Please enter Y or N: ");
            }
        } else {
            printf("Error reading input. Please try again.\n");
            clearerr(stdin);
        }
    }
}

int choice_checked() {
    int choice;
    printf("Choose an option: ");
    while ((scanf("%d", &choice) != 1)) {
        printf("Invalid input. Please enter a number.\n");
        while (getchar() != '\n');
        printf("Choose an option: ");
    }
    return choice;
}
