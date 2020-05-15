all: folder experience

experience: libuigtk.o experience.o
	@ echo "Compiling experience.o and libuigtk.o:"
	gcc `pkg-config --cflags gtk+-3.0` -o experience objects/experience.o objects/libuigtk.o `pkg-config --libs gtk+-3.0`;
	@ echo "Execute experience:"
	./experience
	@ echo "Done!"

libuigtk.o:
	@ echo "Compiling libuigtk.c:"
	gcc -Wall `pkg-config --cflags gtk+-3.0` -o objects/libuigtk.o -c source/libuigtk.c  `pkg-config --libs gtk+-3.0`;


experience.o:
	@ echo "Compiling experience.c:"
	gcc -Wall `pkg-config --cflags gtk+-3.0` -o objects/experience.o -c source/experience.c  `pkg-config --libs gtk+-3.0`;

folder:
	@ echo "Copying library files to the source directory..."
	@ cp "../libuigtk.c" "source/"
	@ cp "../libuigtk.h" "source/"

clean:
	@ echo "Cleaning up..."
	@ rm -f experience
	@ rm -rf objects/*.o
	@ echo "Done!"
