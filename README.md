# KIV/PC - seminar project

**University:**
    [University of Western Bohemia](http://www.zcu.cz/)

**Assignment:**
    [Přebarvování souvislých oblastí ve snímku](http://www.kiv.zcu.cz/studies/predmety/pc/doc/work/sw2017-03.pdf)

## Building

Building was tested on:
* **Ubuntu 17.10** with **GNU GCC** `7.2.0-8ubuntu3`
* **Windows 10** with **GNU GCC** ``MinGW.org GCC-6.3.0-1``

### Linux

`make` and `gcc` need to be installed.

Run the following in the terminal (in the project directory):

```
make clean && make
```

### Windows

**MinGW** needs to be installed and `C:\MinGW\bin` needs to be included in the `PATH` variable.

Run the following in the command-line (in the project directory):

```
mingw32-make -f Makefile.mingw clean
mingw32-make -f Makefile.mingw
```

## Usage

This program can be run on **Linux** with the following command:

```
./pc_sp.exe <input_pgm> <output_pgm>
```

and on **Windows** with the following command:

```
pc_sp.exe <input_pgm> <output_pgm>
```

`input_pgm` is a black and white PGM file to be processed, `output_pgm` is the path to the processed PGM file to be
created by the program.
