#include "libuigtk.h"

static void hello(GtkWidget *widget, gpointer data) {
	uigtk_dialog(0, "Hello!");
}

static void bye(GtkWidget *widget, gpointer data) {
	int result = uigtk_dialog(3, "Do you really want to say goodbye?");
	if (result == 1) {
		uigtk_dialog(0, "Bye!");
		gtk_main_quit();
	}
}

	


int main(int argc, char *argv[]) {
	
	uigtk_init("example.ui");

//	uigtk_handler(gtk_main_quit);
	uigtk_handler(hello);
	uigtk_handler(bye);

	uigtk_main();
	
	
	
	
}
