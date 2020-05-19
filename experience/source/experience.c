#include "libuigtk.h"

static void hello(GtkWidget *widget, gpointer data) {
	uigtk_dialog(0, "Hello", "Hi!");
}

static void bye(GtkWidget *widget, gpointer data) {
	int result = uigtk_dialog(3, "Where are you going?", "Do you really want to say goodbye?");
	if (result == 1) {
		uigtk_dialog(0, NULL, "Bye, bye...");
		gtk_main_quit();
	}
}

int main(int argc, char *argv[]) {
	//printf("dialog:   %d\n", uigtk_dialog(3, "My Title", "My message."));
	//printf("init:     %d\n", uigtk_init("interface/experience.ui"));
	printf("init:     %d\n", uigtk_init("interface/experience.ui"));
	printf("callback: %d\n", uigtk_handler(gtk_main_quit));
	printf("callback: %d\n", uigtk_handler(hello));
	printf("callback: %d\n", uigtk_handler(bye));
	printf("main: %d\n", uigtk_main());
	//printf("dialog:   %d\n", uigtk_dialog(3, "My Title", "My message."));


}
