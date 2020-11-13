# GTK User Interface Builder Library - UIGTK

UIGTK is a C-language written library designed to simplify the creation of the graphical environment with the GTK toolkit from the [GtkBuilder](https://developer.gnome.org/gtk3/stable/GtkBuilder.html) object.

## Handbook

- [https://github.com/wdonadelli/libuigtk/wiki](https://github.com/wdonadelli/libuigtk/wiki)

## Source Code

- [libuigtk.h](https://wdonadelli.github.io/libuigtk/libuigtk.h)
- [libuigtk.c](https://wdonadelli.github.io/libuigtk/libuigtk.c)

## Package

- *libgtk-3-dev* package or equivalent.

## Compilation (GCC) 

To compile the source code it is necessary to use the _pkg-config --cflags gtk+-3.0_ and _pkg-config --libs gtk+-3.0_ flags.

```sh
gcc `pkg-config --cflags gtk+-3.0` -c libuigtk.c  `pkg-config --libs gtk+-3.0`
```

## Versions

### v1.1.0 (2020-05-18)

- Improved function returns; and
- Improved terminal messages.

## Authors

- Willian Donadelli (<wdonadelli@gmail.com>)
