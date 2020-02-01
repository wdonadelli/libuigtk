# GTK User Interface Builder Library - UIGTK

UIGTK is a C-language written library designed to simplify the creation of the graphical environment with the GTK toolkit from the [GtkBuilder](https://developer.gnome.org/gtk3/stable/GtkBuilder.html) object.

## Tools

The library has two types of functions, one to create the interface and another to help with simple tasks that may require many lines of code.

To help understand the library, the following example interface will be used:

```xml
<?xml version="1.0" encoding="UTF-8"?>
<interface>
	<object id="window" class="GtkWindow">
		<property name="title">-- LIBUIGTK --</property>
		<property name="visible">True</property>
		<signal name="destroy" handler="gtk_main_quit" />
		<child>
			<object class="GtkBox" id="box">
				<property name="visible">True</property>
				<property name="orientation">vertical</property>
				<property name="expand">True</property>
				<child>
					<object class="GtkButton" id="btn1">
						<property name="visible">True</property>
						<property name="label">Hello</property>
						<signal name="clicked" handler="hello" />
					</object>
					<packing>
						<property name="expand">True</property>
						<property name="fill">True</property>
					</packing>
				</child>
				<child>
					<object class="GtkButton" id="btn2">
						<property name="visible">True</property>
						<property name="label">Bye</property>
						<signal name="clicked" handler="bye" />
					</object>
					<packing>
						<property name="expand">True</property>
						<property name="fill">True</property>
					</packing>
				</child>
			</object>
		</child>
	</object>
</interface>
```

This results in the following appearance:

![Interface image](images/screen1.png)

The interface file name will be set to "example.ui".

## Interface construction

The construction is divided into three phases:

- Initialize the library indicated an interface file (.ui);
- Connect the interface signals to their respective handlers; and
- Start the main loop.

### Interface initialization

To initialize the interface, the `uigtk_unit()` function must be used, whose characteristics are described below:

```c
void uigtk_init(char *file);
```

#### Parameters

|Name|Description|
|--:|---|
|file|Interface file path|

#### Returns

Forces the application to exit if any identifiable error is found. Possible errors are:

- GTK startup failure;
- Failure to start GTK Builder; and
- Failed to load the interface file.

### Connecting signals

To connect the signals to your handlers, you must use the `uigtk_hadler()` function, whose characteristics are described below:

```c
#define uigtk_handler(handler)
```

#### Parameters
|Name|Description|
|--:|---|
|handler|Name of the handler function.|

#### Returns

Forces the application to exit if any identifiable error is found. Possible errors are:

- Call the function without initializing the interface.

### Start the main loop

To start the main loop, use the `uigtk_main()` function, whose characteristics are described below:

```c
void uigtk_main()
```
#### Returns

Forces the application to exit if any identifiable error is found. Possible errors are:

- Call the function without initializing the interface, and
- If the top-level window (`GtkWindow` object) is not found in situations where `gtk_main_quit()` was not defined by the library.

### Example

Considering that the name of the file containing the source code is called "example.c" and that it is in the same directory as the interface file "example.ui", the following sequence of commands will be necessary to build the interface.

Initially it is necessary to include the library header in our source code. Considering that both are in the same directory:

```c
#include "libuigtk.h"
```

As noted in the interface file, there are two defined handlers whose construction needs to be carried out:

```xml
<signal name="clicked" handler="hello" />
...
<signal name="clicked" handler="bye" />
```

The prototypes of the functions would look like this:

```c
static void hello(GtkWidget *widget, gpointer data);
static void bye(GtkWidget *widget, gpointer data);
```

To start the construction, just call the function to initialize the library indicating the interface file:

```c
uigtk_init("example.ui");
```

After initialization, the manipulators must be connected, according to the signals defined in the interface:

```c
uigtk_handler(gtk_main_quit);
uigtk_handler(hello);
uigtk_handler(bye);
```

Finally, the main loop must be initialized:

```c
uigtk_main();
```

Grouping all the information, we have:

```c
#include "libuigtk.h"

static void hello(GtkWidget *widget, gpointer data);

static void bye(GtkWidget *widget, gpointer data);

void main(int argc, char *argv[]) {

	/* Interface initialization */
	uigtk_init("example.ui");

	/* Connecting signals */
	uigtk_handler(gtk_main_quit);
	uigtk_handler(hello);
	uigtk_handler(bye);

	/* Start the main loop */	
	uigtk_main();

}
```

Don't forget to define the actions of the manipulators.






