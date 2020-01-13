#ifndef LIBRARY_UI_GTK

	#define LIBRARY_UI_GTK

/*-----------------------------------------------------------------------------
	A presente biblioteca exige a biblioteca GTK
-----------------------------------------------------------------------------*/
	#include <gtk/gtk.h>

/*-----------------------------------------------------------------------------
	UIGTK_start() inicia a inteface GTK a partir de um arquivo.ui (builde xml)
	Retornos:
		0 - se algum error ocorrer
		1 - se não ocorrer erros
	Argumentos:
		file - é o nome do arquivo com a interface xml (.ui)
-----------------------------------------------------------------------------*/
	int UIGTK_start(char file[]);

/*-----------------------------------------------------------------------------
	UIGTK_run() inicia o looping do GTK (última ação)
	Retornos:
		0 - se algum erro ocorrer
		1 - se não ocorrer erros
-----------------------------------------------------------------------------*/
	int UIGTK_run(void);

/*-----------------------------------------------------------------------------
	UIGTK_object() obtém o objeto da interface
	Retornos:
		NULL    - se algum error ocorrer
		GObject - se não ocorrer erros
	Argumentos:
		id - identificador do objeto
-----------------------------------------------------------------------------*/
	GObject *UIGTK_object(char *id);

/*-----------------------------------------------------------------------------
	UIGTK_widget() obtém o widget da interface
	Retornos:
		NULL    - se algum error ocorrer
		GObject - se não ocorrer erros
	Argumentos:
		id - identificador do objeto
-----------------------------------------------------------------------------*/
	GtkWidget *UIGTK_widget(char *id);

/*-----------------------------------------------------------------------------
	UIGTK_handler() adiciona disparadores a um objeto da interface
	Retornos:
		0 - se algum error ocorrer
		1 - se não ocorrer erros
	Argumentos:
		id     - identificador do objeto
		event  - evento a ser disparado
		method - função a ser disparada quando o evento ocorrer
-----------------------------------------------------------------------------*/
	int UIGTK_handler(char *id, char *event, void (*method)());

/*-----------------------------------------------------------------------------
	UIGTK_signal() conecta os sinais definidos diretamente na interface
	Retornos:
		não há
	Argumentos:
		name   - nome da função a ser disparada quando o evento ocorrer
		method - função a ser disparada quando o evento ocorrer
-----------------------------------------------------------------------------*/
	void UIGTK_signal(char *name, void (*method)());

#endif
