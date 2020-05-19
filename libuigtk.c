#include "libuigtk.h"

/*___MACROS___________________________________________________________________*/

/* mensagens */

#define UIGTK_MSG_INIT       "UIGTK already initialized."
#define UIGTK_MSG_NO_BUILDER "Error getting GtkBuilder."
#define UIGTK_MSG_NO_WINDOW  "Top level window (GtkWindow) not found."
#define UIGTK_MSG_NO_INIT    "UIGTK not initialized."
#define UIGTK_MSG_NO_QUIT    "gtk_main_quit handler not found."
#define UIGTK_MSG_ERR_QUIT   "Error when setting gtk_main_quit automatically."
#define UIGTK_MSG_NO_FILE    "Interface file not found."

/* protótipos */
#define UIGTK_INIT       "uigtk_init (char *file)"
#define UIGTK_CALLBACK   "void uigtk_callback (char *name, void (*handler)())"
#define UIGTK_HANDLER    "uigtk_handler (handler)"
#define UIGTK_MAIN       "uigtk_main (void)"
#define UIGTK_BUILDER    "uigtk_builder (void)"
#define UIGTK_OBJECT     "uigtk_object (char *id)"
#define UIGTK_SET_OBJECT "uigtk_set_object (var, id)"
#define UIGTK_DIALOG     "uigtk_dialog (int type, char *title, char *text)"

/* mensagem de saída padrão */
#define UIGTK_MSG "\n\
\033[0;1;7;%dm %s \
\033[0m \
\033[0;1;3;96mlibuigtk\
\033[0m > %s\
\033[0;2;3m\n\t%s\
\033[0m\n\n"

/* mensagens de erro */
#define UIGTK_INFO(msg) \
	g_print(UIGTK_MSG, 34, "INFO", _uigtk_prototype, msg)

#define UIGTK_WARN(msg) \
	g_print(UIGTK_MSG, 33, "WARN", _uigtk_prototype, msg)

#define UIGTK_ERROR(msg) \
	g_printerr(UIGTK_MSG, 31, "ERROR", _uigtk_prototype, msg)

/*___VARIÁVEIS_INTERNAS_______________________________________________________*/

static int _uigtk_init = 0;               /* registra a inicialização do uigtk */
static int _uigtk_quit = 0;               /* registra existência de gtk_main_quit */
static int _uigtk_win  = 0;               /* registra existência de toplevel */
static char *_uigtk_prototype     = NULL; /* registra o protótipo da função */
static GtkBuilder *_uigtk_builder = NULL; /* guarda o construtor */
static GtkWindow *_uigtk_window   = NULL; /* guarda a janela principal */

/*___FUNÇÕES_INTERNAS_________________________________________________________*/

static int _uigtk_top_level (void)
/* Localiza window top level: retorna 1 ou 0 (sucesso ou erro) */
{
	/* definindo valor inicial */
	_uigtk_win = 0;

	/* obter objetos */
	/* https://developer.gnome.org/gtk3/stable/GtkBuilder.html#gtk-builder-get-objects */
	GSList *objects = gtk_builder_get_objects(_uigtk_builder);

	/* obter janela */
	/* https://developer.gnome.org/gtk3/stable/GtkWidget.html#gtk-widget-get-toplevel */
	GtkWidget *window  = gtk_widget_get_toplevel((GtkWidget*) objects->data);

	/* limpar memória */
	/* https://developer.gnome.org/glib/stable/glib-Singly-Linked-Lists.html#g-slist-free */
	g_slist_free(objects);

	/* checar sucesso na obtenção da janela */
	/* https://developer.gnome.org/gtk3/stable/GtkWidget.html#gtk-widget-is-toplevel */
	if (gtk_widget_is_toplevel(window)) {
		_uigtk_window = GTK_WINDOW(window);
		_uigtk_win = 1;
	}
	
	return _uigtk_win;
}


/*___FUNÇÕES_DO_CABEÇALHO____________________________________________________*/

int uigtk_init (char *file)
{
	/* definindo protótipo */
	_uigtk_prototype = UIGTK_INIT;
	
	/* Verificando se a inicialização já ocorreu */
	if (_uigtk_init) {
		UIGTK_WARN(UIGTK_MSG_INIT);
		return 0;
	}

	/* Carregando GTK */
	/* https://developer.gnome.org/gtk3/stable/gtk3-General.html#gtk-init */
	gtk_init(0, NULL);

	/* testando a existência do arquivo */
	if (file == NULL || !g_file_test (file, G_FILE_TEST_EXISTS)) {
		UIGTK_ERROR(UIGTK_MSG_NO_FILE);
		return 0;
	}

	/* Carregando GTK Builder */
	/* https://developer.gnome.org/gtk3/stable/GtkBuilder.html#gtk-builder-new-from-file */
	_uigtk_builder = gtk_builder_new_from_file(file);

	/* exagero de cautela: no caso de erro acima, deveria abortar o programa */	
	if (_uigtk_builder == NULL) {
		UIGTK_ERROR(UIGTK_MSG_NO_BUILDER);
		return 0;
	}

	/* Obtendo a window top level */
	if (!_uigtk_top_level()) {
		UIGTK_INFO(UIGTK_MSG_NO_WINDOW);
	}
	
	/* Defindo sucesso na inicialização */
	_uigtk_init = 1;

	return _uigtk_init;
}

/*...........................................................................*/

int uigtk_callback (char *name, void (*handler)())
{
	/* definindo protótipo */
	_uigtk_prototype = UIGTK_CALLBACK;

	/* Verificando se a inicialização já ocorreu */
	if (!_uigtk_init) {
		UIGTK_ERROR(UIGTK_MSG_NO_INIT);
		return 0;
	}

	/* Definindo handlers */
	/* https://developer.gnome.org/gtk3/stable/GtkBuilder.html#gtk-builder-add-callback-symbol */
	gtk_builder_add_callback_symbol(_uigtk_builder, name, G_CALLBACK(handler));

	/* Validando gtk_main_quit */
	/* https://developer.gnome.org/gtk3/stable/gtk3-General.html#gtk-main-quit */
	if (handler == gtk_main_quit) {
		_uigtk_quit = 1;
	}
	
	return 1;
}

/*...........................................................................*/

int uigtk_main (void)
{
	/* variáveis iniciais */
	int gsignal;

	/* definindo protótipo */
	_uigtk_prototype = UIGTK_MAIN;

	/* Verificando se a inicialização já ocorreu */
	if (!_uigtk_init) {
		UIGTK_ERROR(UIGTK_MSG_NO_INIT);
		return 0;
	}

	/* Adicionar gtk_main_quit se não definido */
	/* https://developer.gnome.org/gobject/stable/gobject-Signals.html#g-signal-connect */
	if (!_uigtk_quit && _uigtk_window) {
		UIGTK_INFO(UIGTK_MSG_NO_QUIT);
		gsignal = g_signal_connect(
			G_OBJECT(_uigtk_window), "destroy", G_CALLBACK(gtk_main_quit), NULL
		);
		if (gsignal <= 0) {
			UIGTK_INFO(UIGTK_MSG_ERR_QUIT);
		}
	}

	/* Conectando sinais */
	/* https://developer.gnome.org/gtk3/stable/GtkBuilder.html#gtk-builder-connect-signals */
	gtk_builder_connect_signals(_uigtk_builder, NULL);

	/* GTK main loop */
	/* https://developer.gnome.org/gtk3/stable/gtk3-General.html#gtk-main */
	gtk_main();

	/* Resetando status */
	_uigtk_init    = 0;
	_uigtk_win     = 0;
	_uigtk_quit    = 0;
	_uigtk_builder = NULL;
	_uigtk_window  = NULL;

	return 1;
}

/*...........................................................................*/

GtkBuilder *uigtk_builder (void)
{
	/* definindo protótipo */
	_uigtk_prototype = UIGTK_BUILDER;

	/* Verificando se a inicialização já ocorreu */
	if (!_uigtk_init) {
		UIGTK_ERROR(UIGTK_MSG_NO_INIT);
		return NULL;
	}

	/* retornando valor */
	return _uigtk_builder;
}

/*...........................................................................*/

GObject *uigtk_object (char *id)
{
	/* definindo protótipo */
	_uigtk_prototype = UIGTK_OBJECT;

	/* Verificando se a inicialização já ocorreu */
	if (!_uigtk_init) {
		UIGTK_ERROR(UIGTK_MSG_NO_INIT);
		return NULL;
	};

	/* Retornando objeto */
	/* https://developer.gnome.org/gtk3/stable/GtkBuilder.html#gtk-builder-get-object */
	return gtk_builder_get_object(_uigtk_builder, id);
}

/*...........................................................................*/

int uigtk_dialog (int type, char *title, char *text)
{
		/* definindo protótipo */
	_uigtk_prototype = UIGTK_DIALOG;

	/* Verificando se a inicialização já ocorreu */
	if (!_uigtk_init) {
		UIGTK_INFO(UIGTK_MSG_NO_INIT);
		gtk_init(0, NULL);
	};

	/* Verificando se há window toplevel */
	if (!_uigtk_win) {
		UIGTK_INFO(UIGTK_MSG_NO_WINDOW);
	};

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
	dialog = gtk_message_dialog_new	(
		_uigtk_window,
		GTK_DIALOG_MODAL,
		style,
		button,
		"\n%s",
		(title != NULL ? title : text)
	);

	if (title != NULL) {
		gtk_message_dialog_format_secondary_text(
			GTK_MESSAGE_DIALOG(dialog), "\n%s", text
		);
	}

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

//interessante: g_print("%s", G_OBJECT_TYPE_NAME(uigtk_get_object("btn1")));
