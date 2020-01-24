#include "libuigtk.h"

static void hello(GtkWidget *widget, gpointer data) {
	UIGTK_msg(0, "Hello!");
}

static void bye(GtkWidget *widget, gpointer data) {
	int result = UIGTK_msg(3, "Do you really want to say goodbye?");
	if (result == 1) {
		UIGTK_msg(0, "Bye!");
		gtk_main_quit();
	}
}

int main(int argc, char *argv[]) {
	UIGTK_load("example.ui");

	//Padrão
	UIGTK_signal("gtk_main_quit", gtk_main_quit);
	UIGTK_signal("hello", hello);
	UIGTK_signal("bye", bye);

	//Alternativo
	//UIGTK_handler("window", "destroy", gtk_main_quit);
	//UIGTK_handler("btn1", "pressed", hello);
	//UIGTK_handler("btn2", "pressed", bye);

	UIGTK_run();
}
