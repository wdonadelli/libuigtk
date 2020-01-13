#include "libuigtk.h"

/*-----------------------------------------------------------------------------
UIGTK_builder define o construtor da interface
-----------------------------------------------------------------------------*/
static GtkBuilder *UIGTK_builder = NULL;

/*-----------------------------------------------------------------------------
UIGTK_error define as mensagens de erro do GTK
-----------------------------------------------------------------------------*/
static GError *UIGTK_error = NULL;

/*-----------------------------------------------------------------------------
UIGTK_INIT registra se o GTK já foi iniciado
-----------------------------------------------------------------------------*/
static int UIGTK_INIT = 0;

/*-----------------------------------------------------------------------------
UIGTK_MAIN registra se o looping GTK já foi iniciado
-----------------------------------------------------------------------------*/
static int UIGTK_MAIN = 0;

/*-----------------------------------------------------------------------------
UIGTK_HANDLERS registra a quantidade de disparadores adicionados
-----------------------------------------------------------------------------*/
static int UIGTK_HANDLERS = 0;

/*-----------------------------------------------------------------------------
UIGTK_SIGNALS registra a quantidade de sinais adicionados
-----------------------------------------------------------------------------*/
static int UIGTK_SIGNALS = 0;

/*===========================================================================*/

int UIGTK_start(char file[]) {

	/* Checando se o GTK já não foi iniciado */
	if (UIGTK_INIT) {
		g_print("UIGTK_start WARNING: GTK already started\n");
		return 1;
	}

	/* Iniciar GTK */
	gtk_init(0, NULL);

	/* Definir UIGTK_builder */
	UIGTK_builder = gtk_builder_new();

	/* Carregando interface e checando se ocorreu algum erro */
	if (!gtk_builder_add_from_file(UIGTK_builder, file, &UIGTK_error)) {
		g_printerr("UIGTK_start ERROR: %s\n", UIGTK_error->message);
		g_clear_error(&UIGTK_error);
		return 0;
	}
	
	/* Definir UIGTK_INIT */
	UIGTK_INIT = 1;

	return 1;
}

/*===========================================================================*/

GObject *UIGTK_object(char *id) {

	/* Definindo o valor do objeto */
	GObject *object = UIGTK_INIT ? gtk_builder_get_object(UIGTK_builder, id) : NULL;

	/* Checando se o GTK foi iniciado */
	if (!UIGTK_INIT) {
		g_printerr("UIGTK_object ERROR: GTK not started yet, use UIGTK_start to start it\n");
	} else if (object == NULL) {
		g_printerr("UIGTK_object ERROR: Object not located on interface\n");
	}

	return object;
}

/*===========================================================================*/

GtkWidget *UIGTK_widget(char *id) {

	/* Definindo o valor do objeto */
	GObject *object = UIGTK_object(id);

	/* Checando o valor retornado para o objeto */
	if (object == NULL) {
		g_printerr("UIGTK_widget ERROR: Unable to get widget\n");
		return NULL;
	}

	return GTK_WIDGET(object);
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
	
	/* Definindo UIGTK_HANDLERS */
	UIGTK_HANDLERS++;

	return 1;
}

/*===========================================================================*/

void UIGTK_signal(char *name, void (*method)()) {

	gtk_builder_add_callback_symbol(UIGTK_builder, name, method);
	
	/* Definindo UIGTK_SIGNALS */
	UIGTK_SIGNALS++;
}

/*===========================================================================*/

int UIGTK_run(void) {

	/* Checando se o looping já foi iniciado */
	if (UIGTK_MAIN) {
		g_print("UIGTK_run WARNING: GTK loopping already started\n");
		return 1;
	}

	/* Checando se o GTK foi iniciado */
	if (!UIGTK_INIT) {
		g_printerr("UIGTK_run ERROR: GTK not started yet, use UIGTK_start to start it\n");
		return 0;
	}

	/* Conectando sinais da interface, se houver */
	if (UIGTK_SIGNALS) {
		gtk_builder_connect_signals(UIGTK_builder, NULL);
	}

	/* Checando se algum disparador ou sinal foi enviado */
	if (UIGTK_HANDLERS == 0 && UIGTK_SIGNALS == 0) {
		g_print("UIGTK_run INFO: No trigger or signal was inserted\n");
	}

	/* Iniciando looping GTK */
	gtk_main();
	
	/* Definindo UIGTK_MAIN */
	UIGTK_MAIN = 1;

	return 1;
}



//interessante
//gtk_builder_expose_object() adiciona um objeto ao builder com um id
