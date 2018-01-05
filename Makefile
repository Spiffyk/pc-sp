CFLAGS=-O2 -pedantic

default: pc_sp.exe

clean:
	rm -f *.o
	rm -f pc_sp.exe

pc_sp.exe: main.o gmproc.o pgm.o simple_table.o pgm_stat_printer.o
	$(CC) $^ -o $@

.c.o:
	$(CC) $(CFLAGS) -c $<
