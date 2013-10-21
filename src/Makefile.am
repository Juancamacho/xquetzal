## Process this file with automake to generate a Makefile.in
## To build all programs with GTK+ uncomment these lines.
##AM_CPPFLAGS = @GTK_CFLAGS@
##AM_LDADD = @GTK_LIBS@

bin_PROGRAMS = gui-program cli-program
gui_program_SOURCES = gui.c
cli_program_SOURCES = cli.c

## Only our GUI program uses GTK+ at the moment.
gui_program_CPPFLAGS = @GTK_CFLAGS@
gui_program_LDADD = @GTK_LIBS@

CLEANFILES = *~