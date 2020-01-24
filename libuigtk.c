/*-----------------------------------------------------------------------------
Bibliotecas
-----------------------------------------------------------------------------*/
#include "libuigtk.h"
#include <string.h>

/*-----------------------------------------------------------------------------
Definições (macros)
_______________________________________________________________________________
UIGTK_LEN cumprimento máximo da string
_______________________________________________________________________________
UIGTK_WARN mostra a mensagem informativa
UIGTK_WARN mostra a mensagem de alerta
UIGTK_ERROR mostra a mensagem de erro
Argumentos:
	msg - Mensagem
Observação:
	fname é uma variável local que informa o nome da função

_______________________________________________________________________________
UIGTK_FNAME define a variável local fname e inicializa UIGTK
Argumentos:
	name - nome da função
-----------------------------------------------------------------------------*/
#define UIGTK_LEN 80
#define UIGTK_INFO(msg)  g_print("\n\033[1;34mINFO \033[1;36mlibuigtk.%s()\033[0m:\n\t\"%s\"\n", fname, msg)
#define UIGTK_WARN(msg)  g_print("\n\033[1;33mWARN \033[1;36mlibuigtk.%s()\033[0m:\n\t\"%s\"\n", fname, msg)
#define UIGTK_ERROR(msg) g_print("\n\033[1;31mERROR \033[1;36mlibuigtk.%s()\033[0m:\n\t\"%s\"\n", fname, msg);
#define UIGTK_FNAME(name) char *fname = name; UIGTK_status()

/*-----------------------------------------------------------------------------
Variável global UIGTK que guardará o status do processo
-----------------------------------------------------------------------------*/
static struct {
	/* Registra se a estrutura já foi definida */
	int call;
	/* Guarda ponteiros para os elementos GTK */
	struct {
		GError *error;
		GtkBuilder *builder;
		GtkWindow *window;
	} gtk;
	/* Registra (0/1) se determinado procedimento foi executado */
	struct {
		int init;
		int builder;
		int window;
		int handler;
		int signal;
		int quit;
	} status;
	/* Registra as mensagens da biblioteca */
	struct {
		char init[UIGTK_LEN];
		char builder[UIGTK_LEN];
		char window[UIGTK_LEN];
		char handler[UIGTK_LEN];
		char signal[UIGTK_LEN];
		char signals[UIGTK_LEN];
		char quit[UIGTK_LEN];
		char top[UIGTK_LEN];
		char main[UIGTK_LEN];
		char object[UIGTK_LEN];
	} msg;
} UIGTK;

/*-----------------------------------------------------------------------------
UIGTK_status inicia a estrutura UIGTK
-----------------------------------------------------------------------------*/
static void UIGTK_status(void) {

	/* zerando a variável de erro do GTK */
	if (UIGTK.gtk.error != NULL) {
		g_clear_error(&UIGTK.gtk.error);
	}

	/* Definir valores iniciais do status e ponteiros gtk */
	if (UIGTK.status.init != 1) {
		UIGTK.gtk.builder    = NULL;
		UIGTK.gtk.window     = NULL;
		UIGTK.status.init    = 0;
		UIGTK.status.builder = 0;
		UIGTK.status.window  = 0;
		UIGTK.status.handler = 0;
		UIGTK.status.signal  = 0;
		UIGTK.status.quit    = 0;
	}

	/* Definir mensagens */
	if (UIGTK.call != 1) {
		strcpy(UIGTK.msg.init,    "GTK not initialized successfully.");
		strcpy(UIGTK.msg.builder, "GTK Builder not initialized successfully.");
		strcpy(UIGTK.msg.window,  "GTK top level window not successfully defined.");
		strcpy(UIGTK.msg.handler, "Failed to set handler.");
		strcpy(UIGTK.msg.signal,  "Failed to set signal.");
		strcpy(UIGTK.msg.signals, "No GTK signal was inserted.");
		strcpy(UIGTK.msg.quit,    "The gtk_main_quit function has not been defined.");
		strcpy(UIGTK.msg.top,     "gtk_main_quit function defined in the main window.");
		strcpy(UIGTK.msg.object,  "GTK object not located.");
	}

	UIGTK.call = 1;
}

/*===========================================================================*/

int UIGTK_load(char file[]) {

	UIGTK_FNAME("UIGTK_load");

	/* Variáveis locais */
	GSList    *objects;
	GtkWidget *window;

	/* Inicializar GTK se necessário */
	if (!UIGTK.status.init) {
		if (gtk_init_check(0, NULL) == TRUE) {
			UIGTK.status.init = 1;
		} else {
			UIGTK_ERROR(UIGTK.msg.init);
			return 0;		
		}
	}

	/* Definir GTK Builder se necessário */
	if (!UIGTK.status.builder) {
		UIGTK.gtk.builder = gtk_builder_new();
		if (UIGTK.gtk.builder != NULL) {
			 UIGTK.status.builder = 1;
		} else {
			UIGTK_ERROR(UIGTK.msg.builder);
			return 0;
		}
	}

	/* Carregar interface (arquivo .ui) */
	if (!gtk_builder_add_from_file(UIGTK.gtk.builder, file, &UIGTK.gtk.error)) {
		UIGTK_ERROR(UIGTK.gtk.error->message);
		return 0;
	}

	/* Localizar janela principal da aplicação */
	if (!UIGTK.status.window) {
	
		/* Obtendo todos os objetos */
		objects = gtk_builder_get_objects(UIGTK.gtk.builder);

		/* Tentando obter a janela principal */
		window = gtk_widget_get_toplevel((GtkWidget*) objects->data);

		/* Em caso de sucesso no processo anterior, definir */
		if (gtk_widget_is_toplevel(window)) {
			UIGTK.status.window = 1;
			UIGTK.gtk.window = GTK_WINDOW(window);
		}

		/* Limpando memória */
		g_slist_free(objects);
	}

	return 1;
}

/*===========================================================================*/

GObject *UIGTK_object(char *id) {

	UIGTK_FNAME("UIGTK_object");

	/* Variáveis locais */
	GObject *object;
	
	/* Definindo objeto */
	object = UIGTK.status.builder ? gtk_builder_get_object(UIGTK.gtk.builder, id) : NULL;
	
	/* Checar erro na localização do objeto */
	if (object == NULL) {
		UIGTK_ERROR(!UIGTK.status.builder ? UIGTK.msg.builder : UIGTK.msg.object);
	}

	return object;
}

/*===========================================================================*/

int UIGTK_handler(char *id, char *event, void (*method)()) {

	UIGTK_FNAME("UIGTK_handler");

	/* Variáveis locais */
	GObject *object;
	
	/* Definindo objeto */
	object = UIGTK_object(id);

	/* Checar erro na localização do objeto */
	if (object == NULL) {
		UIGTK_ERROR(UIGTK.msg.object);
		return 0;
	}

	/* Atribuir disparador ao elemento */
	if (g_signal_connect(object, event, G_CALLBACK(method), NULL) <= 0) {
		UIGTK_ERROR(UIGTK.msg.handler);
		return 0;
	}

	/* Verificando se foi adicionada a função para parar o looping do GTK */
	if (!UIGTK.status.quit && method == gtk_main_quit) {
		UIGTK.status.quit = 1;
	}

	UIGTK.status.handler++;

	return 1;
}

/*===========================================================================*/

int UIGTK_signal(char *name, void (*method)()) {

	UIGTK_FNAME("UIGTK_signal");

	/* Checando GTK Builder */
	if (!UIGTK.status.builder) {
		UIGTK_ERROR(UIGTK.msg.builder);
		return 0;
	}

	/* Definindo sinais */
	gtk_builder_add_callback_symbol(UIGTK.gtk.builder, name, method);

	/* Verificando se foi adicionada a função para parar o looping do GTK */
	if (!UIGTK.status.quit && method == gtk_main_quit) {
		UIGTK.status.quit = 1;
	}

	UIGTK.status.signal++;

	return 1;
}

/*===========================================================================*/

int UIGTK_run(void) {

	UIGTK_FNAME("UIGTK_run");

	/* Checando inicialização do GTK */
	if (!UIGTK.status.init) {
		UIGTK_ERROR(UIGTK.msg.init);
		return 0;
	}

	/* Checando janela principal */
	if (!UIGTK.status.window) {
		UIGTK_ERROR(UIGTK.msg.window);
		return 0;
	}

	/* Checando gtk_main_quit */
	if (!UIGTK.status.quit) {
	
		/* Tentar definir na janela principal */
		if (g_signal_connect(G_OBJECT(UIGTK.gtk.window), "destroy", G_CALLBACK(gtk_main_quit), NULL) > 0) {
			UIGTK_INFO(UIGTK.msg.top);
			UIGTK.status.quit = 1;
		} else {
			UIGTK_ERROR(UIGTK.msg.quit);
			return 0;
		}
	}

	/* Conectando sinais da interface, se houver */
	if (UIGTK.status.signal) {
		gtk_builder_connect_signals(UIGTK.gtk.builder, NULL);
	} else {
		UIGTK_INFO(UIGTK.msg.signals);
	}

	/* Iniciando looping GTK */
	gtk_main();

	/* Reiniciando GTK */
	UIGTK.status.init = 0;

	return 1;
}

/*===========================================================================*/

int UIGTK_msg(int type, char *text) {

	UIGTK_FNAME("UIGTK_msg");

	/* Definir variáveis */
	GtkButtonsType button;
	GtkMessageType style;
	GtkWidget *box;
	int result;

	/* Checando se o UIGTK.gtk.init foi definido */
	if (!UIGTK.status.init) {
		UIGTK_ERROR(UIGTK.msg.init);
		return -1;
	}

	/* Se UIGTK.status.window não tiver sido definida */
	if (!UIGTK.status.window) {
		UIGTK_INFO(UIGTK.msg.window);
	}

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
	box = gtk_message_dialog_new(UIGTK.gtk.window, GTK_DIALOG_MODAL, style, button, "%s", text);

	/* Obtendo o resultado */
	result = gtk_dialog_run(GTK_DIALOG(box));	

	/* Destruindo janela */
	gtk_widget_destroy (box);

	/* Retornando o resultado */
	switch (result) {
		case GTK_RESPONSE_OK:
			return 1;
		case GTK_RESPONSE_YES:
			return 1;
		case GTK_RESPONSE_NO:
			return 2;
	}

	return 0;
}
