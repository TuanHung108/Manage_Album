#include "constants.h"
#include "photo/photo.h"
#include "album/album.h"
#include "menu/menu.h"
#include "file_io/file_io.h"

int main() {
    Album *headAlbum = NULL;
    int choice;
    char album_name[MAX_LENGTH];

    const char *file_path = "album_data.csv";
    loadfromFile(&headAlbum, file_path);
    
    while (1) {
        Menu();
        choice = choice_checked();        
        system("cls");

        switch(choice) {
            case 0:
                savetoFile(headAlbum, file_path);
                freeAll(&headAlbum);
                printf("All albums and photos have been freed.\n");
                return 0;
            case 1:
                printf("Enter Album name to acces: ");
                getchar();
                fgets(album_name, MAX_LENGTH, stdin);
                album_name[strcspn(album_name, "\n")] = '\0';
                accessAlbum(headAlbum, album_name);
                break;
            case 2:
                printf("Enter new Album name: ");
                getchar();
                fgets(album_name, MAX_LENGTH, stdin);
                album_name[strcspn(album_name, "\n")] = '\0';
                insertAlbum(&headAlbum, album_name);
                break;
            case 3:
                printf("Enter Album name to delete: ");
                getchar();
                fgets(album_name, MAX_LENGTH, stdin);
                album_name[strcspn(album_name, "\n")] = '\0';
                removeAlbum(&headAlbum, album_name);
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
