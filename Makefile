CC = gcc
CFLAGS = $(shell pkg-config --cflags mxml) -D WIN32
LIBS = $(shell pkg-config --libs mxml)
OBJS = rcbc.o rcbc_data.o rcbc_render_gl.o rcbc_render_textinfo.o rcbc_xml_minixml.o console.o rcbc_xml_minixml_visualscene.o rcbc_xml_minixml_geometries.o
all: rcbcview

rcbcview: rcbcview.c ${OBJS} 
	${CC} $< ${CFLAGS} ${LIBS} -lGL -lGLU -lglut ${OBJS} -o $@ -Wall

clean:
	rm -f rcbcview rcbcview.exe *~ *.o core
