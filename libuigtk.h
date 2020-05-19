/*------------------------------------------------------------------------------
Library UI GTK (v1.1.0) <wdonadelli@gmail.com>

This is a C-language written library designed to facilitate the creation of
the graphical environment with the GTK tool using the kit's GtkBuilder function.

https://developer.gnome.org/gtk3/stable/GtkBuilder.html
https://github.com/wdonadelli/libuigtk
https://wdonadelli.github.io/libuigtk

Ubuntu 18 LTS package: libgtk-3-dev

GCC compilation:
gcc `pkg-config --cflags gtk+-3.0` -c libuigtk.c  `pkg-config --libs gtk+-3.0`

--------------------------------------------------------------------------------
MIT License

Copyright (c) 2020 Willian Donadelli

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
------------------------------------------------------------------------------*/

#ifndef LIBRARY_UI_GTK_H

	#define LIBRARY_UI_GTK_H

/*-----------------------------------------------------------------------------
	A presente biblioteca exige a biblioteca GTK
-----------------------------------------------------------------------------*/
	#include <gtk/gtk.h>
	
/*-----------------------------------------------------------------------------
	uigtk_init() inicia a inteface GTK a partir de um arquivo.ui (builde xml)
	Argumentos:
		file - é o nome do arquivo com a interface xml (.ui)
	Saída:
		0 para erro conhecido
		1 para sucesso
-----------------------------------------------------------------------------*/
	int uigtk_init (char *file);

/*-----------------------------------------------------------------------------
	uigtk_callback() conecta os sinais definidos diretamente na interface
	Argumentos:
		name    - nome da função a ser disparada quando o evento ocorrer
		handler - função a ser disparada quando o evento ocorrer
	Saída:
		0 para erro conhecido
		1 para sucesso
	Sugere-se o uso da macro uigtk_handler()
-----------------------------------------------------------------------------*/
	int uigtk_callback (char *name, void (*handler)());

	#define uigtk_handler(handler) uigtk_callback(#handler, handler)

/*-----------------------------------------------------------------------------
	uigtk_main() inicia o looping principal do GTK (última ação)
	Saída:
		0 para erro conhecido
		1 para sucesso
-----------------------------------------------------------------------------*/
	int uigtk_main (void);

/*-----------------------------------------------------------------------------
	uigtk_builder() retorna o ponteiro do builder
	Saída:
		NULL       - se algum erro conhecido for localizado
		GtkBuilder - se tiver sido inicializada adequadamente
	Sugere-se o uso da macro uigtk_set_builder() se for associar a uma variável
		var é o nome da variável sem necessidade de definição de tipo
-----------------------------------------------------------------------------*/
	GtkBuilder *uigtk_builder (void);
	
	#define uigtk_set_builder(var) GtkBuilder *var = uigtk_builder ()

/*-----------------------------------------------------------------------------
	uigtk_object() obtém o objeto da interface a partir de seu id
	Retornos:
		NULL    - se algum erro conhecido ocorrer ou objeto não encontrado
		GObject - se o objeto for encontrado
	Argumentos:
		id - identificador do objeto
	Sugere-se o uso da macro uigtk_set_object() se for associar a uma variável
		var é o nome da variável sem necessidade de definição de tipo
-----------------------------------------------------------------------------*/
	GObject *uigtk_object (char *id);

	#define uigtk_set_object(var, id) GObject *var = uigtk_object (id)

/*-----------------------------------------------------------------------------
	uigtk_dialog() exibe uma caixa de mensagem
	Retornos:
		-1 - se "não" foi clicado
		 0 - se a caixa foi fechada
		 1 - se "sim" ou "ok" forem clicados
	Argumentos:
		type  - 
			0  - para caixa de informação (OK)
			1  - para caixa de alerta (OK)
			2  - para caixa de erro (OK)
			3  - para caixa de questão (SIM/NÃO)
			4  - para caixa genérica (CANCELAR)
		title - título da caixa
		text  - texto da mensagem
-----------------------------------------------------------------------------*/
	int uigtk_dialog (int type, char *title, char *text);
#endif
