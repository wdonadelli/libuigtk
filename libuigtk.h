/*-----------------------------------------------------------------------------
Library UI GTK (v1.0.0)

This is a C-language written library designed to facilitate the creation of
the graphical environment with the GTK tool using the kit's GtkBuilder function
(see https://developer.gnome.org/gtk3/stable/GtkBuilder.html).

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

Compilation with gcc:
gcc `pkg-config --cflags gtk+-3.0` -c libuigtk.c `pkg-config --libs gtk+-3.0`;

The library is hosted at https://github.com/wdonadelli/libuigtk and the manual
at https://wdonadelli.github.io/libuigtk/.

Willian Donadelli <wdonadelli@gmail.com>
-----------------------------------------------------------------------------*/

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
		Força o encerramento da aplicação se algum erro for encontrado
-----------------------------------------------------------------------------*/
	void uigtk_init(char *file);

/*-----------------------------------------------------------------------------
	uigtk_callback() conecta os sinais definidos diretamente na interface
	Argumentos:
		name    - nome da função a ser disparada quando o evento ocorrer
		handler - função a ser disparada quando o evento ocorrer
	Saída:
		Força o encerramento da aplicação se algum erro for encontrado
	Sugere o uso da macro uigtk_handler()
-----------------------------------------------------------------------------*/
	void uigtk_callback(char *name, void (*handler)());

/*-----------------------------------------------------------------------------
	uigtk_handler() macro para uigtk_callback() (name desnecessário)
-----------------------------------------------------------------------------*/
	#define uigtk_handler(handler) uigtk_callback(#handler, handler)

/*-----------------------------------------------------------------------------
	uigtk_main() inicia o looping principal do GTK (última ação)
	Saída:
		Força o encerramento da aplicação se algum erro for encontrado
-----------------------------------------------------------------------------*/
	void uigtk_main(void);

/*-----------------------------------------------------------------------------
	uigtk_builder() retorna o ponteiro do builder para o argumento
	Argumentos:
		builder - variável a receber o ponteiro do builder
	Saída:
		Força o encerramento da aplicação se algum erro for encontrado
-----------------------------------------------------------------------------*/
	GtkBuilder *uigtk_builder();

/*-----------------------------------------------------------------------------
	uigtk_object() obtém o objeto da interface a partir de seu id
	Retornos:
		NULL    - se o objeto não for encontrado
		GObject - se o objeto for encontrado
	Argumentos:
		id - identificador do objeto
-----------------------------------------------------------------------------*/
	GObject *uigtk_object(char *id);

/*-----------------------------------------------------------------------------
	uigtk_connect() adiciona disparadores a um objeto da interface
	Retornos:
		0 - se não for possível vincular o disparador
		1 - se for possível vincular o disparador
	Argumentos:
		id      - identificador do objeto
		event   - evento a ser disparado
		handler - função a ser disparada quando o evento ocorrer
-----------------------------------------------------------------------------*/
	int uigtk_connect(char *id, char *event, void (*handler)());

/*-----------------------------------------------------------------------------
	uigtk_dialog() exibe uma caixa de mensagem
	Retornos:
		-1 - se "não" foi clicado
		 0 - se a caixa foi fechada
		 1 - se "sim" ou "ok" forem clicados
	Argumentos:
		type - 
			0 - para caixa de informação (OK)
			1 - para caixa de alerta (OK)
			2 - para caixa de erro (OK)
			3 - para caixa de questão (SIM/NÃO)
			4 - para caixa genérica (CANCELAR)
		text - texto da mensagem
-----------------------------------------------------------------------------*/
	int uigtk_dialog(int type, char *text);

#endif
