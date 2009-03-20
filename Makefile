PREFIX = /usr/local

CC = $(shell which cc gcc colorgcc | tail -n 1)
CFLAGS = $(shell pkg-config --cflags mxml)
LIBS = -lmxml -lGL -lGLU -lglut /usr/lib/libSOIL.so
OBJS = rcbc.o rcbc_data.o rcbc_render_gl.o rcbc_render_textinfo.o rcbc_xml_minixml.o console.o rcbc_xml_minixml_visualscene.o rcbc_xml_minixml_geometries.o rcbc_xml_minixml_textures.o rcbc_data_hookup.o ll.o
INC = -I/usr/include/SOIL

all: rcbcview librcbc.a

rcbcview: rcbcview.c ${OBJS} 
	${CC} $< ${CFLAGS} ${INC} ${LIBS} ${OBJS} -o $@ -Wall

librcbc.a: rcbc.o
	ar rcs $@ $<

rcbc_render_gl.o: rcbc_render_gl.c rcbc_render_gl.h
	${CC} ${CFLAGS} ${INC} -c $<

install: librcbc.a rcbcview
	cp librcbc.a ${PREFIX}/lib
	cp rcbcview ${PREFIX}/bin
	cp rcbc.h ${PREFIX}/include

uninstall:
	rm -f ${PREFIX}/lib/librcbc.a
	rm -f ${PREFIX}/bin/rcbcview
	rm -f ${PREFIX}/include/rcbc.h

clean:
	rm -f rcbcview rcbcview.exe *~ *.o *.a core
