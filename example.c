#include "libuigtk.h"

static void hello(GtkWidget *widget, gpointer data) {
	uigtk_msg(0, "Hello!");
}

static void bye(GtkWidget *widget, gpointer data) {
	int result = uigtk_msg(3, "Do you really want to say goodbye?");
	if (result == 1) {
		uigtk_msg(0, "Bye!");
		gtk_main_quit();
	}
}

int main(int argc, char *argv[]) {
	uigtk_init("example.ui");

	//g_print("%s", G_OBJECT_TYPE_NAME(uigtk_object("btn1")));

	//Padrão
	uigtk_signal("gtk_main_quit", gtk_main_quit);
	uigtk_signal("hello", hello);
	uigtk_signal("bye", bye);

	//Alternativo
	//uigtk_handler("windows", "destroy", gtk_main_quit);
	//uigtk_handler("btn1", "pressed", hello);
	//uigtk_handler("btn2", "pressed", bye);

	uigtk_main();
}
