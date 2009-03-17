CC = gcc
CC = colorgcc
CC = $(shell which cc gcc colorgcc | tail -n 1)
CFLAGS = $(shell pkg-config --cflags mxml)
#LIBS = $(shell pkg-config --libs mxml)
LIBS = -lmxml -lGL -lGLU -lglut /usr/lib/libSOIL.so
OBJS = rcbc.o rcbc_data.o rcbc_render_gl.o rcbc_render_textinfo.o rcbc_xml_minixml.o console.o rcbc_xml_minixml_visualscene.o rcbc_xml_minixml_geometries.o rcbc_xml_minixml_textures.o
INC = -I/usr/include/SOIL
all: rcbcview

rcbcview: rcbcview.c ${OBJS} 
	${CC} $< ${CFLAGS} ${INC} ${LIBS} ${OBJS} -o $@ -Wall

rcbc_render_gl.o: rcbc_render_gl.c rcbc_render_gl.h
	${CC} ${CFLAGS} ${INC} -c $<
	
clean:
	rm -f rcbcview rcbcview.exe *~ *.o core
