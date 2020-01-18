#include "libuigtk.h"

static void hello(GtkWidget *widget, gpointer data) {
	g_print("info: %i\n", UIGTK_msg(0, "Info\nè uma longa mensagem só\npara ver o que acontece\ncom essa porra"));
	g_print("Warn: %i\n", UIGTK_msg(1, "Warn"));
	g_print("Error: %i\n", UIGTK_msg(2, "Error"));
	g_print("Question: %i\n", UIGTK_msg(3, "Question"));
	g_print("Other: %i\n", UIGTK_msg(4, "Other"));
}



int main(int argc, char *argv[]) {

	if (!UIGTK_load("example.ui")) exit(1);

	UIGTK_signal("gtk_main_quit", gtk_main_quit);
	UIGTK_signal("hello", hello);

	if (!UIGTK_run()) exit(1);
}
