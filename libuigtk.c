/*___BIBLIOTECAS_____________________________________________________________*/

#include "libuigtk.h"

/*___MACROS__________________________________________________________________*/

/* Mensagens */
#define UIGTK_MSG_NO_INIT "UIGTK not initialized, call uigtk_init ()."
#define UIGTK_MSG_INIT "UIGTK already initialized."
#define UIGTK_MSG_NO_WINDOW "Top level window (GtkWindow) not found."
#define UIGTK_MSG_NO_QUIT "gtk_main_quit handler not found."

/* Tipos de mensagens */
#define UIGTK_MSG "\n\033[1;%dm%s[%d] \033[1;36mlibuigtk.uigtk_%s ()\033[0m:\n\t\"%s\"\n\n"

#define UIGTK_INFO(msg, name) g_print(UIGTK_MSG, 34, "INFO", __LINE__, name, msg)
#define UIGTK_WARN(msg, name) g_print(UIGTK_MSG, 33, "WARN", __LINE__, name, msg)
#define UIGTK_ERROR(msg, name) g_printerr(UIGTK_MSG, 31, "ERROR", __LINE__, name, msg);

#define UIGTK_INIT_CHECK(name) if (!_uigtk_init) {UIGTK_ERROR(UIGTK_MSG_NO_INIT, name); exit(1);}

/*___VARIÁVEIS_GLOBAIS_______________________________________________________*/

/*-- _uigtk guarda o status e os ponteiros --*/
int _uigtk_init = 0;
int _uigtk_quit = 0;
GtkBuilder *_uigtk_builder = NULL;
GtkWindow *_uigtk_window = NULL;

/*___FUNÇÕES_INTERNAS________________________________________________________*/

static void _uigtk_top_level (void)
/*-- Localiza window top level: retorna 1 ou 0 (sucesso ou erro) --*/
{
	/* Obter objetos https://developer.gnome.org/gtk3/stable/GtkBuilder.html#gtk-builder-get-objects */
	GSList *objects = gtk_builder_get_objects(_uigtk_builder);

	/* Obter janela https://developer.gnome.org/gtk3/stable/GtkWidget.html#gtk-widget-get-toplevel */
	GtkWidget *window  = gtk_widget_get_toplevel((GtkWidget*) objects->data);

	/* Limpando memória https://developer.gnome.org/glib/stable/glib-Singly-Linked-Lists.html#g-slist-free */
	g_slist_free(objects);

	/* Checando sucesso https://developer.gnome.org/gtk3/stable/GtkWidget.html#gtk-widget-is-toplevel */
	if (!gtk_widget_is_toplevel(window)) {
		UIGTK_ERROR(UIGTK_MSG_NO_WINDOW, "init");
		exit(1);
	}
	_uigtk_window = GTK_WINDOW(window);
}


/*___FUNÇÕES_DO_CABEÇALHO____________________________________________________*/

void uigtk_init (char *file)
{
	/* Verificando existência de inicialização */
	if (_uigtk_init == 1) {
		UIGTK_ERROR(UIGTK_MSG_INIT, "init");
		exit(1);
	}

	/* Carregando GTK https://developer.gnome.org/gtk3/stable/gtk3-General.html#gtk-init */
	gtk_init(0, NULL);

	/* Carregando GTK Builder https://developer.gnome.org/gtk3/stable/GtkBuilder.html#gtk-builder-new-from-file */
	_uigtk_builder = gtk_builder_new_from_file(file);

	/* Obtendo a window top level */
	_uigtk_top_level();
	
	/* Defindo sucesso na inicialização */
	_uigtk_init = 1;
}

/*...........................................................................*/

void uigtk_callback (char *name, void (*handler)())
{
	UIGTK_INIT_CHECK("handler");

	/* Definindo handlers https://developer.gnome.org/gtk3/stable/GtkBuilder.html#gtk-builder-add-callback-symbol  */
	gtk_builder_add_callback_symbol(_uigtk_builder, name, G_CALLBACK(handler));

	/* Validando gtk_main_quit https://developer.gnome.org/gtk3/stable/gtk3-General.html#gtk-main-quit */
	if (handler == gtk_main_quit) _uigtk_quit = 1;
}

/*...........................................................................*/

void uigtk_main (void)
{
	UIGTK_INIT_CHECK("main");

	/* Adicionar gtk_main_quit se indefinido https://developer.gnome.org/gobject/stable/gobject-Signals.html#g-signal-connect */
	if (!_uigtk_quit && _uigtk_window != NULL) {
		UIGTK_WARN(UIGTK_MSG_NO_QUIT, "main");
		g_signal_connect(G_OBJECT(_uigtk_window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
	}

	/* Conectando sinais https://developer.gnome.org/gtk3/stable/GtkBuilder.html#gtk-builder-connect-signals */
	gtk_builder_connect_signals(_uigtk_builder, NULL);

	/* GTK main loop https://developer.gnome.org/gtk3/stable/gtk3-General.html#gtk-main */
	gtk_main();

	/* Resetando status */
	_uigtk_init = 0;
	_uigtk_quit = 0;
	_uigtk_builder = NULL;
	_uigtk_window = NULL;
}

/*...........................................................................*/

GtkBuilder *uigtk_builder (void)
{
	UIGTK_INIT_CHECK("builder");

	/* retornando valor */
	return _uigtk_builder;
}

/*...........................................................................*/

GObject *uigtk_object (char *id)
{
	UIGTK_INIT_CHECK("object");

	/* Retornando objeto */
	return gtk_builder_get_object(_uigtk_builder, id);
}


	//g_print("%s", G_OBJECT_TYPE_NAME(uigtk_object("btn1")));


/*...........................................................................*/

int uigtk_dialog (int type, char *text)
{
	UIGTK_INIT_CHECK("dialog");

	/* Definir variáveis */
	GtkButtonsType button;
	GtkMessageType style;
	GtkWidget *dialog;
	int result;

	/* definindo o tipo de botão e de mensagem */
	switch(type) {
		case 0:
			style  = GTK_MESSAGE_INFO;
			button = GTK_BUTTONS_OK;
			break;
		case 1:
			style  = GTK_MESSAGE_WARNING;
			button = GTK_BUTTONS_OK;
			break;
		case 2:
			style  = GTK_MESSAGE_ERROR;
			button = GTK_BUTTONS_OK;
			break;
		case 3:
			style  = GTK_MESSAGE_QUESTION;
			button = GTK_BUTTONS_YES_NO;
			break;
		default:
			style  = GTK_MESSAGE_OTHER;
			button = GTK_BUTTONS_OK_CANCEL;
	}

	/* Definindo a caixa de mensagem */
	dialog = gtk_message_dialog_new(
		_uigtk_window,
		GTK_DIALOG_MODAL,
		style,
		button,
		"\n%s\n",
		text
	);

	/* Obtendo o resultado */
	result = gtk_dialog_run(GTK_DIALOG(dialog));	

	/* Destruindo janela */
	gtk_widget_destroy (dialog);

	/* Retornando o resultado */
	switch (result) {
		case GTK_RESPONSE_OK:
			return 1;
		case GTK_RESPONSE_YES:
			return 1;
		case GTK_RESPONSE_NO:
			return -1;
	}
	return 0;
}
