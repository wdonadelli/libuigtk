#include "libuigtk.h"

/*-----------------------------------------------------------------------------
UIGTK_builder define o construtor da interface
-----------------------------------------------------------------------------*/
static GtkBuilder *UIGTK_builder = NULL;

/*-----------------------------------------------------------------------------
UIGTK_window define a janela principal da interface
-----------------------------------------------------------------------------*/
static GtkWindow *UIGTK_window = NULL;

/*-----------------------------------------------------------------------------
UIGTK_error define as mensagens de erro do GTK
-----------------------------------------------------------------------------*/
static GError *UIGTK_error = NULL;

/*-----------------------------------------------------------------------------
UIGTK_main registra se o looping GTK já foi iniciado
-----------------------------------------------------------------------------*/
static int UIGTK_main = 0;

/*-----------------------------------------------------------------------------
UIGTK_handlers registra a quantidade de disparadores adicionados
-----------------------------------------------------------------------------*/
static int UIGTK_handlers = 0;

/*-----------------------------------------------------------------------------
UIGTK_signals registra a quantidade de sinais adicionados
-----------------------------------------------------------------------------*/
static int UIGTK_signals = 0;

/*===========================================================================*/

int UIGTK_load(char file[]) {

	/* Definir variáveis */
	GSList    *objects;
	GtkWidget *window;

	/* Verificar se UIGTK_main já foi definido */
	if (UIGTK_main) {
		g_printerr("UIGTK_load WARNING: GTK loopping already started\n");
		return 0;
	}

	/* Se o UIGTK_builder ainda não foi definido */
	if (UIGTK_builder == NULL) {

		/* Iniciar GTK */
		gtk_init(0, NULL);

		/* Definir UIGTK_builder */
		UIGTK_builder = gtk_builder_new();
	}

	/* Carregando interface e checando se ocorreu algum erro */
	if (!gtk_builder_add_from_file(UIGTK_builder, file, &UIGTK_error)) {
		g_printerr("UIGTK_load ERROR: %s\n", UIGTK_error->message);
		g_clear_error(&UIGTK_error);
		return 0;
	}

	/* Se UIGTK_window ainda não foi definido */
	if (UIGTK_window == NULL) {
	
		/* Obtendo todos os objetos */
		objects = gtk_builder_get_objects(UIGTK_builder);

		/* Tentando obter a janela principal */
		window = gtk_widget_get_toplevel((GtkWidget*) objects->data);

		/* Em caso de sucesso no processo anterior, definir UIGTK_window */
		if (gtk_widget_is_toplevel(window)) {
			UIGTK_window = GTK_WINDOW(window);
		}

		/* Limpando memória */
		g_slist_free(objects);
	}

	return 1;
}

/*===========================================================================*/

GObject *UIGTK_object(char *id) {

	/* Definir variáveis */
	GObject *object;
	
	/* Se o UIGTK_buider ainda não tiver sido definido, retornar erro */
	if (UIGTK_builder == NULL) {
		g_printerr("UIGTK_object ERROR: GTK not started yet, use UIGTK_load to start it\n");
		return NULL;
	}
	
	/* Definindo objeto */
	object = gtk_builder_get_object(UIGTK_builder, id);

	/* Checando se o objeto foi encontrado */
	if (object == NULL) {
		g_printerr("UIGTK_Object ERROR: Object not located on interface\n");
		return NULL;
	}

	return object;
}

/*===========================================================================*/

int UIGTK_handler(char *id, char *event, void (*method)()) {

	/* Definindo o valor do objeto */
	GObject *object = UIGTK_object(id);

	/* Checando o valor do objeto */
	if (object == NULL) {
		g_printerr("UIGTK_handler ERROR: Could not connect signal\n");
		return 0;
	}

	/* Conectando e checando a resposta */
	if (g_signal_connect(object, event, G_CALLBACK(method), NULL) <= 0) {
		g_printerr("UIGTK_handler ERROR: Error attaching trigger to event\n");
		return 0;
	}
	
	/* Definindo UIGTK_handlers */
	UIGTK_handlers++;

	return 1;
}

/*===========================================================================*/

void UIGTK_signal(char *name, void (*method)()) {

	gtk_builder_add_callback_symbol(UIGTK_builder, name, method);
	
	/* Definindo UIGTK_signals */
	UIGTK_signals++;
}

/*===========================================================================*/

int UIGTK_run(void) {

	/* Checando se UIGTK_main já foi definido */
	if (UIGTK_main) {
		g_print("UIGTK_run WARNING: GTK loopping already started\n");
		return 1;
	}

	/* Checando se o UIGTK_builder foi definido */
	if (UIGTK_builder == NULL) {
		g_printerr("UIGTK_run ERROR: GTK not started yet, use UIGTK_load to start it\n");
		return 0;
	}

	/* Conectando sinais da interface, se houver */
	if (UIGTK_signals) {
		gtk_builder_connect_signals(UIGTK_builder, NULL);
	}

	/* Checando se algum disparador ou sinal foi enviado */
	if ((UIGTK_handlers + UIGTK_signals) == 0) {

		/* Informando que nenhum evento ou disparador foi lonçado */
		g_print("UIGTK_run INFO: No trigger or signal was inserted\n");

		/* Definindo gtk_main_quit à janela principal, se definida */
		if (UIGTK_window != NULL) {
			if (g_signal_connect(G_OBJECT(UIGTK_window), "destroy", G_CALLBACK(gtk_main_quit), NULL) > 0) {
				g_print("UIGTK_run INFO: gtk_main_quit function defined in the main window\n");
			}
		}
	}

	/* Iniciando looping GTK */
	gtk_main();
	
	/* Definindo UIGTK_main */
	UIGTK_main = 1;

	return 1;
}

/*===========================================================================*/

int UIGTK_msg(int type, char *text) {

	/* Definir variáveis */
	GtkButtonsType button;
	GtkMessageType style;
	GtkWidget *box;
	int result;

	/* Checando se o UIGTK_builder foi definido */
	if (UIGTK_builder == NULL) {
		g_printerr("UIGTK_run ERROR: GTK not started yet, use UIGTK_load to start it\n");
		return -1;
	}

	/* Se UIGTK_window não tiver sido definida, retornar erro */
	if (UIGTK_window == NULL) {
		g_printerr("UIGTK_msg ERROR: Top level window not found\n");
		return -1;
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
	box = gtk_message_dialog_new(UIGTK_window, GTK_DIALOG_MODAL, style, button, "%s", text);

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





//interessante
//gtk_builder_expose_object() adiciona um objeto ao builder com um id
