/*___BIBLIOTECAS_____________________________________________________________*/
#include "libuigtk.h"

/*___MACROS__________________________________________________________________*/

/* Mensagens */
#define UIGTK_MSG_NO_UIGTK "UIGTK not initialized correctly, see uigtk_init function."
#define UIGTK_MSG_NO_GTK "Failed to initialize the GTK."
#define UIGTK_MSG_NO_BUILDER "Failed to initialize the GTK Builder."
#define UIGTK_MSG_NO_WINDOW "Top level window (GtkWindow object) not found."
#define UIGTK_MSG_NO_SIGNAL "No callback symbols added."
#define UIGTK_MSG_NO_DESTROY "gtk_main_quit not defined and the attempt to set the trigger failed."
#define UIGTK_MSG_DESTROY "gtk_main_quit set automatically."
#define UIGTK_MSG_NO_OBJECT "Object not found."
#define UIGTK_MSG_NO_HANDLER "Failed to link the handler."
#define UIGTK_MSG_USER "\n\033[1;%dm%s \033[1;36mlibuigtk.uigtk_%s() [%d]\033[0m:\n\t\"%s\"\n\n"

/* Valores */
#define UIGTK_BUILDER (_uigtk.init ? (_uigtk.builder) : NULL)

/* Funções */
#define UIGTK_INFO(msg, name) g_print(UIGTK_MSG_USER, 34, "INFO", name, __LINE__, msg)
#define UIGTK_WARN(msg, name) g_print(UIGTK_MSG_USER, 33, "WARN", name, __LINE__, msg)
#define UIGTK_ERROR(msg, name) g_printerr(UIGTK_MSG_USER, 31, "ERROR", name, __LINE__, msg); exit(1)
#define UIGTK_FNAME(name) strcpy(_uigtk.fname, name);
#define UIGTK_CHECK() if (!_uigtk.init) {UIGTK_ERROR(UIGTK_MSG_NO_UIGTK);}





/*___VARIÁVEIS_GLOBAIS_______________________________________________________*/

static struct {
	/* _uigtk guarda o status e os ponteiros */
	int gtk: 1;
	int signal: 1;
	int destroy: 1;
	GtkBuilder *builder;
	GtkWindow *window;
} _uigtk;

/*___FUNÇÕES_INTERNAS________________________________________________________*/

static void _uigtk_reset(void) {
	/*-- Inicializa o status --*/
	_uigtk.gtk = !_uigtk.gtk ? 0 : 1;
	if (_uigtk.builder == NULL) {
		_uigtk.window  = NULL;
		_uigtk.destroy = 0;
		_uigtk.signal  = 0;
	}
}

/*...........................................................................*/

static void _uigtk_gtk(void) {
	/*-- Inicia o GTK --*/

	/* Chamando GTK e verificando erro */
	if (gtk_init_check(0, NULL) != TRUE) {
		UIGTK_ERROR(UIGTK_MSG_NO_GTK, "init");
	}
	_uigtk.gtk = 1;
}

/*...........................................................................*/

static void _uigtk_builder(void) {
	/*-- Carrega o GTK Builder --*/

	/* Definindo GTK Buider */
	_uigtk.builder = gtk_builder_new();
	/* Verificando erro */
	if (_uigtk.builder == NULL) {
		UIGTK_ERROR(UIGTK_MSG_NO_BUILDER, "init");
	}
}

/*...........................................................................*/

static void _uigtk_file(char *file) {
	/*-- Carrega o arquivo .ui --/

	/* Variável de erro */
	GError *error;
	/* Carrega o arquivo e checa o sucesso na operação */
	if (!gtk_builder_add_from_file(_uigtk.builder, file, &error)) {
		UIGTK_ERROR(error->message, "init");
	}
	/* Limpar a memória */
	g_clear_error(&error);
}

/*...........................................................................*/

static void _uigtk_window(void) {
	/*-- Localiza window top level --*/

	/* Tentando obter a janela de nível superior */
	GSList    *objects = gtk_builder_get_objects(_uigtk.builder);
	GtkWidget *window  = gtk_widget_get_toplevel((GtkWidget*) objects->data);
	/* Limpando memória */
	g_slist_free(objects);
	/* Definindo se houve sucesso */
	_uigtk.window = gtk_widget_is_toplevel(window) ? GTK_WINDOW(window) : NULL;
}

/*...........................................................................*/

static void _uigtk_check_destroy(void) {//FIXME tá muito bagunçado isso aqui
	/*-- Define gtk_main_quit à window, caso não exista --*/

	/* Alertar se nenhum sinal for adicionado */
	if (!_uigtk.signal) {
		UIGTK_WARN(UIGTK_MSG_NO_SIGNAL, "main");	
	}
	/* Alertar se window não for localizado */
	if (_uigtk.window == NULL) {
		UIGTK_WARN(UIGTK_MSG_NO_WINDOW, "main");
	}
	/* Sair se destroy e window não existirem */
	if (!_uigtk.destroy && _uigtk.window == NULL) {
		UIGTK_ERROR(UIGTK_MSG_NO_DESTROY, "main");
	}
	/* Tentar adicionar função para parar o looping ao destruir a janela */
	if (!_uigtk.destroy) {
		int result = g_signal_connect(G_OBJECT(_uigtk.window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
		if (result <= 0) {
			UIGTK_ERROR(UIGTK_MSG_NO_DESTROY, "main");
		}
		/* Em caso de sucesso, informar */
		UIGTK_INFO(UIGTK_MSG_DESTROY, "main");
		_uigtk.destroy = 1;
	}
}

/*...........................................................................*/

static void _uigtk_check_uigtk(int builder, char *name) {
	/*-- Checa se as fases do uigtk foram concluídas --*/

	/* Verificando se GTK foi carregado */
	if (!_uigtk.gtk) {
		UIGTK_ERROR(UIGTK_MSG_NO_GTK, name);
	}
	/* Verificando se GTK Builder foi carregado */
	if (builder && _uigtk.builder == NULL) {
		UIGTK_ERROR(UIGTK_MSG_NO_BUILDER, name);
	}
}

/*___FUNÇÕES_DO_CABEÇALHO____________________________________________________*/

/*...........................................................................*/

void uigtk_init(char *file) {

	/* Definindo status inicial */
	_uigtk_reset();
	/* Carregando GTK, se necessário */
	if (!_uigtk.gtk) {
		_uigtk_gtk();
	}
	/* Carregando GTK Builder, se necessário */
	if (_uigtk.builder == NULL) {
		_uigtk_builder();
	}
	/* Carregando arquivo.ui */
	_uigtk_file(file);
}

/*...........................................................................*/

void uigtk_callback(char *name, void (*handler)()) {

	_uigtk_check_uigtk(1, "callback/handler");
	/* Adicionando sinal ao builder */
	gtk_builder_add_callback_symbol(_uigtk.builder, name, handler);
	/* Validando destroy */
	if (handler == gtk_main_quit) {
		_uigtk.destroy = 1;
	}
	/* Validando sinal */
	_uigtk.signal = 1;
}

/*...........................................................................*/

void uigtk_main(void) {

	_uigtk_check_uigtk(1, "main");
	/* Definindo window */
	_uigtk_window();
	/* Checando destroy */
	_uigtk_check_destroy();
	/* Conectando sinais */
	gtk_builder_connect_signals(_uigtk.builder, NULL);
	/* iniciando GTK main loop */
	gtk_main();
	/* Apagando builder após o looping */
	_uigtk.builder = NULL;
}

/*...........................................................................*/

GObject *uigtk_object(char *id) {

	_uigtk_check_uigtk(1, "object");
	/* Obtendo o objeto */
	GObject *object = gtk_builder_get_object(_uigtk.builder, id);
	return object;
}

/*...........................................................................*/
int uigtk_connect(char *id, char *event, void (*handler)()) {

	_uigtk_check_uigtk(1, "connect");
	/* Obtendo o objeto */
	GObject *object = uigtk_object(id);
	/* Checar erro na localização do objeto */
	if (object == NULL) {
		UIGTK_WARN(UIGTK_MSG_NO_OBJECT, "connect");
		return 0;
	}
	/* Tentar Atribuir disparador ao elemento */
	if (g_signal_connect(object, event, G_CALLBACK(handler), NULL) <= 0) {
		UIGTK_WARN(UIGTK_MSG_NO_HANDLER, "connect");
		return 0;
	}
	/* Se for gtk_main_quit, informar */
	if (handler == gtk_main_quit) {
		_uigtk.destroy = 1;
	}
	/* Em caso de sucesso */
	return 1;
}

/*...........................................................................*/
int uigtk_dialog(int type, char *text) {

	_uigtk_check_uigtk(0, "dialog");
	/* Alertar se window não existir */
	if (_uigtk.window == NULL) {
		UIGTK_WARN(UIGTK_MSG_NO_WINDOW, "dialog");
	}
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
		_uigtk.window,
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
