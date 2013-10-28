## Process this file with automake to produce Makefile.in

INCLUDES = \
	@GTK_CFLAGS@

bin_PROGRAMS = ollin

ollin_SOURCES = \
	main.c \
	support.c support.h \
	interface.c interface.h \
	callbacks.c callbacks.h \
	Buffer.c Buffer.h \
	Guardar.c Guardar.h \
	ImageEngine.c ImageEngine.h \
	Objeto.h i_addr.h

ollin_LDADD = @GTK_LIBS@

