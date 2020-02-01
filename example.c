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

	
	//g_print("%s", G_OBJECT_TYPE_NAME(uigtk_object("btn1")));

	//Padrão
	//uigtk_callback("gtk_main_quit", gtk_main_quit);
	//uigtk_callback("hello", hello);
	//uigtk_callback("bye", bye);

	//uigtk_handler(gtk_main_quit);
	//uigtk_handler(hello);
	//uigtk_handler(bye);

	//Alternativo
	//uigtk_connect("window", "destroy", gtk_main_quit);
	//uigtk_connect("btn1", "pressed", hello);
	//uigtk_connect("btn2", "pressed", bye);

	uigtk_main();
}
