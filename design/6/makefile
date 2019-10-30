curses: curses_app.o menu.o records.o
	gcc -o curses curses_app.o menu.o records.o -lncursesw

curses_app.o: curses_app.c ./include/menu.h ./include/records.h
	gcc -c curses_app.c ./include/menu.h ./include/records.h 

records.o: ./include/records.c ./include/records.h
	gcc -c ./include/records.c ./include/records.h	

menu.o: ./include/menu.c ./include/menu.h
	gcc -c ./include/menu.c ./include/menu.h

clean:
	rm menu.o records.o curses_app.o
