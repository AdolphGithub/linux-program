all: application

INCLUDE=/usr/include/gdbm
LIBS=gdbm


CFLAGS=
app_ui.o: app_ui.c cd_data.h
	gcc $(CFLAGS) -c app_ui.c

access.o: cd_access.c cd_data.h
	gcc $(CFLAGS) -c cd_access.c -o access.o

application: app_ui.o access.o
	gcc $(CFLAGS) -o application app_ui.o access.o -l$(LIBS)

clean:
	rm -rf application *.o

nodmbfiles:
	rm -f *.dir *.pag