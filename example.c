#include "libuigtk.h"

static void print_hello(GtkWidget *widget, gpointer data) {
	g_print("Hello World\n");
}



int main(int argc, char *argv[]) {

	if (!UIGTK_start("example.ui")) exit(1);

	UIGTK_signal("gtk_main_quit", gtk_main_quit);
	UIGTK_signal("print_hello", print_hello);

	if (!UIGTK_run()) exit(1);
}
