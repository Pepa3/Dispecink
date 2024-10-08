railway: main.c
	gcc main.c -ggdb -Wall -Wextra -o main -lncurses

dispecink:
	g++ Dispecink.cpp -o Dispecink -static -mwindows -DSFML_STATIC -I/d/vscode -I/d/vscode/SFML-2.6.1/include -L/d/vscode/SFML-2.6.1/lib -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lopengl32 -lwinmm -lgdi32 -lfreetype