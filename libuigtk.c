/*___BIBLIOTECAS_____________________________________________________________*/

#include "libuigtk.h"
#include <string.h>

/*___DEFINIÇÕES_(MACROS)_____________________________________________________*/

/* UIGTK_MSG macro auxiliar para mensagens da biblioteca */
#define UIGTK_MSG(color, type, fname, msg) g_print( \
	"\n\033[1;%dm%s \033[1;36mlibuigtk.%s()\033[0m:\n\t\"%s\"\n\n", \
	color, \
	type, \
	UIGTK.fname, \
	msg \
)

/* UIGTK_FNAME define o nome da função para fins UIGTK_MSG */
#define UIGTK_FNAME(name) strcpy(UIGTK.fname, name); UIGTK_status()

/* UIGTK_INFO exibe uma mensagem (msg) informativa */
#define UIGTK_INFO(msg)  UIGTK_MSG(34, "INFO", fname, msg)

/* UIGTK_WARN exibe uma mensagem (msg) de alerta */
#define UIGTK_WARN(msg)  UIGTK_MSG(33, "WARN", fname, msg)

/* UIGTK_ERROR exibe uma mensagem (msg) de erro */
#define UIGTK_ERROR(msg) UIGTK_MSG(31, "ERROR", fname, msg); exit(1)

/* UIGTK_READY checa se é necessário proceder com a ação */
#define UIGTK_READY() if (UIGTK.init) {return;}

/* UIGTK_CHECK checa se o UIGTK foi iniciado corretamente */
#define UIGTK_CHECK() if (!UIGTK.init) {UIGTK_ERROR(UIGTK.msgLoad);}

/*___VARIÁVEIS_GLOBAIS_______________________________________________________*/

/* UIGTK guarda o status, os ponteiros e as mensagens do aplicativo */
static struct {
	int init;
	int signal;
	int quit;
	GtkBuilder *builder;
	GtkWindow *window;
	char msgLoad[80];
	char fname[80];
} UIGTK;

/*___FUNÇÕES_INTERNAS________________________________________________________*/

/*...........................................................................*/
/* UIGTK_status inicia a estrutura UIGTK */
static void UIGTK_status(void) {

	UIGTK_READY();
	UIGTK.init    = 0;
	UIGTK.signal  = 0;
	UIGTK.quit    = 0;
	UIGTK.builder = NULL;
	UIGTK.window  = NULL;
	strcpy(UIGTK.msgLoad, "UIGTK not initialized correctly, see uigtk_init function.");

}

/*...........................................................................*/
/* UIGTK_gtk inicia o GTK */
static void UIGTK_gtk(void) {

	UIGTK_READY()

	/* Inicializar GTK */
	if (gtk_init_check(0, NULL) != TRUE) {
		UIGTK_ERROR("Failed to initialize the GTK.");
	}
}

/*...........................................................................*/
/* UIGTK_builder inicia o GTK Builder */
static void UIGTK_builder(void) {

	UIGTK_READY();

	/* Definir GTK Builder */
	UIGTK.builder = gtk_builder_new();
	/* Checar erro */
	if (UIGTK.builder == NULL) {
		UIGTK_ERROR("Failed to initialize the GTK Builder.");
	}
}

/*...........................................................................*/
/* UIGTK_file carrega o arquivo .ui */
static void UIGTK_file(char *file) {

	/* Variáveis locais */
	GError *error;

	/* Carregando arquivo .ui */
	if (!gtk_builder_add_from_file(UIGTK.builder, file, &error)) {
		UIGTK_ERROR(error->message);
	}
	/* Limpar memória */	
	g_clear_error(&error);

}

/*...........................................................................*/
/* UIGTK_window tentar localizar window top level */
static void UIGTK_window(void) {

	/* Tentando obter a janela de nível superior */
	GSList    *objects = gtk_builder_get_objects(UIGTK.builder);
	GtkWidget *window  = gtk_widget_get_toplevel((GtkWidget*) objects->data);
	/* Limpando memória */
	g_slist_free(objects);
	/* Checando se houve sucesso */
	if (!gtk_widget_is_toplevel(window)) {
		UIGTK_ERROR("Failed to set the top level window, define a GtkWindow object.");
	}
	/* Definindo valor */
	UIGTK.window = GTK_WINDOW(window);
}

/*...........................................................................*/
/* UIGTK_quit define gtk_main_quit à janela principal, caso não exista */
static void UIGTK_quit(void) {

	/* Caso já tenha sido definida, retornar */
	if (UIGTK.quit) return;
	/* tentando conectar o sinal */
	if (g_signal_connect(G_OBJECT(UIGTK.window), "destroy", G_CALLBACK(gtk_main_quit), NULL) <= 0) {
		UIGTK_ERROR("Error when linking the function gtk_main_quit to the top level window when it is destroyed.");
	}
	UIGTK_INFO("The gtk_main_quit function has been linked to the top level window when it is destroyed.");
}

/*___FUNÇÕES_EXTERNAS_(VER_CABEÇALHO)________________________________________*/

/*...........................................................................*/
void uigtk_init(char *file) {

	UIGTK_FNAME("uigtk_init");
	UIGTK_gtk();
	UIGTK_builder();
	UIGTK_file(file);
	UIGTK.init = 1;
}

/*...........................................................................*/
void uigtk_signal(char *name, void (*method)()) {

	UIGTK_FNAME("uigtk_signal");
	UIGTK_CHECK();

	/* Adicionando sinal ao builder */
	gtk_builder_add_callback_symbol(UIGTK.builder, name, method);
	/* Se for gtk_main_quit, informar */
	if (!UIGTK.quit && method == gtk_main_quit) {
		UIGTK.quit = 1;
	}
	UIGTK.signal = 1;
}

/*...........................................................................*/
void uigtk_main(void) {

	UIGTK_FNAME("uigtk_main");
	UIGTK_CHECK();
	UIGTK_window();
	UIGTK_quit();
	/* Conectando sinais */
	gtk_builder_connect_signals(UIGTK.builder, NULL);
	/* iniciando GTK main loop */
	gtk_main();
	/* Zerando status após sair do loop */
	UIGTK.init = 0;
}

/*...........................................................................*/
GObject *uigtk_object(char *id) {

	UIGTK_FNAME("uigtk_object");
	UIGTK_CHECK();

	/* Obtendo o objeto */
	GObject *object = gtk_builder_get_object(UIGTK.builder, id);
	return object;
}

/*...........................................................................*/
int uigtk_handler(char *id, char *event, void (*method)()) {

	UIGTK_FNAME("uigtk_handler");
	UIGTK_CHECK();

	/* Obtendo o objeto */
	GObject *object = uigtk_object(id);
	/* Checar erro na localização do objeto */
	if (object == NULL) {
		UIGTK_WARN("Object not found.");
		return 0;
	}
	/* Tentar Atribuir disparador ao elemento */
	if (g_signal_connect(object, event, G_CALLBACK(method), NULL) <= 0) {
		UIGTK_WARN("Failed to link the handler.");
		return 0;
	}
	/* Se for gtk_main_quit, informar */
	if (!UIGTK.quit && method == gtk_main_quit) {
		UIGTK.quit = 1;
	}
	return 1;
}

/*...........................................................................*/
int uigtk_msg(int type, char *text) {

	UIGTK_FNAME("uigtk_msg");
	UIGTK_CHECK();

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
		UIGTK.window,
		GTK_DIALOG_MODAL,
		style,
		button,
		"%s",
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
