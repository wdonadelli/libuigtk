all: folder experience

experience: libuigtk.o experience.o
	@ echo "\033[1;3;96mCompiling experience.o and libuigtk.o:\033[0m"
	gcc `pkg-config --cflags gtk+-3.0` -o experience objects/experience.o objects/libuigtk.o `pkg-config --libs gtk+-3.0`;
	@ echo "\033[1;3;96mExecute experience:\033[0m"
	./experience
	@ echo "\033[1;3;96mDone!\033[0m"

libuigtk.o:
	@ echo "\033[1;3;96mCompiling libuigtk.c:\033[0m"
	gcc -Wall `pkg-config --cflags gtk+-3.0` -o objects/libuigtk.o -c source/libuigtk.c  `pkg-config --libs gtk+-3.0`;


experience.o:
	@ echo "\033[1;3;96mCompiling experience.c:\033[0m"
	gcc -Wall `pkg-config --cflags gtk+-3.0` -o objects/experience.o -c source/experience.c  `pkg-config --libs gtk+-3.0`;

folder:
	@ echo "\033[1;3;96mCopying library files to the source directory...\033[0m"
	@ cp "../libuigtk.c" "source/"
	@ cp "../libuigtk.h" "source/"

clean:
	@ echo "\033[1;3;96mCleaning up...\033[0m"
	@ rm -f experience
	@ rm -rf objects/*.o
	@ echo "\033[1;3;96mDone!\033[0m"
