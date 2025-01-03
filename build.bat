gcc -Wall -g -I src -c src/project.c -o build/project.o
gcc -Wall -g -I src -c src/album/album.c -o build/album.o
gcc -Wall -g -I src -c src/photo/photo.c -o build/photo.o
gcc -Wall -g -I src -c src/file_io/file_io.c -o build/file_io.o
gcc -Wall -g -I src -c src/menu/menu.c -o build/menu.o

gcc -I src build/project.o build/album.o build/photo.o build/file_io.o build/menu.o -o bin/program

cd ./bin 

program.exe