CC = gcc
CFLAGS = $(shell pkg-config --cflags mxml)
LIBS = $(shell pkg-config --libs mxml)

all: rcbcview

rcbcview: rcbcview.c rcbc.o rcbc_render_gl.o rcbc_render_textinfo.o rcbc_xml_minixml.o console.o 
	${CC} $< ${CFLAGS} ${LIBS} -lGL -lGLU -lglut rcbc.o rcbc_render_gl.o rcbc_render_textinfo.o rcbc_xml_minixml.o console.o -o $@ -Wall

clean:
	rm -f rcbcview *~ *.o core
