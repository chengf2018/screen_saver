screen_save.exe : main.c
	gcc -O2 $^ -o $@ -lwinmm -lgdi32 -mwindows