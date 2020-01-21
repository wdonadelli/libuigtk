#include "libuigtk.h"
#include <string.h>
#define UIGTK_LEN 80
/*-----------------------------------------------------------------------------
UIGTK_WARN mostra a mensagem informativa
UIGTK_WARN mostra a mensagem de alerta
UIGTK_ERROR mostra a mensagem de erro
Argumentos:
	f - Nome da função onde a mensagem ocorreu
	m - Mensagem
-----------------------------------------------------------------------------*/
#define UIGTK_INFO(f,m) g_print("%s INFO: %s\n", f, m)
#define UIGTK_WARN(f,m,r) g_print("%s WARNING: %s\n", f, m)
#define UIGTK_ERROR(f,m,r) g_printerr("%s ERROR: %s\n", f, m)

/*-----------------------------------------------------------------------------
Definindo a estrutura que guardará o status das construção da interface
-----------------------------------------------------------------------------*/
typedef struct uigtk_status {
	int start;
	/* Guarda ponteiros para os elementos GTK */
	GError *Error;
	GtkBuilder *Builder;
	GtkWindow *Window;
	/* Registra (0/1) se determinado procedimento foi executado */
	int init;
	int builder;
	int window;
	int handler;
	int signal;
	int quit;
	int main;
	/* Registra as mensagens da biblioteca */
	char m_init[UIGTK_LEN];
	char m_builder[UIGTK_LEN];
	char m_window[UIGTK_LEN];
	char m_handler[UIGTK_LEN];
	char m_signal[UIGTK_LEN];
	char m_quit[UIGTK_LEN];
	char m_main[UIGTK_LEN];
	char m_object[UIGTK_LEN];
} t_uigtk;

/*-----------------------------------------------------------------------------
Definindo a variável global UIGTK
-----------------------------------------------------------------------------*/
static t_uigtk UIGTK;

/*-----------------------------------------------------------------------------
UIGTK_start inicia a estrutura UIGTK
-----------------------------------------------------------------------------*/
static void UIGTK_start(void) {
	if (UIGTK.start != 1) {
		UIGTK.start   = 1;
		UIGTK.Error   = NULL;
		UIGTK.Builder = NULL;
		UIGTK.Window  = NULL;
		UIGTK.init    = 0;
		UIGTK.window  = 0;
		UIGTK.handler = 0;
		UIGTK.signal  = 0;
		UIGTK.quit    = 0;
		UIGTK.main    = 0;
		strcpy(UIGTK.m_init,    "GTK not initialized.");
		strcpy(UIGTK.m_builder, "GTK Builder not initialized.");
		strcpy(UIGTK.m_window,  "GTK top level window not defined.");
		strcpy(UIGTK.m_handler, "Failed to set handler.");//"No GTK trigger or signal was inserted.");
		strcpy(UIGTK.m_signal,  "Failed to set signal.");
		strcpy(UIGTK.m_quit,    "gtk_main_quit function defined in the main window");
		strcpy(UIGTK.m_main,    "GTK loopping already started.");
		strcpy(UIGTK.m_object,  "GTK object not located.");
	}
}

/*-----------------------------------------------------------------------------
UIGTK_error define as mensagens de erro do GTK
-----------------------------------------------------------------------------*/
static GError *UIGTK_error = NULL;

/*-----------------------------------------------------------------------------
UIGTK.Builder define o construtor da interface
-----------------------------------------------------------------------------*/
static GtkBuilder *UIGTK.Builder = NULL;

/*-----------------------------------------------------------------------------
UIGTK_window define a janela principal da interface
-----------------------------------------------------------------------------*/
static GtkWindow *UIGTK_window = NULL;

/*-----------------------------------------------------------------------------
UIGTK_init define se o GTK foi inicializado com sucesso
-----------------------------------------------------------------------------*/
static int UIGTK_init = 0;

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

	/* Definir variáveis locais */
	GSList    *objects;
	GtkWidget *window;

	/* Inicializar variável global UIGTK */
	UIGTK_start();

	/* #Alerta Verificar se GTK já iniciou o looping FIXME (Será que isso é mesmo preciso? Ver UIGTK_main)*/
	if (UIGTK.main) {
		UIGTK_WARN("UIGTK_load", UIGTK.m_main);
		return 1;
	}

	/* Inicializar GTK se necessário */
	if (!UIGTK.init) {
		UIGTK.init = gtk_init_check(0, NULL) != TRUE ? 0 : 1;
		if (!UIGTK.init) {
			UIGTK_ERROR("UIGTK_load", UIGTK.m_init);
			return 0;
		}
	}

	/* Definir GTK Builder se necessário */
	if (!UIGTK.builder) {
		UIGTK.Builder = gtk_builder_new();
		UIGTK.builder = UIGTK.Builder == NULL ? 0 : 1;
		if (!UIGTK.builder) {
			UIGTK_ERROR("UIGTK_load", UIGTK.m_builder, 0);
			return 0;
		}
	}

	/* Carregar interface (arquivo .ui) */
	if (!gtk_builder_add_from_file(UIGTK.Builder, file, &UIGTK.Error)) {
		UIGTK_ERROR("UIGTK_load", UIGTK.Error->message);
		g_clear_error(&UIGTK.Error);
		return 0;
	}

	/* Definir janela principal da aplicação */
	if (!UIGTK.Window) {
	
		/* Obtendo todos os objetos */
		objects = gtk_builder_get_objects(UIGTK.Builder);

		/* Tentando obter a janela principal */
		window = gtk_widget_get_toplevel((GtkWidget*) objects->data);

		/* Em caso de sucesso no processo anterior, definir */
		UIGTK.window = !gtk_widget_is_toplevel(window) ? 0 : 1;

		if (!UIGTK.window) {
			UIGTK_INFO("UIGTK_load", UIGTK.m_window);
		}

		/* Limpando memória */
		g_slist_free(objects);
	}

	return 1;
}

/*===========================================================================*/

GObject *UIGTK_object(char *id) {

	/* Definir variáveis locais */
	GObject *object;
	
	/* Definindo objeto */
	object = UIGTK.builder ? gtk_builder_get_object(UIGTK.Builder, id) : NULL;
	
	/* Verificar objeto */
	if (object == NULL) {
		UIGTK_ERROR("UIGTK_object", (!UIGTK.builder ? UIGTK.m_builder : UIGTK.m_object));
	}

	return object;
}

/*===========================================================================*/

int UIGTK_handler(char *id, char *event, void (*method)()) {//FIXME parei aqui

	/* Definindo variáveis locais */
	GObject *object;
	double signal;
	
	/* Definindo objeto */
	object = UIGTK_object(id);

	if (object == NULL) {
		UIGTK_ERROR("UIGTK_handler", UIGTK.m_object);
		return 0;
	}

	/* Conectando sinal */
	signal = g_signal_connect(object, event, G_CALLBACK(method), NULL);

	if (signal <= 0) {
		UIGTK_ERROR("UIGTK_handler", UIGTK.m_handler);
		return 0;
	}
	
	UIGTK.handlers++;

	return 1;
}

/*===========================================================================*/

int UIGTK_signal(char *name, void (*method)()) {

	/* Checando GTK Builder */
	if (!UIGTK.builder == NULL) {
		UIGTK_ERROR("UIGTK_signal", UIGTK.m_builder);
		return 0;
	}

	/* Definindo sinais */
	gtk_builder_add_callback_symbol(UIGTK.Builder, name, method);

	UIGTK.signals++;

	return 1;
}

/*===========================================================================*/

int UIGTK_run(void) {

	/* Checando looping */
	if (UIGTK.main) {
		UIGTK_WARN("UIGTK_run", UIGTK.m_main);
		return 1;
	}

	/* Checando inicialização do GTK */
	if (!UIGTK.init) {
		UIGTK_ERROR("UIGTK_run", UIGTK.m_init);
		return 0;
	}

	/* Conectando sinais da interface, se houver */
	if (UIGTK.signals) {
		gtk_builder_connect_signals(UIGTK.Builder, NULL);
	}

	/* Checando se algum disparador ou sinal foi enviado */
	if ((UIGTK_handlers + UIGTK_signals) == 0) {

		/* Informando que nenhum evento ou disparador foi lonçado */
		g_print("UIGTK_run No GTK trigger or signal was inserted\n");

		/* Definindo gtk_main_quit à janela principal, se definida */
		if (UIGTK_window != NULL) {
			if (g_signal_connect(G_OBJECT(UIGTK_window), "destroy", G_CALLBACK(gtk_main_quit), NULL) > 0) {
				g_print("UIGTK_run gtk_main_quit function defined in the main window\n");
			}
		}
	}

	/* Iniciando looping GTK */
	gtk_main();
	
	/* Definindo UIGTK_main FIXME (isso vai acontecer enquanto main_quit não for chamada?)*/
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

	/* Checando se o UIGTK.Builder foi definido */
	if (UIGTK.Builder == NULL) {
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
