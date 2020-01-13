clear;

gcc `pkg-config --cflags gtk+-3.0` -c libuigtk.c `pkg-config --libs gtk+-3.0`;
gcc `pkg-config --cflags gtk+-3.0` -c example.c  `pkg-config --libs gtk+-3.0`;
gcc `pkg-config --cflags gtk+-3.0` -o example example.o libuigtk.o `pkg-config --libs gtk+-3.0`;

./example;
